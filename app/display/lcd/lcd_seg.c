/*--------------------------------------------------------------------------*/
/**@file lcd_seg.C
   @brief 断码屏的各种显示函数
   @details 断码屏显示中使用的函数
   @author LYF
   @date   2010-08-17
   @note   none
*/
/*----------------------------------------------------------------------------*/
#include "include.h"

#if (DISP_TYPE==DISP_LCD_SEG)

const u8 segTable[] = {SEG_0,SEG_1,SEG_2,SEG_3,SEG_4,SEG_5,SEG_6,SEG_7,SEG_8,SEG_9};

const u8 LCD_seg_repeatTable[4][3] =
{
	{SEG_A,SEG_L,SEG_L},
	{SEG_R,SEG_A,SEG_N},
	{SEG_0,SEG_N,SEG_E},
	{SEG_F,SEG_0,SEG_D}
};

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示数字
   @param 无
   @return 无
   @note
   @note       COM_T
   @note       1----COM0 H-------------(COM2 M)---(COM3M)---(COM4M)
   @note       2----COM0 L----------------------------------------
   @note       3----COM0 M----COM1 H------------------------------
   @note       4--------------COM1 L------------------------------
   @note       5--------------COM1 M----COM2 H--------------------
   @note       6------------------------COM2 L--------------------
   @note       7------------------------COM2 M----COM3 H----------
   @note       8----------------------------------COM3 L----------
   @note       9----------------------------------COM3 M----COM4 H
   @note       10-------------------------------------------COM4 L
