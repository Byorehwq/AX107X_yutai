/*****************************************************************************
 * Module    : user
 * File      : user_key.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 按键操作的相关函数定义
 *****************************************************************************/
#ifndef USER_KEY_H
#define USER_KEY_H


#define ADKEDIR_CHAN            0x00        //mix IO input(P10,P03,P02,P27), ADC0
#define ADKEY0_CHAN             0x01        //P01, ADC1
#define ADKEY_POWER_CHAN        0x01        //P01, ADC1
#define ADHPDECT_CHAN           0x01        //P01, ADC1	// 2017/05/12 update
#define ADSDDECT_CHAN           0x04        //P16, ADC4
#define ADBAT_CHAN              0x05        //LDO in, ADC5
#define ADBAND_GAP_CHAN         0x06        //LDO Band GAP, ADC6
#define ADSONG_CHAN				0x01		//P42, ADC9	
#define ADSEL_CHAN				0x02		//P43, ADC10

void ir_init(void);
void key_init(void);
extern void key_scan(void);
extern void KEY_Whire1(void);
extern void KEY_Whire2(void);
void touch_key_init(void);
void touch_key_start(void);
void touch_key_rest(void);
u8 get_bat_val(void);
u8 key_deal(u8 key_val);
u8 get_pwrkey_adcval(void);
void IO_KeyScan(void);

extern s8 SW1_WHIRL_cnt;	// A1B1 的计数旋转值, 正转与反转数, 搭配事件 T_KEY_SW1_WHIRL 操作
extern s8 SW2_WHIRL_cnt;	// A2B2 的计数旋转值, 正转与反转数, 搭配事件 T_KEY_SW2_WHIRL 操作
extern s8 SW3_WHIRL_cnt;	// A3B3 的计数旋转值, 正转与反转数, 搭配事件 T_KEY_SW3_WHIRL 操作


enum
{
    ADC_TINY,   //20K上拉 + 2.2K上拉
    ADC_TOP,    //20K上拉 + 30k下拉
    ADC_BIG,    //20K上拉
};

enum
{
    ADC_KEDIR,		// 0
    ADC_KEY,		// 1
    ADC_KEY2,		// 2
    ADC_KEYPOWER,   // 3 软开关的power键AD采样
    ADC_BAT,		// 4
    ADC_BAND_GAP,	// 5
    ADC_SD_DECT,	// 6
    ADC_HP_DECT,    // 7 耳机检测 (use vcmbuf)
    ADC_SONG,       // 8 曲目选择
	ADC_SEL,		// 9 定时选择
	ADC_NULL,		// 10 空
};

enum
{
    VOL_INIT,              //未完成电压判断
    VOL_OTHER,             //其它值
    VOL_LESS_31V,          //电压小于3.1v
    VOL_LESS_32V,          //电压小于3.2v
    VOL_LESS_33V,          //电压小于3.3v
    VOL_LESS_34V,          //电压小于3.4v
    VOL_LESS_35V,          //电压小于3.5v
    VOL_LESS_36V,          //电压小于3.6v
    VOL_LESS_37V,          //电压小于3.7v
    VOL_LESS_38V,          //电压小于3.8v
    VOL_LESS_39V,          //电压小于3.9v
    VOL_LESS_40V,          //电压小于4.0v
    VOL_LESS_41V,          //电压小于4.1v
    VOL_LESS_42V,          //电压小于4.2v
    VOL_LESS_43V,          //电压小于4.3v
    VOL_LESS_44V,          //电压小于4.4v
    VOL_GREATER_44V,       //电压大于4.4v
};
#define VOL_AD_30V    150  //3.0v
#define VOL_AD_31V    155  //3.1v
#define VOL_AD_32V    160  //3.2v
#define VOL_AD_33V    165  //3.3v
#define VOL_AD_34V    170  //3.4v
#define VOL_AD_35V    175  //3.5v
#define VOL_AD_36V    180  //3.6v
#define VOL_AD_37V    185  //3.7v
#define VOL_AD_38V    190  //3.8v
#define VOL_AD_39V    195  //3.9v
#define VOL_AD_40V    200  //4.0v
#define VOL_AD_41V    205  //4.1v
#define VOL_AD_42V    210  //4.2v
#define VOL_AD_43V    215  //4.3v
#define VOL_AD_44V    220  //4.4v
#define VOL_AD_45V    225  //4.5v

