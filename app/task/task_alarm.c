/*****************************************************************************
 * Module    : Task
 * File      : task_alarm.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 闹钟任务流程
 *****************************************************************************/
#include "include.h"

#if ALARM_EN

type_task_alarm t_alarm;

#ifdef AX207X_TAG

//闹钟PCM数据,正弦波的4个点. 16bit, 小端(低8bit在前),先左声道,后右声道
#pragma constseg="TASK_ALARM_CONST"
IAR_CONST u8 alarm_pcm_buf[16] =
{
	0x00, 0x00, 0x00, 0x00, 0xDA, 0xBF, 0xDA, 0xBF,
	0x00, 0x00, 0x00, 0x00, 0x26, 0x40, 0x26, 0x40
};
#pragma constseg=default

#else
//AX107X上，这个buf移入了mrom
extern IAR_CONST u8 alarm_pcm_buf[16];

#endif

//进入alarm任务
#pragma location="TASK_ALARM_SEG"
static void task_alarm_enter(void)
{
	SELECT_IROM2();
	t_alarm.alarm_cnt = 0;
	t_alarm.delay_10ms_cnt = 0;
	t_alarm.delay_1s_cnt = 0;
	t_alarm.vol_tmp = sys_ctl.volume;
	dac_out_init(DAC_SPA_16K);
#if !DAC_FADE_EN
	b_dac.digvol_en = 1;
#endif
	user_set_volume(10);
	mute_disable();
#if DAC_FADE_EN
	dac_fade_in(0);
	dac_fade_wait();
#endif
	flush_msg();
}

//alarm任务退出
#pragma location="TASK_ALARM_SEG"
static void task_alarm_exit(void)
{
	dac_clr_buf();
	sys_ctl.volume = t_alarm.vol_tmp;
	user_set_volume(sys_ctl.volume);
#if DAC_FADE_EN
	dac_fade_out();
#endif
	RTC_ALARM_EN();//重新打开闹钟
}

//alarm任务事件处理，响铃方式是:短响4声,短停4声,再长停4声
#pragma location="TASK_ALARM_SEG"
static void task_alarm_event(void)
{
	u8 i;
	WATCHDOG_CLR();
	if(t_alarm.alarm_cnt < 4)
	{
		t_alarm.delay_10ms_cnt = 0; //短响时间
		while(t_alarm.delay_10ms_cnt < ALARM_RING_CYCLE/10)
		{
			//直推PCM到DAC，left_l,left_h,right_l,right_h
			for(i=0; i<16;)
			{
				while(!(AUCON7 & BIT(7)));
				AUCON5 = alarm_pcm_buf[i++];
				AUCON5 = alarm_pcm_buf[i++];
			}
		}

		t_alarm.delay_10ms_cnt = 0; //短停时间.
		while(t_alarm.delay_10ms_cnt < ALARM_SILENCE_CYCLE_A/10)
		{
			for(i=0; i<2; i++)
			{
				while(!(AUCON7 & BIT(7)));
				AUCON5 = 0;
				AUCON5 = 0;
			}
		}
		t_alarm.alarm_cnt++;
	}
	else
	{
		disp_clear();
		t_alarm.delay_10ms_cnt = 0; //长停时间.
		while(t_alarm.delay_10ms_cnt < ALARM_SILENCE_CYCLE_B/10)
		{
			for(i=0; i<2; i++)
			{
				while(!(AUCON7 & BIT(7)));
				AUCON5 = 0;
				AUCON5 = 0;
			}
		}

		sec2date(&t_time, irtc_read_date());
		t_alarm.alarm_cnt = 0;
	}
}

//alarm任务消息处理
#pragma location="TASK_ALARM_SEG"
static void task_alarm_deal_msg(u8 msg)
{
	//按任意按键停止闹钟,返回原来的工作模式
	if(((msg&0xE0)==T_KEY_SHORT_UP) || ((msg&0xE0)==T_KEY_LONG_UP)\
	        || t_alarm.delay_1s_cnt >= 60)//60s
	{
		task_ctl.work_sta = task_ctl.last_work;
	}
}

//alarm任务
#pragma location="TASK_ALARM_SEG"
void task_alarm(void)
{
	printf("alarm mode\n");
	task_alarm_enter();
	while(task_ctl.work_sta == TASK_ALARM)
	{
		task_alarm_event();
		task_alarm_deal_msg(get_msg());
		ocx_time_set(t_time.hour, t_time.minute);
		disp_time();
	}
	task_alarm_exit();
}

#pragma location="TASK_ALARM_SEG"
void task_alarm_check(void)
{
	if(IRTCON & BIT(5))
	{
		IRTCON &= ~BIT(5);
		RTC_ALARM_DIS();
		task_ctl.work_sta = TASK_ALARM;
	}
}

#endif
