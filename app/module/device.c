/*****************************************************************************
 * Module    : Module
 * File      : device.c
 * Author    :
 * Email     :
 * Function  :
 *****************************************************************************/
#include "include.h"
#include "device.h"

#define DBG_DEV(...)                //printf(__VA_ARGS__)

#define DEV_TOTAL       9           //总共8个设备

type_dev_ctl dev_ctl;

#pragma location="DEVICE_INIT"
void device_init(void)
{
	memset(&dev_ctl, 0, sizeof(dev_ctl));
	dev_ctl.sd.delay = 40;
	dev_ctl.sd1.delay = 40;             //延时20*5=100ms
	dev_ctl.pc.delay = 60;                //值太小容易误检
	dev_ctl.udisk.delay = 40;
	dev_ctl.udisk1.delay = 40;
#if USE_7P7S_LED_AUX_MUX
	dev_ctl.line.delay = 10;
#else
	dev_ctl.line.delay = 40;
#endif
	dev_ctl.ear.delay = 40;
	dev_ctl.mic.delay = 40;
}

//获取设备总数
#pragma location="DEVICE_SEG"
u8 get_total_dev(void)
{
	return DEV_TOTAL;
}

//获取所有激活的设备
#pragma location="DEVICE_COM_SEG"
u8 device_get_actived(void)
{
	return dev_ctl.dev_actived;
}

//获取所有可用的设备
#pragma location="DEVICE_COM_SEG"
u8 device_get_valid(void)
{
	return (~dev_ctl.dev_aborted) & dev_ctl.dev_actived;
}

//设置设备不可用
#pragma location="DEVICE_COM_SEG"
void device_set_aborted(u8 dev_num)
{
	dev_ctl.dev_aborted |= BIT(dev_num);//设置设备中止
}


//检查设备是否可用
#pragma location="DEVICE_COM_SEG"
bool device_is_valid(u8 dev_num)
{
	if(dev_num >= get_total_dev())
	{
		return false;
	}
	return ((~dev_ctl.dev_aborted) & dev_ctl.dev_actived & BIT(dev_num));
}

bool usb_host_mass_read_end(void);

#pragma location="USB_SEG"
u8 host_select_phy(u8 phy_num)
{
	bool tmpie_sint;
	u8 cur_host_phy = usb_chk.host_phy;
	u8 cur_dev = fs_cur_dev();
	if(phy_num != cur_host_phy)
	{
		if(cur_dev == (DEVICE_UDISK + cur_host_phy))
		{
			tmpie_sint = IE_SINT;
			IE_SINT = 0;                            //暂停正在播放的UDISK
			usb_host_mass_read_end();
			usb_host_suspend();
		}
		usb_chk.host_phy = phy_num;
		usb_port_init(usb_chk.host_phy);
		if(device_activate_udisk_do())
		{
			//printf("change phy[%d]\n",usb_chk.host_phy);
			return 1;
		}
		if(cur_dev == (DEVICE_UDISK + cur_host_phy))
		{
			usb_chk.host_phy = cur_host_phy;       //恢复当前播放的UDISK
			usb_port_init(usb_chk.host_phy);
			device_activate_udisk_do();
			IE_SINT = tmpie_sint;
		}
		//printf("change phy[%d]\n",usb_chk.host_phy);
		return 2;
	}
	return 0;
}

//尝试去激活初始化设备
#pragma location="DEVICE_SEG"
bool device_activate_do(u8 dev_num)
{
	if((dev_num == DEVICE_SDMMC) || (dev_num == DEVICE_SDMMC1))
	{
		if(!sd_active_safe(dev_num))
		{
			printf("sd %d init fail\n",dev_num);
#ifdef TAG_AX2070
			if(dev_num == DEVICE_SDMMC)
			{
				dev_ctl.sd.actived = DEV_INVALID;           //初始化无效
			}
			else
			{
				dev_ctl.sd1.actived = DEV_INVALID;           //初始化无效
			}
#endif
			return false;
		}
		//printf("sd %d init ok\n",dev_num);
	}
	else if((dev_num == DEVICE_UDISK)
#if UDISK_DOUBLE_EN
	        || (dev_num == DEVICE_UDISK1)
#endif
	       )
	{
		usb_host_tune_set();
		u8 enum_sta = host_select_phy(dev_num - DEVICE_UDISK);
		//printf("enum_sta:%d\n",enum_sta);
		if(0 == enum_sta)
		{
			if(!device_activate_udisk_do())
			{
				return false;
			}
		}
		else if(2 == enum_sta)
		{
			return false;
		}
		usb_host_enable_testready(1);
	}
	device_activate_try(dev_num);
	dev_ctl.dev_actived |= BIT(dev_num);
	return true;
}