typedef struct
{
	u8 adc_val;
	u8 key_code;
} type_adc_key;

typedef struct
{
	u8 adc_ch;
	u8 adc_val;
	u8 kedir_ch;
	u8 scan_cnt;                //扫描次数。每隔几次，需要扫一次电池
	u8 bat_val;                 //LDO BAND GAP
	u8 band_gap_val;            //LDO IN
#if USE_SDCLK_SDDET_PHONE_MUX
	u8 sddect_val;              //复用I/O的AD采样值
#endif
#if 1 //(HP_DET_EN && DAC_BUF_EN)
	u8 hpdect_val;              //HPDECT(use vcmbuf)的AD采样值
#endif
	//u8 song_val;				// P42 曲目选择AD值		20180611
	//u8 sel_val;					// P43 定时选择AD值
#if Emcode_Enable
	u8 disp_cnt;				// 显示计数值
	bool disp_timer_flag;			// 0: 不显示  1: 显示定时
	bool disp_volume_flag;		// 0: 不显示  1: 显示音量
	bool disp_song_flag;			// 0: 不显示  1: 显示曲目
	bool disp_clock_set_flag;		// 0: 不显示  1: 显示时钟设置
	bool disp_light_set_flag;		// 0: 不显示  1: LED亮度设置

	bool disp_scan_flag;			// 0: 不扫描 A1B1 A2B2   1: 扫描 A1B1 A2B2

	u8 timer_flag;				// 定时开始标志位:  0: 不定时  1: 定时15min  2: 30min  3: 60min  4: 7h
	u16 count_1min;				// 1min计时器

	u8 count_KU_TIMER;			// 时钟设置短按计数值   0:
#endif 
} type_scan_ctl;
extern IAR_XDATA_A type_scan_ctl scan_ctl;

typedef struct
{
	u8 key_val;                 //上一个按键值
	u16 key_cnt;                 //按键值持续计数
	u8 key_up;                  //抬键计数
	u8 scan_times;              //按键防抖扫描次数
	u8 up_times;                //抬键防抖扫描次数
} type_key_ctl;
extern IAR_XDATA_A type_key_ctl key_ctl;

#define KEY_SCAN_TIMES          8           //按键防抖的扫描次数
#define KEY_UP_TIMES            5           //抬键防抖的扫描次数
#define KEY_LONG_TIMES          400         //长按键的次数
#define KEY_HOLD_TIMES          30          //连接的频率次数
#define KEY_LONG_HOLD_TIMES     (KEY_LONG_TIMES + KEY_HOLD_TIMES)

#define T_KEY_SHORT             0x00
#define T_KEY_SHORT_UP          0x80
#define T_KEY_LONG              0xA0
#define T_KEY_LONG_UP           0xC0
#define T_KEY_HOLD              0xE0

#define QSYSTEM_1S              0x7f
#define QSYSTEM_500MS           0x7e
#define QSYSTEM_PICK            0x7d
#define QUSBDEV_SET_VOL         0x7c
#define Q_NUM_SEL               0x7b

#define NO_KEY                  0
#define T_KEY_1                 0x1
#define T_KEY_2                 0x2
#define T_KEY_3                 0x3
#define T_KEY_4                 0x4
#define T_KEY_5                 0x5
#define T_KEY_6                 0x6
#define T_KEY_7                 0x7
#define T_KEY_8                 0x8
#define T_KEY_9                 0x9
#define T_KEY_10                0xa
#define T_KEY_11                0xb
#define T_KEY_12                0xc
#define T_KEY_13                0xd
#define T_KEY_14                0xe
#define T_KEY_15                0xf
#define T_KEY_16                0x10
#define T_KEY_17                0x11
#define T_KEY_18				0x12

