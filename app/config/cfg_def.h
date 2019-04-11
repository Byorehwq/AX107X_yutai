#ifndef __CFG_DEF_H__
#define __CFG_DEF_H__

//已定义用户列表
#define USER_TEST                       0           //FPGA测试板
#define USER_AX2070_DEMO                1           //AX2070 LQFP48 DEMO
#define USER_AX2073_DEMO                2           //AX2073 SOP16 DEMO
#define USER_AX1070_DEMO                5           //AX1070 LQFP48 DEMO
#define USER_AX1071_DEMO                6           //AX1071 SOP28 DEMO
#define USER_AX1073_DEMO                7           //AX1073 SOP16 DEMO
#define USER_AX1076_DEMO                8           //AX1076 SSOP24 DEMO


//SPI Flash模式选择
#define SPI_TWO_WIRES_MODE              0           //SPI 2线模式
#define SPI_THREE_WIRES_MODE            1           //SPI 3线模式



//晶振频率选项
#define CRY_32K                         0
#define CRY_4M                          1
#define CRY_12M                         2
#define CRY_24M                         3
#define CRY_RC                          4           //无晶振
#define CRY_AUTO                        5           //自动判断是否有32K或无晶振

//晶振引脚选项
#define CRY_1PIN                        0           //1pin mode(晶振一脚接XOSCO，一脚接GND)
#define CRY_2PIN                        1           //2pin mode(晶振一脚接XOSCO，一脚接XOSCI)

//记忆体选项
#define MEM_NO                          0
#define MEM_RTCRAM                      1           //RTCRAM记忆
#define MEM_INTERNAL                    2           //内置记忆
#define MEM_E2PROM                      3           //EEPROM记忆
#define MEM_DEVICE                      4           //设备记忆

//DAC初始化选项
#define DAC_VCM_WITH_CAP                0           //有VCM电容的DAC初始化
#define DAC_VCM_NO_CAP                  1           //无VCM电容的DAC初始化
#define DAC_SIMPLE                      2           //简易的DAC初始化,代码量少,时间短.有明显噪声,适合有MUTE的应用

//FM驱动方式
#define USE_MODULE_CRYSTAL		        1           //使用模块独立晶振
#define USE_IO_32KHZ_CLK		        2           //主控有晶振，IO输出32K时钟(方波)给FM.IO有:P16/P15/P06/P32/P33/P34/P35
#define USE_IO_12MHZ_CLK		        3           //IO输出12M(PLL分频)时钟(方波)给FM.IO有:P16/P15/P06/P32/P33/P34/P35，可用于主控无晶振方案
#define SHARE_CRYSTAL_32KHz		        4           //与MCU共用32K晶振
#define SHARE_CRYSTAL_12MHz		        5           //与MCU共用12M晶振，未调试
#define SHARE_CRYSTAL_24MHz		        6           //与MCU共用24M晶振，未调试
#define USE_SD_IO_32KHZ_CLK             7           //SDCLK输出32K时钟，SDCLK不能复用检测,主要用于主控无晶振方案，用于QN8065时IIC不能与SD复用
#define USE_SD_IO_12MHZ_CLK             8           //SDCLK输出12M时钟，SDCLK不能复用检测,主要用于主控无晶振方案，用于QN8065时IIC不能与SD复用

//repeat模式选择
#define PLAYMODE_FOUR                   1           //四个播放模式(ALL、RAMDOM、SINGLE、FOLDER)
#define PLAYMODE_TWO                    2           //二个播放模式(ALL、SINGLE)

//显示屏选择
#define DISP_NO                         0
#define DISP_LED_5C7S                   1           //LED数码管显示
#define DISP_LCD_SEG                    2           //LCD断码屏显示
#define DISP_LED                        3           //LED显示
#define DISP_LED_7P7S                   4           //LED7脚数码管显示

//三合一功能选择
#define KEDIR_KEY_IR_LED                0           //按键&红外&LED全复用
#define KEDIR_KEY_IR                    1           //复用按键&红外
#define KEDIR_KEY_LED                   2           //复用按键&LED
#define KEDIR_IR_LED                    3           //复用红外&LED
#define KEDIR_KEY                       4           //只做ADKEY使用

//三合一IO口选择
#define KEDIR_P10                       0           //P10
#define KEDIR_P03                       1           //P03
#define KEDIR_P02                       2           //P02
#define KEDIR_P27                       3           //P27

//红外IO选择
#define IR_P10                          0           //P10
#define IR_P03                          1           //P03
#define IR_P02                          2           //P02
#define IR_P27                          3           //P27

//输出时钟IO口选择
#define CLK_P16                         1           //P16
#define CLK_P15                         2           //P15
#define CLK_P06                         3           //P06
#define CLK_P32                         4           //P32
#define CLK_P33                         5           //P33
#define CLK_P34                         6           //P34
#define CLK_P35                         7           //P35

//USB检测选择
#define USBDET_OTG                      0           //主从检测
#define USBDET_HOST_ONLY                1           //只做USB HOST检测
#define USBDET_DEV_ONLY                 2           //只做USB DEVICE检测

#define MUTE_NO                         0			// 无 MUTE控制
#define MUTE_LOW                        1			// Low MUTE, High UNMUTE
#define MUTE_HIGH                       2			// High MUTE, Low UNMUTE
#define MUTE_DIGITAL					3			// Low MUTE, High _-_---- UNMUTE
#endif
