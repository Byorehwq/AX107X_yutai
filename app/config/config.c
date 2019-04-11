/*****************************************************************************
 * Module    : Config
 * File      : config.c
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : 用户配置文件，点击左侧目录展开，可编辑相应的配置
 *****************************************************************************/
#include "config.h"

/*****************************************************************************
【注意事项】
AX207X包括:
    AX2070(LQFP48);
    AX2073(SOP16);
    AX2073B(SOP16);
AX107X包括:
    AX1070(LQFP48);
    AX1071(SOP28);
    AX1073(SOP16);
    AX1073B(SOP16);
    AX1076(SSOP24);

晶振配置和DAC配置在不同封装上有差别，具体如下：
AX2070:
    1.DAC初始化选项，有VCM:    #define DAC_INIT_TYPE     DAC_VCM_WITH_CAP
    2.VDDHP与VDDIO分开:        #define VDDHP_VDDIO_SHT   0

AX2073:
    1.系统晶振选择，无晶振：   #define SYS_CRY           CRY_RC
    2.DAC初始化选项，无VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    3.VDDHP与VDDIO邦一起:      #define VDDHP_VDDIO_SHT   1

AX2073B:
    1.系统晶振选择，单PIN晶振：#define CRY_PIN_NUM       CRY_1PIN
    2.DAC初始化选项，无VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    3.VDDHP与VDDIO邦一起:      #define VDDHP_VDDIO_SHT   1

AX2075:
    1.系统晶振选择，无晶振：   #define SYS_CRY           CRY_RC
    2.DAC初始化选项，无VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    3.VDDHP与VDDIO邦一起:      #define VDDHP_VDDIO_SHT   1

AX1070:
    1.使用multi-bit DAC:       #define USE_MULTI_BIT_DAC 1
    2.DAC初始化选项，有VCM:    #define DAC_INIT_TYPE     DAC_VCM_WITH_CAP
    3.VDDHP与VDDIO分开:        #define VDDHP_VDDIO_SHT   0

AX1071:
    1.系统晶振选择，单PIN晶振：#define CRY_PIN_NUM       CRY_1PIN
    2.使用multi-bit DAC:       #define USE_MULTI_BIT_DAC 1
    3.DAC初始化选项，无VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    4.VDDHP与VDDIO分开:        #define VDDHP_VDDIO_SHT   0

AX1073:
    1.系统晶振选择，无晶振：   #define SYS_CRY           CRY_RC
    2.使用multi-bit DAC:       #define USE_MULTI_BIT_DAC 1
    3.DAC初始化选项，无VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    4.VDDHP与VDDIO邦一起:      #define VDDHP_VDDIO_SHT   1

AX1073B:
    1.系统晶振选择，单PIN晶振：#define CRY_PIN_NUM       CRY_1PIN
    2.使用multi-bit DAC:       #define USE_MULTI_BIT_DAC 1
    3.DAC初始化选项，无VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    4.VDDHP与VDDIO邦一起:      #define VDDHP_VDDIO_SHT   1

AX1076:
    1.系统晶振选择，单PIN晶振：#define CRY_PIN_NUM       CRY_1PIN
    2.使用multi-bit DAC:       #define USE_MULTI_BIT_DAC 1
    3.DAC初始化选项，无VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    4.VDDHP与VDDIO邦一起:      #define VDDHP_VDDIO_SHT   1

 *****************************************************************************/
