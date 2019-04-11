#include "include.h"

#pragma location="USB_SEG"
void usb_switch_stable_device(void)
{
#if ((USB_HOST_PHY == USB_DEV_PHY) || (USB_DET != USBDET_OTG))
	usb_switch_stable_device_std(usb_chk.dev_phy);
#else
	usb_switch_stable_device_var();
#endif
}

#pragma location="USB_SEG"
void usb_switch_stable_host(void)
{
#if ((USB_HOST_PHY == USB_DEV_PHY) || (USB_DET != USBDET_OTG))
	usb_switch_stable_host_std(usb_chk.host_phy);
#else
	usb_switch_stable_host_var();
#endif
}

void decoder_stream_end(void);      //位流结束


#pragma inline=forced
#pragma location="DEVICE_COM_SEG"
void device_remove_usb(void)
{
	USBCON1 &= ~BIT(3);             //RESET
	asm("nop");
	asm("nop");
	USBCON1 |= BIT(3);
	usb_host_enable_testready(0);
#if ((USB_HOST_PHY == USB_DEV_PHY) || (USB_DET != USBDET_OTG))
	usb_switch_host_std(usb_chk.host_phy);
#else
	usb_switch_host(usb_chk.host_phy);
#endif
	usb_ctrl.error = 1;
}

//设备移除
#pragma location="DEVICE_COM_SEG"
bool device_remove(u8 dev_num)
{
	if(!device_status_stable(dev_num, 0))   //for lark 0=DEV_INVALID
	{
		return false;
	}
	if((dev_num == DEVICE_SDMMC) || (dev_num == DEVICE_SDMMC1))
	{
		if(dev_num == sd_chk.group)
		{
			sd_remove();
		}

		if(task_ctl.work_sta != TASK_FLASH)
		{
			if(fs_cur_dev() == dev_num)
			{
				decoder_stream_end();
			}
		}
	}
	else if((dev_num == DEVICE_UDISK)
#if UDISK_DOUBLE_EN
	        || (dev_num == DEVICE_UDISK1)
#endif
	       )
	{
		if((!device_is_actived(DEVICE_UDISK))
#if UDISK_DOUBLE_EN
		        && (!device_is_actived(DEVICE_UDISK1))
#endif
		  )
		{
			device_remove_usb();
		}
		usb_switch_otg(dev_num - DEVICE_UDISK);
		if(fs_cur_dev() == dev_num)
		{
			decoder_stream_end();
		}
	}
	return true;
}

