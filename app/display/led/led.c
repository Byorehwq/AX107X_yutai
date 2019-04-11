/*****************************************************************************
* Module    : Display
* File      : led.c
* Author    :
* Email     :
* Function  : LED��ʾ
*****************************************************************************/
#include "include.h"
#include "kedir.h"

//#if (DISP_TYPE==DISP_LED)

void led_init(void) AT(LED_SEG);
void led_init(void)
{
	ocx.led_sta = LED_STA_OFF;
	LED_INIT();
}


void led_scan(void) AT(LED_SEG);
void led_scan(void)
{
	static u8 scan_cnt = 0;
	scan_cnt++;

	switch(ocx.led_sta)
	{
	case LED_STA_ON:            //��
		LED_ON();
		break;

	case LED_STA_FAST_GLITTER:  //����(��200ms����200ms)
		if(scan_cnt >= 100)
		{
			scan_cnt = 0;
			LED_GLITTER();
		}
		break;

	case LED_STA_SLOW_GLITTER:  //����(��500ms����500ms)
		if(scan_cnt >= 250)
		{
			scan_cnt = 0;
			LED_GLITTER();
		}
		break;

	case LED_STA_OFF:           //��
	default:
		scan_cnt = 0;
		LED_OFF();
		break;

	}
}
//#endif


