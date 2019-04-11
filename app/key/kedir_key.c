#include "include.h"
#include "kedir.h"

#if USE_KEDIR

#if(USE_KEDIR_TYPE == KEDIR_KEY)

#pragma constseg="KEDIR_CONST_TABLE"

#ifdef AX207X_TAG
//三合一只用KEY时最多支持16个按键

#define TINY_MIN 0xFF
#define TINY_MAX 0xFF
#define BIG_MIN  0x00
#define BIG_MAX  0xF6
#define TOP_MIN  0x13
#define TOP_MAX  0xFB

//adc_val必须从小到大排列
IAR_CONST type_adc_key tbl_key1[] =
{
	//TINY,此组不可用
	{TINY_MIN, NO_KEY},
	{TINY_MAX, NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	//TOP
	{TOP_MIN,  NO_KEY},
	{0x33,     T_KEY_PLAY},     //180k
	{0x5C,     T_KEY_VOL_UP},   //75k
	{0x89,     T_KEY_VOL_DOWN}, //36k
	{0xB2,     T_KEY_NEXT},     //18k
	{0xD4,     T_KEY_PREV},     //9.1k
	{0xED,     T_KEY_MODE},     //3.9k
	{TOP_MAX,  T_KEY_EQ},       //1.3k
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	//BIG
	{BIG_MIN,  NO_KEY},
	{0x08,     T_KEY_PLAYMODE}, //0k
	{0x11,     NO_KEY},         //0.68k
	{0x22,     NO_KEY},         //2k
	{0x3C,     NO_KEY},         //4.3k
	{0x62,     NO_KEY},         //9.1k
	{0x8F,     NO_KEY},         //18k
	{0xBA,     NO_KEY},         //39k
	{0xDF,     NO_KEY},         //82k
	{BIG_MAX,  NO_KEY},         //220k
	{0xFF,     NO_KEY},
};
#endif//AX207X_TAG

#ifdef AX107X_TAG
//三合一只用KEY时最多支持16个按键

#define TINY_MIN 0xFF
#define TINY_MAX 0xFF
#define BIG_MIN  0x00
#define BIG_MAX  0xF8
#define TOP_MIN  0x1A
#define TOP_MAX  0xF8

//adc_val必须从小到大排列
IAR_CONST type_adc_key tbl_key1[] =
{
	//TINY,此组不可用
	{TINY_MIN, NO_KEY},
	{TINY_MAX, NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	//TOP
	{TOP_MIN,  NO_KEY},
	{0x3A,     T_KEY_PLAY},     //150k
	{0x5F,     T_KEY_VOL_UP},   //68k
	{0x85,     T_KEY_VOL_DOWN}, //36k
	{0xA9,     T_KEY_NEXT},     //20k
	{0xC8,     T_KEY_PREV},     //11k
	{0xE2,     T_KEY_MODE},     //5.6k
	{TOP_MAX,  T_KEY_EQ},       //2k
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	//BIG
	{0x07,     T_KEY_PLAYMODE}, //0k
	{0x1B,     NO_KEY},         //1.5k
	{0x34,     NO_KEY},         //3.9k
	{0x50,     NO_KEY},         //7.5k
	{0x70,     NO_KEY},         //13k
	{0x92,     NO_KEY},         //22k
	{0xB7,     NO_KEY},         //39k
	{0xDA,     NO_KEY},         //82k
	{BIG_MAX,  NO_KEY},         //220k
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
};
#endif//AX107X_TAG

IAR_CONST u8 kedir_key_range[][2] =
{
	{TINY_MIN, TINY_MAX},
	{TOP_MIN,  TOP_MAX},
	{BIG_MIN,  BIG_MAX}
};

#pragma constseg=default

u8 kedir_group_num(void)
{
	return sizeof(tbl_key1)/sizeof(type_adc_key)/3;
}

#endif/*(USE_KEDIR_TYPE == KEDIR_KEY)*/

#endif/*USE_KEDIR*/
