#ifndef  _LED_7P7S_H_
#define  _LED_7P7S_H_

#define LED_7P7S_DIR        P2DIR
#define LED_7P7S_PORT       P2


#define BIT_A           BIT(0)	// P20
#define BIT_B           BIT(1)	// P21
#define BIT_C           BIT(2)	// P25
#define BIT_D           BIT(3)	// P24
#define BIT_E           BIT(4)	// P23
#define BIT_F           BIT(5)	// P26
#define BIT_G           BIT(6)	// P22
#define BIT_DP          BIT(7)	// P22
//#define BIT_H           BIT(7)	// P22


//Õº±Í≈‰÷√
#define LED_7P7S_PLAY   BIT_A
#define LED_7P7S_PAUSE  BIT_B
#define LED_7P7S_USB    BIT_C
#define LED_7P7S_SD     BIT_D
#define LED_7P7S_DDOT   BIT_E
#define LED_7P7S_FM     BIT_F
#define LED_7P7S_MP3    BIT_G

//ASCII ≈‰÷√
#define LED_7P7S__      (BIT_G)
#define LED_7P7S_0      (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F)
#define LED_7P7S_1      (BIT_B | BIT_C)
#define LED_7P7S_2      (BIT_A | BIT_B | BIT_G | BIT_E | BIT_D)
#define LED_7P7S_3      (BIT_A | BIT_B | BIT_G | BIT_C | BIT_D)
#define LED_7P7S_4      (BIT_B | BIT_C | BIT_F | BIT_G)
#define LED_7P7S_5      (BIT_A | BIT_C | BIT_D | BIT_F | BIT_G)
#define LED_7P7S_6      (BIT_A | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G)
#define LED_7P7S_7      (BIT_A | BIT_B | BIT_C)
#define LED_7P7S_8      (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G)
#define LED_7P7S_9      (BIT_A | BIT_B | BIT_C | BIT_D | BIT_F | BIT_G)

#define LED_7P7S_A      (BIT_A | BIT_B | BIT_C | BIT_E | BIT_F | BIT_G)
#define LED_7P7S_b      (BIT_C | BIT_D | BIT_E | BIT_F | BIT_G)
#define LED_7P7S_C      (BIT_A | BIT_D | BIT_E | BIT_F)
#define LED_7P7S_d      (BIT_B | BIT_C | BIT_D | BIT_E | BIT_G)
#define LED_7P7S_E      (BIT_A | BIT_D | BIT_E | BIT_F | BIT_G)
#define LED_7P7S_F      (BIT_A | BIT_E | BIT_F | BIT_G)
#define LED_7P7S_g      (BIT_A | BIT_B | BIT_C | BIT_D | BIT_F | BIT_G)
#define LED_7P7S_H      (BIT_B | BIT_C | BIT_E | BIT_F | BIT_G)
#define LED_7P7S_I      (BIT_B | BIT_C)
#define LED_7P7S_J      (BIT_B | BIT_C | BIT_D | BIT_E)
#define LED_7P7S_K      (BIT_E | BIT_F | BIT_G)
#define LED_7P7S_L      (BIT_D | BIT_E | BIT_F)
#define LED_7P7S_m      (BIT_C | BIT_E | BIT_G)
#define LED_7P7S_n      (BIT_A | BIT_B | BIT_C | BIT_E | BIT_F)
#define LED_7P7S_O      (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F)
#define LED_7P7S_P      (BIT_A | BIT_B | BIT_E | BIT_F | BIT_G)
#define LED_7P7S_q      (BIT_A | BIT_B | BIT_C | BIT_F | BIT_G)
#define LED_7P7S_r      (BIT_E | BIT_G)
#define LED_7P7S_S      (BIT_A | BIT_C | BIT_D | BIT_F | BIT_G)
#define LED_7P7S_t      (BIT_D | BIT_E | BIT_F | BIT_G)
#define LED_7P7S_U      (BIT_B | BIT_C | BIT_D | BIT_E | BIT_F)
#define LED_7P7S_V      (BIT_B | BIT_C | BIT_D | BIT_F | BIT_E)
#define LED_7P7S_w      (BIT_C | BIT_D | BIT_E)
#define LED_7P7S_X      (BIT_B | BIT_C | BIT_E | BIT_F)
#define LED_7P7S_y      (BIT_B | BIT_C | BIT_D | BIT_F | BIT_G)
#define LED_7P7S_Z      (BIT_A | BIT_B | BIT_D | BIT_E | BIT_G)


void led_7p7s_init(void);
void led_7p7s_scan(void);
void led_7p7s_value_set(void);
void led_7p7s_clear(void);

void led_7p7s_disp_str(const char __code *str);
void led_7p7s_dispnum(u16 num);
void led_7p7s_disp_vol(void);
void led_7p7s_disp_time(void);
void led_7p7s_disp_ch(u8 ch);
void led_7p7s_disp_freq(void);
void led_7p7s_disp_device(void);
void led_7p7s_disp_blink(void);
void led_7p7s_disp_play_pause(void);
void led_7p7s_disp_numsel_icon(void);
void led_7p7s_disp_rtc_blink(u8 index, u8 num);
void led_7p7s_disp_flag(u8 flag);
void led_7p7s_disp_energy(void);

void led_7p7s_disp_song(void);
void led_7p7s_disp_timer(void);
void led_7p7s_disp_clock(void);

#endif
