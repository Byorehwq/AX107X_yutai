/*****************************************************************************
 * Module    : Config
 * File      : config_default.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : 默认用户参数设置，会自动选择用户没有设置的选项
 *****************************************************************************/
#ifndef CONFIG_DEFAULT_H
#define CONFIG_DEFAULT_H


/*****************************************************************************
 *  Module    : SRAM板调试选择
 *****************************************************************************/
/*【注意】：
  1.使用SRAM板（NO.13-844）调试前，需要手动填写pll trim值、三合一trim值，
    每一块SRAM板的trim值是不同的，可以在板上的贴纸找到这些数值;
  2.SRAM板上使用uart调试专用IO,而不是使用P35,只要在大红板上'UART_JUMP'处加跳线;
*/
//#ifndef SRAM_DEBUG
//#define SRAM_DEBUG                      0   //(def)是否开启SRAM板调试功能
//#endif

//pll trim值
#ifndef PLL_INTL
#define PLL_INTL                        0x05
#endif

#ifndef PLL_FRACL
#define PLL_FRACL                       0x00
#endif

#ifndef PLL_FRACH
#define PLL_FRACH                       0x80
#endif

//三合一trim值
#ifndef LED_TRIM
#define LED_TRIM                        0x20
#endif


/*****************************************************************************
 * Module    : 系统调试选择
 *****************************************************************************/
#ifndef PRINTF_EN
#define PRINTF_EN                       1   //(def)是否开启UART打印功能
#endif


/*****************************************************************************
 * Module    : 系统晶振选择
 *****************************************************************************/
#ifndef SYS_CRY
#define SYS_CRY                         CRY_32K
#endif

#ifndef CRY_PIN_NUM
#define CRY_PIN_NUM                     CRY_2PIN
#endif

/*****************************************************************************
 * Task      : 系统任务及功能相关配置
 *****************************************************************************/
#ifndef TASK_AUX_EN
#define TASK_AUX_EN                     1   //(def)是否打开LINEIN功能
#endif

#ifndef TASK_FM_EN
#define TASK_FM_EN                      1   //(def)是否打开FM功能
#endif

#ifndef TASK_SPEAKER_EN
#define TASK_SPEAKER_EN                 1   //(def)是否打开扩音器功能
#endif

#ifndef TASK_RTC_EN
#define TASK_RTC_EN                     1   //(def)是否打开RTC功能
#endif

#ifndef TASK_USBDEV_EN
#define TASK_USBDEV_EN                  1   //(def)是否打开USBDEV功能
#endif

#ifndef TASK_POWEROFF_EN
#define TASK_POWEROFF_EN                1   //(def)是否打开关机功能
#endif

#ifndef TASK_LOWPOWER_EN
#define TASK_LOWPOWER_EN                1   //(def)是否打开低功耗模式功能
#endif

#ifndef TASK_IDLE_EN
#define TASK_IDLE_EN                    1   //(def)是否打开空闲功能
#endif

#ifndef TASK_FMAM_DETECT_EN
#define TASK_FMAM_DETECT_EN             0   //(def)是否打开FM/AM频率检测功能(仅在AX207X上支持)
#endif


/*****************************************************************************
 * Task      : 音乐播放任务的功能定义
 *****************************************************************************/
#ifndef WAV_SUPPORT
#define WAV_SUPPORT                     0   //(def)是否支持WAV播放
#endif

#ifndef MPX_SUPPORT 						
#define MPX_SUPPORT 					0	//(def)是否支持MPX
#endif
#ifndef MUSIC_SWITCH_DEV
#define MUSIC_SWITCH_DEV                1   //(def)是否支持模式键切播放设备
#endif

#ifndef AUTO_CHANGE_DEVICE
#define AUTO_CHANGE_DEVICE              1   //(def)下一曲到最后一首时自动切设备
#endif

#ifndef AB_REPEAT_EN
#define AB_REPEAT_EN                    0   //(def)是否使用AB复读功能
#endif

#ifndef KARAOKE_EN
#define KARAOKE_EN                      0   //(def)是否使用卡拉OK功能(MIC_DET_EN为1时有效,不支持混响效果)
#endif

#ifndef USE_BREAK_POINT
#define USE_BREAK_POINT                 1   //(def)是否使用断点记忆
#endif

