/*****************************************************************************
 * Module    : Task
 * File      : task_rtc.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 时钟任务流程
 *****************************************************************************/
#include "include.h"

#if TASK_RTC_EN

#define DBG_RTC(...)        //printf(__VA_ARGS__)

type_task_rtc __idata t_rtc;
void task_rtc_display(void);

//时钟和闹钟之间切换
#pragma location="TASK_RTC_SEG"
void task_rtc_select(u8 select)
{
	t_rtc.select = select;

#if ALARM_EN
	if(t_rtc.select)
	{
		ocx_blink_set(0, 0);
		sec2date(&t_time, irtc_read_alarm());
		DBG_RTC("alarm: ");
	}
	else
	{
		ocx_blink_set(ICON_DOT, 0);
		sec2date(&t_time, irtc_read_date());
		DBG_RTC("time : ");
	}
#else
	ocx_blink_set(ICON_DOT, 0);
	sec2date(&t_time, irtc_read_date());
	DBG_RTC("time : ");
#endif
#if RTC_DATE_EN
	DBG_RTC("%02d-%02d-%02d\n", t_time.year, t_time.month, t_time.day);
#endif
	DBG_RTC("%02d:%02d:%02d\n", t_time.hour, t_time.minute, t_time.second);
}

//调整时间
#pragma location="TASK_RTC_SEG"
void task_rtc_set_time(u8 inc)
{
	if(t_rtc.index&0x01)
	{
		deal_hour(inc);
	}
	else
	{
		deal_minute(inc);
	}
}

#if ALARM_EN && RTC_DATE_EN
//闹钟自动校准响铃日期时间
#pragma location="TASK_RTC_SEG"
void task_alarm_match_time(bool mode)
{
	u32 alarm_total_seconds_of_a_day = irtc_read_alarm()%SECONDS_OF_A_DAY;
	sec2date(&t_time, irtc_read_date());//把闹钟日期与时钟日期同步
	if(mode && (alarm_total_seconds_of_a_day <= (irtc_read_date()%SECONDS_OF_A_DAY))) //当闹钟时间不大于时钟时间时闹钟时间自加一
	{
		t_time.day += 1;
	}
	sec2time(&t_time, alarm_total_seconds_of_a_day);//把闹钟时间更改回去
	irtc_write_alarm(date2sec(&t_time));
}
#endif

//任务初始化
#pragma location="TASK_RTC_SEG"
static void task_rtc_enter(void)
{
	SELECT_IROM2();
	smemset_i(&t_rtc, 0x00, sizeof(t_rtc));
	task_rtc_select(0);

#if (DISP_TYPE==DISP_LED)
	ocx.led_sta = LED_STA_ON;
#endif

#if KEY_VOICE_EN
	AMUX_AIN_EN();
	mute_disable();
#endif
}

//任务退出
#pragma location="TASK_RTC_SEG"
static void task_rtc_exit(void)
{
}

//任务事件处理
#pragma location="TASK_RTC_SEG"
static void task_rtc_event(void)
{
	comm_event(1);

#if USE_IR_NUM
	if(ocx.event == 1)
	{
		ocx.event = 0;
		if(t_rtc.setting)
		{
			if(t_rtc.index)
			{
				if(ocx.number <= 23)
				{
					t_time.hour = ocx.number;
				}
			}
			else
			{
				if(ocx.number <= 59)
				{
					t_time.minute = ocx.number;
				}
			}
			irtc_write_date(date2sec(&t_time));
		}
	}
#endif
}

