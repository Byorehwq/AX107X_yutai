/*****************************************************************************
 * Module    : API
 * File      : api.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : API标准函数列表
 *****************************************************************************/
#ifndef API_H
#define API_H

//系统函数
#include "api_system.h"

//调试接口
//#include "api_debug.h"

//文件系统API接口
#include "api_filesystem.h"

//音乐播放API接口
#include "api_music.h"

//RTC相关
#include "api_rtc.h"

//FM相关
#include "api_fm.h"

//设备管理的相应接口
#include "api_device.h"

//记忆信息
//#include "api_param.h"

//USB_HOST相关
#include "api_usbhost.h"

//USB_DEV相关
#include "api_usbdev.h"

//SD相关
#include "api_sd.h"


#endif