#ifndef MUSIC_STOP
#define MUSIC_STOP                      0   //(def)是否使用STOP功能
#endif

#ifndef MUSIC_PLAYMODE
#define MUSIC_PLAYMODE                  1   //(def)是否调需要调整PLAYMODE
#endif

#ifndef MUSIC_EQ
#define MUSIC_EQ                        1   //(def)是否调需要调整EQ
#endif

#ifndef FAST_FORW_BACKW_EN
#define FAST_FORW_BACKW_EN              1   //(def)是否使用快进快退功能
#endif

#ifndef PLAUSE_FAST_FORW_BACKW
#define PLAUSE_FAST_FORW_BACKW          0   //(def)暂停后是否需要快进快退功能(FAST_FORW_BACKW_EN为1时有效)
#endif

#ifndef PLAY_STARTMUSIC
#define PLAY_STARTMUSIC                 0   //(def)是否播放开机音乐
#endif

#ifndef DIR_SWITCH_EN
#define DIR_SWITCH_EN                   0   //(def)是否支持文件夹切换
#endif

#ifndef FS_SCAN_SPEED_EN
#define FS_SCAN_SPEED_EN                1   //(def)是否使用搜文件加速功能
#endif

//播放模式数量选择
#ifndef PLAYMODE_NUM
#define PLAYMODE_NUM                    PLAYMODE_FOUR
#endif


/*****************************************************************************
 * Task      : AUX任务的功能定义
 *****************************************************************************/
#ifndef AUX_EQ_EN
#define AUX_EQ_EN                       0   //(def)是否使用带有EQ功能的LINEIN
#endif


/*****************************************************************************
 * Task      : 扩音器任务的功能定义
 *****************************************************************************/
#ifndef MIX_DEEP_EN
#define MIX_DEEP_EN                     0   //(def)是否加深混响效果，0为不加深0~10级，1为加深0~15级
#endif

#ifndef LIMIT_MIX_NOISE_EN
#define LIMIT_MIX_NOISE_EN              1   //(def)是否使用扩音器降噪功能
#endif


/*****************************************************************************
 * Task      : FM任务的功能定义
 *****************************************************************************/
#ifndef FM_SEMIAUTOMATIC_SEEK
#define FM_SEMIAUTOMATIC_SEEK           0   //(def)FM半自动搜台功能
#endif

//FM是否采用50KHz步进搜台，目前只有QN8065、RTC6218调试好50KHz步进搜台，主要用于主控省晶振方案时提高搜台效果
#ifndef FM_SEEK_50K_STEP
#define FM_SEEK_50K_STEP                0	//(def)
#endif

#ifndef FM_CLOCK
#define FM_CLOCK			            USE_MODULE_CRYSTAL  //(def)FM时钟源选择
#endif

#ifndef FM_CLK_IO
#define FM_CLK_IO                       CLK_P34  //(def)提供时钟的IO选择
#endif

#ifndef AMUX_FM_EN
#define AMUX_FM_EN                      1        //(def)FM声音是否进AMUX 1:进主控 0：不进主控
#endif

//FM模块选择,可以视程序空间打开多少个FM芯片
#ifndef FM_RDA5807_EN
#define FM_RDA5807_EN                   0		//(def)
#endif
#ifndef FM_BK1080_EN
#define FM_BK1080_EN                    0		//(def)
#endif
#ifndef FM_QN8035_EN
#define FM_QN8035_EN                    0		//(def)
#endif
#ifndef FM_QN8065_EN
#define FM_QN8065_EN                    0		//(def)
#endif
#ifndef FM_KT0830_EN
#define FM_KT0830_EN                    0		//(def)
#endif
#ifndef FM_SI484X_EN
#define FM_SI484X_EN                    0   //(def)未调试
#endif
#ifndef FM_AR1010_EN
#define FM_AR1010_EN                    0   //(def)未调试
#endif
#ifndef FM_AR1019_EN
#define FM_AR1019_EN                    0   //(def)未调试
#endif
#ifndef FM_CL6017S_EN
#define FM_CL6017S_EN                   0   //(def)未调试
#endif
#ifndef FM_CL6017G_EN
#define FM_CL6017G_EN                   0   //(def)未调试
#endif

