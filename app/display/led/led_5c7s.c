/*--------------------------------------------------------------------------*/
/**@file led_5c7s.C
   @brief LED各种显示函数
   @details LED显示中使用的函数
   @author LZZ
   @date   2010-08-17
   @note   none
*/
/*----------------------------------------------------------------------------*/
#include "include.h"


#if (DISP_TYPE==DISP_LED_5C7S)

IAR_DATA_A u8 dispbuf[5];

#pragma constseg="LED_DISP_TBL"
IAR_CONST u8 tbl_led_num[10] =
{
	LED_5C7S_0, LED_5C7S_1, LED_5C7S_2, LED_5C7S_3, LED_5C7S_4,
	LED_5C7S_5, LED_5C7S_6, LED_5C7S_7, LED_5C7S_8, LED_5C7S_9,
};

IAR_CONST u8 tbl_led_alpha[26] =
{
	LED_5C7S_A, LED_5C7S_b, LED_5C7S_C, LED_5C7S_d, LED_5C7S_E,
	LED_5C7S_F, LED_5C7S_g, LED_5C7S_H, LED_5C7S_I, LED_5C7S_J,
	LED_5C7S_K, LED_5C7S_L, LED_5C7S_m, LED_5C7S_n, LED_5C7S_O,
	LED_5C7S_P, LED_5C7S_q, LED_5C7S_r, LED_5C7S_S, LED_5C7S_t,
	LED_5C7S_U, LED_5C7S_V, LED_5C7S_w, LED_5C7S_X, LED_5C7S_y,
	LED_5C7S_Z
};

IAR_CONST u8 tbl_led_energy[3] =
{
	(BIT_E | BIT_F), LED_5C7S_C, LED_5C7S_0,
};
#pragma constseg=default


#pragma location="LED_DISP_SEG"
//显示 -, 0~9, a~z的字符串(常量)
void led_5c7s_disp_str(const char __code *str)
{
	u8 i;
	u8 ch;

	for(i=0; i!=3; i++)
	{
		ch = str[i];
		if(ch >= '0' && ch <= '9')
		{
			dispbuf[i] = tbl_led_num[ch - '0'];
		}
		else if(ch >= 'A' && ch <= 'Z')
		{
			dispbuf[i] = tbl_led_alpha[ch - 'A'];
		}
		else if(ch >= 'a' && ch <= 'z')
		{
			dispbuf[i] = tbl_led_alpha[ch - 'a'];
		}
		else if(ch == ' ')
		{
			dispbuf[i] = 0;
		}
		else
		{
			dispbuf[i] = LED_5C7S__;
		}
	}
//    dispbuf[4] = 0;
}