//任务消息处理
#pragma location="TASK_RTC_SEG"
static void task_rtc_deal_msg(u8 msg)
{
	switch(msg)
	{
	case QSYSTEM_500MS:
		if((t_rtc.select | t_rtc.setting) == 0)
		{
			if(IRTCON & BIT(3))
			{
				IRTCON &= ~BIT(3);
#if !ALARM_EN
				sec2date(&t_time, irtc_read_date());
#else
#if RTC_DATE_EN
				if(irtc_read_date()%SECONDS_OF_A_DAY == 0) //新一天的0点0分0秒
				{
					task_alarm_match_time(0);
				}
				sec2date(&t_time, irtc_read_date());
#else
				sec2date(&t_time, irtc_read_date());
				if(irtc_read_date() > SECONDS_OF_A_DAY) //时钟时间始终保持小于一天
				{
					irtc_write_date(date2sec(&t_time));
				}
#endif
#endif
				DBG_RTC("QSYSTEM_500MS:");
				DBG_RTC("%02d:%02d:%02d\n", t_time.hour, t_time.minute, t_time.second);
			}
#if ALARM_EN
			task_alarm_check();
#endif
		}
		break;

	case KU_NEXT:
#if USE_IR_NUM
		if(ocx.status == OCX_NUMSELBOX)
		{
			break;
		}
#endif
		if(t_rtc.setting)
		{
			t_rtc.index++;
			ocx_blink_set(0, (t_rtc.index&0x01)? 0x0c : 0x03);
		}
#if ALARM_EN
		else
		{
			task_rtc_select(!t_rtc.select);
		}
#endif
		break;

	case KU_PREV:
#if USE_IR_NUM
		if(ocx.status == OCX_NUMSELBOX)
		{
			break;
		}
#endif
		if(t_rtc.setting)
		{
			t_rtc.index--;
			ocx_blink_set(0, (t_rtc.index&0x01)? 0x0c : 0x03);
		}
#if ALARM_EN
		else
		{
			task_rtc_select(!t_rtc.select);
		}
#endif
		break;

	case KU_PLAY:
		t_rtc.setting = !t_rtc.setting;
		if(t_rtc.setting)
		{
			DBG_RTC("setting\n");
			t_rtc.index = 0;
			ocx_blink_set(0, 0x03);
#if ALARM_EN
			if(t_rtc.select)
			{
				RTC_ALARM_EN();
#if RTC_DATE_EN
				task_alarm_match_time(1);
#endif
			}
#endif
		}
		else
		{
			DBG_RTC("cancel\n");
			task_rtc_select(t_rtc.select);
		}
		break;

	case KL_PLAY:
		if(t_rtc.setting)
		{
			DBG_RTC("ok\n");
			t_rtc.setting = 0;
#if ALARM_EN
			if(t_rtc.select)
			{
				irtc_write_alarm(date2sec(&t_time));
#if RTC_DATE_EN
				task_alarm_match_time(1);
#endif
			}
			else
			{
				irtc_write_date(date2sec(&t_time));
			}
#else
			irtc_write_date(date2sec(&t_time));
#endif
			task_rtc_select(0);
		}
		break;

	case KU_VOL_UP:
	case KH_VOL_UP:
		if(t_rtc.setting)
		{
			task_rtc_set_time(1);
		}
		break;

	case KU_VOL_DOWN:
	case KH_VOL_DOWN:
		if(t_rtc.setting)
		{
			task_rtc_set_time(0);
		}
		break;

#if USE_IR_NUM
	case T_KEY_NUM_0:
	case T_KEY_NUM_1:
	case T_KEY_NUM_2:
	case T_KEY_NUM_3:
	case T_KEY_NUM_4:
	case T_KEY_NUM_5:
	case T_KEY_NUM_6:
	case T_KEY_NUM_7:
	case T_KEY_NUM_8:
	case T_KEY_NUM_9:
		shownumsel(msg - T_KEY_NUM_0);
		if(ocx.number > 99)
		{
			ocx.show_time = 0;
		}
		break;
#endif

	default:
		deal_msg(msg);
		break;
	}
}

//任务主流程
#pragma location="TASK_RTC_SEG"
void task_rtc(void)
{
	printf("task_rtc\n");
	task_rtc_enter();
	while(task_ctl.work_sta == TASK_RTC)
	{
		task_rtc_event();
		task_rtc_deal_msg(get_msg());
		task_rtc_display();
		//PCON0 |= BIT(2);			//进入IDLE
		//asm("nop");asm("nop");asm("nop");
	}
	task_rtc_exit();
}

#endif
