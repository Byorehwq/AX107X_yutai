#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "lcd_seg.h"
#include "led_5c7s.h"
#include "led_7p7s.h"
#include "led.h"

#if (DISP_TYPE==DISP_LED_5C7S)
#define disp_port_init()    led_5c7s_init(); //ocx.led_sta = LED_STA_ON; LED_INIT()
#define disp_scan()         led_5c7s_scan();
#define disp_clear()        led_5c7s_clear()
#define disp_str(n)         led_5c7s_disp_str(n)
#define disp_vol()          led_5c7s_disp_vol()
#define disp_time()         led_5c7s_disp_time()
#define disp_freq()         led_5c7s_disp_freq()
#define disp_channel(n)     led_5c7s_disp_ch(n)
#define disp_device()       led_5c7s_disp_device()
#define disp_numsel(n)      led_5c7s_dispnum(n)
#define disp_play_pause()   led_5c7s_disp_play_pause()
#define disp_numsel_icon()  led_5c7s_disp_numsel_icon()
#define disp_energy()       led_5c7s_disp_energy()

#define showhello()         disp_str("888")
#define shownum(n)          ocx_shownum(n)
#define shownumsel(n)       ocx_shownumsel(n)
#define showvol()           ocx_showvol()
#define showeq()            ocx_showeq()
#define show_playmode()     ocx_show_playmode()
#define showch(n)           ocx_showch(n)
#define showpause()         disp_str("  p");
#define showoff()           disp_str("off");
#define msgbox(n)           ocx_msgbox(n)
#define MAX_DIGITAL_INPUT   999
#define ICON_DOT            LED_5C7S_DDOT
#define ICON_SD             LED_5C7S_SD
#define ICON_USB            LED_5C7S_USB
#define CHAR_HOURH          dispbuf[0]
#define CHAR_HOURL          dispbuf[1]
#define CHAR_MINH           dispbuf[2]
//#define CHAR_MINL           dispbuf[3]
// #define CHAR_ICON           dispbuf[4]


#elif (DISP_TYPE==DISP_LCD_SEG)
#define disp_port_init()
#define disp_scan()         //display_seg()
#define disp_clear()        //lcd_seg_clear()
#define disp_str(str)       //lcd_disp(str)
#define disp_vol()          //lcd_disp_vol()
#define disp_time()         //lcd_disp_time()
#define disp_freq()         //lcd_disp_freq()
#define disp_channel(n)     //lcd_disp_ch(n)
#define disp_device()       //lcd_disp_device()
#define disp_numsel(n)      //lcd_dispnum(n)
#define disp_play_pause()   //lcd_disp_play_pause()
#define disp_numsel_icon()  //lcd_disp_numsel_icon()
#define disp_energy()       //lcd_disp_energy()

#define showhello()         //disp_str(" hi ")
#define shownum(n)          //ocx_shownum(n)
#define shownumsel(n)       //ocx_shownumsel(n)
#define showvol()           //ocx_showvol(n)
#define showeq()            //ocx_showeq(n)
#define show_playmode()     //ocx_show_playmode()
#define showch()            //ocx_showch()
#define showstop()          //disp_str(" stp");
#define showoff()           //disp_str(" off");
#define msgbox(n)           //ocx_msgbox(n)
#define MAX_DIGITAL_INPUT   999
#define ICON_DOT            0
#define ICON_SD
#define ICON_USB

#elif (DISP_TYPE==DISP_LED)
#define disp_port_init()    led_init()
#define disp_scan()         led_scan()
#define disp_clear()
#define disp_str(n)
#define disp_vol()
#define disp_time()
#define disp_freq()
#define disp_channel(n)
#define disp_device()
#define disp_numsel(n)
#define disp_play_pause()
#define disp_numsel_icon()
#define disp_energy()

#define showhello()
#define shownum(n)
#define shownumsel(n)       ocx_shownumsel(n)
#define showvol()
#define showeq()
#define show_playmode()
#define showch(n)
#define showstop()
#define showoff()
#define msgbox(n)

#define MAX_DIGITAL_INPUT   999
#define ICON_DOT            0
#define ICON_SD
#define ICON_USB

#elif (DISP_TYPE==DISP_LED_7P7S)
#define disp_port_init()    led_7p7s_init()//;led_init();
#define disp_scan()         led_7p7s_scan()//;led_scan();
#define disp_clear()        led_7p7s_clear()
#define disp_str(n)         led_7p7s_disp_str(n)
#define disp_vol()          led_7p7s_disp_vol()
#define disp_time()         led_7p7s_disp_time()
#define disp_freq()         led_7p7s_disp_freq()
#define disp_channel(n)     led_7p7s_disp_ch(n)
#define disp_device()       led_7p7s_disp_device()
#define disp_numsel(n)      led_7p7s_dispnum(n)
#define disp_play_pause()   led_7p7s_disp_play_pause()
#define disp_numsel_icon()  led_7p7s_disp_numsel_icon()
#define disp_energy()       led_7p7s_disp_energy()

#define disp_clock()        led_7p7s_disp_clock() //led_7p7s_disp_time()
#define disp_song()			led_7p7s_disp_song()
#define disp_timer()		led_7p7s_disp_timer()

