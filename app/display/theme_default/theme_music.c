/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_music.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ���ֲ���������ʾ����
 *****************************************************************************/
#include "include.h"
bool vol_less_34V_blink;
#pragma location="DISP_MUSIC_SEG"
void task_music_display(void)
{
#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
	ocx_event();
#if ENERGY_DISP_EN
	if(ocx.disp_energy)
	{
		ocx_energy_set(tbl_energy_music);
	}
	else
#endif
	{
		ocx_time_set(u_msc.cur_time.min, u_msc.cur_time.sec);
	}

	if(!disp.update)
	{
		return;                     //����Ҫ������ʾ
	}
	disp.update = 0;

	//ˢ����ʾ����
	switch(ocx.disp_status)
	{
	case OCX_NULL:
//#if MUSIC_STOP
//		if(b_msc.stop)
//		{
//			showstop();
//		}
//		else
//#endif
//		{
//#if ENERGY_DISP_EN
//			if(ocx.disp_energy)
//			{
//				disp_energy();
//			}
//			else
//#endif
//			{
				//
				//disp_numsel_icon();
			if(vol_less_34V_blink == 0)
			{
				if(b_msc.pause == 0){

					if(task_ctl.work_sta == TASK_FLASH)
						disp_numsel(u_msc.music_num+1); // � SPI-Flashʱ,	��ʾֵҪ +1
					else
						disp_numsel(u_msc.music_num);
				}
				else
				{
					showpause();
				}
			}
			else
			{
					disp_clear();
			}
			break;
		case OCX_NUMBOX:
#if USE_IR_NUM
		case OCX_NUMSELBOX:
#endif	
			disp_numsel(ocx.number);
			disp_numsel_icon();
			break;
		default:
			ocx_display();
			break;
	//	}
	}
#endif

#if ((DISP_TYPE == DISP_LED) && (USE_IR_NUM))
	ocx_event();
#endif
}