//数字按键，不支持长按、连发
#define T_KEY_NUM_0             0x20
#define T_KEY_NUM_1             0x21
#define T_KEY_NUM_2             0x22
#define T_KEY_NUM_3             0x23
#define T_KEY_NUM_4             0x24
#define T_KEY_NUM_5             0x25
#define T_KEY_NUM_6             0x26
#define T_KEY_NUM_7             0x27
#define T_KEY_NUM_8             0x28
#define T_KEY_NUM_9             0x29

//功能键
#define T_KEY_PREV              T_KEY_1
#define T_KEY_PLAY              T_KEY_2
#define T_KEY_NEXT              T_KEY_3
#define T_KEY_VOL_DOWN          T_KEY_4
#define T_KEY_VOL_UP            T_KEY_5
#define T_KEY_MODE              T_KEY_6
#define T_KEY_EQ                T_KEY_7
#define T_KEY_PLAYMODE          T_KEY_8

#define T_KEY_POWER             T_KEY_15

#define T_KEY_MUTE              T_KEY_10
//#define T_KEY_STOP              T_KEY_11	// 停用, 改为 编码开关使用
//#define T_KEY_CH_DOWN           T_KEY_9	// 停用, 改为 编码开关使用

//#define T_KEY_CH_UP             T_KEY_13	// 停用, 改为 编码开关使用
#define T_KEY_SOFT_POWER        T_KEY_14
#define T_KEY_MINUS             T_KEY_12
#define T_KEY_PLUS              T_KEY_16
#define T_KEY_PLAY_AND_MODE     T_KEY_17
#define T_KEY_PREV_NEXT			T_KEY_18

#define	T_KEY_PREV_VOL_DOWN		T_KEY_10
#define	T_KEY_NEXT_VOL_UP		T_KEY_11

#if Emcode_Enable
#define T_KEY_SW1_WHIRL			T_KEY_9		// A1B1 的转动触发, 需搭配 SW1_WHIRL_cnt 处理转动次数
#define T_KEY_SW2_WHIRL			T_KEY_11	// A2B2 的转动触发, 需搭配 SW2_WHIRL_cnt 处理转动次数
#define T_KEY_SW3_WHIRL			T_KEY_13	// A3B3 的转动触发, 需搭配 SW3_WHIRL_cnt 处理转动次数

#define T_KEY_SET_TIMER			T_KEY_7		// 定时时间设置按键
#define T_KEY_SET_LIGHT			T_KEY_12	// 亮度调整按键
#define T_KEY_LED_OFF			T_KEY_1 	// 亮度调整按键


#define	Whire_Right  1	// 正转
#define Whire_None   0	// 没转
#define Whire_Left  -1	// 反转

#define Disp_Timer	 2	// 数码管显示延时时间，包括音量、曲目和定时
#endif


//按键详细定义
/*
 * K_   Key Short Down
 * KU_  Key Short Up
 * KL_  Key Long Down
 * KLU_ Key Long Up
 * KH_  Key Hold
 */
#define KU_LED_OFF               (T_KEY_LED_OFF | T_KEY_SHORT_UP)

#define K_TIMER                  (T_KEY_SET_TIMER | T_KEY_SHORT)
#define KU_TIMER                 (T_KEY_SET_TIMER | T_KEY_SHORT_UP)
#define KL_TIMER                 (T_KEY_SET_TIMER | T_KEY_LONG)
#define KLU_TIMER                (T_KEY_SET_TIMER | T_KEY_LONG_UP)
#define KH_TIMER                 (T_KEY_SET_TIMER | T_KEY_HOLD)

