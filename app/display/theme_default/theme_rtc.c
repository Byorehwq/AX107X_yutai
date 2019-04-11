/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_idle.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : RTC任务显示主题
 *****************************************************************************/
#include "include.h"

#pragma location="DISP_RTC_SEG"
void task_rtc_display(void)
{
	ocx_event();
	ocx_time_set(t_time.hour, t_time.minute);

	if(disp.update == 0)
	{
		return;
	}
	disp.update = 0;

	disp_time();
}