/*----------------------------------------------------------------------------*/
/**@brief LED初始化函数，不显示
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void led_5c7s_init(void) AT(LED_5C7S_INIT);
void led_5c7s_init(void)
{
	LED_5C7S_PORT_INIT(); 
	ALL_LED_5C7S_OFF();
}

/*----------------------------------------------------------------------------*/
/**@brief 5com7seg LED 清除显示缓存
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void led_5c7s_clear(void) AT(LED_5C7S_INIT);
void led_5c7s_clear(void)
{
	smemset_i(dispbuf, 0, 3);
}

//5c7s闪烁控制
void led_5c7s_blink(void) AT(LED_5C7S_SEG);
void led_5c7s_blink(void)
{
#if 0
	if(ocx.blink_sta)
	{
		dispbuf[4] |= ocx.blink_icon;
		if(ocx.blink_num & 0x0c)
		{
			CHAR_HOURH = ocx.blink_numh;
			CHAR_HOURL = ocx.blink_numl;
		}
		else if(ocx.blink_num & 0x03)
		{
			CHAR_MINH = ocx.blink_numh;
			CHAR_MINL = ocx.blink_numl;
		}
	}
	else
	{
		dispbuf[4] &= ~ocx.blink_icon;
		if(ocx.blink_num & 0x0c)
		{
			CHAR_HOURH = 0;
			CHAR_HOURL = 0;
		}
		else if(ocx.blink_num & 0x03)
		{
			CHAR_MINH = 0;
			CHAR_MINL = 0;
		}
	}
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief LED扫描函数，逐个点亮LED
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void led_5c7s_scan(void) AT(LED_5C7S_SEG);
void led_5c7s_scan(void)
{
	static u8 scan_cnt = 0;

	ALL_LED_5C7S_OFF();

	//led_5c7s_blink();

	scan_cnt++;
	if(scan_cnt > 2)
	{
		scan_cnt = 0;
	}

#if LED_5C7S_EXT_RES_EN
	//使用外部限流电阻,对要点亮的段设为输出0
	LED_5C7S_S_PORT &= ~dispbuf[scan_cnt];
#else
	//使用内部限流电阻,对要点亮的段设为输入下拉
//	LED_5C7S_S_DIR |= dispbuf[scan_cnt];

// SEG = P42, P03/6, P22~6
// COM = P43, P04/5

// COM0 = P43 = SCH.SEG5 = LED.P12
// COM1 = P04 = SCH.SEG1 = LED.P8
// COM2 = P05 = SCH.SEG2 = LED.P9

// SEG_A = P42 = SCH.SEG4 = LED.P11
// SEG_B = P06 = SCH.SEG0 = LED.P7
// SEG_C = P23 = SCH.SEG9 = LED.P4
// SEG_D = P25 = SCH.SEG7 = LED.P2
// SEG_E = P26 = SCH.SEG6 = LED.P1
// SEG_F = P03 = SCH.SEG3 = LED.P10
// SEG_G = P22 = SCH.SEG10 = LED.P5
// SEG_DP = P24 = SCH.SEG8 = LED.P3
	
    if(dispbuf[scan_cnt] & BIT_A)     P4 |= BIT(2);
    if(dispbuf[scan_cnt] & BIT_B)     P0 |= BIT(6);
    if(dispbuf[scan_cnt] & BIT_C)     P2 |= BIT(3);
    if(dispbuf[scan_cnt] & BIT_D)     P2 |= BIT(5);
    if(dispbuf[scan_cnt] & BIT_E)     P2 |= BIT(6);
    if(dispbuf[scan_cnt] & BIT_F)     P0 |= BIT(3);
    if(dispbuf[scan_cnt] & BIT_G)    	P2 |= BIT(2); 
    if(dispbuf[scan_cnt] & BIT_DP)   P2 |= BIT(4); 
	
#endif

	switch(scan_cnt)
	{
	case 0:
		LED_5C7S_C0_PORT &= ~LED_5C7S_C0_BIT;
		break;
	case 1:
		LED_5C7S_C1_PORT &= ~LED_5C7S_C1_BIT;
		break;
	case 2:
		LED_5C7S_C2_PORT &= ~LED_5C7S_C2_BIT;
		break;
	default:
		break;
	}
}


//显示数字
void led_5c7s_dispnum(u16 num) AT(LED_5C7S_SEG);
void led_5c7s_dispnum(u16 num)
{
#if OLD
	u8 i;
	const	u8 batt[3]={LED_5C7S_L,LED_5C7S_O,0};
	if(LESS_BATT==1)
	{
		for(i=2; i!=0xff; i--)
		{
			dispbuf[i] = batt[i];
		}
	}
	else
	{
		for(i=2; i!=0xff; i--)
		{
			dispbuf[i] =tbl_led_num[num % 10];
			num /= 10;
		}
	}
#else
	// 3位数码管，仅能显示 几百数值
	dispbuf[0] = tbl_led_num[num/100];
	dispbuf[1] = tbl_led_num[num%100/10];
	dispbuf[2] = tbl_led_num[num%10];
#endif
}

//显示音量
void led_5c7s_disp_vol(void) AT(LED_5C7S_SEG);
void led_5c7s_disp_vol(void)
{
	disp_numsel(sys_ctl.volume);
	//dispbuf[0] = 0;
	dispbuf[0] = LED_5C7S_V;
}

//显示时间
void led_5c7s_disp_time(void) AT(LED_5C7S_SEG);
void led_5c7s_disp_time(void)
{
	bool ie_ea = IE_EA;
	IE_EA = 0;

	dispbuf[0] = tbl_led_num[ocx.timeh / 10];
	dispbuf[1] = tbl_led_num[ocx.timeh % 10];
	dispbuf[2] = tbl_led_num[ocx.timel / 10];
#if 0

	if(ocx.blink_num & 0x0c)
	{
		ocx.blink_numh = CHAR_HOURH;
		ocx.blink_numl = CHAR_HOURL;
	}
	else if(ocx.blink_num & 0x03)
	{
		ocx.blink_numh = CHAR_MINH;
		ocx.blink_numl = CHAR_MINL;
	}
#endif
	IE_EA = ie_ea;
}

//显示当前设备
void led_5c7s_disp_device(void) AT(LED_5C7S_SEG);
void led_5c7s_disp_device(void)
{
	u8 i;
const u8 p1[3]={LED_5C7S__,LED_5C7S_1,LED_5C7S__};
const u8 p2[3]={LED_5C7S__,LED_5C7S_2,LED_5C7S__};

#if 1
	if(task_ctl.work_sta == TASK_FLASH)
	{
		for(i=2; i!=0xff; i--)
		{
			dispbuf[i] =p1[i];
		}

	}
	if(task_ctl.work_sta == TASK_MUSIC)
	{
		for(i=2; i!=0xff; i--)
		{
			dispbuf[i] =p2[i];
		}
	}
#endif	
}

//显示播放界面
void led_5c7s_disp_play_pause(void) AT(LED_5C7S_SEG);
void led_5c7s_disp_play_pause(void)
{
	//disp_time();
	//dispbuf[4] = LED_5C7S_DDOT | LED_5C7S_MP3 | (b_msc.pause ? LED_5C7S_PAUSE : LED_5C7S_PLAY);
	 disp_device();
}

//显示播放界面
void led_5c7s_disp_numsel_icon(void) AT(LED_5C7S_SEG);
void led_5c7s_disp_numsel_icon(void)
{
	// dispbuf[4] = LED_5C7S_MP3;
     disp_device();
}

//显示FM频道
void led_5c7s_disp_ch(u8 ch) AT(LED_5C7S_SEG);
void led_5c7s_disp_ch(u8 ch)
{
	disp_numsel(ch);
	dispbuf[0] = LED_5C7S_C;
	dispbuf[1] = LED_5C7S_H;
}

//显示FM频点
void led_5c7s_disp_freq(void) AT(LED_5C7S_SEG);
void led_5c7s_disp_freq(void)
{
	disp_numsel(ocx.freq);
	if(dispbuf[0] == LED_5C7S_0)
	{
		dispbuf[0] = 0;
	}
	// dispbuf[4] = LED_5C7S_MHZ;
}

//能量显示
#if ENERGY_DISP_EN
void led_5c7s_disp_energy(void) AT(LED_5C7S_SEG);
void led_5c7s_disp_energy(void)
{
	u8 num1, num2, i;

	num1 = ocx.energy_cnt / 3;
	num2 = ocx.energy_cnt % 3;
	//printf("num1=%dx num2=%d\n", num1, num2);
	for(i=0; i!=3; i++)
	{
		if(i < num1)
		{
			dispbuf[i] = tbl_led_energy[2];
		}
		else if(i == num1)
		{
			dispbuf[i] = tbl_led_energy[num2];
		}
		else
		{
			dispbuf[i] = 0;
		}
	}
}
#endif
#endif /* (DISP_TYPE==DISP_LED_5C7S) */


