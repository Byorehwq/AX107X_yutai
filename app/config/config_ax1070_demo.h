/*****************************************************************************
 * Module    : Config
 * File      : config_ax1070_demo.h
 * Author    :
 * Email     :
 * Function  : AX1070 DEMO板功能配置
 *****************************************************************************/
#ifndef CONFIG_AX1070_DEMO_H
#define CONFIG_AX1070_DEMO_H


#define SRAM_DEBUG                      0   //是否开启SRAM板调试功能, AX1070 no SRAM_DEBUG function

/*****************************************************************************
 * Module    : 系统调试选择
 *****************************************************************************/
#define PRINTF_EN                       1   //是否开启UART打印功能


/*****************************************************************************
 * Module    : 系统晶振选择
 *****************************************************************************/
#define SYS_CRY                         CRY_32K //CRY_RC
#define ADD_BY_WUPENGFEI                1

/*****************************************************************************
 * Task      : 系统任务及功能相关配置
 *****************************************************************************/
#define TASK_AUX_EN                     0   //是否打开LINEIN功能
#define TASK_FM_EN                      0   //是否打开FM功能
#define TASK_SPEAKER_EN                 0   //是否打开扩音器功能
#define TASK_RTC_EN                     0   //是否打开RTC功能
#define TASK_USBDEV_EN                  0   //是否打开USBDEV功能
#define TASK_POWEROFF_EN                1   //是否打开关机功能
#define TASK_LOWPOWER_EN                0   //是否打开低功耗模式功能
#define TASK_IDLE_EN                    1   //是否打开空闲功能
#define TASK_FLASH_EN                   1   //是否打开播放SPI_FLASH功能
#define TASK_TIPS_EN                    0   //是否支持提示音功能

/*****************************************************************************
 * Task      : 音乐播放任务的功能定义
 *****************************************************************************/
#define WAV_SUPPORT                     0   //是否支持WAV播放
#define MPX_SUPPORT 					1	//是否支持MPX
#define MUSIC_SWITCH_DEV                0   //是否支持模式键切播放设备
#define AUTO_CHANGE_DEVICE              0   //下一曲到最后一首时自动切设备
#define AB_REPEAT_EN                    0   //是否使用AB复读功能
#define KARAOKE_EN                      0   //是否使用卡拉OK功能(MIC_DET_EN为1时有效,不支持混响效果)
#define USE_BREAK_POINT                 1   //是否使用断点记忆
#define USE_Flash_BREAK_POINT			1	//Flash底下是否使用断点记忆

#define MUSIC_STOP                      0   //是否使用STOP功能
#define MUSIC_PLAYMODE                  0   //是否调需要调整PLAYMODE
#define MUSIC_EQ                        0   //是否调需要调整EQ
#define FAST_FORW_BACKW_EN              0   //是否使用快进快退功能
#define PLAUSE_FAST_FORW_BACKW          0   //暂停后是否需要快进快退功能(FAST_FORW_BACKW_EN为1时有效)
#define PLAY_STARTMUSIC                 0   //是否播放开机音乐
#define DIR_SWITCH_EN                   0   //是否支持文件夹切换
#define FS_SCAN_SPEED_EN                0   //是否使用搜文件加速功能
#define REPEAT_AS_ONE                   0   //是否使用无断点循环播放功能


#define Emcode_Enable					1	// 编码器使能	20180626	
#define BIT0							0x01
#define BIT1							0x02
#define BIT2							0x04
#define BIT3							0x08
#define BIT4							0x10
#define BIT5							0x20
#define BIT6							0x40
#define BIT7							0x80

//播放模式数量选择
#define PLAYMODE_NUM                    PLAYMODE_TWO


/*****************************************************************************
 * Task      : AUX任务的功能定义
 *****************************************************************************/
#define AUX_EQ_EN                       0   //是否使用带有EQ功能的LINEIN (AX107X不支持)


/*****************************************************************************
 * Task      : 扩音器任务的功能定义
 *****************************************************************************/
#define MIX_DEEP_EN                     0   //是否加深混响效果，0为不加深0~10级，1为加深0~15级
#define LIMIT_MIX_NOISE_EN              0   //是否使用扩音器降噪功能