#define K_LIGHT                  (T_KEY_SET_LIGHT | T_KEY_SHORT)
#define KU_LIGHT                 (T_KEY_SET_LIGHT | T_KEY_SHORT_UP)
#define KL_LIGHT                 (T_KEY_SET_LIGHT | T_KEY_LONG)
#define KLU_LIGHT                (T_KEY_SET_LIGHT | T_KEY_LONG_UP)
#define KH_LIGHT                 (T_KEY_SET_LIGHT | T_KEY_HOLD)

#define KL_PREV_NEXT			(T_KEY_PREV_NEXT | T_KEY_LONG)
#define KLU_PREV_NEXT			(T_KEY_PREV_NEXT | T_KEY_LONG_UP)

#define K_PREV                  (T_KEY_PREV | T_KEY_SHORT)
#define KU_PREV                 (T_KEY_PREV | T_KEY_SHORT_UP)
#define KL_PREV                 (T_KEY_PREV | T_KEY_LONG)
#define KLU_PREV                (T_KEY_PREV | T_KEY_LONG_UP)
#define KH_PREV                 (T_KEY_PREV | T_KEY_HOLD)

#define K_PLAY                  (T_KEY_PLAY | T_KEY_SHORT)
#define KU_PLAY                 (T_KEY_PLAY | T_KEY_SHORT_UP)
#define KL_PLAY                 (T_KEY_PLAY | T_KEY_LONG)
#define KLU_PLAY                (T_KEY_PLAY | T_KEY_LONG_UP)
#define KH_PLAY                 (T_KEY_PLAY | T_KEY_HOLD)

#define K_NEXT                  (T_KEY_NEXT | T_KEY_SHORT)
#define KU_NEXT                 (T_KEY_NEXT | T_KEY_SHORT_UP)
#define KL_NEXT                 (T_KEY_NEXT | T_KEY_LONG)
#define KLU_NEXT                (T_KEY_NEXT | T_KEY_LONG_UP)
#define KH_NEXT                 (T_KEY_NEXT | T_KEY_HOLD)

#define K_VOL_DOWN              (T_KEY_VOL_DOWN | T_KEY_SHORT)
#define KU_VOL_DOWN             (T_KEY_VOL_DOWN | T_KEY_SHORT_UP)
#define KL_VOL_DOWN             (T_KEY_VOL_DOWN | T_KEY_LONG)
#define KLU_VOL_DOWN            (T_KEY_VOL_DOWN | T_KEY_LONG_UP)
#define KH_VOL_DOWN             (T_KEY_VOL_DOWN | T_KEY_HOLD)

#define K_VOL_UP                (T_KEY_VOL_UP | T_KEY_SHORT)
#define KU_VOL_UP               (T_KEY_VOL_UP | T_KEY_SHORT_UP)
#define KL_VOL_UP               (T_KEY_VOL_UP | T_KEY_LONG)
#define KLU_VOL_UP              (T_KEY_VOL_UP | T_KEY_LONG_UP)
#define KH_VOL_UP               (T_KEY_VOL_UP | T_KEY_HOLD)

#define K_PREV_VOL_DOWN                (T_KEY_PREV_VOL_DOWN | T_KEY_SHORT)
#define KU_PREV_VOL_DOWN               (T_KEY_PREV_VOL_DOWN | T_KEY_SHORT_UP)
#define KL_PREV_VOL_DOWN               (T_KEY_PREV_VOL_DOWN | T_KEY_LONG)
#define KLU_PREV_VOL_DOWN              (T_KEY_PREV_VOL_DOWN | T_KEY_LONG_UP)
#define KH_PREV_VOL_DOWN               (T_KEY_PREV_VOL_DOWN | T_KEY_HOLD)

#define K_NEXT_VOL_UP               (T_KEY_NEXT_VOL_UP | T_KEY_SHORT)
#define KU_NEXT_VOL_UP               (T_KEY_NEXT_VOL_UP | T_KEY_SHORT_UP)
#define KL_NEXT_VOL_UP               (T_KEY_NEXT_VOL_UP | T_KEY_LONG)
#define KLU_NEXT_VOL_UP              (T_KEY_NEXT_VOL_UP | T_KEY_LONG_UP)
#define KH_NEXT_VOL_UP               (T_KEY_NEXT_VOL_UP | T_KEY_HOLD)

