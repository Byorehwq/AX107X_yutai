#ifndef _USER_USBDEV_H_
#define _USER_USBDEV_H_


extern IAR_DATA_A type_usb_audio_ctl usb_audio_ctl;

void usb_device_init(void);
void usb_device_reset(void);
void usbdev_init(void);
void usbdev_exit(void);
void usb_hid_vol(bool direct);
void usbdev_init_double_sd(void);
bool usb_hid_timeout(void);
bool usb_device_is_online(void);
void usb_hid_sta_get(void);
bool usb_hid_is_valid(void);

/* 以下是自动选择的宏 */
#define USB_SER_NUM0     ('0'+USB_MASS_SUPPORT)
#define USB_SER_NUM1     ('0'+USB_SPK_SUPPORT)
#define USB_SER_NUM2     ('0'+USB_MIC_SUPPORT)
#define USB_SER_NUM3     ('0'+USB_HID_SUPPORT)

#define DEV_TYPE                ((USB_MASS_SUPPORT<<DEV_MASS_BIT)\
                                |(USB_SPK_SUPPORT<<DEV_SPK_BIT)\
                                |(USB_MIC_SUPPORT<<DEV_MIC_BIT)\
                                |(USB_HID_SUPPORT<<DEV_HID_BIT))

#endif