/*****************************************************************************
 * Task      : FM任务的功能定义
 *****************************************************************************/
#define FM_SEMIAUTOMATIC_SEEK           0   //FM半自动搜台功能

//FM是否采用50KHz步进搜台，目前只有QN8065、RTC6218调试好50KHz步进搜台，主要用于主控省晶振方案时提高搜台效果
#define FM_SEEK_50K_STEP                0

//FM时钟源选择
#define FM_CLOCK			            SHARE_CRYSTAL_32KHz

//FM模块选择,可以视程序空间打开多少个FM芯片
#define FM_RTC6218_EN                   0

/*****************************************************************************
 * Task      : RTC任务的功能定义
 *****************************************************************************/
#define ALARM_EN                        0   //是否需要闹铃功能
#define ALARM_WAKEUP_EN                 0   //是否需要闹钟唤醒硬件开机


/*****************************************************************************
 * Task      : USBDEV任务的功能定义
 *****************************************************************************/
#define USB_MASS_SUPPORT                0   //是否使用读卡器功能
#define USB_SPK_SUPPORT                 0   //是否使用USBAUDIO功能
#define USB_MIC_SUPPORT                 0   //是否使用USBMIC功能
#define USB_HID_SUPPORT                 0   //是否使用控制音量功能


/*****************************************************************************
 * Module    : 系统功能细节定义
 *****************************************************************************/
#define KEY_VOICE_EN                    0   //是否需要按键音
#define BATTERY_CHECK                   1   //是否需要电池电量检测
#define AUX_DET_EN                      0   //是否需要LineIn检测
#define MIC_DET_EN                      0   //是否需要Mic检测
#define HP_DET_EN                       0   //是否需要Headphone检测
#define PLL_TUNE_EN                     0   //是否打开PLL TUNE功能（无晶振方案有效）
#define ENERGY_DISP_EN                  0   //是否需要能量显示(AX107X不支持)

//功放MUTE类型选择
#define MUTE_TYPE                       MUTE_HIGH

//显示屏选择
#define DISP_TYPE                       DISP_LED_7P7S //DISP_LED_5C7S	 //  DISP_LED_7P7S //
#define DISP_Show_Device				0	// 1: 显示设备名称, 0:不显示设备名称 "-1-" or "-2-"

//记忆体选择
#define SYS_MEM                         MEM_RTCRAM  //(AX107X不支持MEM_INTERNAL)


/*****************************************************************************
 * Module    : DAC、AMUX相关配置定义
 *****************************************************************************/
#define DAC_FADE_EN                     1   //是否需要DAC濙入淡出//已在task_flash.c的task_flash_music_enter函数中关掉淡入淡出

#define DAC_POWER_SPEED                 0   //是否需要减少DAC初始化占用的开机时间，打开会增加100多字节

#define USE_MULTI_BIT_DAC               1    //是否使用multi-bit DAC

#define DAC_BUF_EN                      0   //是否使用VCM BUF（只在AX107X上可选), 这里是AX107x 可以按需求选取

//DAC初始化选择
#define DAC_INIT_TYPE                   DAC_VCM_WITH_CAP

//LINEIN通道选择 
#define AMUX_LINEIN                     AMUX_CH_23 
 
//LINEIN ADC采样通道选择(AUX EQ时使用)
#define AUADC_LINEIN                    AUADC_CH_AUX_LR

//FM通道选择
#define AMUX_FM                         AMUX_CH_45

#define AMUX_FM_WITH_NO_CAP             0   //FM声音进主控是否省AMUX电容，暂时只支持107X

/*****************************************************************************
 * Module    : EQ功能定义
 *****************************************************************************/
#define EQ_FADE_EN                      0   //调EQ是否需要渐变
#define EQ_MODIFY_DB_EN                 0   //是否需要修改EQ的DB值，对应tbl_eq_db[6][9]
#define EQ_MODIFY_FILTER_EN             0   //是否需要修改EQ滤波器系数，对应eq_coeff[54]