#define K_MODE                  (T_KEY_MODE | T_KEY_SHORT)
#define KU_MODE                 (T_KEY_MODE | T_KEY_SHORT_UP)
#define KL_MODE                 (T_KEY_MODE | T_KEY_LONG)
#define KLU_MODE                (T_KEY_MODE | T_KEY_LONG_UP)
#define KH_MODE                 (T_KEY_MODE | T_KEY_HOLD)

#define K_EQ                    (T_KEY_EQ | T_KEY_SHORT)
#define KU_EQ                   (T_KEY_EQ | T_KEY_SHORT_UP)
#define KL_EQ                   (T_KEY_EQ | T_KEY_LONG)
#define KLU_EQ                  (T_KEY_EQ | T_KEY_LONG_UP)
#define KH_EQ                   (T_KEY_EQ | T_KEY_HOLD)

#define K_PLAYMODE              (T_KEY_PLAYMODE | T_KEY_SHORT)
#define KU_PLAYMODE             (T_KEY_PLAYMODE | T_KEY_SHORT_UP)
#define KL_PLAYMODE             (T_KEY_PLAYMODE | T_KEY_LONG)
#define KLU_PLAYMODE            (T_KEY_PLAYMODE | T_KEY_LONG_UP)
#define KH_PLAYMODE             (T_KEY_PLAYMODE | T_KEY_HOLD)

#define K_MUTE                  (T_KEY_MUTE | T_KEY_SHORT)
#define KU_MUTE                 (T_KEY_MUTE | T_KEY_SHORT_UP)
#define KL_MUTE                 (T_KEY_MUTE | T_KEY_LONG)
#define KLU_MUTE                (T_KEY_MUTE | T_KEY_LONG_UP)
#define KH_MUTE                 (T_KEY_MUTE | T_KEY_HOLD)

#define K_STOP                  (T_KEY_STOP | T_KEY_SHORT)
#define KU_STOP                 (T_KEY_STOP | T_KEY_SHORT_UP)
#define KL_STOP                 (T_KEY_STOP | T_KEY_LONG)
#define KLU_STOP                (T_KEY_STOP | T_KEY_LONG_UP)
#define KH_STOP                 (T_KEY_STOP | T_KEY_HOLD)

#define K_POWER                 (T_KEY_POWER | T_KEY_SHORT)
#define KU_POWER                (T_KEY_POWER | T_KEY_SHORT_UP)
#define KL_POWER                (T_KEY_POWER | T_KEY_LONG)
#define KLU_POWER               (T_KEY_POWER | T_KEY_LONG_UP)
#define KH_POWER                (T_KEY_POWER | T_KEY_HOLD)

#define K_SOFT_POWER            (T_KEY_SOFT_POWER | T_KEY_SHORT)
#define KU_SOFT_POWER           (T_KEY_SOFT_POWER | T_KEY_SHORT_UP)
#define KL_SOFT_POWER           (T_KEY_SOFT_POWER | T_KEY_LONG)
#define KLU_SOFT_POWER          (T_KEY_SOFT_POWER | T_KEY_LONG_UP)
#define KH_SOFT_POWER           (T_KEY_SOFT_POWER | T_KEY_HOLD)

#define K_MINUS                 (T_KEY_MINUS | T_KEY_SHORT)
#define KU_MINUS                (T_KEY_MINUS | T_KEY_SHORT_UP)
#define KL_MINUS                (T_KEY_MINUS | T_KEY_LONG)
#define KLU_MINUS               (T_KEY_MINUS | T_KEY_LONG_UP)
#define KH_MINUS                (T_KEY_MINUS | T_KEY_HOLD)

