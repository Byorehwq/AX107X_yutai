#ifndef _KEDIR_H_
#define _KEDIR_H_

#if (KEDIR_PORT == KEDIR_P10)
#define KEDIR_PDIR                      P1DIR
#define KEDIR_P                         P1
#define KEDIR_BIT                       0
#elif (KEDIR_PORT == KEDIR_P03)
#define KEDIR_PDIR                      P0DIR
#define KEDIR_P                         P0
#define KEDIR_BIT                       3
#elif (KEDIR_PORT == KEDIR_P02)
#define KEDIR_PDIR                      P0DIR
#define KEDIR_P                         P0
#define KEDIR_BIT                       2
#elif (KEDIR_PORT == KEDIR_P27)
#define KEDIR_PDIR                      P2DIR
#define KEDIR_P                         P2
#define KEDIR_BIT                       7
#endif

#define KEDIR_ENABLE_EOC()                  LEDCON1 |=  BIT(4)
#define KEDIR_DISABLE_EOC()                 LEDCON1 &= ~BIT(4)

#define KEDIR_DISABLE_TINY()                LEDCON1 &= ~BIT(0)
#define KEDIR_ENABLE_TINY()                 LEDCON1 |=  BIT(0)
#define KEDIR_DISABLE_BIG()                 LEDCON1 &= ~BIT(1)
#define KEDIR_ENABLE_BIG()                  LEDCON1 |=  BIT(1)
#define KEDIR_DISABLE_TOP()                 LEDCON1 &= ~BIT(2)
#define KEDIR_ENABLE_TOP()                  LEDCON1 |=  BIT(2)

#if SRAM_DEBUG
#define KEDIR_DISABLE_TINY_1()              KEDIR_DISABLE_TINY()
#define KEDIR_ENABLE_TINY_1()               KEDIR_ENABLE_TINY()
#define KEDIR_DISABLE_BIG_1()               KEDIR_DISABLE_BIG()
#define KEDIR_ENABLE_BIG_1()                KEDIR_ENABLE_BIG()
#else
#define KEDIR_DISABLE_TINY_1()              LEDCON1 |= BIT(0)
#define KEDIR_ENABLE_TINY_1()               LEDCON1 &= ~BIT(0)
#define KEDIR_DISABLE_BIG_1()               LEDCON1 |= BIT(1)
#define KEDIR_ENABLE_BIG_1()                LEDCON1 &= ~BIT(1)
#endif


#define KEDIR_SCAN_BIG()                    KEDIR_ENABLE_BIG();   KEDIR_DISABLE_TINY();   KEDIR_DISABLE_TOP()
#define KEDIR_SCAN_BIG_1()                  KEDIR_ENABLE_BIG_1(); KEDIR_DISABLE_TINY_1(); KEDIR_DISABLE_TOP()
#define KEDIR_SCAN_TOPB()                   KEDIR_ENABLE_BIG();   KEDIR_DISABLE_TINY();   KEDIR_ENABLE_TOP()
#define KEDIR_SCAN_TOPB_1()                 KEDIR_ENABLE_BIG_1(); KEDIR_DISABLE_TINY_1(); KEDIR_ENABLE_TOP()
#define KEDIR_SCAN_TOPO()                   KEDIR_DISABLE_BIG();  KEDIR_DISABLE_TINY();   KEDIR_ENABLE_TOP()
#define KEDIR_SCAN_TOPO_1()                 KEDIR_DISABLE_BIG_1();KEDIR_DISABLE_TINY_1(); KEDIR_ENABLE_TOP()

#define KEDIR_ENABLE_CAPTURE()              LEDCON1 |=  BIT(5)
#define KEDIR_DISABLE_CAPTURE()             LEDCON1 &= ~BIT(5)

#define KEDIR_ADCGO()                       LEDCON |= BIT(7)

#define KEDIR_CLEAR()                       LEDCON = 0
#define KEDIR_ENABLE_IR()                   LEDCON |= BIT(3)
#define KEDIR_ENABLE_KEY()                  LEDCON |= BIT(5)
#define KEDIR_ENABLE_LED()                  LEDCON |= BIT(6)
#define KEDIR_LED_OFF()                     LEDCON |= BIT(4)



extern IAR_CONST type_adc_key tbl_key1[];
void kedir_init(void);
void key_keidr_scan(void);
u8 kedir_group_num(void);
u8 kedir_process(u8 mode);
u8 kedir_process_sram(u8 mode);
bool kedir_pause(void);
void kedir_resume(void);

#endif
