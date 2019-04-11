/*****************************************************************************
* Module    : Module
* File      : ir.c
* Author    : jingfa
* Email     :
* Function  : 红外驱动相关
*****************************************************************************/
#include "include.h"
#include "kedir.h"
#include "ir.h"

#if USE_IR

volatile __data u8 ir_state;
volatile __data u8 ir_cnt;
volatile __data u8 repeat_cnt;
volatile __data u16 ir_data;
volatile __data u16 ir_code;
volatile bool ir_key_sta;

#define IR_TABLE_DIM  2

#pragma constseg="IR_CONST_TABLE"
#if (IR_TABLE_DIM != 2)     //一维表

//0xFF00(21KEY)
IAR_CONST u8 ir_table_FF00[96] =
{
// 	   0              1 		     2	            3             4               5             6                 7
	/*0*/  NO_KEY,        NO_KEY,        NO_KEY,        NO_KEY,       NO_KEY,         NO_KEY,       NO_KEY,           T_KEY_PREV,
	T_KEY_NUM_4,   T_KEY_PLAY,    NO_KEY,	    NO_KEY,		  T_KEY_NUM_1,	  T_KEY_NUM_0,	NO_KEY,			  NO_KEY,
	/*1*/  NO_KEY,        NO_KEY,        NO_KEY,        NO_KEY,       NO_KEY,         T_KEY_NEXT,   T_KEY_VOL_DOWN,	  NO_KEY,
	T_KEY_NUM_2,   T_KEY_VOL_UP,  NO_KEY,        NO_KEY,	      T_KEY_NUM_5,	  NO_KEY,	    NO_KEY,			  NO_KEY,
	/*2*/  NO_KEY,        NO_KEY,        NO_KEY,        NO_KEY,       NO_KEY,         NO_KEY,	    NO_KEY,	     	  NO_KEY,
	NO_KEY,	      NO_KEY,	     NO_KEY,	    NO_KEY,	      NO_KEY,	      NO_KEY,	    NO_KEY,	     	  NO_KEY,
	/*3*/  NO_KEY,        NO_KEY,        NO_KEY,        NO_KEY,       NO_KEY,         NO_KEY,	    NO_KEY,			  NO_KEY,
	NO_KEY,		  NO_KEY,		 NO_KEY,	    NO_KEY,		  NO_KEY,		  NO_KEY,	    NO_KEY,			  NO_KEY,
	/*4*/  T_KEY_PLAYMODE,NO_KEY,        T_KEY_NUM_7,   T_KEY_EQ,     T_KEY_MODE,     T_KEY_POWER,  T_KEY_STOP,       T_KEY_MUTE,
	NO_KEY,        NO_KEY,      	 T_KEY_NUM_9,   NO_KEY,       NO_KEY,      	  NO_KEY,       NO_KEY,      	  NO_KEY,
	/*5*/  NO_KEY,        NO_KEY,        T_KEY_NUM_8,   NO_KEY,       NO_KEY,         NO_KEY,	    NO_KEY,			  NO_KEY,
	NO_KEY,		  NO_KEY,		 T_KEY_NUM_6,	NO_KEY,		  NO_KEY,		  NO_KEY,	    T_KEY_NUM_3,	  NO_KEY,
};

//0xBF00(18KEY)
IAR_CONST u8 ir_table_BF00[32] =
{
	//  0              1              2              3             4               5             6                 7
	/*0*/  T_KEY_POWER,   T_KEY_PLAY,    T_KEY_MODE,    NO_KEY,       T_KEY_NUM_1,    T_KEY_NUM_2,  T_KEY_NUM_3,      NO_KEY,
	T_KEY_NUM_4,   T_KEY_NUM_5,   T_KEY_NUM_6,   NO_KEY,       T_KEY_NUM_7,    T_KEY_NUM_8,  T_KEY_NUM_9,      NO_KEY,
	/*1*/  T_KEY_NUM_0,   NO_KEY,        T_KEY_VOL_UP,  NO_KEY,       T_KEY_PREV,     T_KEY_NEXT,   T_KEY_VOL_DOWN,   NO_KEY,
	NO_KEY,        NO_KEY,        NO_KEY,        NO_KEY,       NO_KEY,         NO_KEY,       NO_KEY,           NO_KEY,
};

