#include "include.h"

/*
extern func
*/
bool usb_disk_read(void *buf, u32 lba);
bool sd_disk_read(void *buf, u32 lba);
bool usb_disk_read_tune(void *buf, u32 lba);
void host_tune_init(void);
void devie_tune_init(void);


#pragma location="FS_INIT_SEG"
void usb_disk_func_init(void)
{
#if PLL_TUNE_EN

#if (SYS_CRY == CRY_RC)
	fs_hook.disk_read = usb_disk_read_tune;
#elif(SYS_CRY == CRY_AUTO)
	if(b_sys.xosc_flag)
	{
		fs_hook.disk_read = usb_disk_read;
	}
	else
	{
		fs_hook.disk_read = usb_disk_read_tune;
	}
#else
	fs_hook.disk_read = usb_disk_read;
#endif

#else   //PLL_TUNE_EN
	fs_hook.disk_read = usb_disk_read;
#endif  //PLL_TUNE_EN

	fs_hook.disk_write = usb_host_mass_write;
}

#pragma location="DEVICE_SEG"
void usb_host_tune_set(void)
{
#if PLL_TUNE_EN
#if (SYS_CRY == CRY_RC)
	host_tune_init();
#elif(SYS_CRY == CRY_AUTO)
	if(!b_sys.xosc_flag)
	{
		host_tune_init();
	}
#endif
#endif
}

#pragma location="DEVICE_SEG"
void usb_device_tune_set(void)
{
#if PLL_TUNE_EN
#if (SYS_CRY == CRY_RC)
	devie_tune_init();
#elif(SYS_CRY == CRY_AUTO)
	if(!b_sys.xosc_flag)
	{
		devie_tune_init();
	}
#endif
#endif
}

