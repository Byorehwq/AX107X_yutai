/*****************************************************************************
* Module    : Device
* File      : device.h
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : �豸����Ľӿ�
*****************************************************************************/
#ifndef DEVICE_H
#define DEVICE_H


#ifdef AX207X_TAG
enum
{
    DEV_UNACTIVE = 0,   //�豸δ��ʼ��
    DEV_ACTIVED,        //�豸��Ч
    DEV_INVALID,        //�豸��Ч
    DEV_NO_MEDIUM,      //�豸�����ߣ���Զ���������
};
#else
enum
{
	DEV_INVALID = 0,    //�豸��Ч
	DEV_UNACTIVE,       //�豸�ղ��룬δ��ʼ��
	DEV_ACTIVED,        //�豸��Ч
	DEV_NO_MEDIUM,      //û�д洢���ʣ���Զ���������
};
#endif

extern type_dev_ctl dev_ctl;


bool device_activate_try(u8 dev_num);                   //����ȥ�����ʼ���豸���������
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
