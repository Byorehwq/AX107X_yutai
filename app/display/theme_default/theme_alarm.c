/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_idle.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ALARM任务显示主题
 *****************************************************************************/
#include "include.h"

#pragma location="DISP_ALARM_SEG"
void task_alarm_display(void)
{
	ocx_event();

	ocx_blink_set(0xff);
	ocx_time_set(alarm_date.hour,alarm_date.minute);

	if(disp.update == 0)
	{
		return;
	}
	disp.update = 0;

	disp_flag(0);
	disp_flag(LED_5C7S_DDOT);
	disp_rtc_blink(1, 5);
}

