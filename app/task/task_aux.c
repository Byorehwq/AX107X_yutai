/*****************************************************************************
 * Module    : Task
 * File      : task_aux.c
 * Author    : jingfa
 * Email     :
 * Function  : AUX任务
 *****************************************************************************/
#include "include.h"

#if TASK_AUX_EN

#if AUX_EQ_EN
#pragma location="TASK_AUX_SEG"
void task_aux_eq_process(void)
{
	if(AUADCCON6 & BIT(3))
	{
		AUADCCON6 &= ~BIT(3);
		auxeq_dac_out(ADC_BUF_ADDR, AUX_EQ_SAMPLE/2);
	}
	if(AUADCCON6 & BIT(4))
	{
		AUADCCON6 &= ~BIT(4);
		auxeq_dac_out(ADC_BUF_ADDR+AUX_EQ_INPUT_ADDR, AUX_EQ_SAMPLE/2);
	}
}
#endif

//任务初始化
#pragma location="TASK_AUX_SEG"
static void task_aux_enter(void)
{
#if AUX_DET_EN
	if(!device_activate(DEVICE_LINEIN))
	{
		task_ctl.work_sta = TASK_EXIT;
		return;
	}
#endif

#if((defined AX107X_TAG) && (AMUX_LINEIN == AMUX_CH_MM))
	mic_enable();
	DACACON6 = (DACACON6 & 0x83) | (GAIN_0DB <<2);

#else

#if((defined AX107X_TAG) && ((AMUX_LINEIN == AMUX_CH_0M)||(AMUX_LINEIN == AMUX_CH_2M)||(AMUX_LINEIN == AMUX_CH_4M)))
	DACACON6 |= BIT(0);//打开MIC用作LINE功能
#endif

	amux_init(AMUX_LINEIN);                 //初始化LINEIN AMUX
#endif  //((defined AX107X_TAG) && (AMUX_LINEIN == AMUX_CH_MM))

#if AUX_EQ_EN
	AMUX_AIN_DIS();
	auadc_init(CH_SEL_LINEIN);              //初始化AUADC
	dac_out_init(DAC_SPA_S32K);
	memset((void *)ADC_BUF_ADDR, 0, 0x1600);
	auadc_start();
	mute_disable();
#if DAC_FADE_EN
	dac_fade_in(1);
#endif
#else  //AUX_EQ_EN
#if REC_AUX_EN
	record_init(CH_SEL_LINEIN);
#endif

	mute_disable();
#if DAC_FADE_EN
#if KEY_VOICE_EN
	dac_fade_in(1);
#else
	dac_fade_in(0);
#endif
#else
#if KEY_VOICE_EN
	b_dac.digvol_en = 1;
#else
	b_dac.digvol_en = 0;
#endif
	dac_set_volume_direct(sys_ctl.volume);
#endif
#if (!KEY_VOICE_EN)
	DAC_ANL_DIS();
#endif
#endif  //AUX_EQ_EN

#if ENERGY_DISP_EN
	energy_init();
#endif

#if (DISP_TYPE==DISP_LED)
	ocx.led_sta = LED_STA_ON;
#endif
}

//任务退出
#pragma location="TASK_AUX_SEG"
static void task_aux_exit(void)
{
#if DAC_FADE_EN
	dac_fade_out();
	dac_fade_wait();
#endif

#if ((!AUX_EQ_EN) && (!KEY_VOICE_EN))
	DAC_ANL_EN();
#endif

#if ENERGY_DISP_EN
	energy_stop();
#endif
#if AUX_EQ_EN
	auadc_stop();
#endif

#if((defined AX107X_TAG) && (AMUX_LINEIN == AMUX_CH_MM))
	mic_disable();
#else
#if((defined AX107X_TAG) && ((AMUX_LINEIN == AMUX_CH_0M)||(AMUX_LINEIN == AMUX_CH_2M)||(AMUX_LINEIN == AMUX_CH_4M)))
	DACACON6 &= ~BIT(0);//关闭MIC用作LINE功能
#endif
	amux_exit();
#endif
}

//任务事件
#pragma location="TASK_AUX_SEG"
static void task_aux_event(void)
{
	comm_event(1);
#if AUX_DET_EN
	if(!device_activate(DEVICE_LINEIN))
	{
		task_ctl.work_sta = task_ctl.pre_sta;
	}
#endif
}

//任务消息处理
#pragma location="TASK_AUX_SEG"
static void task_aux_deal_msg(u8 msg)
{
	switch(msg)
	{
	case KU_PLAY:
	case KU_PLAY_AND_MODE:
#if !AUX_EQ_EN
		if(b_dac.mute_en)
		{
			AMUX_AIN_EN();
		}
		else
		{
			AMUX_AIN_DIS();
		}
#endif
		user_set_mute();
		disp.update = 1;
		break;
#if AUX_EQ_EN
	case KU_EQ:
		user_set_eq(sys_ctl.eq_num + 1);
		showeq();
		break;
#endif
#if ENERGY_DISP_EN
	case KL_PLAYMODE:
		if(!ocx.disp_energy)
		{
			energy_start();
		}
		else
		{
			energy_close();
		}
		disp.update = 1;
		break;
#endif
	default:
		deal_msg(msg);
		break;
	}
}

//任务主流程
#pragma location="TASK_AUX_SEG"
void task_aux(void)
{
	printf("task_aux\n");
	task_aux_enter();
	while(task_ctl.work_sta == TASK_AUX)
	{
		task_aux_event();
		task_aux_deal_msg(get_msg());
		task_aux_display();
	}
	task_aux_exit();
}
#endif
