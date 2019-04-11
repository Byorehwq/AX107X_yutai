/*****************************************************************************
 * Module    : Theme
 * File      : led_default_tomcat.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 汤姆猫任务显示主题
 *****************************************************************************/
#include "include.h"


#pragma location="DISP_FM_SEG"
void task_fm_display(void)
{
#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
	ocx_event();

	ocx_freq_set(fm_ctl.fm_freq);
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
			disp_freq();
		}
	}
#if USE_IR_NUM
	else if(ocx.disp_status == OCX_NUMSELBOX)
	{
		disp_numsel(ocx.number);
	}
#endif
	else
	{
		ocx_display();
	}
#endif

#if ((DISP_TYPE == DISP_LED) && (USE_IR_NUM))
	ocx_event();
#endif
}