//0xFD02(20KEY)
IAR_CONST u8 ir_table_FD02[32] =
{
	//  0              1              2              3             4               5               6               7
	/*0*/  T_KEY_PLAY,    T_KEY_CH_DOWN, T_KEY_CH_UP,   NO_KEY,       T_KEY_EQ,       T_KEY_VOL_DOWN, T_KEY_VOL_UP,   NO_KEY,
	T_KEY_NUM_0,   T_KEY_PREV,    T_KEY_NEXT,    NO_KEY,       T_KEY_NUM_1,    T_KEY_NUM_2,    T_KEY_NUM_3,    NO_KEY,
	/*1*/  T_KEY_NUM_4,   T_KEY_NUM_5,   T_KEY_NUM_6,   NO_KEY,       T_KEY_NUM_7,    T_KEY_NUM_8,    T_KEY_NUM_9,    NO_KEY,
	NO_KEY,        NO_KEY,        T_KEY_MODE,    NO_KEY,       NO_KEY,         NO_KEY,         NO_KEY,         NO_KEY,
};

//0xFE01(21KEY)
IAR_CONST u8 ir_table_FE01[32] =
{
	// 0               1              2               3             4              5               6               7
	/*0*/ NO_KEY,         T_KEY_EQ,      T_KEY_VOL_DOWN, T_KEY_VOL_UP, T_KEY_NUM_0,   T_KEY_PREV,     T_KEY_NEXT,     T_KEY_NUM_1,
	T_KEY_NUM_2,    T_KEY_NUM_3,   T_KEY_NUM_4,    NO_KEY,       T_KEY_NUM_7,   T_KEY_NUM_8,    T_KEY_NUM_9,    T_KEY_POWER,
	/*1*/ NO_KEY,         NO_KEY,        T_KEY_PLAY,     NO_KEY,       NO_KEY,        NO_KEY,         NO_KEY,         NO_KEY,
	NO_KEY,         T_KEY_MODE,    T_KEY_CH_DOWN,  T_KEY_NUM_5,  NO_KEY,        NO_KEY,         T_KEY_CH_UP,    T_KEY_NUM_6,
};

#else //(IR_TABLE_DIM == 2) 二维表

//0xFF00(21KEY)
IAR_CONST u8 ir_table_FF00[][2] =
{
	{0x07, T_KEY_PREV},
	{0x09, T_KEY_PLAY},
	{0x15, T_KEY_NEXT},
	{0x16, T_KEY_VOL_DOWN},
	{0x19, T_KEY_VOL_UP},
	{0x44, T_KEY_MODE},
	{0x40, T_KEY_PLAYMODE},
	{0x43, T_KEY_EQ},
	{0x45, T_KEY_POWER},
	{0x46, T_KEY_STOP},
	{0x47, T_KEY_MUTE},
	{0x0d, T_KEY_NUM_0},
	{0x0c, T_KEY_NUM_1},
	{0x18, T_KEY_NUM_2},
	{0x5e, T_KEY_NUM_3},
	{0x08, T_KEY_NUM_4},
	{0x1c, T_KEY_NUM_5},
	{0x5a, T_KEY_NUM_6},
	{0x42, T_KEY_NUM_7},
	{0x52, T_KEY_NUM_8},
	{0x4a, T_KEY_NUM_9},
	{0xFF, NO_KEY}   //以0xFF结束
};

//0xBF00(18KEY)
IAR_CONST u8 ir_table_BF00[][2] =
{
	{0x14, T_KEY_PREV},
	{0x01, T_KEY_PLAY},
	{0x15, T_KEY_NEXT},
	{0x16, T_KEY_VOL_DOWN},
	{0x12, T_KEY_VOL_UP},
	{0x02, T_KEY_MODE},
	{0x00, T_KEY_POWER},
	{0x10, T_KEY_NUM_0},
	{0x04, T_KEY_NUM_1},
	{0x05, T_KEY_NUM_2},
	{0x06, T_KEY_NUM_3},
	{0x08, T_KEY_NUM_4},
	{0x09, T_KEY_NUM_5},
	{0x0A, T_KEY_NUM_6},
	{0x0C, T_KEY_NUM_7},
	{0x0D, T_KEY_NUM_8},
	{0x0E, T_KEY_NUM_9},
	{0xFF, NO_KEY}   //以0xFF结束
};

