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
	u16 show_time;                  //显示的时间控制

	u8 status;                      //状态
	u8 disp_status;                 //当前显示的状态

	u8 blink_time;                  //闪烁时间控制
	u8 blink_sta;                   //闪烁状态
	u8 blink_icon;                  //闪烁控制
	u8 blink_num;                   //闪烁数字
	u8 blink_numh;                  //闪烁数字高位
	u8 blink_numl;                  //闪烁数字低位

	u8 event;                       //ocx事件发生

	struct                          //时间显示使用
	{
		u8 timeh;
		u8 timel;
	};
	u16 number;                      //numbox使用，需要显示的数字
	u16 freq;                        //FM频率显示标志
//#if (DISP_TYPE==DISP_LED)
	u8  led_sta;
//#endif
#if ENERGY_DISP_EN
	u8  disp_energy;
	u8  energy_cnt;                  //能量显示级
#endif
} type_ocx_ctl;

typedef struct
{
	u8 update:              1;

	//u8 led1;					// led1状态  1: on  0: off
	//u8 led2;					// led1状态  1: on  0: off
	//u8 led3;					// led1状态  1: on  0: off
	u8 display_timer;			// 按键按下，led灯需显示时间  10s
	//u8 led1_last;
	//u8 led2_last;
	//u8 led3_last;
	bool display_dp_flag;			// 7脚屏冒号闪烁标志位	0: 亮  1: 灭
	bool display_led_flag;		// 7脚屏数码管闪烁标志位	0: 亮  1: 灭
} type_disp_flag;

enum
{
    OCX_NULL,
    OCX_NUMBOX,             //数显框
    OCX_NUMSELBOX,          //数选框
    OCX_VOLBOX,             //音量框
    OCX_MSGBOX,             //消息框
    OCX_STR,				//字符串框
};

enum
{
    NORMAL,             //正常
    POP,                //流行
    ROCK,               //摇滚
    JAZZ,               //爵士
    CLASSIC,            //古典
    COUNTRY,            //乡村
};

extern IAR_IDATA_A type_ocx_ctl ocx;
extern IAR_BDATA_A type_disp_flag disp;

extern u8 count_led_off;  //15mins led off
extern bool Timer1_flag;
extern u8 count_1min;
extern u8 vcmbuf_flag;


void ocx_shownum(u16 number);           //显示数字编号
void ocx_shownumsel(u8 number);         //显示数选框
void ocx_showvol(void);                 //显示音量
void ocx_showeq(void);                  //显示EQ
void ocx_show_playmode(void);           //显示播放模式
void ocx_showch(u8 ch);                 //显示频道
void ocx_showstop(void);                //显示音乐停止界面
void ocx_msgbox(const char __code *str);
void ocx_show_device(void);


void ocx_time_set(u8 timeh, u8 timel);
void ocx_blink_set(u8 icon, u8 num);
void ocx_energy_set(const u16 __code *tbl); //能量显示设置
void ocx_freq_set(u16 freq);                //FM频率设置
void ocx_delay(void);                       //控件延时，5ms调用一次
void ocx_clear(void);
void ocx_event(void);                       //控件事件
void ocx_display(void);                     //控件显示

void disp_init(void);

#endif