/*****************************************************************************
 * Module    : 按键等定义d
 *****************************************************************************/
#define USE_ADKEY                       0   //是否使用ADKEY
#define USE_ADKEY2                      1   //是否使用第二组ADKEY
#define USE_TOUCH_KEY                   0   //是否使用触摸按键 (FPGA上不能使用)
#define USE_KEDIR                       0   //是否使用三合一功能 (FPGA上不能使用)
#define USE_MIXIO_AD                    0   //是否使用三合一IO做普通IO的AD (USE_KEDIR为0时有效，FPGA上可用)


/*****************************************************************************
 * Module    : 红外遥控等定义
 *****************************************************************************/
#define USE_IR                          0   //是否使用红外遥控
#define USE_HW_IR                       0   //是否使用硬件IR，为0时使用timer捕捉解析红外（USE_IR为1时有效）
#define USE_IR_NUM                      0   //是否使用遥控数字键

//IR通道选择
#define IR_PORT                         IR_P03

//IR用户码选择，可以打开一个或多个
#define IR_FF00_EN                      0   //是否支持用户码0xFF00
#define IR_BF00_EN                      0   //是否支持用户码0xBF00
#define IR_FD02_EN                      0   //是否支持用户码0xFD02
#define IR_FE01_EN                      0   //是否支持用户码0xFE01


/*****************************************************************************
 * Module    : 三合一功能定义
 *****************************************************************************/
//三合一功选能择
#define USE_KEDIR_TYPE                  KEDIR_KEY

//三合一通道选择
#define KEDIR_PORT                      KEDIR_P03 


/*****************************************************************************
 * Module    : 触摸按键功能定义
 *****************************************************************************/
#define TOUCH_KEY_0_EN                  0   //是否使用TK0   P10
#define TOUCH_KEY_1_EN                  0   //是否使用TK1   P11
#define TOUCH_KEY_2_EN                  0   //是否使用TK2   P12
#define TOUCH_KEY_3_EN                  0   //是否使用TK3   P13
#define TOUCH_KEY_4_EN                  0   //是否使用TK4   P14
#define TOUCH_KEY_5_EN                  0   //是否使用TK5   P01


/******************************************************************** *********
 * Module    : 软关机功能定义
 *****************************************************************************/
#define SOFT_POWEROFF_EN                1   //是否打开软关机功能(TASK_POWEROFF_EN为1时才有效)
#define POWERON_LONG_KEY                1   //是否打开长按POWER键开机功能，0为短按POWER键开机
#define DCIN_DET_EN                     0   //是否需要DCIN检测
#define SOFT_POWER_CTL_INIT()           P0DIR &= ~BIT(0),P0 &= ~BIT(0)	
#define SOFT_POWER_CTL_ON()             P0DIR &= ~BIT(0),P0 |= BIT(0)	
#define SOFT_POWER_CTL_OFF()            P0DIR &= ~BIT(0),P0 &= ~BIT(0)	 

/*****************************************************************************
 * Module    : 设备、IO与复用等定义
 *****************************************************************************/
#define UDISK_DOUBLE_EN                 0    //是否使用双UDISK功能

#define SD_DOUBLE_EN                    0    //是否使用双SD卡功能

//sd0选择一组SDIO，0~3
#define SD_PORT                         2

//sd1选择一组SDIO
#define SD1_PORT                        2

//SD CLK与检测脚是否复用。为0时，则需要独立的SD检测脚
#define USE_SD_CLK_DET                  0

//SD CMD、SPI DAT复用
#define USE_SDCMD_IIC_SPIDAT            0

//I2C CLK和DATA是否与SD复用。为0时，需要独立的I2C CLK和DATA
#define USE_I2C_SD_MUX                  1

//USB HOST(即读取U盘)的PHY接口选择。0为P42 P43，1为P40 P41 （单UDISK时此宏才有用）
#define USB_HOST_PHY                    1

//USB DEV(即连接PC)的PHY接口选择。0为P42 P43，1为P40 P41
#define USB_DEV_PHY                     1

