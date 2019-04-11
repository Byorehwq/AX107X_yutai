#include "include.h"

#pragma constseg="KEY_CONST_TABLE"

#if USE_ADKEY
#if (USER_SEL == USER_AX1071_DEMO)
IAR_CONST type_adc_key tbl_adkey[] =
{
	{0x0B, T_KEY_2},            //PLAY      S1
	{0x1F, T_KEY_17},           //PLAY/MODE S2
	{0x34, T_KEY_6},            //MODE      S3
	{0x4A, T_KEY_4},            //VOL-      S4
	{0x61, T_KEY_5},            //VOL+      S5
	{0x76, T_KEY_1},            //PREV      S6
	{0x8B, T_KEY_3},            //NEXT      S7
	{0xA0, T_KEY_15},           //MINUS     S8
	{0xB5, T_KEY_16},           //PLUS      S9
	{0xCA, T_KEY_7},            //EQ        S10
	{0xDF, T_KEY_8},            //PLAYMODE  S11
	{0xF4, NO_KEY},             //          S12
	{0xFF, NO_KEY},             //          END
};
#else
IAR_CONST type_adc_key tbl_adkey[] =
{
	{0x0B, NO_KEY},            //PREV S1
	{0x1F, NO_KEY},            //PLAY S2
	{0x34, NO_KEY},            //NEXT S3
	{0x4A, NO_KEY},            //VOL- S4
	{0x61, NO_KEY},            //VOL+ S5
	{0x76, NO_KEY},            //MODE S6
	{0x8B, NO_KEY},            //EQ   S7
	{0xA0, NO_KEY},            //S8
	{0xB5, NO_KEY},            //S9
	{0xCA, NO_KEY},           //S10
	{0xDF, NO_KEY},             //S11
	{0xF4, NO_KEY},             //S12
	{0xFF, NO_KEY},             //END
};
#endif
#endif

#if USE_ADKEY2
IAR_CONST type_adc_key tbl_adkey2[] =
{
#ifdef AX207X_TAG
// FOR DEBUG: 0, 15H, 2BH, 40H
	{0x0A, T_KEY_PLAY},				//PREV S1 AD:0x00
	{0x36, T_KEY_PLAYMODE},			//NEXT S3 AD:0x2B
	{0x48, T_KEY_PREV_VOL_DOWN},	//VOL- S4 AD:0x40
	{0x64, T_KEY_NEXT_VOL_UP},		//VOL+ S5 AD:0x55
	{0x78, T_KEY_POWER},			//Power S6 AD:0x6B
	{0xFF, NO_KEY},					//END
#else
	// 重新编定阻值范围, 让 POWER的事件不会因为按键的变化过程中产生触发
	{66, T_KEY_POWER},				// Diode,	0.60v	// 此电压值相当於 2K电阻的使用
	{107, T_KEY_PLAY},				// 5K1,	1.12v
	{151, T_KEY_PLAYMODE},			// 10K,	1.65v
	{192, T_KEY_NEXT_VOL_UP},		// 22K,	2.26v
	{232, T_KEY_PREV_VOL_DOWN},		// 47K,	2.72v
	{0xFF, NO_KEY},					//END
#endif
};
#endif

IAR_CONST type_adc_key tbl_ad_song[] =
{								//	实际值	中间值
	{0x0D, NO_KEY},            	//S1   	0	0x0D (13)
	{0x29, T_KEY_1},            //S2		27	0x29 (41)
	{0x46, T_KEY_2},            //S3		56	0x46 (70)
	{0x62, T_KEY_3},            //S4		84	0x62 (98)
	{0x7E, T_KEY_4},            //S5		112	0x7E (126)
	{0x9A, T_KEY_5},            //S6		140	0x9A (154)
	{0xB7, T_KEY_6},            //S7		169	0xB7 (183)
	{0xD3, T_KEY_7},            //S8		197	0xD3 (211)
	{0xEF, T_KEY_8},            //S9		225	0xEF (239)
	{0xFF, T_KEY_9},           	//S10	254	0xFF
//	{0xFF, T_KEY_10},           //S11
//	{0xFF, T_KEY_11},           //S12
	{0xFF, NO_KEY},             //END
};

IAR_CONST type_adc_key tbl_ad_sel[] =
{								//	实际值	中间值
	{0x2A, NO_KEY},            	//S1   	0	0x2A (42)
	{0x7E, T_KEY_11},           //S2		84	0x7E (126)
	{0xD3, T_KEY_12},           //S3		169	0xD3 (211)
	{0xFE, T_KEY_13},           //S4		254	0xFE (254)
	{0xFF, NO_KEY},             //END
};


#pragma constseg=default



