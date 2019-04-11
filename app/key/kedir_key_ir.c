#include "include.h"
#include "kedir.h"

#if USE_KEDIR

#if(USE_KEDIR_TYPE == KEDIR_KEY_IR)

#pragma constseg="KEDIR_CONST_TABLE"

#ifdef AX207X_TAG
//三合一复用KEY、IR时最多支持15个按键

#define TINY_MIN 0xFF
#define TINY_MAX 0xFF
#define BIG_MIN  0x00
#define BIG_MAX  0xF6
#define TOP_MIN  0x27
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
	{0x54,     T_KEY_PLAY},     //150k
	{0x84,     T_KEY_VOL_UP},   //47k
	{0xB3,     T_KEY_VOL_DOWN}, //20k
	{0xD6,     T_KEY_NEXT},     //9.1k
	{0xED,     T_KEY_PREV},     //3.9k
	{TOP_MAX,  T_KEY_MODE},     //1.3k
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	//BIG
	{BIG_MIN,  NO_KEY},
	{0x09,     T_KEY_EQ},       //0k
	{0x14,     T_KEY_PLAYMODE}, //0.75k
	{0x2B,     NO_KEY},         //2.4k
	{0x4A,     NO_KEY},         //5.6k
	{0x71,     NO_KEY},         //11k
	{0x9C,     NO_KEY},         //22k
	{0xC4,     NO_KEY},         //43k
	{0xE2,     NO_KEY},         //91k
	{BIG_MAX,  NO_KEY},         //220k
	{0xFF,     NO_KEY},
};
#endif//AX207X_TAG

#ifdef AX107X_TAG
//三合一复用KEY、IR时最多支持15个按键

#define TINY_MIN 0xFF
#define TINY_MAX 0xFF
#define BIG_MIN  0x00
#define BIG_MAX  0xF8
#define TOP_MIN  0x2C
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
	{0x54,     NO_KEY},                 //130K
	{0x7C,     T_KEY_PLAYMODE},         //51K
	{0xA5,     T_KEY_VOL_UP},           //24K
	{0xC7,     T_KEY_VOL_DOWN},         //12K
	{0xE2,     T_KEY_NEXT},             //5.6K
	{TOP_MAX,  T_KEY_PREV},             //2K
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	{0xFF,     NO_KEY},
	//BIG
	{0x07,     T_KEY_PLAY},             //0k
	{0x18,     T_KEY_PLAY_AND_MODE},    //1.2K
	{0x31,     T_KEY_MODE},             //3.3K
	{0x4F,     T_KEY_EQ},               //6.8K
	{0x72,     T_KEY_MINUS},            //12K
	{0x96,     T_KEY_PLUS},             //22K
	{0xBB,     NO_KEY},                 //39K
	{0xDC,     NO_KEY},                 //82K
	{BIG_MAX,  NO_KEY},                 //220K
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

#endif/*(USE_KEDIR_TYPE == KEDIR_KEY_IR)*/

#endif/*USE_KEDIR*/
