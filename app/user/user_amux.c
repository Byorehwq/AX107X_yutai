/*****************************************************************************
 * Module    : User
 * File      : user_amux.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : AMUX的相关配置
 *****************************************************************************/
#include "include.h"

IAR_XDATA_A volatile u8 energy_cnt;
IAR_XDATA_A volatile u8 energy_flag;

#if AUX_EQ_EN
#define task_aux_process            task_aux_eq_process
#elif REC_AUX_EN
#define task_aux_process            task_record_process
#else
#define task_aux_process            dummy_void
#endif

#if REC_FM_EN
#define task_fm_process             task_record_process
#else
#define task_fm_process             dummy_void
#endif

#if TASK_SPEAKER_EN
void task_speaker_process(void);
#else
#define task_speaker_process        dummy_void
#endif

#if USB_MIC_SUPPORT
void task_usbmic_process_fix(void);
#else
#define task_usbmic_process_fix     dummy_void
#endif



#pragma constseg="USER_AMUX_CONST"
IAR_CONST type_auadc_ctl auadc_ch_sel[4] =
{
	//adc通道,        采样率,        滤波器系数,          buf地址,           样点数,         处理函数
	{AUADC_LINEIN,    AUADC_SPR_32K, AUADC_FS128K_FP12K, (ADC_BUF_ADDR>>7), AUX_ADC_SAMPLE, task_aux_process},        //LINEIN
	{AUADC_CH_AUX_LR, AUADC_SPR_32K, AUADC_FS128K_FP12K, (ADC_BUF_ADDR>>7), 0x0078,         task_fm_process},         //FM
	{AUADC_CH_MIC,    AUADC_SPR_16K, AUADC_FS128K_FP5750, (ADC_BUF_ADDR>>7), MIX_ADC_SAMPLE, task_speaker_process},   //SPEAKER
	{AUADC_CH_MIC,    AUADC_SPR_32K, AUADC_FS128K_FP12K, (ADC_BUF_ADDR>>7), 0x02ff,         task_usbmic_process_fix}, //USBMIC
};

IAR_CONST u16 tbl_energy_music[12] =
{
	0x0300, 0x0600, 0x0900, 0x0c00,  0xf00, 0x1200, 0x1500, 0x1800,
	0x1b00, 0x1e00, 0x2100, 0xffff,
};

IAR_CONST u16 tbl_energy_amux[12] =
{
	0x100, 0x200, 0x300, 0x400, 0x500, 0x600, 0x700, 0x800,
	0x900, 0xa00, 0xb00, 0xffff,
};
#pragma constseg=default

#ifdef AX207X_TAG
#if (TASK_SPEAKER_EN || USB_MIC_SUPPORT || AUX_EQ_EN || ENERGY_DISP_EN)
//audio_adc中断
#pragma vector = VECTOR_OFFSET(AUDIO_ADC_INT)
#pragma register_bank=1
#pragma location="INTERRUPT"
__interrupt void auadc_isr(void)
{
	ISR_ENTER();

#if ENERGY_DISP_EN
	energy_calc_process();
#endif
	(*auadc_ctl.auadc_process)();

	ISR_EXIT();
}
#endif
#endif

//此函数是为了解决FM/Linein音量为0时仍有微弱声音的问题
#pragma location="USER_AMUX_SEG"
void amux_volume_set(void)
{
	if((task_ctl.work_sta == TASK_AUX)
#if AMUX_FM_EN
	        || (task_ctl.work_sta == TASK_FM)
#endif
	  )
	{
		if(sys_ctl.volume)
		{
#if !AUX_EQ_EN
			AMUX_AIN_EN();
#endif
#if AMUX_FM_EN
			if(task_ctl.work_sta == TASK_FM)
			{
				fm_rx_set_vol(VOLUME_MAX);
			}
#endif
		}
		else
		{
			AMUX_AIN_DIS();
#if AMUX_FM_EN
			if(task_ctl.work_sta == TASK_FM)
			{
				fm_rx_set_vol(0);
			}
#endif
		}
	}
}