#ifndef FM_RTC6218_EN
#define FM_RTC6218_EN                   0	//(def)
#endif

#if FM_RDA5807_EN
#define RDA5807_PE_SP                   1   //(def)PE/SP版本，调试时3种都打开要调空间，烧写时可3种都打开
#define RDA5807_HS_HP                   0   //(def)HS/HP版本
#define RDA5807_M_MP                    0   //(def)M/MP版本
#endif

#if FM_SI484X_EN
#define IIC_DATAFILL_EN                 1   //(def)使用SI芯片时，可以打开此宏降低对FM的干扰
#define USE_CAPTURE_MODE                0   //(def)使用timer0 Capture功能获取SI的IRQ信号
#define USE_PORT_INTERRUPT              1   //(def)使用外部中断获取SI的IRQ
#endif

/*
打开该宏可以省进FM AMUX电容，但由于FM输出直连到主控导致调节主控声音时要先把DAC trim到B（带FM）值
调节完声音后再trim会A（初始）值，导致调节声音速度过慢，故打开该宏在FM模式下通过调节FM声音来调节声音，
由于DAC trim需要时间会增加整个淡入淡出的时间，该时间随FM输出直流偏置和AMUX输入直流偏置增大而增大。
*/
#ifndef AMUX_FM_WITH_NO_CAP
#define AMUX_FM_WITH_NO_CAP             0   //(def)FM声音进主控是否省AMUX电容，暂时只支持107X
#endif

/*****************************************************************************
 * Task      : RTC任务的功能定义
 *****************************************************************************/
#ifndef ALARM_EN
#define ALARM_EN                        0   //(def)是否需要闹铃功能
#endif

#ifndef ALARM_WAKEUP_EN
#define ALARM_WAKEUP_EN                 0   //(def)设定闹钟后软关机，在闹钟时间到后，WKORTC会输出高电平，以支持硬件开机
#endif

/*****************************************************************************
 * Task      : USBDEV任务的功能定义
 *****************************************************************************/
#ifndef USB_MASS_SUPPORT
#define USB_MASS_SUPPORT                1   //(def)是否使用读卡器功能
#endif

#ifndef USB_SPK_SUPPORT
#define USB_SPK_SUPPORT                 1   //(def)是否使用USBAUDIO功能
#endif

#ifndef USB_MIC_SUPPORT
#define USB_MIC_SUPPORT                 1   //(def)是否使用USBMIC功能
#endif

#ifndef USB_HID_SUPPORT
#define USB_HID_SUPPORT                 1   //(def)是否使用控制音量功能
#endif


/*****************************************************************************
 * Module    : 系统功能细节定义
 *****************************************************************************/
#ifndef KEY_VOICE_EN
#define KEY_VOICE_EN                    1   //(def)是否需要按键音
#endif

//#ifndef BATTERY_CHECK
//#define BATTERY_CHECK                   0   //(def)是否需要电池电量检测
//#endif

#ifndef AUX_DET_EN
#define AUX_DET_EN                      1   //(def)是否需要LineIn检测
#endif

#ifndef MIC_DET_EN
#define MIC_DET_EN                      1   //(def)是否需要Mic检测
#endif

#ifndef HP_DET_EN
#define HP_DET_EN                       0   //(def)是否需要Headphone检测
#endif

#ifndef MUTE_TYPE
#define MUTE_TYPE                       MUTE_NO //(def)功放MUTE类型选择
#endif

#ifndef PLL_TUNE_EN
#define PLL_TUNE_EN                     0   //(def)是否打开PLL TUNE功能（无晶振方案有效）
#endif

#ifndef ENERGY_DISP_EN
#define ENERGY_DISP_EN                  0   //(def)是否需要能量显示
#endif

#ifndef PA_MODE_SEL_EN
#define PA_MODE_SEL_EN                  0   //(def)是否为D/AB类功放(例XPT8871)
#endif

//显示屏选择
#ifndef DISP_TYPE
#define DISP_TYPE                       DISP_LED_5C7S	//(def)
#endif

//记忆体选择
#ifndef SYS_MEM
#define SYS_MEM                         MEM_RTCRAM		//(def)
#endif


