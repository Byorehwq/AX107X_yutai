#include "include.h"
#include "kedir.h"

#if USE_KEDIR

#if(USE_KEDIR_TYPE == KEDIR_KEY_LED)

#pragma constseg="KEDIR_CONST_TABLE"

#ifdef AX207X_TAG
//三合一复用KEY、LED时最多支持7个按键

#define TINY_MIN 0xA8
#define TINY_MAX 0xF5
#define BIG_MIN  0xB7
#define BIG_MAX  0xF6
#define TOP_MIN  0xBF
#define TOP_MAX  0xF7

//adc_val必须从小到大排列
IAR_CONST type_adc_key tbl_key1[] =
{
	//TINY
	{TINY_MIN, NO_KEY},
	{0xC9,     T_KEY_PLAY},     //3.6k
	{0xE3,     T_KEY_VOL_UP},   //7.5k
	{TINY_MAX, T_KEY_VOL_DOWN}, //18k
	{0xFF,     NO_KEY},
	//TOP
	{TOP_MIN,  NO_KEY},
	{0xE2,     T_KEY_NEXT},     //9.1k
	{TOP_MAX,  T_KEY_PREV},     //2.2k
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	//BIG
	{BIG_MIN,  NO_KEY},
	{0xDE,     T_KEY_MODE},     //82k
	{BIG_MAX,  T_KEY_EQ},       //220k
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
};
#endif//AX207X_TAG

#ifdef AX107X_TAG
//三合一复用KEY、LED时最多支持9个按键

#define TOP_MIN  0xAA
#define TOP_MAX  0xF8
#define BIG_MIN  0x9F
#define BIG_MAX  0xF9
#define TINY_MIN 0x8C
#define TINY_MAX 0xF8

//adc_val必须从小到大排列
IAR_CONST type_adc_key tbl_key1[] =
{
	// TINY 0
	{TINY_MIN, NO_KEY},
	{0xBC, T_KEY_PLAY},         //3.3k
	{0xDF, T_KEY_VOL_UP},       //8.2k
	{TINY_MAX, T_KEY_VOL_DOWN}, //22k
	{0xFF, NO_KEY},
	{0xFF, NO_KEY},
	// TOP 1
	{TOP_MIN, NO_KEY},
	{0xC9, T_KEY_NEXT},         //24k
	{0xE2, T_KEY_PREV},         //7.5k
	{TOP_MAX, NO_KEY},          //2.4k
	{0xFF, NO_KEY},
	{0xFF, NO_KEY},
	// BIG 2
	{BIG_MIN, NO_KEY},
	{0xBF, T_KEY_MODE},         //43k
	{0xDF, T_KEY_EQ},           //82k
	{BIG_MAX, T_KEY_PLAYMODE},  //220k
	{0xFF, NO_KEY},
	{0xFF, NO_KEY},
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

#endif/*(USE_KEDIR_TYPE == KEDIR_KEY_LED)*/

#endif/*USE_KEDIR*/