#define K_PLUS                  (T_KEY_PLUS | T_KEY_SHORT)
#define KU_PLUS                 (T_KEY_PLUS | T_KEY_SHORT_UP)
#define KL_PLUS                 (T_KEY_PLUS | T_KEY_LONG)
#define KLU_PLUS                (T_KEY_PLUS | T_KEY_LONG_UP)
#define KH_PLUS                 (T_KEY_PLUS | T_KEY_HOLD)

#define K_PLAY_AND_MODE         (T_KEY_PLAY_AND_MODE | T_KEY_SHORT)
#define KU_PLAY_AND_MODE        (T_KEY_PLAY_AND_MODE | T_KEY_SHORT_UP)
#define KL_PLAY_AND_MODE        (T_KEY_PLAY_AND_MODE | T_KEY_LONG)
#define KLU_PLAY_AND_MODE       (T_KEY_PLAY_AND_MODE | T_KEY_LONG_UP)
#define KH_PLAY_AND_MODE        (T_KEY_PLAY_AND_MODE | T_KEY_HOLD)


/*ADC通道切换顺序：
KEDIR -> KEY -> KEY2 -> BAT -> BAND -> SDDECT -> KEYPOWER -> HPDECT(use vcmbuf)
*/

////////////////////////////////////////////////////
//HPDECT(use vcmbuf)通道
////////////////////////////////////////////////////
#if (USE_KEDIR && (USE_KEDIR_TYPE != KEDIR_IR_LED))
#define ADC_CH_HPDECT_NEXT()  {scan_ctl.adc_ch = ADC_KEDIR;\
                                    key_keidr_scan();\
                                    ADCCON = BIT(3) | ADKEDIR_CHAN;\
                                    KEDIR_ADCGO();}
#else
#define ADC_CH_HPDECT_NEXT()  ADC_CH_KEDIR_NEXT()
#endif


////////////////////////////////////////////////////
//ADC_KEYPOWER通道
////////////////////////////////////////////////////
#if (HP_DET_EN)	// && DAC_BUF_EN)
#define ADC_CH_KEYPOWER_NEXT()  {scan_ctl.adc_ch = ADC_HP_DECT;\
                                    ADCCON = BIT(7) | BIT(3) | ADHPDECT_CHAN;}
#else
#define ADC_CH_KEYPOWER_NEXT()  ADC_CH_HPDECT_NEXT()
#endif


////////////////////////////////////////////////////
//SDDECT通道
////////////////////////////////////////////////////
#if SOFT_POWEROFF_EN //(SOFT_POWEROFF_EN && (defined AX207X_TAG))
#define ADC_CH_SDDECT_NEXT()    {scan_ctl.adc_ch = ADC_KEYPOWER;\
                                    ADCCON = BIT(7) | BIT(3) | ADKEY_POWER_CHAN;}
#else
#define ADC_CH_SDDECT_NEXT()    ADC_CH_KEYPOWER_NEXT()
#endif

////////////////////////////////////////////////////
//BAT通道
////////////////////////////////////////////////////
#if (BATTERY_CHECK || SOFT_POWEROFF_EN)
#define ADC_CH_BAT_NEXT()       {scan_ctl.adc_ch = ADC_BAND_GAP;\
                                    ADCCON = BIT(7) | BIT(3) | ADBAND_GAP_CHAN;}
#else
#define ADC_CH_BAT_NEXT()       ADC_CH_BAND_NEXT()
#endif

#if USE_SDCLK_SDDET_PHONE_MUX
#define ADC_CH_BAND_NEXT()      {scan_ctl.adc_ch = ADC_SD_DECT;\
                                    ADCCON = BIT(7) | BIT(3) | ADSDDECT_CHAN;}
#else
#define ADC_CH_BAND_NEXT()      ADC_CH_SDDECT_NEXT()
#endif


////////////////////////////////////////////////////
//ADC_KEY2通道
////////////////////////////////////////////////////
#define ADC_CH_KEY2_KICK()           {ADCCON = BIT(7) | BIT(3) | ADKEDIR_CHAN;}