/*****************************************************************************
 * Module    : DAC、AMUX相关配置定义
 *****************************************************************************/
#ifndef USE_MULTI_BIT_DAC
#define USE_MULTI_BIT_DAC               0   //(def)是否使用multi-bit DAC
#endif

#ifndef DAC_FADE_EN
#define DAC_FADE_EN                     0   //(def)是否需要DAC濙入淡出
#endif

#ifndef DAC_POWER_SPEED
#define DAC_POWER_SPEED                 0   //(def)是否需要减少DAC初始化占用的开机时间，打开会增加100多字节
#endif

#ifndef DAC_BUF_EN
//可省DAC输出电容，耳机检测需要使用ADC检测，开内部上拉10K）
#define DAC_BUF_EN                      0   //(def)是否使用VCM BUF（只在AX107X上可选)
#endif

#if DAC_BUF_EN
#undef DAC_POWER_SPEED
#define DAC_POWER_SPEED                 0	//(def)
#endif

//DAC初始化选择
#ifndef DAC_INIT_TYPE
#define DAC_INIT_TYPE                   DAC_VCM_WITH_CAP	//(def)
#endif

#ifndef VDDHP_VDDIO_SHT
#define VDDHP_VDDIO_SHT                 0   //(def)VDDHP与VDDIO是否邦一起
#endif

#ifndef DACR_ONLY
#define DACR_ONLY                       0   //(def)DAC合并到DACR单声道输出
#endif

//LINEIN通道选择
#ifndef AMUX_LINEIN
#define AMUX_LINEIN                     AMUX_CH_23	//(def)
#endif

//LINEIN ADC采样通道选择(AUX EQ时使用)
#ifndef AUADC_LINEIN
#define AUADC_LINEIN                    AUADC_CH_AUX_LR	//(def)
#endif

//FM通道选择
#ifndef AMUX_FM
#define AMUX_FM                         AMUX_CH_45	//(def)
#endif


/*****************************************************************************
 * Module    : EQ功能定义
 *****************************************************************************/
#ifndef EQ_FADE_EN
#define EQ_FADE_EN                      1   //(def)调EQ是否需要渐变
#endif

#ifndef EQ_MODIFY_DB_EN
#define EQ_MODIFY_DB_EN                 0   //(def)是否需要修改EQ的DB值，对应tbl_eq_db[6][9]
#endif

#ifndef EQ_MODIFY_FILTER_EN
#define EQ_MODIFY_FILTER_EN             0   //(def)是否需要修改EQ滤波器系数，对应eq_coeff[54]
#endif


/*****************************************************************************
 * Module    : 按键等定义
 *****************************************************************************/
#ifndef USE_ADKEY
#define USE_ADKEY                       1   //(def)是否使用ADKEY
#endif

#ifndef USE_TOUCH_KEY
#define USE_TOUCH_KEY                   0   //(def)是否使用触摸按键 (FPGA上不能使用)
#endif

#ifndef USE_KEDIR
#define USE_KEDIR                       0   //(def)是否使用三合一功能 (FPGA上不能使用)
#endif

#ifndef USE_MIXIO_AD
#define USE_MIXIO_AD                    0   //(def)是否使用三合一IO做普通IO的AD (USE_KEDIR为0时有效，FPGA上可用)
#endif


/*****************************************************************************
 * Module    : 红外遥控等定义
 *****************************************************************************/
#ifndef USE_IR
#define USE_IR                          1   //(def)是否使用红外遥控
#endif

#ifndef USE_HW_IR
#define USE_HW_IR                       1   //(def)是否使用硬件IR，为0时使用timer捕捉解析红外（USE_IR为1时有效）
#endif

#ifndef USE_IR_NUM
#define USE_IR_NUM                      1   //(def)是否使用遥控数字键
#endif

//IR通道选择
#ifndef IR_PORT
#define IR_PORT                         IR_P10	//(def)
#endif

//IR用户码选择，可以打开一个或多个
#ifndef IR_FF00_EN
#define IR_FF00_EN                      1   //(def)是否支持用户码0xFF00
#endif

#ifndef IR_BF00_EN
#define IR_BF00_EN                      0   //(def)是否支持用户码0xBF00
#endif

