#include "include.h"
#include "kedir.h"

#if USE_KEDIR

#if(USE_KEDIR_TYPE == KEDIR_IR_LED)

#pragma constseg="KEDIR_CONST_TABLE"

#ifdef AX207X_TAG
//三合一复用KEY、IR、LED时最多支持7个按键"

#define TINY_MIN 0xA8
#define TINY_MAX 0xF5
#define BIG_MIN  0xB7
#define BIG_MAX  0xF6
#define TOP_MIN  0xBF
#define TOP_MAX  0xF7

#endif//AX207X_TAG


#ifdef AX107X_TAG
//三合一复用KEY、IR、LED时最多支持9个按键

#define TOP_MIN 0xAA
#define TOP_MAX 0xF8
#define BIG_MIN 0x9F
#define BIG_MAX 0xF9
#define TINY_MIN 0x8C
#define TINY_MAX 0xF8

#endif//AX107X_TAG

IAR_CONST u8 kedir_key_range[][2] =
{
	{TINY_MIN, TINY_MAX},
	{TOP_MIN,  TOP_MAX},
	{BIG_MIN,  BIG_MAX}
};

#pragma constseg=default

#endif/*(USE_KEDIR_TYPE == KEDIR_KEY_IR_LED)*/

#endif/*USE_KEDIR*/