//AUX检测IO
#define AUX_CHECK_IO_SET
#define AUX_CHECK_PORT                  P3
#define AUX_CHECK_DIR                   P3DIR
#define AUX_CHECK_PU                    PUP3
#define AUX_CHECK_PIN                   1

//MIC检测IO
#define MIC_CHECK_IO_SET
#define MIC_CHECK_PORT                  P1
#define MIC_CHECK_DIR                   P1DIR
#define MIC_CHECK_PU                    PUP1
#define MIC_CHECK_PIN                   3

//HEADPHONE检测IO     //耳机插入的侦测由P10(NET:LINE-IN_DET)改到P01(PHIN_ADC)  2017/05/09  王群修改
#define HP_CHECK_IO_SET
#define HP_CHECK_PORT                   P0
#define HP_CHECK_DIR                    P0DIR
#define HP_CHECK_PU                     PUP0
#define HP_CHECK_PIN                    6

//DCIN检测IO
#define DCIN_CHECK_IO_SET
#define DCIN_CHECK_PORT                 P1
#define DCIN_CHECK_DIR                  P1DIR
#define DCIN_CHECK_PU                   PUP1
#define DCIN_CHECK_PIN                  0

//MUTE控制IO       //将静音(NET:SHDN)的控制由 P11 改到 P00    2017/05/09  王群修改
#define MUTE_IO_SET
#define MUTE_PORT                       P0
#define MUTE_DIR                        P0DIR
#define MUTE_PIN                        0

/*****************************************************************************
 * Module    : LED的相关配置
 *****************************************************************************/
//0=使用外部限流电阻，1=使用内部限流电阻
#define LED_7C7S_S0_DIR                 P2DIR
#define LED_7C7S_S0_PORT                P2
#define LED_7C7S_S0_PIN                 1 


#define LED_7C7S_S1_DIR                 P2DIR
#define LED_7C7S_S1_PORT                P2
#define LED_7C7S_S1_PIN                 2


#define LED_7C7S_S2_DIR                 P2DIR
#define LED_7C7S_S2_PORT                P2
#define LED_7C7S_S2_PIN                 3


#define LED_7C7S_S3_DIR                 P2DIR
#define LED_7C7S_S3_PORT                P2
#define LED_7C7S_S3_PIN                 4

#define LED_7C7S_S4_DIR                 P2DIR
#define LED_7C7S_S4_PORT                P2
#define LED_7C7S_S4_PIN                 5


#define LED_7C7S_S5_DIR                 P2DIR
#define LED_7C7S_S5_PORT                P2
#define LED_7C7S_S5_PIN                 6

#define LED_7C7S_S6_DIR                 P2DIR
#define LED_7C7S_S6_PORT                P2
#define LED_7C7S_S6_PIN                 7

//LED控制IO
#define LED0_IO_SET						// P05
#define LED0_PORT                       P0
#define LED0_DIR                        P0DIR
#define LED0_PIN                        5
#define MOV_Carry_To_LED_O()		asm("MOV	P0.5 , C");		// LED 0 -->  P0.5 = Carry;

#define LED1_IO_SET						// P04
#define LED1_PORT                    	P0
#define LED1_DIR                      	P0DIR
#define LED1_PIN                      	4
#define MOV_Carry_To_LED_T()		asm("MOV	P0.4 , C");		// LED 1 -->  P0.4 = Carry;

#define LED_LIGHT1	   0x2A			// LED亮度值
#define LED_LIGHT2	   0x7F
#define LED_LIGHT3	   0xD4


/*****************************************************************************
 * IO key配置
 *****************************************************************************/
#define S1_PORT						P0			// P03
#define S1_BIT						3
#define S1_DIR						P0DIR
#define S1_PU						PUP0

#define S2_PORT						P0			// P06
#define S2_BIT						6
#define S2_DIR						P0DIR
#define S2_PU						PUP0

#define S3_PORT						P0			// P07	仿真  实际为WKORTC
#define S3_BIT						7
#define S3_DIR						P0DIR
#define S3_PU						PUP0

/*****************************************************************************
 * Module    : 确认芯片选择
 *****************************************************************************/
#ifndef AX107X_TAG
#error "请选择AX107X_Debug"
#endif

#endif