//打开MIC
#pragma location="USER_AMUX_SEG"
void mic_enable(void)
{
#ifdef AX207X_TAG
	AIPCON10 |= BIT(5);             //enable the switch to output MCOx
	AIPCON10 &= ~(BIT(3) | BIT(4)); //BIT3-disable mute the MIC Pre-Amp; BIT4-disable mute the MIC LPF
	AIPCON10 &= ~(BIT(0) | BIT(1)); //disable the R/L channel LPF for Mixed signal from DAC
	AUADCCON0 = 0x02;               //enable MIC analog block

#if KARAOKE_EN
	AIPCON11 = (AIPCON11 & 0xF0) | USER_MIC_GAIN;  //MIC gain control
#endif

#else //AX107X
#if KARAOKE_EN
	DACACON6 = (DACACON6 & 0x83) | (USER_MIC_GAIN <<2);
#endif
	DACACON6 &= ~BIT(7);
	DACACON6 |= BIT(1);            //enable MIC
#endif
}

//关闭MIC
#pragma location="USER_AMUX_SEG"
void mic_disable(void)
{
#ifdef AX207X_TAG
	AIPCON10 &= ~BIT(5);
	AIPCON10 |= BIT(3) | BIT(4);    //BIT3-mute the MIC Pre-Amp; BIT4-mute the MIC LPF
	AUADCCON0 = 0x00;               //disable MIC analog block
#else
	DACACON6 &= ~BIT(1);            //disable MIC
#endif

}

#if ENERGY_DISP_EN
//能量显示中断过程
#pragma location="USER_AMUX_SEG"
void energy_calc_process(void)
{
	if(AUADCCON6 & BIT(0))
	{
		AUADCCON6 &= ~BIT(0);    // clear energy pending
		energy_flag = 1;
	}
}

//音乐播放下能量显示初始化
#pragma location="USER_AMUX_SEG"
void energy_music_init(void)
{
	AIPCON10 |= (BIT(0) | BIT(1));      //enable L & R LPF
	AIPCON10 &= ~BIT(2);                //use aout L/R as input
	AIPCON10 &= ~BIT(4);                //open LPF
	energy_auadc_init();
}

//能量显示初始化
#pragma location="USER_AMUX_SEG"
void energy_init(void)
{
	AUADCCON5 |= 0x02;   //energy filter 128k,5750Hz
	switch(task_ctl.work_sta)
	{
	case TASK_MUSIC:
		energy_music_init();
		break;
#if USB_SPK_SUPPORT
	case TASK_USBDEV:
		energy_music_init();
		break;
#endif
	case TASK_AUX:
#if AUX_EQ_EN
		AUADCCON2 |= 0x03;
#else
		energy_auadc_init();
#endif
		break;
	case TASK_FM:
		energy_auadc_init();
		break;
	case TASK_SPEAKER:
		AUADCCON2 |= 0x01;
		break;
	}
}

//开始能量计算
#pragma location="USER_AMUX_SEG"
void energy_start(void)
{
	energy_flag = 0;
	ocx.energy_cnt = 0xff;
	ocx.disp_energy = 1;
	AUADCCON6 &= ~BIT(0);        //clear energy pending
	AUADCCON2 |= 0x08;           //energy calc interrupt
	IE1 |= BIT(7);               //audio_adc interrupt
	AUADCCON0 |= 0x03;           //analog & digital block
}

//关闭能量显示
#pragma location="USER_AMUX_SEG"
void energy_close(void)
{
	AUADCCON2 &= ~BIT(3);       //disable energy calc interrupt
	energy_flag = 0;
	ocx.disp_energy = 0;
}

//关闭ADC模块和能量显示
#pragma location="USER_AMUX_SEG"
void energy_stop(void)
{
	energy_close();
	AUADCCON2 &= ~(BIT(0) | BIT(1));
	auadc_stop();
}

#pragma location="USER_AMUX_SEG"
void energy_calc(const u16 __code *tbl)
{
	u16 calc_energy=0;

	if(!energy_flag)
	{
		return;
	}
	energy_flag = 0;
	calc_energy  = ENGYDISPDATH<<8;
	calc_energy |= ENGYDISPDATL;
	energy_cnt = 0;
	while(tbl[energy_cnt] < calc_energy)
	{
		energy_cnt++;
	}
}
#endif
