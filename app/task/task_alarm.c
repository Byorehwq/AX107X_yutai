/*****************************************************************************
 * Module    : Task
 * File      : task_alarm.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ������������
 *****************************************************************************/
#include "include.h"

#if ALARM_EN

type_task_alarm t_alarm;

#ifdef AX207X_TAG

//����PCM����,���Ҳ���4����. 16bit, С��(��8bit��ǰ),��������,��������
#pragma constseg="TASK_ALARM_CONST"
IAR_CONST u8 alarm_pcm_buf[16] =
{
	0x00, 0x00, 0x00, 0x00, 0xDA, 0xBF, 0xDA, 0xBF,
	0x00, 0x00, 0x00, 0x00, 0x26, 0x40, 0x26, 0x40
};
#pragma constseg=default

#else
//AX107X�ϣ����buf������mrom
extern IAR_CONST u8 alarm_pcm_buf[16];

#endif

//����alarm����
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

//alarm�����˳�
#pragma location="TASK_ALARM_SEG"
static void task_alarm_exit(void)
{
	dac_clr_buf();
	sys_ctl.volume = t_alarm.vol_tmp;
	user_set_volume(sys_ctl.volume);
#if DAC_FADE_EN
	dac_fade_out();
#endif
	RTC_ALARM_EN();//���´�����
}

//alarm�����¼��������巽ʽ��:����4��,��ͣ4��,�ٳ�ͣ4��
#pragma location="TASK_ALARM_SEG"
static void task_alarm_event(void)
{
	u8 i;
	WATCHDOG_CLR();
	if(t_alarm.alarm_cnt < 4)
	{
		t_alarm.delay_10ms_cnt = 0; //����ʱ��
		while(t_alarm.delay_10ms_cnt < ALARM_RING_CYCLE/10)
		{
			//ֱ��PCM��DAC��left_l,left_h,right_l,right_h
			for(i=0; i<16;)
			{
				while(!(AUCON7 & BIT(7)));
				AUCON5 = alarm_pcm_buf[i++];
				AUCON5 = alarm_pcm_buf[i++];
			}
		}

		t_alarm.delay_10ms_cnt = 0; //��ͣʱ��.
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
		t_alarm.delay_10ms_cnt = 0; //��ͣʱ��.
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

//alarm������Ϣ����
#pragma location="TASK_ALARM_SEG"
static void task_alarm_deal_msg(u8 msg)
{
	//�����ⰴ��ֹͣ����,����ԭ���Ĺ���ģʽ
	if(((msg&0xE0)==T_KEY_SHORT_UP) || ((msg&0xE0)==T_KEY_LONG_UP)\
	        || t_alarm.delay_1s_cnt >= 60)//60s
	{
		task_ctl.work_sta = task_ctl.last_work;
	}
}

//alarm����
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