//0xFD02(20KEY)
IAR_CONST u8 ir_table_FD02[][2] =
{
	{0x09, T_KEY_PREV},
	{0x00, T_KEY_PLAY},
	{0x0A, T_KEY_NEXT},
	{0x05, T_KEY_VOL_DOWN},
	{0x06, T_KEY_VOL_UP},
	{0x1A, T_KEY_MODE},
	{0x04, T_KEY_EQ},
	{0x01, T_KEY_CH_DOWN},
	{0x02, T_KEY_CH_UP},
	{0x08, T_KEY_NUM_0},
	{0x0C, T_KEY_NUM_1},
	{0x0D, T_KEY_NUM_2},
	{0x0E, T_KEY_NUM_3},
	{0x10, T_KEY_NUM_4},
	{0x11, T_KEY_NUM_5},
	{0x12, T_KEY_NUM_6},
	{0x14, T_KEY_NUM_7},
	{0x15, T_KEY_NUM_8},
	{0x16, T_KEY_NUM_9},
	{0xFF, NO_KEY}   //以0xFF结束
};

//0xFE01(21KEY)
IAR_CONST u8 ir_table_FE01[][2] =
{
	{0x05, T_KEY_PREV},
	{0x12, T_KEY_PLAY},
	{0x06, T_KEY_NEXT},
	{0x02, T_KEY_VOL_DOWN},
	{0x03, T_KEY_VOL_UP},
	{0x19, T_KEY_MODE},
	{0x01, T_KEY_EQ},
	{0x0F, T_KEY_POWER},
	{0x1A, T_KEY_CH_DOWN},
	{0x1E, T_KEY_CH_UP},
	{0x04, T_KEY_NUM_0},
	{0x07, T_KEY_NUM_1},
	{0x08, T_KEY_NUM_2},
	{0x09, T_KEY_NUM_3},
	{0x0A, T_KEY_NUM_4},
	{0x1B, T_KEY_NUM_5},
	{0x1F, T_KEY_NUM_6},
	{0x0C, T_KEY_NUM_7},
	{0x0D, T_KEY_NUM_8},
	{0x0E, T_KEY_NUM_9},
	{0xFF, NO_KEY}   //以0xFF结束
};
#endif //(IR_TABLE_DIM != 2)
#pragma constseg=default

#pragma location="IR_SCAN"
u8 ir_key_get_1(IAR_CONST u8 ir_table[][2], u8 ir_data)
{
	u8 keyval = NO_KEY;

	for(u8 i=0;; i++)
	{
		if((ir_table[i][0] == ir_data) || (ir_table[i][0] == 0xFF))
		{
			keyval = ir_table[i][1];
			break;
		}
	}
	return keyval;
}