*/
/*----------------------------------------------------------------------------*/
void display_seg(void)@"LCD_SEG_API"
{
	COM_T++;
#if (LCD_SEG_TYPE == LCD_SEG39)
	if(COM_T>6)
		COM_T = 1;
#endif

//--COM0--
	if(COM_T == 1)
	{
#if (LCD_SEG_TYPE == LCD_SEG39)
		LCD_COM2_DIR |= (1<<LCD_COM2_bit);                                //COM2 M
#endif
		LCD_COM0 |= (1<<LCD_COM0_bit);
		LCD_COM0_DIR &= ~(1<<LCD_COM0_bit);     //COM0 H

		LCDS = ~LCD_S_0;
#if (LCD_SEG_TYPE == LCD_SEG39)
		if(LCD_SS_0)
		{
			LCD_S8 &= ~(1<<LCD_S8_bit);
		}
		else
		{
			LCD_S8 |= (1<<LCD_S8_bit);
		}
#endif
	}
	else if(COM_T == 2)
	{
		LCD_COM0 &= ~(1<<LCD_COM0_bit);                                       //COM0 L

		LCDS = LCD_S_0;
#if (LCD_SEG_TYPE == LCD_SEG39)
		if(LCD_SS_0)
		{
			LCD_S8 |= (1<<LCD_S8_bit);
		}
		else
		{
			LCD_S8 &= ~(1<<LCD_S8_bit);
		}
#endif
	}

//--COM1--
	else if(COM_T == 3)
	{
		LCD_COM0_DIR |= (1<<LCD_COM0_bit);                                    //COM0 M
		LCD_COM1 |= (1<<LCD_COM1_bit);
		LCD_COM1_DIR &= ~(1<<LCD_COM1_bit);     //COM1 H

		LCDS = ~LCD_S_1;
#if (LCD_SEG_TYPE == LCD_SEG39)
		if(LCD_SS_1)
		{
			LCD_S8 &= ~(1<<LCD_S8_bit);
		}
		else
		{
			LCD_S8 |= (1<<LCD_S8_bit);
		}
#endif
	}
	else if(COM_T == 4)
	{
		LCD_COM1 &= ~(1<<LCD_COM1_bit);                                      //COM1 L

		LCDS = LCD_S_1;
#if (LCD_SEG_TYPE == LCD_SEG39)
		if(LCD_SS_1)
		{
			LCD_S8 |= (1<<LCD_S8_bit);
		}
		else
		{
			LCD_S8 &= ~(1<<LCD_S8_bit);
		}
#endif
	}

//--COM2--
	else if(COM_T == 5)
	{
		LCD_COM1_DIR |= (1<<LCD_COM1_bit);                                   //COM1 M
		LCD_COM2 |= (1<<LCD_COM2_bit);
		LCD_COM2_DIR &= ~(1<<LCD_COM2_bit);    //COM2 H

		LCDS = ~LCD_S_2;
#if (LCD_SEG_TYPE == LCD_SEG39)
		if(LCD_SS_2)
		{
			LCD_S8 &= ~(1<<LCD_S8_bit);
		}
		else
		{
			LCD_S8 |= (1<<LCD_S8_bit);
		}
#endif
	}
	else if(COM_T == 6)
	{
		LCD_COM2 &= ~(1<<LCD_COM2_bit);                                      //COM2 L

		LCDS = LCD_S_2;
#if (LCD_SEG_TYPE == LCD_SEG39)
		if(LCD_SS_2)
		{
			LCD_S8 |= (1<<LCD_S8_bit);
		}
		else
		{
			LCD_S8 &= ~(1<<LCD_S8_bit);
		}
#endif
	}
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示数字
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void LCDseg39_value_set(void)@"LCD_SEG_API"
{
	//---COM0
	if(DD2&0X04)  LCD_S_0 |= BIT(0); //SEG1
	if(DD2&0X02)  LCD_S_0 |= BIT(1); //SEG2
	if(DD1_S1)    LCD_S_0 |= BIT(2); //SEG3
	if(DD3&0X04)  LCD_S_0 |= BIT(3); //SEG4
	if(DD3&0X02)  LCD_S_0 |= BIT(4); //SEG5
	if(DD1_SD)    LCD_S_0 |= BIT(5); //SEG6
	if(DD4&0X04)  LCD_S_0 |= BIT(6); //SEG7
	if(DD4&0X02)  LCD_S_0 |= BIT(7); //SEG8
	if(DD1_USB)   LCD_SS_0 = 1;

	//---COM1
	if(DD2&0X10)  LCD_S_1 |= BIT(0); //SEG1
	if(DD2&0X08)  LCD_S_1 |= BIT(1); //SEG2
	if(DD1_VOL)   LCD_S_1 |= BIT(2); //SEG3
	if(DD3&0X10)  LCD_S_1 |= BIT(3); //SEG4
	if(DD3&0X08)  LCD_S_1 |= BIT(4); //SEG5
	if(DD1_PMHZ)  LCD_S_1 |= BIT(5); //SEG6
	if(DD4&0X10)  LCD_S_1 |= BIT(6); //SEG7
	if(DD4&0X08)  LCD_S_1 |= BIT(7); //SEG8
	if(DD1_S2)    LCD_SS_1 = 1;

	//---COM2
	if(DD2&0X80)  LCD_S_2 |= BIT(0); //SEG1
	if(DD2&0X40)  LCD_S_2 |= BIT(1); //SEG2
	if(DD2&0X20)  LCD_S_2 |= BIT(2); //SEG3
	if(DD3&0X80)  LCD_S_2 |= BIT(3); //SEG4
	if(DD3&0X40)  LCD_S_2 |= BIT(4); //SEG5
	if(DD3&0X20)  LCD_S_2 |= BIT(5); //SEG6
	if(DD4&0X80)  LCD_S_2 |= BIT(6); //SEG7
	if(DD4&0X40)  LCD_S_2 |= BIT(7); //SEG8
	if(DD4&0X20)  LCD_SS_2 = 1;
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏清屏
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void LCDseg_CLR(void)@"LCD_SEG_API"
{
	DD1_VOL = 0;
	DD1_USB = 0;
	DD1_PMHZ = 0;
	DD1_SD = 0;
	DD1_S2 = 0;
	DD1_S1 = 0;

	DD2 = 0;
	DD3 = 0;
	DD4 = 0;

	/*icon_music = 0;
	//icon_col = 0;
	icon_dot = 0;
	icon_USB = 0;
	icon_SD = 0;
	icon_VOL = 0;
	icon_aux = 0;
	icon_play = 0;
	icon_pause = 0;*/

	LCD_S_0 = 0;
	LCD_S_1 = 0;
	LCD_S_2 = 0;
	LCD_S_3 = 0;
	LCD_SS_0 = 0;
	LCD_SS_1 = 0;
	LCD_SS_2 = 0;
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏初始化
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void LCDInit_SEG(void)@"LCD_INIT"
{
	SEG_PORT_INIT();
}

/*----------------------------------------------------------------------------*/
/**@brief 十六进制转十进制API
   @param input：输入的十六进制数字
   @return aa：十六进制的千位
   @return bb：十六进制的百位
   @return cc：十六进制的十位
   @return dd：十六进制的个位
   @note
*/
/*----------------------------------------------------------------------------*/
void pattern_1(u16 input)@"LCD_SEG_API"
{
	aa = input/1000;
	input = input%1000;
	bb = input/100;
	input = input%100;
	cc = input/10;
	dd = input%10;
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，调节音量时显示的菜单
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_VOL_1(void)@"LCD_SEG_API"
{
	pattern_1(volume);
#if (LCD_SEG_TYPE == LCD_SEG47)
	DD2 = SEG_U;
#else
	DD2 = SEG_NC;
#endif

	DD3 = segTable[cc];
	DD4 = segTable[dd];
	DD1_VOL  = 1;
	icon_music = 1;
	if(playnPause)
		icon_play = 1;
	else
		icon_pause = 1;
	icon_VOL = 1;
	if(deviceOnline != NO_DEVICE)
	{
		if(CurCheckedDev&BIT(0))      icon_SD = 1;
		else if(CurCheckedDev&BIT(1))  icon_USB = 1;
	}
}

#define  FRE_POT  0
/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，FM模式下显示正在播放的频率
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_FRE_1(void)@"LCD_SEG_API"
{
	pattern_1(fmFrequency);
	DD2 = segTable[bb];
	DD3 = segTable[cc];
	DD4 = segTable[dd];

	DD1_PMHZ = 1;

	if(fmFrequency/1000) DD1_S1 = 1;

	if(deviceOnline != NO_DEVICE)
	{
		if(CurCheckedDev&BIT(0))      icon_SD = 1;
		else if(CurCheckedDev&BIT(1))  icon_USB = 1;
	}
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，显示输入的数字
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_Input_Number_1(void)@"LCD_SEG_API"
{
	pattern_1(inputNum);
	DD2 = segTable[bb];
	DD3 = segTable[cc];
	DD4 = segTable[dd];

	if(inputNum/1000) DD1_S1 = 1;
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，MP3播放模式下播放歌曲时的显示菜单
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void MENU_Play_1(void)@"LCD_SEG_API"
{
	menu_Next_1();

	icon_play = 1;
	icon_music = 1;

	if(CurCheckedDev&BIT(0))
	{
		DD1_SD = 1;
		icon_SD = 1;
	}
	else                      {DD1_USB = 1; icon_USB = 1;}
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，MP3播放模式下显示播放模式
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_Play_Mode_1(void)@"LCD_SEG_API"
{
	DD2 = LCD_seg_repeatTable[playMode][0];
	DD3 = LCD_seg_repeatTable[playMode][1];
	DD4 = LCD_seg_repeatTable[playMode][2];
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，MP3播放模式下显示正在播放的歌曲号
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_Next_1(void)@"LCD_SEG_API"
{
	pattern_1(t_fat.file_number);
	DD2 = segTable[bb];
	DD3 = segTable[cc];
	DD4 = segTable[dd];

	if(t_fat.file_number/1000)  DD1_S1 = 1;

	if(CurCheckedDev&BIT(0))
	{
		DD1_SD = 1;
	}
	else                      {DD1_USB = 1;}
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，FM模式下显示频道号
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_FMNext_1(void)@"LCD_SEG_API"
{
	pattern_1(Cur_Fre_Num);
	DD2 = SEG_P;
	DD3 = segTable[cc];
	DD4 = segTable[dd];
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，关机，不显示
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_OFF_1(void)@"LCD_SEG_API"
{
	DD2 = SEG_NC;
	DD3 = SEG_NC;
	DD4 = SEG_NC;
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，显示“NO”
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_NULL_1(void)@"LCD_SEG_API"
{
	DD2 = SEG_N;
	DD3 = SEG_0;
	DD4 = SEG_NC;
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，mp3播放时没有媒体显示，显示“NO”
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_NO_Device_1(void)@"LCD_SEG_API"
{
	menu_NULL_1();
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，暂停或者停止时显示，显示“PAU”或者“STP”
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_Pause_1(void)@"LCD_SEG_API"
{
	if(bStopFlag == true)
	{
		DD2 = SEG_S;
		DD3 = SEG_T;
		DD4 = SEG_P;
	}
	else
	{
		DD2 = SEG_P;
		DD3 = SEG_A;
		DD4 = SEG_U;
	}
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，调EQ时显示
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_EQ_1(void)@"LCD_SEG_API"
{
	DD2 = SEG_E;
	DD3 = segTable[eq+1];
	DD4 = SEG_NC;
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，LINEIN模式显示函数
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void MENU_linein_1(void)@"LCD_SEG_API"
{
	DD2 = SEG_I;
	DD3 = SEG_N;
	DD4 = SEG_NC;
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，waiting时显示，显示“---”
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void MENU_Waiting_1(void)@"LCD_SEG_API"
{
	DD2 = SEG__;
	DD3 = SEG__;
	DD4 = SEG__;

}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，NO FILE显示，显示“NO”
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_No_File_1(void)@"LCD_SEG_API"
{
	menu_NULL_1();
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，USB AUDIO模式显示函数
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_USBDevice_1(void)@"LCD_SEG_API"
{
	DD2 = SEG_P;
	DD3 = SEG_C;
	DD4 = SEG_NC;
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，RTC模式下的显示，暂时没做
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_Time_1(void)@"LCD_SEG_API"
{
}

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数，开机时全点亮屏
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void menu_Power_On_1(void)@"LCD_SEG_API"
{
	DD2 = SEG_8;
	DD3 = SEG_8;
	DD4 = SEG_8;
	DD1_VOL  = 1;
	DD1_USB  = 1;
	DD1_PMHZ = 1;
	DD1_SD   = 1;
	DD1_S1   = 1;
	icon_music = 1;
	icon_USB = 1;
	icon_SD = 1;
	icon_VOL = 1;
	icon_aux = 1;
	icon_play = 1;
	icon_pause = 1;
}


//-----LCD_SEG-----
#pragma constseg=LCD_SEG_TBL
const H menu_1[] =
{
	menu_Power_On_1,
	menu_EQ_1,
	menu_VOL_1,
	menu_Input_Number_1,
	menu_No_File_1,
	MENU_Play_1,
	menu_Play_Mode_1,
	menu_Next_1,
	menu_Pause_1,
	menu_NO_Device_1,
	MENU_Waiting_1,
	MENU_linein_1,
	menu_FRE_1,
	menu_FMNext_1,
	menu_USBDevice_1,
	menu_OFF_1,
	menu_Time_1
};
#pragma constseg=default

/*----------------------------------------------------------------------------*/
/**@brief 断码屏显示函数
   @param menu_number 需要显示的菜单号码
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="LCD_SEG_API"
void DispMenu_1(u8 menu_number)
{
	void (*p)(void);

	if(menu_number != MENU_INPUT_NUM)
		inputNum = 0;

	returnCount = 0;
	LCDseg_CLR();
	p = menu_1[menu_number];
	(*p)();

#if (LCD_SEG_TYPE == LCD_SEG39)
	LCDseg39_value_set();
#endif
}

#endif /* (DISP_TYPE==DISP_LCD_SEG) */

