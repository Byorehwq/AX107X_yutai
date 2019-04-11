/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_idle.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 空闲任务显示主题
 *****************************************************************************/
#include "include.h"

#pragma location="DISP_IDLE_SEG"
void task_idle_display(void)
{
#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
	ocx_event();
	if(disp.update == 0)
	{
		return;
	}
	disp.update = 0;

	disp_str("idle");
#endif

   

}
