/*****************************************************************************
 * Module    : Config
 * File      : config.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : 配置文件
 *****************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#include "cfg_def.h"

/*****************************************************************************
 * SDK版本   :
 *****************************************************************************/


/*****************************************************************************
 * Module    : 用户选择
 *****************************************************************************/
//配置前请阅读文件config.c中【注意事项】

#ifdef AX207X_TAG
#define USER_SEL            USER_AX2070_DEMO    //用户选择
#endif

#ifdef AX107X_TAG
#define USER_SEL            USER_AX1070_DEMO    //用户选择 (这里用户使用的是 AX1071)
#endif


//加载用户自定义配置
#if (USER_SEL == USER_TEST)
#include "config_test.h"                //测试板
#elif (USER_SEL == USER_AX2070_DEMO)
#include "config_ax2070_demo.h"         //AX2070 DEMO板
#elif (USER_SEL == USER_AX2073_DEMO)
#include "config_ax2073_demo.h"         //AX2073 DEMO板?
#elif (USER_SEL == USER_AX1070_DEMO)
#include "config_ax1070_demo.h"         //AX1070 DEMO板
#elif (USER_SEL == USER_AX1071_DEMO)
#include "config_ax1071_demo.h"         //AX1071 DEMO板?
#elif (USER_SEL == USER_AX1073_DEMO)
#include "config_ax1073_demo.h"         //AX1073 DEMO板?
#elif (USER_SEL == USER_AX1076_DEMO)
#include "config_ax1076_demo.h"         //AX1076 DEMO板
#endif


#include "config_default.h"                 //默认配置


#endif
