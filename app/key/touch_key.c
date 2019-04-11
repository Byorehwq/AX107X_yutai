#include "include.h"

#if USE_TOUCH_KEY

#define TOUCH_KEY_RESET_CNT     15      //reset间隔时间

IAR_XDATA_A u8 touch_key_reset_flag;
IAR_XDATA_A u8 touch_key_reset_cnt;

#pragma constseg="TOUCH_KEY_TABLE"
IAR_CONST u8 tbl_touch_key[][2] =
{
	{0x01, T_KEY_PREV},
	{0x02, T_KEY_NEXT},
	{0x03, T_KEY_PLAY},
	{0x04, T_KEY_VOL_UP},
	{0x06, T_KEY_VOL_DOWN},
	{0x08, NO_KEY},
	{0x0C, NO_KEY},
	{0x10, T_KEY_MODE},
	{0x18, NO_KEY},
	{0x20, NO_KEY},
	{0x21, NO_KEY},
	{0x30, NO_KEY},
};

//单按键的阈值设置
IAR_CONST u16 tbl_touch_key_thres[][2] =
{
	{0x01, 0x81C0},     //TKTHC0 = 0x0B
	{0x02, 0x81C0},     //TKTHC1 = 0x0B
	{0x04, 0x8180},     //TKTHC2 = 0x0A
	{0x08, 0x8100},     //TKTHC3 = 0x08
	{0x10, 0x8140},     //TKTHC4 = 0x09
	{0x20, 0x81C0},     //TKTHC5 = 0x0B
};
#pragma constseg=default

#pragma location="TOUCH_KEY_SEG"
u16 touch_timer1_cap(u8 psr)
{
	u16 tmp;
	TMR1CNTH = 0;
	TMR1CNTL = 0;
	TMR1PRH = 0xFF;
	TMR1PRL = 0xFF;
	TMR1PWMH = 0;
	TMR1PWML = 0;

	TMR1CON1 = psr;
	TMR1CON0 = 0x37;

	while((TMR1CON1 & 0x40) != 0x40);
	TMR1CON1 &= ~0xC0;
	TMR1CNTH = 0;
	TMR1CNTL = 0;

	while((TMR1CON1 & 0x40) != 0x40);
	TMR1CON1 = 0;
	TMR1CON0 = 0;
	u8 pwml = TMR1PWML;
	tmp = (TMR1PWMH << 8) | pwml;

	return tmp;
}

#pragma location="TOUCH_KEY_SEG"
void touch_baud_calc(void)
{
	u16 tmp, cnt;
	u8 tmp2;

	tmp2 = 0x03;
	tmp = touch_timer1_cap(tmp2);
	cnt = 24000;

	while(tmp >= 256)
	{
		tmp2++;
		cnt = cnt >> 1;
		tmp = touch_timer1_cap(tmp2);
	}
	TKBAUD = cnt / (tmp & 0xff) - 1;
	//printf("TKBAUD=%02x\n", TKBAUD);
}

#pragma location="TOUCH_KEY_SEG"
void touch_key_init(void)
{
#if (SYS_CRY == CRY_RC)
	//touch_baud_calc();
	TKBAUD = 124;       //不准确，有待完善
	TKCCC = (0x09<<4) | (0x0E<<0);  //充放电周期为48,RC 10M
#else
	TKBAUD = 124;
	TKCCC = (0x09<<4) | (0x0C<<0);  //充放电周期为48,24分频
#endif
	TKTCC = (0x01<<6) | (0x03<<3) | (0x02<<0);  //基准电容计算时间2s,按键有效防抖点数=12point,按键无效防抖点数=8point
	TKPND = 0;      //clear pending
	TKAPC = 0x25;
	TKENC = 0;

	//根据复合按键设定初始阈值
#if TOUCH_KEY_0_EN
	TKENC |= 0x01;  //TK0
	TKTHC0 = 0x08;
	TKAC0 = 0x77;
#endif
#if TOUCH_KEY_1_EN
	TKENC |= 0x02;  //TK1
	TKTHC1 = 0x08;
	TKAC1 = 0x77;
#endif
#if TOUCH_KEY_2_EN
	TKENC |= 0x04;  //TK2
	TKTHC2 = 0x07;
	TKAC2 = 0x77;
#endif
#if TOUCH_KEY_3_EN
	TKENC |= 0x08;  //TK3
	TKTHC3 = 0x05;
	TKAC3 = 0x77;
#endif
#if TOUCH_KEY_4_EN
	TKENC |= 0x10;  //TK4
	TKTHC4 = 0x06;
	TKAC4 = 0x77;
#endif
#if TOUCH_KEY_5_EN
	TKENC |= 0x20;  //TK5
	TKTHC5 = 0x06;
	TKAC5 = 0x77;
#endif

	touch_key_reset_flag = 0;
	touch_key_reset_cnt = 0;
}

#pragma location="TOUCH_KEY_SEG"
void touch_key_start(void)
{
	TKCON = 0xa1;
}

#pragma location="TOUCH_KEY_SEG"
u8 touch_key_scan(void)
{
	u8 knum = 0;
	u8 cnt = 0;
	u16 *ptr = (u16 *)0x1400;

	touch_key_reset_flag = 0;
	if(TKPND & 0xBF)
	{
		TKPND &= ~0x80;
		//printf("TKPND=%02x\n", TKPND);
		while(TKPND != tbl_touch_key_thres[cnt][0])     //cnt=6为复合按键，cnt<6初步认为是单按键
		{
			cnt++;
			if(cnt == 6)
			{
				break;
			}
		}
		if(cnt != 6)        //单按键需要根据tbl_touch_key_thres设置的阈值再次判断
		{
			u16 thres = ptr[cnt*8+1];
			u16 thres_cur = ptr[cnt*8];
			u16 thres_new;
			thres_new = (u16)((u32)tbl_touch_key_thres[cnt][1] * thres >> 15);
			//printf("thres=%04x cur=%04x new=%04x\n", thres, thres_cur, thres_new);
			if(thres_cur < thres_new)
			{
				return NO_KEY;
			}
		}
		touch_key_reset_flag = 1;
	}
	else
	{
		return NO_KEY;
	}
	//printf("TKPND=%02x\n", TKPND);
	while(TKPND != tbl_touch_key[knum][0])
	{
		knum++;
		if(knum > 10)
		{
			return NO_KEY;
		}
	}
	//printf("knum=%d\n", knum);
	return tbl_touch_key[knum][1];
}

#pragma location="TOUCH_KEY_SEG"
void touch_key_rest(void)
{
	if(touch_key_reset_flag)
	{
		touch_key_reset_cnt++;
		//printf("cnt=%d\n", touch_key_reset_cnt);
	}
	else
	{
		touch_key_reset_cnt = 0;
	}
	if(touch_key_reset_cnt >= TOUCH_KEY_RESET_CNT)
	{
		touch_key_reset_flag = 0;
		touch_key_reset_cnt = 0;
		TKCON &= ~BIT(0);   //disable touch key
		asm("nop");         //必须延时
		asm("nop");
		asm("nop");
		TKCON |= BIT(0);    //enable touch key
		printf("reset\n");
	}
}
#endif