#ifndef IR_FD02_EN
#define IR_FD02_EN                      0   //(def)是否支持用户码0xFD02
#endif

#ifndef IR_FE01_EN
#define IR_FE01_EN                      0   //(def)是否支持用户码0xFE01
#endif


/*****************************************************************************
 * Module    : 三合一功能定义
 *****************************************************************************/
//三合一功选能择
#ifndef USE_KEDIR_TYPE
#define USE_KEDIR_TYPE                  KEDIR_KEY_IR_LED	//(def)
#endif

//三合一通道选择
#ifndef KEDIR_PORT
#define KEDIR_PORT                      KEDIR_P02	//(def)
#endif


/*****************************************************************************
 * Module    : 触摸按键功能定义
 *****************************************************************************/
#ifndef TOUCH_KEY_0_EN
#define TOUCH_KEY_0_EN                  1   //(def)是否使用TK0
#endif

#ifndef TOUCH_KEY_1_EN
#define TOUCH_KEY_1_EN                  1   //(def)是否使用TK1
#endif

#ifndef TOUCH_KEY_2_EN
#define TOUCH_KEY_2_EN                  1   //(def)是否使用TK2
#endif

#ifndef TOUCH_KEY_3_EN
#define TOUCH_KEY_3_EN                  1   //(def)是否使用TK3
#endif

#ifndef TOUCH_KEY_4_EN
#define TOUCH_KEY_4_EN                  1   //(def)是否使用TK4
#endif

#ifndef TOUCH_KEY_5_EN
#define TOUCH_KEY_5_EN                  0   //(def)是否使用TK5
#endif


/*****************************************************************************
 * Module    : 软关机功能定义
 *****************************************************************************/
//#ifndef SOFT_POWEROFF_EN
//#define SOFT_POWEROFF_EN                0   //(def)是否打开软关机功能
//#endif

//#ifndef POWERON_LONG_KEY
//#define POWERON_LONG_KEY                1   //(def)是否打开长按POWER键开机功能
//#endif

#ifndef DCIN_DET_EN
#define DCIN_DET_EN                     0   //(def)是否需要DCIN检测
#endif


/*****************************************************************************
 * Module    : 设备、IO与复用等定义
 *****************************************************************************/
#ifndef UDISK_DOUBLE_EN
#define UDISK_DOUBLE_EN                 1	//(def)
#endif

#ifndef SD_DOUBLE_EN
#define SD_DOUBLE_EN                    1	//(def)
#endif

//选择一组SDIO，0~3
#ifndef SD_PORT
#define SD_PORT                         0	//(def)
#endif

//选择一组SDIO，0~3
#ifndef SD1_PORT
#define SD1_PORT                        1	//(def)
#endif

//SD CLK与检测脚是否复用。为0时，则需要独立的SD检测脚
#ifndef USE_SD_CLK_DET
#define USE_SD_CLK_DET                  1	//(def)
#endif

//SD CLK、SD检测、耳机检测复用,用ADC功能实现。为0时，需要独立的耳机检测脚
#ifndef USE_SDCLK_SDDET_PHONE_MUX
#define USE_SDCLK_SDDET_PHONE_MUX       0	//(def)
#endif

//SD CMD、IIC CLK、SD检测复用 (FPGA上不能使用)
#ifndef USE_SDCMD_IICCLK_SDDET_MUX
#define USE_SDCMD_IICCLK_SDDET_MUX      0	//(def)
#endif

//SD CMD、IIC CLK、MIC检测复用 (FPGA上不能使用)
#ifndef USE_SDCMD_IICCLK_MIC_MUX
#define USE_SDCMD_IICCLK_MIC_MUX        0	//(def)
#endif

//SD CMD、IIC CLK、PHONE检测复用 (FPGA上不能使用)
#ifndef USE_SDCMD_IICCLK_PHONE_MUX
#define USE_SDCMD_IICCLK_PHONE_MUX      0	//(def)
#endif

//复用7P_LED屏的IO进行LINEIN 检测
#ifndef USE_7P7S_LED_AUX_MUX
#define USE_7P7S_LED_AUX_MUX            0	//(def)
#endif

//I2C CLK和DATA是否与SD复用。为0时，需要独立的I2C CLK和DATA
#ifndef USE_I2C_SD_MUX
#define USE_I2C_SD_MUX                  1	//(def)
#endif

