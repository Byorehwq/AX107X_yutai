/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_idle.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 关机任务显示主题
 *****************************************************************************/
#include "include.h"

#pragma location="DISP_POWEROFF_SEG"
void task_poweroff_display(void)
{
#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
	ocx_event();
	if(disp.update == 0)
	{
		return;
	}
	disp.update = 0;

	disp_clear();
#endif
}