//尝试去激活初始化设备
#pragma location="DEVICE_COM_SEG"
bool device_activate(u8 dev_num)
{
	if(device_is_actived(dev_num))
	{
		return true;
	}
	if(!device_need_activate(dev_num))
	{
		return false;
	}
	return device_activate_do(dev_num);
}

//获取所有激活的设备数量
#pragma location="DEVICE_COM_SEG"
u8 device_get_actived_num(void)
{
	u8 dev_actived, dev_num = 0;
	dev_actived = dev_ctl.dev_actived;
	//printf("lib:%02x\n",dev_actived);
	for(u8 i = 0; i < MEDIA_DEVICE_NUM; i++)
	{
		if(device_is_online(i))
		{
			if(dev_actived & 0x01)
			{
				dev_num++;
			}
		}
		dev_actived >>= 1;
	}
	return dev_num;
}

//检查，并加载设备
#pragma location="DEVICE_SEG"
u8 device_change_check(void)
{

	u8 change_flag = DEVICE_IDLE;

	//检查是否有设备插入
	for(u8 i=0; i<MEDIA_DEVICE_NUM; i++)
	{
		if(device_need_activate(i))
		{
			device_check_stop(i);
			if(device_activate(i))
			{
				t_fat.dev_new = i;
				change_flag = DEVICE_CHANGE;
			}
		}
	}

	//检查当前设备的在线情况
	u8 cur_dev = fs_cur_dev();
	if((t_fat.dev_new == DEVICE_INVALID) && (cur_dev != DEVICE_INVALID) && (!device_is_actived(cur_dev)))
	{

		device_check_stop(cur_dev);
		change_flag = DEVICE_REMOVE;

		for(u8 i=0; i<MEDIA_DEVICE_NUM-1; i++)
		{
			if(++cur_dev >= MEDIA_DEVICE_NUM)
			{
				cur_dev = 0;
			}
			if(device_is_actived(cur_dev))          //如果有可用设备，则选择新设备
			{
				t_fat.dev_new = cur_dev;
				break;
			}
		}
	}
	return change_flag;
}

//获取USB_HOST的激活状态
#pragma location="DEVICE_SEG"
u8 usb_host_get_actived(void)
{
	if(usb_chk.host_phy == 0)
	{
		return dev_ctl.udisk.actived;
	}
	else
	{
		return dev_ctl.udisk1.actived;
	}
}

//设置USB_HOST的激活状态
#pragma location="DEVICE_SEG"
void usb_host_set_actived(u8 actived)
{
	if(usb_chk.host_phy == 0)
	{
		dev_ctl.udisk.actived = actived;
		if(actived == DEV_ACTIVED)
		{
			dev_ctl.dev_actived |= BIT(DEVICE_UDISK);
		}
		else
		{
			dev_ctl.dev_actived &= ~BIT(DEVICE_UDISK);
			if(actived == DEV_INVALID)
			{
				usb_host_enable_testready(0);
			}
		}
	}
#if UDISK_DOUBLE_EN
	else
	{
		dev_ctl.udisk1.actived = actived;
		if(actived == DEV_ACTIVED)
		{
			dev_ctl.dev_actived |= BIT(DEVICE_UDISK1);
		}
		else
		{
			dev_ctl.dev_actived &= ~BIT(DEVICE_UDISK1);
			if(actived == DEV_INVALID)
			{
				usb_host_enable_testready(0);
			}
		}
	}
#endif
}

#pragma location="DEVICE_SEG"
bool usb_host_unactived(void)
{
	if(usb_chk.host_phy == 0)
	{
		if(dev_ctl.udisk.actived != DEV_INVALID)
		{
			dev_ctl.udisk.actived = DEV_UNACTIVE;
			return true;
		}
	}
	else
	{
		if(dev_ctl.udisk1.actived != DEV_INVALID)
		{
			dev_ctl.udisk1.actived = DEV_UNACTIVE;
			return true;
		}
	}
	return false;
}

#pragma location="DEVICE_SEG"
bool usb_host_need_testready(void)
{
	if(usb_chk.host_phy == 0)
	{
		if(dev_ctl.udisk.actived == DEV_ACTIVED || dev_ctl.udisk.actived == DEV_NO_MEDIUM)
		{
			return true;
		}
	}
	else
	{
		if(dev_ctl.udisk1.actived == DEV_ACTIVED || dev_ctl.udisk1.actived == DEV_NO_MEDIUM)
		{
			return true;
		}
	}
	return false;
}

IAR_BIT_A bool udisk_suspend_flag,udisk1_suspend_flag;