#define showhello()         disp_str(" on")
#define shownum(n)          ocx_shownum(n)
#define shownumsel(n)       ocx_shownumsel(n)
#define showvol()           ocx_showvol()
#define showeq()            ocx_showeq()
#define show_playmode()     ocx_show_playmode()
#define showch(n)           ocx_showch(n)
#define showpause()			disp_str("  p");
#define showstop()          disp_str("stp");
#define showoff()           disp_str("off");
#define msgbox(n)           ocx_msgbox(n)
#define MAX_DIGITAL_INPUT   999
#define ICON_DOT            LED_7P7S_DDOT
#define ICON_SD             LED_7P7S_SD
#define ICON_USB            LED_7P7S_USB
#define CHAR_HOURH          dispbuf[0]
#define CHAR_HOURL          dispbuf[1]
#define CHAR_MINH           dispbuf[2]
#define CHAR_MINL           dispbuf[3]
#define CHAR_ICON           dispbuf[4]

#else
#define disp_port_init()
#define disp_scan()
#define disp_clear()
#define disp_str(n)
#define disp_vol()
#define disp_time()
#define disp_freq()
#define disp_channel(n)
#define disp_device()
#define disp_numsel(n)
#define disp_play_pause()
#define disp_numsel_icon()
#define disp_energy()

#define showhello()
#define shownum(n)
#define shownumsel(n)       ocx_shownumsel(n)
#define showvol()
#define showeq()
#define show_playmode()
#define showch(n)
#define showstop()
#define showoff()
#define msgbox(n)

#define MAX_DIGITAL_INPUT   999
#define ICON_DOT            0
#define ICON_SD
#define ICON_USB
#endif



#define                 BLINK_TIME          100


typedef struct
{
	u16 show_time;                  //��ʾ��ʱ�����

	u8 status;                      //״̬
	u8 disp_status;                 //��ǰ��ʾ��״̬

	u8 blink_time;                  //��˸ʱ�����
	u8 blink_sta;                   //��˸״̬
	u8 blink_icon;                  //��˸����
	u8 blink_num;                   //��˸����
	u8 blink_numh;                  //��˸���ָ�λ
	u8 blink_numl;                  //��˸���ֵ�λ

	u8 event;                       //ocx�¼�����

	struct                          //ʱ����ʾʹ��
	{
		u8 timeh;
		u8 timel;
	};
	u16 number;                      //numboxʹ�ã���Ҫ��ʾ������
	u16 freq;                        //FMƵ����ʾ��־
//#if (DISP_TYPE==DISP_LED)
	u8  led_sta;
//#endif
#if ENERGY_DISP_EN
	u8  disp_energy;
	u8  energy_cnt;                  //������ʾ��
#endif
} type_ocx_ctl;

typedef struct
{
	u8 update:              1;

	//u8 led1;					// led1״̬  1: on  0: off
	//u8 led2;					// led1״̬  1: on  0: off
	//u8 led3;					// led1״̬  1: on  0: off
	u8 display_timer;			// �������£�led������ʾʱ��  10s
	//u8 led1_last;
	//u8 led2_last;
	//u8 led3_last;
	bool display_dp_flag;			// 7����ð����˸��־λ	0: ��  1: ��
	bool display_led_flag;		// 7�����������˸��־λ	0: ��  1: ��
} type_disp_flag;

enum
{
    OCX_NULL,
    OCX_NUMBOX,             //���Կ�
    OCX_NUMSELBOX,          //��ѡ��
    OCX_VOLBOX,             //������
    OCX_MSGBOX,             //��Ϣ��
    OCX_STR,				//�ַ�����
};

enum
{
    NORMAL,             //����
    POP,                //����
    ROCK,               //ҡ��
    JAZZ,               //��ʿ
    CLASSIC,            //�ŵ�
    COUNTRY,            //���
};

extern IAR_IDATA_A type_ocx_ctl ocx;
extern IAR_BDATA_A type_disp_flag disp;

extern u8 count_led_off;  //15mins led off
extern bool Timer1_flag;
extern u8 count_1min;
extern u8 vcmbuf_flag;


void ocx_shownum(u16 number);           //��ʾ���ֱ��
void ocx_shownumsel(u8 number);         //��ʾ��ѡ��
void ocx_showvol(void);                 //��ʾ����
void ocx_showeq(void);                  //��ʾEQ
void ocx_show_playmode(void);           //��ʾ����ģʽ
void ocx_showch(u8 ch);                 //��ʾƵ��
void ocx_showstop(void);                //��ʾ����ֹͣ����
void ocx_msgbox(const char __code *str);
void ocx_show_device(void);


void ocx_time_set(u8 timeh, u8 timel);
void ocx_blink_set(u8 icon, u8 num);
void ocx_energy_set(const u16 __code *tbl); //������ʾ����
void ocx_freq_set(u16 freq);                //FMƵ������
void ocx_delay(void);                       //�ؼ���ʱ��5ms����һ��
void ocx_clear(void);
void ocx_event(void);                       //�ؼ��¼�
void ocx_display(void);                     //�ؼ���ʾ

void disp_init(void);

#endif

