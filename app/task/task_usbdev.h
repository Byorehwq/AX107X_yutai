/*****************************************************************************
 * Module    : Task
 * File      : task_usbdev.h
 * Author    :
 * Email     :
 * Function  : USBDEV任务相关定义
 *****************************************************************************/
#ifndef TASK_USBDEV_H
#define TASK_USBDEV_H

void task_usbdev_deal_msg(u8 msg);
void task_usbdev_display(void);
bool usb_host_unactived(void);
void usb_device_tune_set(void);
void sd_unactived(void);

#endif
