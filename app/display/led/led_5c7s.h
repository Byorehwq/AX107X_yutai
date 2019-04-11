#ifndef  _LED_5COM7SEG_H_
#define  _LED_5COM7SEG_H_


/*----------------------------------------------------------------------------*/
/* 以下是自动定义的宏 */
/*----------------------------------------------------------------------------*/
// 共阴极LED屏
// COM0 = P43 = SCH.SEG5 = LED.P12
// COM1 = P04 = SCH.SEG1 = LED.P8
// COM2 = P05 = SCH.SEG2 = LED.P9

// SEG_A = P42 = SCH.SEG4 = LED.P11
// SEG_B = P06 = SCH.SEG0 = LED.P7
// SEG_C = P23 = SCH.SEG9 = LED.P4
// SEG_D = P25 = SCH.SEG7 = LED.P2
// SEG_E = P26 = SCH.SEG6 = LED.P1
// SEG_F = P03 = SCH.SEG3 = LED.P10
// SEG_G = P22 = SCH.SEG10 = LED.P5
// SEG_DP = P24 = SCH.SEG8 = LED.P3

#define	LED_5C7S_IO_SET	1
#define LED_5C7S_C0_BIT         BIT(3)		// P43	DP2
#define LED_5C7S_C1_BIT         BIT(4)		// P04
#define LED_5C7S_C2_BIT         BIT(5)		// P05


#define LED_5C7S_C0_PORT                P4
#define LED_5C7S_C1_PORT                P0
#define LED_5C7S_C2_PORT                P0


#if LED_5C7S_EXT_RES_EN
#define SET_LED_5C7S_S_DIR  LED_5C7S_S_DIR &= ~LED_5C7S_S_MASK;
#define LED_5C7S_S_ON       LED_5C7S_S_PORT &= ~LED_5C7S_S_MASK;
#else //LED_5C7S_EXT_RES_EN


#endif //LED_5C7S_EXT_RES_EN

// P42/3, P03~6, P22~6
// SEG = P42, P03/6, P22~6
#define LED_5C7S_PORT_INIT()    P4DIR &= 0XF3; P0DIR &= 0X87; P2DIR &= 0X83

//#define ALL_LED_5C7S_ON()

// COM = P43, P04/5
#define ALL_LED_5C7S_OFF()      P4 &= 0XF3; P0 &= 0X87; P2 &= 0X83; P4 |= 0X08; P0 |= 0X30


/*----------------------------------------------------------------------------*/
/* 字符和图标定义 */
/*----------------------------------------------------------------------------*/
/*
#define BIT_A           BIT(0)	// P20
#define BIT_B           BIT(1)	// P21
#define BIT_C           BIT(2)	// P25
#define BIT_D           BIT(3)	// P24
#define BIT_E           BIT(4)	// P23
#define BIT_F           BIT(5)	// P26
#define BIT_G           BIT(6)	// P22
#define BIT_DP           BIT(7)	// P22
//#define BIT_H           BIT(7)	// P22

//图标配置
#define LED_5C7S_PLAY   BIT_A
#define LED_5C7S_PAUSE  BIT_B
#define LED_5C7S_USB    BIT_C
#define LED_5C7S_SD     BIT_D
#define LED_5C7S_DDOT   BIT_E
#define LED_5C7S_MP3    BIT_F
#define LED_5C7S_MHZ    BIT_G


//ASCII 配置
#define LED_5C7S__      (BIT_G)
#define LED_5C7S_0      (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F)
#define LED_5C7S_1      (BIT_B | BIT_C)
#define LED_5C7S_2      (BIT_A | BIT_B | BIT_G | BIT_E | BIT_D)
#define LED_5C7S_3      (BIT_A | BIT_B | BIT_G | BIT_C | BIT_D)
#define LED_5C7S_4      (BIT_B | BIT_C | BIT_F | BIT_G)
#define LED_5C7S_5      (BIT_A | BIT_C | BIT_D | BIT_F | BIT_G)
#define LED_5C7S_6      (BIT_A | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G)
#define LED_5C7S_7      (BIT_A | BIT_B | BIT_C)
#define LED_5C7S_8      (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G)
#define LED_5C7S_9      (BIT_A | BIT_B | BIT_C | BIT_D | BIT_F | BIT_G)

#define LED_5C7S_A      (BIT_A | BIT_B | BIT_C | BIT_E | BIT_F | BIT_G)
#define LED_5C7S_b      (BIT_C | BIT_D | BIT_E | BIT_F | BIT_G)
#define LED_5C7S_C      (BIT_A | BIT_D | BIT_E | BIT_F)
#define LED_5C7S_d      (BIT_B | BIT_C | BIT_D | BIT_E | BIT_G)
#define LED_5C7S_E      (BIT_A | BIT_D | BIT_E | BIT_F | BIT_G)
#define LED_5C7S_F      (BIT_A | BIT_E | BIT_F | BIT_G)
#define LED_5C7S_g      (BIT_A | BIT_B | BIT_C | BIT_D | BIT_F | BIT_G)
#define LED_5C7S_H      (BIT_B | BIT_C | BIT_E | BIT_F | BIT_G)
#define LED_5C7S_I      (BIT_B | BIT_C)
#define LED_5C7S_J      (BIT_B | BIT_C | BIT_D | BIT_E)
#define LED_5C7S_K      (BIT_E | BIT_F | BIT_G)
#define LED_5C7S_L      (BIT_D | BIT_E | BIT_F)
#define LED_5C7S_m      (BIT_C | BIT_E | BIT_G)
#define LED_5C7S_n      (BIT_A | BIT_B | BIT_C | BIT_E | BIT_F)
#define LED_5C7S_O      (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F)
#define LED_5C7S_P      (BIT_A | BIT_B | BIT_E | BIT_F | BIT_G)
#define LED_5C7S_q      (BIT_A | BIT_B | BIT_C | BIT_F | BIT_G)
#define LED_5C7S_r      (BIT_E | BIT_G)
#define LED_5C7S_S      (BIT_A | BIT_C | BIT_D | BIT_F | BIT_G)
#define LED_5C7S_t      (BIT_D | BIT_E | BIT_F | BIT_G)
#define LED_5C7S_U      (BIT_B | BIT_C | BIT_D | BIT_E | BIT_F)
#define LED_5C7S_V      (BIT_B | BIT_C | BIT_D | BIT_F | BIT_E)
#define LED_5C7S_w      (BIT_C | BIT_D | BIT_E)
#define LED_5C7S_X      (BIT_B | BIT_C | BIT_E | BIT_F)
#define LED_5C7S_y      (BIT_B | BIT_C | BIT_D | BIT_F | BIT_G)
#define LED_5C7S_Z      (BIT_A | BIT_B | BIT_D | BIT_E | BIT_G)


extern IAR_DATA_A u8 dispbuf[5];

void led_5c7s_init(void);
void led_5c7s_clear(void);
void led_5c7s_scan(void);

void led_5c7s_disp_str(const char __code *str);
void led_5c7s_dispnum(u16 num);
void led_5c7s_disp_vol(void);
void led_5c7s_disp_time(void);
void led_5c7s_disp_ch(u8 ch);
void led_5c7s_disp_freq(void);
void led_5c7s_disp_device(void);
void led_5c7s_disp_play_pause(void);
void led_5c7s_disp_numsel_icon(void);
void led_5c7s_disp_energy(void);
*/
#endif

