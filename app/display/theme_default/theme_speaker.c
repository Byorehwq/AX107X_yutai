/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_idle.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 空闲任务显示主题
 *****************************************************************************/
#include "include.h"

#pragma location="DISP_SPEAKER_SEG"
void task_speaker_display(void)
{
#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
	ocx_event();
#if ENERGY_DISP_EN
	ocx_energy_set(tbl_energy_amux);
#endif

	if(disp.update == 0)
	{
		return;
	}
	disp.update = 0;

	//刷新显示界面
	if(ocx.disp_status == OCX_NULL)
	{
#if ENERGY_DISP_EN
		if(ocx.disp_energy)
		{
			disp_energy();
		}
		else
#endif
		{
			disp_str("o--o");
		}
	}
	else if(ocx.disp_status == OCX_NUMBOX)
	{
		disp_numsel(ocx.number);
	}
	else
	{
		ocx_display();
	}
#endif
}