#pragma location="IR_SCAN"
void ir_cap_isr(void)
{
	u16  ir_tmp;
	static u8 __idata ir_scan = 0;

	if(TMR1CON1 & 0x40)              //capture
	{
		TMR1CNTL = 0;
		TMR1CNTH = 0;
		ir_tmp = TMR1PWMH<<8;
		u8 aa = TMR1PWML;
		ir_tmp |= aa;
		TMR1CON1 &= ~BIT(6);
		ir_tmp /= (24000 / 64);      //1ms为单位
		ir_key_sta = 1;
	}
	else                             //overflow
	{
#if USE_KEDIR && !USE_HW_IR && ((USE_KEDIR_TYPE == KEDIR_KEY_IR_LED) || (USE_KEDIR_TYPE == KEDIR_KEY_IR))
		ir_state = 1;
#endif
		TMR1CON1 &= ~BIT(7);
		ir_tmp = 125;
	}

	/* 有些红外干扰较严重，要用下面程序处理一下 */
	if(ir_cnt == 32)
	{
#if USE_KEDIR && !USE_HW_IR && ((USE_KEDIR_TYPE == KEDIR_KEY_IR_LED) || (USE_KEDIR_TYPE == KEDIR_KEY_IR))
		ir_state = 1;
#endif
		if((ir_tmp >= 10) && (ir_tmp <= 12))             //连发码=11.25ms
		{
			ir_scan = 0;
#if USE_KEDIR && !USE_HW_IR && ((USE_KEDIR_TYPE == KEDIR_KEY_IR_LED) || (USE_KEDIR_TYPE == KEDIR_KEY_IR))
			ir_state = 0;
#endif
		}
		else
		{
#if USE_KEDIR && !USE_HW_IR && ((USE_KEDIR_TYPE == KEDIR_KEY_IR_LED) || (USE_KEDIR_TYPE == KEDIR_KEY_IR))
			ir_scan = ir_tmp;
#else
			ir_scan += ir_tmp;
#endif
			if(ir_scan > 120)        //超过108ms的连发间隔，则认为抬键
			{
				ir_cnt = 0;
				ir_key_sta = 0;
			}
		}
		return;
	}
	else if((ir_tmp > 7) || (ir_tmp == 0))
	{
#if USE_KEDIR && !USE_HW_IR && ((USE_KEDIR_TYPE == KEDIR_KEY_IR_LED) || (USE_KEDIR_TYPE == KEDIR_KEY_IR))
		if((ir_tmp > 7) && (ir_tmp != 125))
		{
			ir_state = 0;
		}
		if(ir_tmp == 0)
		{
			ir_state = 1;
		}
#endif
		/*引导码是13.5ms;
		NEC标准 位0是1.125ms,位1是2.25ms;
		PHILIPS标准位0位1都是1.788ms;
		其他值是干扰.*/
		ir_scan = 0;
		ir_cnt = 0;
		ir_key_sta = 0;
		return;
	}
	ir_data >>= 1;
	ir_cnt++;

	if(ir_tmp == 2)
	{
		ir_data |= 0x8000;
	}

	if(ir_cnt == 16)
	{
		ir_code = ir_data;
	}
	else if(ir_cnt == 32)
	{
		if((ir_data&0xFF) >= 96)
		{
			ir_data = NO_KEY;
		}
	}
}

#pragma location="IR_SCAN"
u8 get_ir_val(void)
{
	u8 ir_val = NO_KEY;

#if IR_FF00_EN
	if(ir_code == USERCODE_FF00)
	{
		//printf("ir_code = %02x,ir_data = %02x\n",ir_code,ir_data&0xff);
#if (IR_TABLE_DIM == 2)
		ir_val = ir_key_get_1(ir_table_FF00, ir_data);
#else
		ir_val = ir_table_FF00[ir_data & 0xff];
#endif
	}
#endif

#if IR_BF00_EN
	if(ir_code == USERCODE_BF00)
	{
		//printf("ir_code = %02x,ir_data = %02x\n",ir_code,ir_data&0xff);
#if (IR_TABLE_DIM == 2)
		ir_val = ir_key_get_1(ir_table_BF00, ir_data);
#else
		ir_val = ir_table_BF00[ir_data & 0xff];
#endif
	}
#endif

#if IR_FD02_EN
	if(ir_code == USERCODE_FD02)
	{
		//printf("ir_code = %02x,ir_data = %02x\n",ir_code,ir_data&0xff);
#if (IR_TABLE_DIM == 2)
		ir_val = ir_key_get_1(ir_table_FD02, ir_data);
#else
		ir_val = ir_table_FD02[ir_data & 0xff];
#endif
	}
#endif

#if IR_FE01_EN
	if(ir_code == USERCODE_FE01)
	{
		//printf("ir_code = %02x,ir_data = %02x\n",ir_code,ir_data&0xff);
#if (IR_TABLE_DIM == 2)
		ir_val = ir_key_get_1(ir_table_FE01, ir_data);
#else
		ir_val = ir_table_FE01[ir_data & 0xff];
#endif
	}
#endif
	return ir_val;
}

#pragma location="IR_SCAN"
u8 ir_process(void)
{
	u8 ir_val = NO_KEY;

#if USE_HW_IR
	if(ir_cnt < 35)           //108ms - 58.5ms(引导码+用户码+数据码的最短时间) + 108ms = 157.5ms
	{
		ir_cnt++;
	}
	else
	{
		ir_data = 0;
		ir_cnt = IR_TIMEOUT_CNT;      //time out
		repeat_cnt = 0;
		ir_code = 0;
		ir_key_sta = 0;
	}

	ir_val = get_ir_val();

#else   //USE_HW_IR
	if(ir_cnt == 32)
	{
		ir_val = get_ir_val();
	}
#endif
	return ir_val;
}