#pragma location="USB_SEG"
void usb_host_resume(void)
{
	if(usb_chk.host_phy == 0)
	{
		if(udisk_suspend_flag)
		{
			//uart_putc('-');
			udisk_suspend_flag = 0;
			usb_host_resume_m();
			if(dev_ctl.udisk.actived == DEV_ACTIVED)
			{
				dev_ctl.udisk.actived = DEV_UNACTIVE; //resume后重新枚举，解决suspend有问题的读卡器或U盘
			}
		}
	}
	else
	{
		if(udisk1_suspend_flag)
		{
			//uart_putc('-');
			udisk1_suspend_flag = 0;
			usb_host_resume_m();
			if(dev_ctl.udisk.actived == DEV_ACTIVED)
			{
				dev_ctl.udisk.actived = DEV_UNACTIVE; //resume后重新枚举，解决suspend有问题的读卡器或U盘
			}
		}
	}
}

#pragma location="USB_SEG"
void usb_host_suspend(void)
{
	//uart_putc('|');
	if(usb_chk.host_phy == 0)
	{
		udisk_suspend_flag = 1;
	}
	else
	{
		udisk1_suspend_flag = 1;
	}
	usb_host_suspend_m();
}

#pragma location="USB_SEG"
void usb_host_disclk(void)
{
	if(usb_chk.host_phy == 0)
	{
		if(dev_ctl.udisk.actived == DEV_ACTIVED)
		{
			usb_host_suspend();
			dev_ctl.udisk.actived = DEV_UNACTIVE;
		}
	}
	else
	{
		if(dev_ctl.udisk1.actived == DEV_ACTIVED)
		{
			usb_host_suspend();
			dev_ctl.udisk1.actived = DEV_UNACTIVE;
		}
	}
}

#ifndef AX207X_TAG
//设置sd状态
#pragma location="SD_SEG"
void sd_set_status(u8 sta)
{
	if(sd_chk.group == DEVICE_SDMMC)
	{
		dev_ctl.sd.actived = sta;
		if(sta == DEV_ACTIVED)
		{
			dev_ctl.dev_actived |= BIT(DEVICE_SDMMC);
		}
		else
		{
			dev_ctl.dev_actived &= ~BIT(DEVICE_SDMMC);
		}
	}
	else if(sd_chk.group == DEVICE_SDMMC1)
	{
		dev_ctl.sd1.actived = sta;
		if(sta == DEV_ACTIVED)
		{
			dev_ctl.dev_actived |= BIT(DEVICE_SDMMC1);
		}
		else
		{
			dev_ctl.dev_actived &= ~BIT(DEVICE_SDMMC1);
		}
	}
}

//获取sd状态
#pragma location="SD_SEG"
u8 sd_get_status(void)
{
	if(sd_chk.group == DEVICE_SDMMC)
	{
		return dev_ctl.sd.actived;
	}
	else if(sd_chk.group == DEVICE_SDMMC1)
	{
		return dev_ctl.sd1.actived;
	}
	return 0;	// add by Merlin
}

#pragma location="SD_SEG"
void sd_unactived(void)
{
#if SD_DOUBLE_EN
	dev_ctl.sd.actived = DEV_ACTIVED;
	dev_ctl.sd1.actived = DEV_ACTIVED;
	dev_ctl.dev_actived |= BIT(DEVICE_SDMMC)|BIT(DEVICE_SDMMC1);
#else
	sd_clr_status(DEV_UNACTIVE);
#endif
}
#endif

#pragma location="SD_SEG"
bool sd_disk_read(void *buf, u32 lba)
{
	t_fat.t_item.win_cnt++;
	t_fat.t_item.dir_wincnt++;
#ifdef AX207X_TAG
	bool res = sd_read(buf, lba);
	if(res)
	{
		if(sd_chk.group == DEVICE_SDMMC)
		{
			dev_ctl.sd.actived = DEVICE_INVALID;
			dev_ctl.dev_actived &= ~BIT(DEVICE_SDMMC);
		}
		else if(sd_chk.group == DEVICE_SDMMC1)
		{
			dev_ctl.sd1.actived = DEVICE_INVALID;
			dev_ctl.dev_actived &= ~BIT(DEVICE_SDMMC1);
		}
	}
	return res;
#else
	return sd_read(buf, lba);
#endif
}


//SD_DECT复用SD_CLK时，检测到SD卡拔出需要比较长的时间，需要及早淡出,避免拔卡噪声
#pragma location="SD_SEG"
bool sd_active_a(void)
{
	bool res;

#if (DAC_FADE_EN && USE_SD_CLK_DET)
	if(task_ctl.work_sta == TASK_MUSIC)
	{
		dac_fade_out();
	}
#endif

	res = sd_active();

#if (DAC_FADE_EN && USE_SD_CLK_DET)
	if(res && (music_get_status() == STATUS_PLAY_PLAYING))
	{
		dac_fade_in(1);
	}
#endif
	return res;
}
