/*****************************************************************************
 * Module    : Task
 * File      : task_speaker.c
 * Author    : Shicheng
 * Email     :
 * Function  : 扩音器任务流程
 *****************************************************************************/
#include "include.h"

#if TASK_SPEAKER_EN

IAR_XDATA_A u16 echo_input                @"ECHO_XDATA";
IAR_XDATA_A u16 echo_read                 @"ECHO_XDATA";
IAR_XDATA_A u16 echo_write                @"ECHO_XDATA";
IAR_XDATA_A u8  echo_factor[2]            @"ECHO_XDATA";

#pragma constseg="TASK_SPEAKER_CONST"
IAR_CONST u8 tbl_micgain[ECHO_MAX+1] =
{
	GAIN_N3DB,
	GAIN_9DB,
	GAIN_12DB,
	GAIN_15DB,
	GAIN_18DB,
	GAIN_21DB,
	GAIN_24DB,
	GAIN_27DB,
	GAIN_30DB,
	GAIN_33DB,
	GAIN_36DB,
};
#pragma constseg=default


#pragma location="TASK_SPEAKER_SEG"
static void buf_loop(void)
{
	if(echo_read == ECHO_BUF_END_0)
		echo_read = ECHO_BUF_START_1;
	else if(echo_read == ECHO_BUF_END_1)
		echo_read = ECHO_BUF_START_0;

	if(echo_write == ECHO_BUF_END_0)
		echo_write = ECHO_BUF_START_1;
	else if(echo_write == ECHO_BUF_END_1)
		echo_write = ECHO_BUF_START_0;
}

#pragma location="TASK_SPEAKER_SEG"
static void mix_step_set(u8 step)
{
#ifdef AX207X_TAG
	IE1 &= ~BIT(7);
	if(step)
	{
		if(step == ECHO_MAX)
		{
			echo_write = echo_read;
		}
		else
		{
#if 1
			echo_read = ECHO_BUF_START_0;
			echo_write = ECHO_BUF_START_1 + (ECHO_STEP*step);
#else
			echo_write = echo_read + ECHO_BUF_END_0 - ECHO_BUF_START_0 + (ECHO_STEP*step);
			if((echo_read >= ECHO_BUF_START_1))
			{
				if(echo_write >= ECHO_BUF_END_1)
				{
					echo_write = echo_write - ECHO_BUF_END_1 + ECHO_BUF_START_0;
					if(echo_write >= ECHO_BUF_END_0)
					{
						echo_write = echo_write - ECHO_BUF_END_0 + ECHO_BUF_START_1;
					}
				}
			}
			else if(echo_read <= ECHO_BUF_END_0)
			{
				if(echo_write >= ECHO_BUF_END_0)
				{
					echo_write = echo_write - ECHO_BUF_END_0 + ECHO_BUF_START_1;
					if(echo_write >= ECHO_BUF_END_1)
					{
						echo_write = echo_write - ECHO_BUF_END_1 + ECHO_BUF_START_0;
					}
				}
			}
#endif
		}
		echo_factor[0] = 90;
		echo_factor[1] = 48;
	}
	else
	{
		echo_factor[0] = 108;
		echo_factor[1] = 0;
	}
	IE1 |= BIT(7);

#else  //AX107X_TAG
	DACACON6 = (DACACON6 & 0x83) | (tbl_micgain[step]<<2);
#endif
}

#pragma location="TASK_SPEAKER_SEG"
void mix_proc(u16 samples, u8 type)
{
#if LIMIT_MIX_NOISE_EN
	s8 max, min;
	max = lim_val((s16 *)echo_input, samples, 1);
	min = lim_val((s16 *)echo_input, samples, 0);

	if((max - min) < LIMIT_VALUE)
	{
		memset((void *)echo_input, 0, samples*2);
	}
#endif

#if MIX_DEEP_EN
	mix_proc_deep(ECHO_SAMPLE);
#else
	mix_proc_fix(ECHO_SAMPLE, 0);
#endif
}