//USB HOST(即读取U盘)的PHY接口选择。0为P42 P43，1为P40 P41 (FPGA上不能使用第0组)
#ifndef USB_HOST_PHY
#define USB_HOST_PHY                    1	//(def)
#endif

//USB DEV(即连接PC)的PHY接口选择。0为P42 P43，1为P40 P41 (FPGA上不能使用第0组)
#ifndef USB_DEV_PHY
#define USB_DEV_PHY                     1	//(def)
#endif

//USB检测选择
#ifndef USB_DET
#define USB_DET                         USBDET_OTG	//(def)
#endif

//AUX检测IO
#ifndef AUX_CHECK_IO_SET
#define AUX_CHECK_PORT                  P1	//(def)
#define AUX_CHECK_DIR                   P1DIR	//(def)
#define AUX_CHECK_PU                    PUP1	//(def)
#define AUX_CHECK_PIN                   4	//(def)
#endif

//MIC检测IO
#ifndef MIC_CHECK_IO_SET
#define MIC_CHECK_PORT                  P2	//(def)
#define MIC_CHECK_DIR                   P2DIR	//(def)
#define MIC_CHECK_PU                    PUP2	//(def)
#define MIC_CHECK_PIN                   7	//(def)
#endif

//HEADPHONE检测IO                //耳机插入的侦测由P10(NET:LINE-IN_DET)改到P01(PHIN_ADC)  2017/05/09  王群修改
#ifndef HP_CHECK_IO_SET
#define HP_CHECK_PORT                   P0//(def)
#define HP_CHECK_DIR                    P0DIR//(def)
#define HP_CHECK_PU                     PUP0//(def)
#define HP_CHECK_PIN                    1//(def)
#endif

//DCIN检测IO
#ifndef DCIN_CHECK_IO_SET
#define DCIN_CHECK_PORT                 P1	//(def)
#define DCIN_CHECK_DIR                  P1DIR	//(def)
#define DCIN_CHECK_PU                   PUP1	//(def)
#define DCIN_CHECK_PIN                  0	//(def)
#endif

//MUTE控制IO
#ifndef MUTE_IO_SET
#define MUTE_PORT                       P0	//(def)
#define MUTE_DIR                        P0DIR	//(def)
#define MUTE_PIN                        0	//(def)
#endif


/*****************************************************************************
 * Module    : LED的相关配置
 *****************************************************************************/
//0=使用外部限流电阻，1=使用内部限流电阻
#ifndef LED_5C7S_EXT_RES_EN
#define LED_5C7S_EXT_RES_EN             0	//(def)
#endif

#ifndef LED_5C7S_IO_SET
#define LED_5C7S_C0_DIR                 P0DIR
#define LED_5C7S_C1_DIR                 P0DIR
#define LED_5C7S_C2_DIR                 P2DIR
#define LED_5C7S_C3_DIR                 P0DIR
#define LED_5C7S_C4_DIR                 P0DIR

#define LED_5C7S_C0_PORT                P4
#define LED_5C7S_C1_PORT                P0
#define LED_5C7S_C2_PORT                P0
#define LED_5C7S_C3_PORT                P0
#define LED_5C7S_C4_PORT                P0

#define LED_5C7S_C0_PIN                 3
#define LED_5C7S_C1_PIN                 4
#define LED_5C7S_C2_PIN                 5
#define LED_5C7S_C3_PIN                 6
#define LED_5C7S_C4_PIN                 7

#define LED_5C7S_S_DN_SEL               PDN2L
#define LED_5C7S_S_DN_EN                PDN2
#define LED_5C7S_S_DIR                  P2DIR
#define LED_5C7S_S_PORT                 P2  //(P20~P26)
#endif


/*****************************************************************************
** 下面这些为自动选择的宏，请不要修改
 *****************************************************************************/
#if (!TASK_POWEROFF_EN)
#undef  SOFT_POWEROFF_EN
#define SOFT_POWEROFF_EN            0	//(def)
#endif
#if (!SOFT_POWEROFF_EN)
#undef  DCIN_DET_EN
#define DCIN_DET_EN                 0	//(def)
#endif

