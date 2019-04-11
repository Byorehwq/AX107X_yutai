/*****************************************************************************
 * Module    : Task
 * File      : task_usbdev.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : USB DEVICE��������
 *****************************************************************************/
#include "include.h"

#if TASK_USBDEV_EN

bool usbdev_sta;
#pragma location="TASK_USBDEV_SEG"
static void task_usbdev_enter(void)
{
	SELECT_IROM2();             //USBDEV��ʼ��ʱѡIROM2
	if(!device_is_actived(DEVICE_PC))
	{
		task_ctl.work_sta = TASK_EXIT;
		usbdev_sta = 0;
		return;
	}
	else
	{
		usbdev_sta = 1;
	}

	if(usb_chk.host_phy != usb_chk.dev_phy)
	{
		usb_port_init(USB_DEV_PHY);
		usb_host_enable_testready(0);
	}

	dac_out_init(DAC_SPA_S48K);
	user_set_volume((VOLUME_MAX+1)/2);//��������
	mute_disable();
#if DAC_FADE_EN
	dac_fade_in(1);
#endif

	fs_unmount();

#if USB_MIC_SUPPORT
	amux_mic_init(GAIN_27DB);           //��ʼ��MIC��AMUX������ΪMIC������
	auadc_init(CH_SEL_USBMIC);          //��ʼ��ADC
#endif

	usb_device_tune_set();
#if USB_MASS_SUPPORT
#if UDISK_DOUBLE_EN
#ifdef AX207X_TAG
	usbdev_init_double_sd();
#else
	usbdev_init();
#endif
#else
	usbdev_init();
#endif
#endif
	usb_device_init();
	usb_device_reset();

#if ENERGY_DISP_EN
	energy_init();
#endif
#if (DISP_TYPE==DISP_LED)
	ocx.led_sta = LED_STA_ON;
#endif
}

#pragma location="TASK_USBDEV_SEG"
static void task_usbdev_exit(void)
{
	//printf("task_usbdev_exit\n");

	if(!usbdev_sta)
	{
		return;
	}

#if DAC_FADE_EN
	dac_fade_out();
	dac_fade_wait();
#endif

	USBIE_DIS();

#if ENERGY_DISP_EN
	energy_stop();
#endif
#if USB_MASS_SUPPORT
	usb_mass_exit();
#endif
#if USB_MIC_SUPPORT
	auadc_stop();
	amux_exit();
#endif

#if USB_MASS_SUPPORT
	usbdev_exit();
#endif

#if (USB_DET == USBDET_DEV_ONLY)
	usb_switch_otg(USB_DEV_PHY);            //�л���IDLE״̬
#else
	if(!device_is_actived(DEVICE_PC))
	{
		usb_switch_host(USB_DEV_PHY);       //��DPDM�л���HOST���״̬
	}
	else
	{
		usb_switch_otg(USB_DEV_PHY);        //�л���IDLE״̬
	}
#endif

	if(usb_chk.host_phy != usb_chk.dev_phy)
	{
		usb_port_init(usb_chk.host_phy);
		if(usb_host_unactived())
		{
			b_sys.dev_change = 1;
		}
	}
#ifndef AX207X_TAG
	sd_unactived();
	if(device_is_online(DEVICE_SDMMC)||device_is_online(DEVICE_SDMMC1))
	{
		b_sys.dev_change = 1;
	}
#endif

#if USB_DOWNLOAD_FLASH
	spi_sd_mux_exit();
#endif
}
extern void spi_download(void);
#pragma location="TASK_USBDEV_SEG"
static void task_usbdev_event(void)
{
	WATCHDOG_CLR();
	mem_event();

	usb_ep0_process();
#if USB_MASS_SUPPORT

#if USB_DOWNLOAD_FLASH
	if(device_is_online(DEVICE_SDMMC)||device_is_online(DEVICE_SDMMC1))
	{
		usb_mass_process();  //����sd��
	}
	else
	{
		spi_sd_mux_enter();
		spi_port_init();
		spi_download();      //����flash
	}
	//spi_download();      //����flash
#else
	usb_mass_process();  //����sd��
#endif

#endif
#if USB_HID_SUPPORT
	usb_hid_sta_get();
#endif


	if(!device_is_actived(DEVICE_PC))
	{
		task_ctl.work_sta = TASK_EXIT;
	}
}

/*----------------------------------------------------------------------------*/
/**@brief  USBDEV��Ϣ����
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="TASK_USBDEV_SEG"
static void task_usbdev_deal_msg(u8 msg)
{
	switch(msg)
	{
#if USB_HID_SUPPORT

	case KU_MUTE:
		usb_hid_send(HID_CTL_MUTE);
		break;
		/*
		        case KU_STOP:
		            usb_hid_key(HID_CTL_STOP);
		            break;
		*/
	case KU_VOL_UP:
	case KH_VOL_UP:
	case KH_PLUS:
		usb_hid_vol(1);
		showvol();
		break;

	case KU_VOL_DOWN:
	case KH_VOL_DOWN:
	case KH_MINUS:
		usb_hid_vol(0);
		showvol();
		break;

	case QUSBDEV_SET_VOL:
		showvol();
		break;

	case KU_NEXT:
	case KU_PLUS:
		usb_hid_key(HID_CTL_NEXTFILE);
		break;

	case KU_PREV:
	case KU_MINUS:
		usb_hid_key(HID_CTL_PREFILE);
		break;

	case KL_NEXT:
	case KH_NEXT:
		usb_hid_key(HID_CTL_FASTFORWARD);
		break;

	case KU_PLAY:
		usb_hid_key(HID_CTL_PLAYPAUSE);
		break;
#else
	case KU_VOL_UP:
	case KH_VOL_UP:
	case KU_VOL_DOWN:
	case KH_VOL_DOWN:
		break;
#endif
	case KU_EQ:
		user_set_eq(sys_ctl.eq_num + 1);
		showeq();
		break;

#if ENERGY_DISP_EN
	case KL_PLAYMODE:
		if(!ocx.disp_energy)
		{
			energy_start();
		}
		else
		{
			energy_close();
		}
		disp.update = 1;
		break;
#endif
	case KU_POWER:
		break;

	default:
		deal_msg(msg);
		break;
	}
}

#pragma location="TASK_USBDEV_SEG"
void task_usbdev(void)
{
	printf("task_usbdev\n");
	task_usbdev_enter();
	while(task_ctl.work_sta == TASK_USBDEV)
	{
		task_usbdev_event();
		task_usbdev_deal_msg(get_msg());
		task_usbdev_display();
	}
	task_usbdev_exit();
}

#endif
