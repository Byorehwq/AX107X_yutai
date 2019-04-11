#include "include.h"
#include "led_5c7s.h"
#include "led_7p7s.h"
#include "Display.h"



IAR_IDATA_A type_ocx_ctl ocx;
IAR_BDATA_A type_disp_flag disp;



#pragma constseg="LED_DISP_TBL"
IAR_CONST char led_eq_str[6][5] =
{
/*
	" nor",
	" pop",
	" roc",
	" jaz",
	" cla",
	" cou",
*/
	" 0 ",
	" 1 ",
	" 2 ",
	" 3 ",
	" 4 ",
	" 5 ",
};

IAR_CONST char led_playmode_str[4][5] =
{
	"all",
	"rdo",
	"one",
	"fod",
};
#pragma constseg=default

//���ֿ�
#pragma location="LED_DISP_SEG"
void ocx_shownum(u16 number)
{
	ocx.number = number;
	ocx.status = OCX_NUMBOX;
	ocx.show_time = 300;
	disp.update = 1;
}



//�豸�ַ�
#pragma location="LED_DISP_SEG"
void ocx_show_device(void)
{
	if(fs_cur_dev() == DEVICE_SDMMC)
	{
#if DISP_Show_Device
		ocx_msgbox("-2-");
#endif
	}
	else if(fs_cur_dev() == DEVICE_SDMMC1)
	{	// ���� TF�� ������ Flash
#if DISP_Show_Device
		ocx_msgbox("-1-");
#endif
	}
	else
	{
		ocx_msgbox("---");
	}

}


//�����
#pragma location="LED_DISP_SEG"
void ocx_shownumsel(u8 number)
{
#if USE_IR_NUM
	if(ocx.status != OCX_NUMSELBOX || ocx.number > 999)
	{
		ocx.number = number;
		ocx.status = OCX_NUMSELBOX;
	}
	else
	{
		ocx.number = (ocx.number * 10) + number;
	}
	ocx.show_time = 300;
	disp.update = 1;
#endif
}

//������
#pragma location="LED_DISP_SEG"
void ocx_showvol(void)
{
	ocx.status = OCX_VOLBOX;
	ocx.show_time = 300;
	disp.update = 1;
}

//��Ϣ��
#pragma location="LED_DISP_SEG"
void ocx_msgbox(const char __code *str)
{
	disp_str(str);
	ocx.status = OCX_MSGBOX;
	ocx.show_time = 300;
	disp.update = 1;
}

//����ģʽ��
#pragma location="LED_DISP_SEG"
void ocx_show_playmode(void)
{
	ocx_msgbox(led_playmode_str[sys_ctl.mode_play]);
}

//EQ��
#pragma location="LED_DISP_SEG"
void ocx_showeq(void)
{
	ocx_msgbox(led_eq_str[sys_ctl.eq_num]);
}

//FMƵ����
#pragma location="LED_DISP_SEG"
void ocx_showch(u8 ch)
{
	disp_channel(ch);
	ocx.status = OCX_MSGBOX;
	ocx.show_time = 200;
	disp.update = 1;
}

//Ƶ������
#pragma location="DISP_OCX_SEG"
void ocx_freq_set(u16 freq)
{
	if(ocx.disp_status != OCX_NULL)
	{
		return;
	}
	if(ocx.freq == freq)
	{
		return;
	}
	ocx.freq = freq;
	disp.update = 1;            //���ø��±�־
}

//ʱ����ʾ����
#pragma location="DISP_OCX_SEG"
void ocx_time_set(u8 timeh, u8 timel)
{
#if (DISP_TYPE != DISP_LED)
	if(ocx.disp_status != OCX_NULL)
	{
		return;
	}
	if(ocx.timeh == timeh && ocx.timel == timel)
	{
		return;
	}
	ocx.timeh = timeh;
	ocx.timel = timel;
	disp.update = 1;            //���ø��±�־
#endif
}

//������ʾ����
#if ENERGY_DISP_EN
#pragma location="DISP_OCX_SEG"
void ocx_energy_set(const u16 __code *tbl)
{
	energy_calc(tbl);
	if(ocx.energy_cnt == energy_cnt)
	{
		return;
	}
	ocx.energy_cnt = energy_cnt;
	disp.update = 1;            //���ø��±�־
}
#endif

//��˸����
#pragma location="DISP_OCX_SEG"
void ocx_blink_set(u8 icon, u8 num)
{
#if (DISP_TYPE != DISP_LED)
	ocx.blink_icon = icon;
	if(ocx.blink_num != num)
	{
		ocx.blink_num = num;
		disp_time();
	}
	disp.update = 1;            //���ø��±�־
#endif
}

//�ؼ���ʱ��5ms����һ��
#pragma location="DISP_OCX_SEG"
void ocx_delay(void)
{
#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
	if(ocx.show_time != 0)
	{
		ocx.show_time--;
	}

	if(ocx.blink_time != 0)
	{
		ocx.blink_time--;
		if(ocx.blink_time == BLINK_TIME)
		{
			ocx.blink_sta = 0;
		}
	}
	else
	{
		ocx.blink_time = (BLINK_TIME * 2);
		ocx.blink_sta = 1;
	}
#endif

#if ((DISP_TYPE == DISP_LED) && (USE_IR_NUM))
	if(ocx.show_time != 0)
	{
		ocx.show_time--;
	}
#endif
}

#pragma location="DISP_OCX_SEG"
void ocx_clear(void)
{
#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
	ocx.blink_sta = 0;
	ocx.blink_icon = 0;
	ocx.blink_num = 0;
	ocx.status = OCX_NULL;
	disp.update = 1;    //���ø��±�־
#endif

#if ((DISP_TYPE == DISP_LED) && (USE_IR_NUM))
	ocx.status = OCX_NULL;
#endif
}

//�ؼ��¼�
#pragma location="DISP_OCX_SEG"
void ocx_event(void)
{
#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
	if(ocx.show_time == 0)
	{
		if(ocx.status == OCX_NUMSELBOX)
		{
			ocx.event = 1;
		}
		ocx.status = OCX_NULL;
	}
	if(ocx.status != ocx.disp_status)
	{
		ocx.disp_status = ocx.status;
		disp.update = 1;            //���ø��±�־
	}
#endif

#if ((DISP_TYPE == DISP_LED) && (USE_IR_NUM))
	if(ocx.show_time == 0)
	{
		if(ocx.status == OCX_NUMSELBOX)
		{
			ocx.event = 1;
		}
		ocx.status = OCX_NULL;
	}
#endif
}

//��ʾ�ؼ�
#pragma location="DISP_OCX_SEG"
void ocx_display(void)
{
	switch(ocx.disp_status)
	{
		//case OCX_MSGBOX:
		//    break;
	case OCX_VOLBOX:
		disp_vol();
		break;
	}
}

//��ʾ��ʼ��
#pragma location="DISP_OCX_SEG"
void disp_init(void)
{
	disp_port_init();
#if SOFT_POWEROFF_EN
	disp_clear();
#else
	showhello();
#endif
}