//红外遥控初始化函数
#pragma location="IR_INIT"
void ir_init(void)
{
#if (USE_KEDIR && !USE_HW_IR)
	ir_state = 0;
#endif
	ir_cnt = 0;
	ir_key_sta = 0;
	repeat_cnt = 0;
	ir_data = 0x00;
	ir_code = 0x00;

#if USE_KEDIR
#if USE_HW_IR
#if ((SYS_CRY == CRY_32K) && (CRY_PIN_NUM == CRY_2PIN))
	CLKCON2 = (CLKCON2 | BIT(5)) & (~BIT(4)); //IR clock select XOSC 32KHz（容错性更强）

	IRCFG = 0x5e;    //ONEFULL
	IRCFG = 0x28;    //ZEROCYC
	IRCFG = 0x04;    //REPEATCNT
	IRCFG = 0x08;    //ENDCONT
	IRCFG = 0x12;    //BEGINCNT

	LEDHIGH = 0x00;
#else
	CLKCON2 &= ~(BIT(4)|BIT(5));    //IR clock select PLL 1M
#endif
#endif

	LEDCON2 |= 0x83;                //IR input from comparator, comparator enable, comparator voltage enable
	LEDCON1 = (LEDCON1&0x3f) | (KEDIR_PORT<<6);
#if USE_HW_IR
	KEDIR_ENABLE_IR();
	IRCON |= BIT(1);
	asm("setb 0xBA");
	IE_TM1 = 1;
#else
#if USE_KEDIR && ((USE_KEDIR_TYPE == KEDIR_KEY_IR_LED) || (USE_KEDIR_TYPE == KEDIR_KEY_IR))
	KEDIR_ENABLE_IR();
#endif
	IE_TM1 = 0;
	TMR1CON0 = 0x7F;                //timer1 capture MIX IR
	TMR1CON1 = 0x36;
	TMR1PRH = 0xA1;
	TMR1PRL = 0x22;
	asm("ORL 0XB8,#0X04");
	IE_TM1 = 1;
#endif
	IRCON |= 1;                     //ir enable

#else //USE_KEDIR

#if USE_HW_IR
#if (SYS_CRY == CRY_32K)
	CLKCON2 = (CLKCON2 | BIT(5)) & (~BIT(4)); //IR clock select XOSC 32KHz（容错性更强）

	IRCFG = 0x5e;    //ONEFULL
	IRCFG = 0x28;    //ZEROCYC
	IRCFG = 0x04;    //REPEATCNT
	IRCFG = 0x08;    //ENDCONT
	IRCFG = 0x12;    //BEGINCNT
#else
	CLKCON2 &= ~(BIT(4)|BIT(5));    //IR clock select PLL 1M
#endif

	//IR通道选择:(00-P10; 01-P03; 10-P02; 11-P27)
#if (IR_PORT == IR_P10)
	LEDCON1 &= ~(BIT(7) | BIT(6));  //P10
#elif (IR_PORT == IR_P03)
	LEDCON1 &= ~BIT(7);             //P03
	LEDCON1 |= BIT(6);
#elif (IR_PORT == IR_P02)
	LEDCON1 |= BIT(7);              //P02
	LEDCON1 &= ~BIT(6);
#elif (IR_PORT == IR_P27)
	LEDCON1 |= BIT(7) | BIT(6);     //P27
#endif

	IRCON |= BIT(0) | BIT(1);
	asm("setb 0xBA");
	IE_TM1 = 1;
#else
	P0DIR |= BIT(7);
	IE_TM1 = 0;
	TMR1CON0 = 0x73;                //timer1 capture P07
	TMR1CON1 = 0x36;
	TMR1PRH = 0xA1;
	TMR1PRL = 0x22;
	asm("ORL 0XB8,#0X04");
	IE_TM1 = 1;
#endif

#endif //USE_KEDIR
}

#endif //USE_IR

