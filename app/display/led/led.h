#ifndef _LED_H_
#define _LED_H_

enum
{
	LED_STA_OFF,            //LEDÃð
    LED_STA_ON,             //LEDÁÁ
    LED_STA_FAST_GLITTER,   //LED¿ìÉÁ
    LED_STA_SLOW_GLITTER,   //LEDÂýÉÁ
};

#if ( (USE_KEDIR) && ( (USE_KEDIR_TYPE == KEDIR_KEY_IR_LED) || (USE_KEDIR_TYPE == KEDIR_KEY_LED) || (USE_KEDIR_TYPE == KEDIR_IR_LED) ))
#define LED_INIT()        {LEDCON |= BIT(6);KEDIR_PDIR &= ~BIT(KEDIR_BIT);KEDIR_P &= ~BIT(KEDIR_BIT);}
#define LED_ON()           LEDCON  &= ~BIT(4)
#define LED_OFF()          LEDCON  |=  BIT(4)
#define LED_GLITTER()      LEDCON  ^=  BIT(4)
#else
#define LED_INIT()			P0DIR&=~BIT(1),PUP0|=BIT(1),P0&=~BIT(1)
#define LED_ON()			P0DIR&=~BIT(1),P0|=BIT(1)
#define LED_OFF()			P0DIR&=~BIT(1),P0&=~BIT(1)
#define LED_GLITTER()		P0DIR&=~BIT(1),P0^=BIT(1)
#endif

void led_init(void);
void led_scan(void);


#endif


