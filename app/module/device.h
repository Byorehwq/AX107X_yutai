/*****************************************************************************
* Module    : Device
* File      : device.h
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : 设备管理的接口
*****************************************************************************/
#ifndef DEVICE_H
#define DEVICE_H


#ifdef AX207X_TAG
enum
{
    DEV_UNACTIVE = 0,   //设备未初始化
    DEV_ACTIVED,        //设备有效
    DEV_INVALID,        //设备无效
    DEV_NO_MEDIUM,      //设备不在线，针对读卡器而言
};
#else
enum
{
	DEV_INVALID = 0,    //设备无效
	DEV_UNACTIVE,       //设备刚插入，未初始化
	DEV_ACTIVED,        //设备有效
	DEV_NO_MEDIUM,      //没有存储介质，针对读卡器而言
};
#endif

extern type_dev_ctl dev_ctl;


bool device_activate_try(u8 dev_num);                   //尝试去激活初始化设备，仅管理层
void device_check_stop(u8 dev_num);
void usb_host_tune_set(void);
bool sd_active(void);
bool sd_active_hook(void);
void usb_host_resume_m(void);
void usb_host_suspend_m(void);
u8 read_usb_reg(u8 reg_addr);
bool usb_host_enum(void);
bool device_cardreader_check(void);
void sd_clr_status(u8 sta);
bool sd_active_safe(u8 sd_group);
u8 host_select_phy(u8 phy_num);
bool device_activate_udisk_do(void);
#endif
