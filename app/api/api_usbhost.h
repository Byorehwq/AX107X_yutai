#ifndef _API_USB_HOST_H_
#define _API_USB_HOST_H_

__root void usb_host_resume(void);
__root void usb_host_suspend(void);
__root bool usb_host_mass_read(void *buf, u32 lba);
__root bool usb_host_mass_write(void *buf, u32 lba);

#define usb_read		usb_host_mass_read
#define usb_write		usb_host_mass_write

void usb_host_enable_testready(u8 enable);

#endif