#pragma location="TASK_SPEAKER_SEG"
void task_speaker_process(void)
{
	if(AUADCCON6 & BIT(1))
	{
		AUADCCON6 &= ~BIT(1);
		echo_input = ADC_BUF_ADDR;
		mix_proc(ECHO_SAMPLE, 0);
		buf_loop();
	}
	if(AUADCCON6 & BIT(2))
	{
		AUADCCON6 &= ~BIT(2);
		mix_proc(ECHO_SAMPLE, 0);
		buf_loop();
	}
}

#pragma location="TASK_SPEAKER_SEG"
static void task_speaker_deal_msg(u8 msg)
{
	u8 mix_step = sys_ctl.mix_step;

	switch(msg)
	{
	case K_PREV:
	case KH_PREV:
		mix_step -= 2;
	case K_NEXT:
	case KH_NEXT:
		mix_step++;
		if(mix_step <= ECHO_MAX)
		{
			sys_ctl.mix_step = mix_step;
		}
		shownum(sys_ctl.mix_step);
		mix_step_set(sys_ctl.mix_step);
		break;

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

/*****************************************************************************
 * Module    : 扩音器任务的公共消息与事件
 *****************************************************************************/
#pragma location="TASK_SPEAKER_SEG"
static void task_speaker_event(void)
{
	comm_event(1);
#if MIC_DET_EN
	if(!device_activate(DEVICE_MIC))
	{
		task_ctl.work_sta = task_ctl.pre_sta;
	}
#endif
}

/*****************************************************************************
 * Module    : 扩音器任务入口
 *****************************************************************************/
//任务初始化
#pragma location="TASK_SPEAKER_SEG"
static void task_speaker_enter(void)
{
	//【注意】扩音器模式AX207X支持混响效果；AX107X不支持混响效果；
#if MIC_DET_EN
	if(!device_activate(DEVICE_MIC))
	{
		task_ctl.work_sta = TASK_EXIT;
		return;
	}
#endif
#ifdef AX207X_TAG
	amux_mic_init(GAIN_27DB);               //初始化MIC的AMUX，参数为MIC的增益
	auadc_init(CH_SEL_SPK);                 //初始化ADC
#else
	mic_enable();
#endif
#if ENERGY_DISP_EN
	energy_init();
#endif
#ifdef AX207X_TAG
	dac_out_init(DAC_SPA_S16K);
	echo_read  = ECHO_BUF_START_0;          //初始化必须要配置，否则0级会死机
	echo_write = ECHO_BUF_START_0;          //初始化必须要配置，否则0级会死机
#endif
	mix_step_set(sys_ctl.mix_step);
#ifdef AX207X_TAG
	memset((void *)ADC_BUF_ADDR, 0, 0x1600);
	memset((void *)ECHO_BUF_START_0, 0, 0x0800);
	auadc_start();
#endif
	mute_disable();
#if DAC_FADE_EN
	dac_fade_in(1);
#endif
#if (DISP_TYPE==DISP_LED)
	ocx.led_sta = LED_STA_ON;
#endif
}

//任务退出
#pragma location="TASK_SPEAKER_SEG"
static void task_speaker_exit(void)
{
#if DAC_FADE_EN
	dac_fade_out();
	dac_fade_wait();
#endif

#if ENERGY_DISP_EN
	energy_stop();
#endif
#ifdef AX207X_TAG
	auadc_stop();
	amux_exit();
#else
	mic_disable();
#endif
}

//任务主流程
#pragma location="TASK_SPEAKER_SEG"
void task_speaker(void)
{
	printf("task_spaker\n");
	task_speaker_enter();
	while(task_ctl.work_sta == TASK_SPEAKER)
	{
		task_speaker_event();
		task_speaker_deal_msg(get_msg());
		task_speaker_display();
	}
	task_speaker_exit();
}

#endif