#if (BATTERY_CHECK || SOFT_POWEROFF_EN)
#define ADC_CH_KEY2_NEXT()       {scan_ctl.adc_ch = ADC_BAT;\
                                     ADCCON = BIT(7) | BIT(3) | ADBAT_CHAN;}
#else
#define ADC_CH_KEY2_NEXT()       ADC_CH_BAT_NEXT()
#endif


////////////////////////////////////////////////////
//ADC_KEY通道
////////////////////////////////////////////////////
#if (USER_SEL == USER_AX1071_DEMO)
#define ADC_CH_KEY_KICK()           {ADCCON = BIT(7) | BIT(3) | ADKEY0_CHAN;\
                                     ADCMODE |= BIT(5);}
#else
#define ADC_CH_KEY_KICK()           {ADCCON = BIT(7) | BIT(3) | ADKEDIR_CHAN;}
#endif

#if (USER_SEL == USER_AX1071_DEMO)
#if USE_ADKEY2
#define ADC_CH_KEY_NEXT()       {scan_ctl.adc_ch = ADC_KEY2;\
                                    ADCMODE &= ~BIT(5);\
                                    ADCCON = BIT(7) | BIT(3) | ADKEDIR_CHAN;}
#else
#define ADC_CH_KEY_NEXT()       {ADC_CH_KEY2_NEXT();\
                                     ADCMODE &= ~BIT(5);}
#endif
#else
#if USE_ADKEY2
#define ADC_CH_KEY_NEXT()       {scan_ctl.adc_ch = ADC_KEY2;\
									ADCMODE &= ~BIT(5);\
                                    ADCCON = BIT(7) | BIT(3) | ADKEDIR_CHAN;}
#else
#define ADC_CH_KEY_NEXT()       {ADC_CH_KEY2_NEXT();}

#endif
#endif

#define ADC_CH_SONG_NEXT()		{scan_ctl.adc_ch = ADC_SONG;\
									AIPCON7 |= BIT(6) | BIT(7) | BIT(1);\
									ADCMODE |= BIT(5);\
                                    ADCCON = BIT(7) | BIT(3) | ADSONG_CHAN;}

#define ADC_CH_SEL_NEXT()		{scan_ctl.adc_ch = ADC_SEL;\
									AIPCON7 |= BIT(6) | BIT(7) | BIT(1);\
									ADCMODE |= BIT(5);\
                                    ADCCON = BIT(7) | BIT(3) | ADSEL_CHAN;}


////////////////////////////////////////////////////
//ADC_KEDIR通道
////////////////////////////////////////////////////
#define ADC_CH_KEDIR_KICK()         key_keidr_scan();\
                                    {if (scan_ctl.kedir_ch == ADC_BIG) {\
                                        if (DCN == 0x00) {\
                                            KEDIR_SCAN_BIG_1();\
                                        } else {\
                                            KEDIR_SCAN_BIG();\
                                        }\
                                    }\
                                    ADCCON = BIT(3) | ADKEDIR_CHAN;\
                                    KEDIR_ADCGO();}

#if USE_ADKEY
#if (USER_SEL == USER_AX1071_DEMO)
#define ADC_CH_KEDIR_NEXT()     {KEDIR_DISABLE_EOC();\
                                    scan_ctl.adc_ch = ADC_KEY;\
                                    ADCMODE |= BIT(5);\
                                    ADCCON = BIT(7) | BIT(3) | ADKEDIR_CHAN;}
#else
#define ADC_CH_KEDIR_NEXT()     {KEDIR_DISABLE_EOC();\
                                    scan_ctl.adc_ch = ADC_KEY;\
                                    ADCCON = BIT(7) | BIT(3) | ADKEDIR_CHAN;}
#endif
#else
#define ADC_CH_KEDIR_NEXT()     ADC_CH_KEY_NEXT()
#endif


#endif

