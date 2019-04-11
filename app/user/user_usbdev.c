#include "include.h"



#if TASK_USBDEV_EN

IAR_DATA_A type_usb_audio_ctl usb_audio_ctl @ "USBDEV_DATA";

#pragma constseg="USBDEV_TBL"
//序列号
IAR_CONST u8 tbl_usb_str_serial_number[24] =
{
	24,         // Num bytes of this descriptor
	3,          // String descriptor
	'2',    0,
	'0',    0,
	'1',    0,
	'3',    0,
	USB_SER_NUM0,    0,
	USB_SER_NUM1,    0,
	USB_SER_NUM2,    0,
	USB_SER_NUM3,    0,
	'p',    0,
	'h',    0,
	'0',    0
};

const u8 __code *const __code tbl_string_point[4]=
{
	tbl_usb_language_id,
	tbl_usb_str_manufacturer,
	tbl_usb_str_product,
	tbl_usb_str_serial_number
};
#pragma constseg=default


/*----------------------------------------------------------------------------*/
/**@brief  usbdev获取字符串描述符
   @param  无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="TASK_USBDEV_SEG"
const u8 __code *usb_get_str_desc(u8 index)
{
	return tbl_string_point[index];
}

#if USB_HID_SUPPORT
#pragma location="TASK_USBDEV_SEG"
void usb_hid_vol(bool direct)
{
	if(usb_hid_is_valid())
	{
		if(direct)
		{
#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))   //不显示音量值时，可以不要以下处理
			if(usb_audio_ctl.spk_vol_set < VOLUME_MAX)
			{
				usb_audio_ctl.spk_vol_set++;
				usb_audio_ctl.spk_vol_dir = 1;
			}
#endif
			usb_hid_key(HID_CTL_VOLUP);
		}
		else
		{
#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
			if(usb_audio_ctl.spk_vol_set > 0)
			{
				usb_audio_ctl.spk_vol_set--;
				usb_audio_ctl.spk_vol_dir = -1;
			}
#endif
			usb_hid_key(HID_CTL_VOLDOWN);
		}

#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
		sys_ctl.hid_vol_delay_cnt = 30;  //300ms超时退出
		while(usb_audio_ctl.spk_vol_dir != 0)
		{
			WATCHDOG_CLR();
			usb_ep0_process();
			if(!usb_device_is_online() || !usb_hid_is_valid())
			{
				break;
			}
			if(!sys_ctl.hid_vol_delay_cnt)
			{
				break;
			}
		}
#endif
	}
}
#endif

#if USB_SPK_SUPPORT
/*----------------------------------------------------------------------------*/
/**@brief  usbspk设置音量
   @param  无
   @return 无
   @note 供USB中断调用
*/
/*----------------------------------------------------------------------------*/
#pragma location="TASK_USBDEV_SEG"
void usb_spk_set_volume(void)
{
	u8 ret = usb_spk_set_volume_do();

#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
	if(ret == 3)
	{
		user_set_volume(usb_audio_ctl.spk_vol_set);
		put_msg(QUSBDEV_SET_VOL);
	}
	else if(ret == 2)
	{
		usb_hid_key(HID_CTL_VOLUP);
	}
	else if(ret == 1)
	{
		usb_hid_key(HID_CTL_VOLDOWN);
	}
#else
	user_set_volume(usb_audio_ctl.spk_vol_cur);
#endif
}

#endif /* USB_SPK_SUPPORT */

#if USB_MIC_SUPPORT
//修复USBMIC AUADCCON10初始化时没有减1问题
#pragma location="TASK_USBDEV_SEG"
void task_usbmic_process_fix(void)
{
	if(auadc_ctl.discard_cnt)
	{
		if(AUADCCON6 & (BIT(3)|BIT(1)))
		{
			AUADCCON6 &= ~(BIT(3)|BIT(1));
		}
		if(AUADCCON6 & (BIT(4)|BIT(2)))
		{
			AUADCCON6 &= ~(BIT(4)|BIT(2));
			auadc_ctl.discard_cnt--;
		}
		usb_audio_ctl.mic_speed = 2;
		usb_audio_ctl.adc_total = 0;
	}
	else
	{
		if(AUADCCON6 & (BIT(3)|BIT(1)))
		{
			AUADCCON6 &= ~(BIT(3)|BIT(1));
			usb_audio_ctl.adc_cnt += 0x300;
		}
		else if(AUADCCON6 & (BIT(4)|BIT(2)))
		{
			AUADCCON6 &= ~(BIT(4)|BIT(2));
			usb_audio_ctl.adc_cnt += 0x300;
		}
		usb_audio_ctl.adc_total = usb_audio_ctl.adc_cnt;
	}
}

/*----------------------------------------------------------------------------*/
/**@brief  打开usbmic
   @param  无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="TASK_USBDEV_SEG"
void usb_mic_enable(void)
{
	//printf("mic begin\n");
	memset((void *)FILTER_ADDR, 0, 0xA0);
	usb_mic_set_ptr((u8 *)(ADC_BUF_ADDR));
	auadc_start();
	iso_in_packet(64);
}

/*----------------------------------------------------------------------------*/
/**@brief  关闭usbmic
   @param  无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="TASK_USBDEV_SEG"
void usb_mic_disable(void)
{
	//printf("mic end\n");
	auadc_stop();
}
#endif /* USB_MIC_SUPPORT */

/*----------------------------------------------------------------------------*/
/**@brief  USBDEV初始化
   @param  无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="TASK_USBDEV_SEG"
void usb_device_init(void)
{
	usb_ep0_init();

#if USB_MASS_SUPPORT
	usb_mass_init();
#endif

#if (USB_SPK_SUPPORT || USB_MIC_SUPPORT)
	usb_audio_init(VOLUME_MAX+1);
#endif

#if USB_HID_SUPPORT
	usb_hid_init();
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief  USBDEV复位处理
   @param  无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="TASK_USBDEV_SEG"
void usb_device_reset(void)
{
	USBIE_DIS();

	usb_ep0_reset(DEV_TYPE);

#if USB_MASS_SUPPORT
	usb_mass_reset();
#endif

#if (USB_SPK_SUPPORT || USB_MIC_SUPPORT)
	usb_audio_reset();
#endif

#if USB_HID_SUPPORT
	usb_hid_reset();
#endif

	USBIE_EN();
}

/*----------------------------------------------------------------------------*/
/**@brief  USB中断
   @param  无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma vector = VECTOR_OFFSET(USB_CTL_INT)
#pragma register_bank=1
#pragma location="INTERRUPT"
__interrupt void usb_isr(void)
{
	ISR_ENTER();
	usb_isr_mask();
	ISR_EXIT();
}
#endif