#if (!FAST_FORW_BACKW_EN)
#undef  PLAUSE_FAST_FORW_BACKW
#define PLAUSE_FAST_FORW_BACKW      0	//(def)
#endif

#if (!TASK_AUX_EN)
#undef  AUX_DET_EN
#define AUX_DET_EN                  0	//(def)
#endif

#if (!TASK_SPEAKER_EN && !KARAOKE_EN)
#undef  MIC_DET_EN
#define MIC_DET_EN                  0	//(def)
#endif

#if ( ((FM_CLOCK == USE_IO_32KHZ_CLK) || (FM_CLOCK == USE_IO_12MHZ_CLK))\
    && (((FM_CLK_IO == CLK_P16) && (SD_PORT == 0)) || ((FM_CLK_IO == CLK_P06) && (SD_PORT == 3))) )
//复用SD_CLK的IO输出时钟给FM
#define FM_CLK_IO_MUX_SDCLK         1	//(def)
#else
#define FM_CLK_IO_MUX_SDCLK         0	//(def)
#endif

#if (!TASK_FM_EN)
#undef  AMUX_FM_EN
#undef  FM_RDA5807_EN
#undef  FM_BK1080_EN
#undef  FM_QN8035_EN
#undef  FM_QN8065_EN
#undef  FM_KT0830_EN
#undef  FM_SI484X_EN
#undef  FM_AR1010_EN
#undef  FM_AR1019_EN
#undef  FM_CL6017S_EN
#undef  FM_CL6017G_EN
#undef  FM_RTC6218_EN
#define AMUX_FM_EN                  0
#define FM_RDA5807_EN               0
#define FM_BK1080_EN                0
#define FM_QN8035_EN                0
#define FM_QN8065_EN                0
#define FM_KT0830_EN                0
#define FM_SI484X_EN                0
#define FM_AR1010_EN                0
#define FM_AR1019_EN                0
#define FM_CL6017S_EN               0
#define FM_CL6017G_EN               0
#define FM_RTC6218_EN               0

#undef FM_CLK_IO_MUX_SDCLK
#define FM_CLK_IO_MUX_SDCLK         0	//(def)
#endif

#if (!TASK_RTC_EN)
#undef  ALARM_EN
#define ALARM_EN                    0	//(def)
#endif

#if (!ALARM_EN)
#undef  ALARM_WAKEUP_EN
#define ALARM_WAKEUP_EN             0	//(def)
#endif

#if (!TASK_USBDEV_EN)
#undef  USB_MASS_SUPPORT
#undef  USB_SPK_SUPPORT
#undef  USB_MIC_SUPPORT
#define USB_MASS_SUPPORT            0   //(def)读卡器功能
#define USB_SPK_SUPPORT             0   //(def)USBAUDIO功能
#define USB_MIC_SUPPORT             0   //(def)USBMIC功能

#undef USB_DET
#define USB_DET                     USBDET_HOST_ONLY        //(def)没有USB DEVICE模式，只进行HOST检测
#endif

#if (!USB_SPK_SUPPORT)
#undef  USB_HID_SUPPORT
#define USB_HID_SUPPORT             0   //(def)控制音量功能
#endif


#if (SYS_MEM == MEM_INTERNAL)
#ifndef MEM_WRITE_COMPARE
#define MEM_WRITE_COMPARE           0	//(def)
#endif
#endif

#if (!USE_IR)
#undef  USE_HW_IR
#undef  USE_IR_NUM
#define USE_HW_IR                   0	//(def)
#define USE_IR_NUM                  0	//(def)
#endif

#if MIC_DET_EN
#undef  TOUCH_KEY_3_EN
#define TOUCH_KEY_3_EN              0	//(def)
#endif

#if HP_DET_EN
#undef  TOUCH_KEY_4_EN
#define TOUCH_KEY_4_EN              0	//(def)
#endif

#if (DISP_TYPE == DISP_LED_7P7S)
#undef  LED_7P7S_SCAN_HALF_MS
#define LED_7P7S_SCAN_HALF_MS       1   //(def0LED7脚数码管用0.125ms扫描
#else
#undef  LED_7P7S_SCAN_HALF_MS
#define LED_7P7S_SCAN_HALF_MS       0	//(def)
#endif

#endif
