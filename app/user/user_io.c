/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户的IO设置，设备检测等
 *****************************************************************************/
#include "include.h"
#include "task.h"

extern void sd_init(void);
extern void usb_init(u8 host, u8 dev);
extern void usb_init_dev_only(u8 host, u8 dev);
extern void usb_init_std(u8 phy);
extern void usb_init_dev_only_std(u8 phy);
bool SD_FLAG=1;
extern u8 t_pwn_duty1, t_pwn_duty2;   // Duty range from 0x00 to 0xFF
//IO初始化
#pragma location="USER_IO_SEG"
void port_init(void)
{
#if 0  //!USE_SD_CLK_DET
	//SD检测脚
	SD_CHECK_DIR |= BIT(SD_CHECK_PIN);  //设为输入
	SD_CHECK_PU |= BIT(SD_CHECK_PIN);   //上拉设置
#if SD_DOUBLE_EN
	//SD检测脚
	SD1_CHECK_DIR |= BIT(SD1_CHECK_PIN);	//设为输入
	SD1_CHECK_PU |= BIT(SD1_CHECK_PIN);	//上拉设置
#endif
#endif
	//sd_group_sel(DEVICE_SDMMC);
	//sd_init();
#if 0
#if ((USB_HOST_PHY == USB_DEV_PHY) || (USB_DET != USBDET_OTG))
#if (USB_DET == USBDET_DEV_ONLY)
	usb_init_dev_only_std(USB_DEV_PHY);
#else
	usb_init_std(USB_HOST_PHY);
#endif
#else
	usb_init(USB_HOST_PHY, USB_DEV_PHY);
#endif
#endif
#if (AUX_DET_EN && !USE_7P7S_LED_AUX_MUX)
	//LineIn检测
	AUX_CHECK_DIR |= BIT(AUX_CHECK_PIN);
	AUX_CHECK_PU |= BIT(AUX_CHECK_PIN);
#endif
#if MIC_DET_EN
	//Mic检测
	MIC_CHECK_DIR |= BIT(MIC_CHECK_PIN);
	MIC_CHECK_PU |= BIT(MIC_CHECK_PIN);
#endif
#if (HP_DET_EN && !USE_SDCLK_SDDET_PHONE_MUX && !USE_SDCMD_IICCLK_PHONE_MUX)
	//耳机检测
	HP_CHECK_DIR |= BIT(HP_CHECK_PIN);	// P01
	HP_CHECK_PU |= BIT(HP_CHECK_PIN);
	#if (DAC_BUF_EN)		//HPDECT(use vcmbuf)
		PIE0 &= ~(BIT(1));	//  Set P01 Digital input disable
	#endif
#endif
#if DCIN_DET_EN
	//DC检测
	DCIN_CHECK_DIR |= BIT(DCIN_CHECK_PIN);
#endif

#if (MUTE_TYPE != MUTE_NO)
	MUTE_DIR &= ~BIT(MUTE_PIN);     //MUTE控制，开MUTE   由P11改为P00
	EN_MUTE();
#endif

#if PA_MODE_SEL_EN
	PA_MODE_DIR &= ~BIT(PA_MODE_PIN);
	PA_MODE_PORT &= ~BIT(PA_MODE_PIN);   //选择AB类功放
#endif

	P0DIR |= (BIT(1) | BIT(2));						// P01: A1  P02: B1
	PUP0 |= (BIT(1) | BIT(2));
	
	P1DIR |= (BIT(0) | BIT(1) | BIT(2) | BIT(3));	// P10: A2  P11: B2     P12: A3  P13: B3
	PUP1 |= (BIT(0) | BIT(1) | BIT(2) | BIT(3));


	LED0_DIR &= ~(BIT(LED0_PIN));					// P04 P05  LED显示
	LED0_PORT &= ~(BIT(LED0_PIN));
	
	LED1_DIR &= ~(BIT(LED1_PIN));
	LED1_PORT &= ~(BIT(LED1_PIN));
	t_pwn_duty1 = LED_LIGHT2;								// 初始化亮度  0xAA中等亮度
	t_pwn_duty2 = LED_LIGHT2;
	
#if TASK_FLASH_EN
	spi_port_init();
#endif
}

#pragma location="SD_SEG"
void sd_group_sel(u8 sd_group)
{
	if(sd_group == DEVICE_SDMMC)
	{
		SD_PORT_SEL();
	}
	else if(sd_group == DEVICE_SDMMC1)
	{
		SD1_PORT_SEL();
	}
	sd_chk.group = sd_group;
	//printf("sd group:%d\n",sd_chk.group);
}

//SD PORT初始化
#if SD_DOUBLE_EN
#pragma location="SD_SEG"
void sd_port_init(u8 type)
{
	SD_CMD_DIR |= BIT(SD_CMD_PIN);              //CMD
	SD_DAT0_DIR |= BIT(SD_DAT0_PIN);            //DAT0
	SD_DAT0_PU |= BIT(SD_DAT0_PIN);             //DAT0上拉
#if (USE_SDCMD_IICCLK_MIC_MUX || USE_SDCMD_IICCLK_PHONE_MUX)
	SD_CMD_PU500R_EN();                         //CMD开上拉500R
#else
	SD_CMD_PU |= BIT(SD_CMD_PIN);               //CMD上拉10K
#endif

	SD1_CMD_DIR |= BIT(SD1_CMD_PIN);            //SD1 CMD
	SD1_DAT0_DIR |= BIT(SD1_DAT0_PIN);          //SD1 DAT0
	SD1_DAT0_PU |= BIT(SD1_DAT0_PIN);           //SD1 DAT0上拉
	SD1_CMD_PU |= BIT(SD1_CMD_PIN);             //SD1 CMD上拉10K

#if !USE_SD_CLK_DET
	SD_CLK_DIR &= ~BIT(SD_CLK_PIN);             //CLK输出
	SD1_CLK_DIR &= ~BIT(SD1_CLK_PIN);           //SD1 CLK输出
#else
	if(sd_chk.group == DEVICE_SDMMC)
	{
		if((type == 1)
#if (/*(!USE_SDCLK_SDDET_PHONE_MUX)&&*/(!USE_SDCMD_IICCLK_PHONE_MUX)&&(!USE_SDCMD_IICCLK_MIC_MUX))
		        || (type == 3)
#endif
		  )
		{
			SD_CLK_PU &= ~BIT(SD_CLK_PIN);          //关闭上拉
			SD_CLK_DIR &= ~BIT(SD_CLK_PIN);         //CLK用于时钟输出
		}
		else
		{
			SD_CLK_DIR |= BIT(SD_CLK_PIN);          //CLK用于SD0卡检测
#if !USE_SDCLK_SDDET_PHONE_MUX  //SD0 CLK、SD检测、耳机检测复用，使用外部10K上拉电阻
			SD_CLK_PU |= BIT(SD_CLK_PIN);           //打开上拉
#endif
		}
		SD1_CLK_DIR |= BIT(SD1_CLK_PIN);            //CLK用于SD1卡检测
		SD1_CLK_PU |= BIT(SD1_CLK_PIN);             //打开上拉
	}
	else if(sd_chk.group == DEVICE_SDMMC1)
	{
		if((type == 1)
#if (/*(!USE_SDCLK_SDDET_PHONE_MUX)&&*/(!USE_SDCMD_IICCLK_PHONE_MUX)&&(!USE_SDCMD_IICCLK_MIC_MUX))
		        || (type == 3)
#endif
		  )
		{
			SD1_CLK_PU &= ~BIT(SD1_CLK_PIN);        //关闭上拉
			SD1_CLK_DIR &= ~BIT(SD1_CLK_PIN);       //CLK用于时钟输出
		}
		else
		{
			SD1_CLK_DIR |= BIT(SD1_CLK_PIN);        //CLK用于SD1卡检测
			SD1_CLK_PU |= BIT(SD1_CLK_PIN);         //打开上拉
		}
		SD_CLK_DIR |= BIT(SD_CLK_PIN);              //CLK用于SD0卡检测
#if !USE_SDCLK_SDDET_PHONE_MUX  //SD0 CLK、SD检测、耳机检测复用，使用外部10K上拉电阻
		SD_CLK_PU |= BIT(SD_CLK_PIN);               //打开上拉
#endif
	}
#endif
}

#else
//SD PORT初始化
#pragma location="SD_SEG"
void sd_port_init(u8 type)
{
	SD_PORT_SEL();

	SD_CMD_DIR |= BIT(SD_CMD_PIN);              //CMD
	SD_DAT0_DIR |= BIT(SD_DAT0_PIN);            //DAT0
	SD_DAT0_PU |= BIT(SD_DAT0_PIN);             //DAT0上拉
#if (USE_SDCMD_IICCLK_MIC_MUX || USE_SDCMD_IICCLK_PHONE_MUX || USE_SDCMD_IICCLK_SDDET_MUX)
	SD_CMD_PU500R_EN();                         //CMD开上拉500R
#else
	SD_CMD_PU |= BIT(SD_CMD_PIN);               //CMD上拉10K
#endif

#if !USE_SD_CLK_DET
	SD_CLK_DIR &= ~BIT(SD_CLK_PIN);             //CLK输出
	SD1_CLK_DIR &= ~BIT(SD1_CLK_PIN);           //SD1 CLK输出
#else
	if((type == 1)
#if (/*(!USE_SDCLK_SDDET_PHONE_MUX)&&*/(!USE_SDCMD_IICCLK_PHONE_MUX)&&(!USE_SDCMD_IICCLK_MIC_MUX)&&(!USE_SDCMD_IICCLK_SDDET_MUX))
	        || (type == 3)
#endif
	  )
	{
		SD_CLK_PU &= ~BIT(SD_CLK_PIN);          //关闭上拉
		SD_CLK_DIR &= ~BIT(SD_CLK_PIN);         //CLK用于时钟输出
	}
	else
	{
		SD_CLK_DIR |= BIT(SD_CLK_PIN);          //CLK用于SD卡检测
#if !USE_SDCLK_SDDET_PHONE_MUX  //SD CLK、SD检测、耳机检测复用，使用外部10K上拉电阻
		SD_CLK_PU |= BIT(SD_CLK_PIN);           //打开上拉
#endif
	}
#endif
}
#endif /*END SD_DOUBLE_EN*/

//SD检测, 外接 SD卡
#pragma location="SD_SEG"
void sd_detect(void)
{
#if USE_SDCLK_SDDET_PHONE_MUX  //SD CLK、SD检测、耳机检测复用，使用外部10K上拉电阻
	if(0 == (SD_CHECK_DIR & BIT(SD_CHECK_PIN)))
	{
		return;   //IO输出状态
	}

	if((scan_ctl.sddect_val < 115)
	        || ((scan_ctl.sddect_val > 145) && (scan_ctl.sddect_val < 190)))
	{
#else

#if USE_SDCMD_IICCLK_SDDET_MUX
	if(!(sd_port_sta()))
	{
		return;
	}
	SD_CMD_PU |= BIT(SD_CMD_PIN);           //CMD开上拉10K
	SD_CMD_PU500R_DIS();                    //CMD关上拉500R
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
#else
	if(0 == (SD_CHECK_PU & BIT(SD_CHECK_PIN)))
	{
		return;
	}
#endif
	if(0==(SD_CHECK_PORT & BIT(SD_CHECK_PIN)))// SD_FLAG)       //SD卡插入检测IO会被拉低
	{
#endif
		if(device_insert(DEVICE_SDMMC))
		{
			b_sys.dev_change = 1; 
			sd_insert();
			printf("SD0 IN\n");
		}
	}
	else
	{
		if(device_remove(DEVICE_SDMMC))
		{
			if(fs_cur_dev() == DEVICE_SDMMC)
			{
				b_sys.dev_change = 1;
			}
			//sd_remove();
			printf("SD0 OUT\n");
		}
	}

#if USE_SDCMD_IICCLK_SDDET_MUX
	SD_CMD_PU &= ~BIT(SD_CMD_PIN);          //CMD关上拉10K
	SD_CMD_PU500R_EN();                     //CMD开上拉500R
#endif
}

// 内接 SD卡, 如果使用内接SPI-Flash时, 将停用内接 SD卡的侦测
void sd1_detect(void)
{
	if(u_spi.spi_online)
	{
		return;
	}
	if(0 == (SD1_CHECK_PU & BIT(SD1_CHECK_PIN)))
	{
		return;
	}
	if(!(SD1_CHECK_PORT & BIT(SD1_CHECK_PIN)))        //SD卡插入检测IO会被拉低
	{
		if(device_insert(DEVICE_SDMMC1))
		{
			b_sys.dev_change = 1;
			sd_insert();
			printf("SD1 IN\n");
		}
	}
	else
	{
		if(device_remove(DEVICE_SDMMC1))
		{
			if(fs_cur_dev() == DEVICE_SDMMC1)
			{
				b_sys.dev_change = 1;
			}
			printf("SD1 OUT\n");
		}
	}
}

//sddect的I/O状态
#pragma location="SD_SEG"
bool sd_port_sta(void)
{
	if(0 == (SD_CHECK_DIR & BIT(SD_CHECK_PIN)))
	{
		return 0;   //IO输出状态
	}
	else
	{
		return 1;   //IO输入状态
	}
}

//USB检测函数
#pragma location="USER_IO_SEG"
void usb_detect(void)
{
#if UDISK_DOUBLE_EN
	asm("push 0xdc");//USBCON1
	u8 usb_conn;
	for(u8 i = 0; i < 2; i++)        //检测PHY0,PHY1上的主从设备
	{
		usb_conn = usb_connected(i);
#if (!UDISK_DOUBLE_EN)
		if(USB_HOST_PHY == i)       //单UDISK功能
#endif
		{
			if(USB_CONNECTED_UDISK == usb_conn)
			{
				if(device_insert(DEVICE_UDISK + i))
				{
					b_sys.dev_change = 1;
					printf("UDISK[%d] IN\n",i);
				}
			}
			else
			{
				if(device_remove(DEVICE_UDISK + i))
				{
					if(fs_cur_dev() == DEVICE_UDISK+i)
					{
						b_sys.dev_change = 1;
					}
					printf("UDISK[%d] OUT\n",i);
				}
			}
		}

#if TASK_USBDEV_EN
		if(USB_DEV_PHY == i)                    //指定一组作为DEVICE
		{
			//printf("usb_conn:%d  ",usb_conn);
			if(USB_CONNECTED_PC == usb_conn)
			{
				if(device_insert(DEVICE_PC))
				{
					printf("PC IN\n");
				}
			}
			else
			{
				if(device_remove(DEVICE_PC))
				{
					printf("PC OUT\n");
				}
			}
		}
#endif
	}


	asm("pop 0xdc");
#else
#if ((USB_HOST_PHY == USB_DEV_PHY) || (USB_DET != USBDET_OTG))
	asm("push 0xdc");//USBCON1
	u8 usb_conn = usb_connected(USB_HOST_PHY);

#if (USB_DET != USBDET_DEV_ONLY)
	if(USB_CONNECTED_UDISK == usb_conn)
	{
		if(device_insert(DEVICE_UDISK))
		{
			b_sys.dev_change = 1;
			//printf("UDISK IN\n");
		}
	}
	else
	{
		if(device_remove(DEVICE_UDISK))
		{
			if(fs_cur_dev() == DEVICE_UDISK)
			{
				b_sys.dev_change = 1;
			}
			//printf("UDISK OUT\n");
		}
	}
#endif
#if TASK_USBDEV_EN
	if(USB_CONNECTED_PC == usb_conn)
	{
		if(device_insert(DEVICE_PC))
		{
			printf("PC IN\n");
		}
	}
	else
	{
		if(device_remove(DEVICE_PC))
		{
			printf("PC OUT\n");
		}
	}
#endif
#else
	//USB DEVICE与HOST用的PHY不同
	u8 usb_conn = usb_connected(USB_DEV_PHY);
#if TASK_USBDEV_EN
	if(USB_CONNECTED_PC == usb_conn)
	{
		if(device_insert(DEVICE_PC))
		{
			printf("PC IN\n");
		}
	}
	else
	{
		if(device_remove(DEVICE_PC))
		{
			printf("PC OUT\n");
		}
	}
#endif
	usb_conn = usb_connected(USB_HOST_PHY);
	if(USB_CONNECTED_UDISK == usb_conn)
	{
		if(device_insert(DEVICE_UDISK))
		{
			b_sys.dev_change = 1;
			//printf("UDISK IN\n");
		}
	}
	else
	{
		if(device_remove(DEVICE_UDISK))
		{
			if(fs_cur_dev() == DEVICE_UDISK)
			{
				b_sys.dev_change = 1;
			}
			//printf("UDISK OUT\n");
		}
	}
#endif
	asm("pop 0xdc");
#endif
}

#if AUX_DET_EN
//LineIn检测函数
#pragma location="USER_IO_SEG"
void aux_detect(void)
{
	if(!(AUX_CHECK_PORT & BIT(AUX_CHECK_PIN)))
	{
		if(device_insert(DEVICE_LINEIN))
		{
			//printf("AUX IN\n");
		}
	}
	else
	{
		if(device_remove(DEVICE_LINEIN))
		{
			//printf("AUX OUT\n");
		}
	}
}
#endif

#if MIC_DET_EN
//Mic检测函数
#pragma location="USER_IO_SEG"
void mic_detect(void)
{
#if USE_SDCMD_IICCLK_MIC_MUX
	if(!(sd_port_sta()))
	{
		return;
	}
	SD_CMD_PU |= BIT(SD_CMD_PIN);           //CMD开上拉10K
	SD_CMD_PU500R_DIS();                    //CMD关上拉500R
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

#else
	if((0 == (MIC_CHECK_PU & BIT(MIC_CHECK_PIN)))
	        || (0 == (MIC_CHECK_DIR & BIT(MIC_CHECK_PIN))))
	{
		return;
	}
#endif

	if(!(MIC_CHECK_PORT & BIT(MIC_CHECK_PIN)))
	{
		if(device_insert(DEVICE_MIC))
		{
			//printf("MIC IN\n");
		}
	}
	else
	{
		if(device_remove(DEVICE_MIC))
		{
			//printf("MIC OUT\n");
		}
	}

#if USE_SDCMD_IICCLK_MIC_MUX
	SD_CMD_PU &= ~BIT(SD_CMD_PIN);          //CMD关上拉10K
	SD_CMD_PU500R_EN();                     //CMD开上拉500R
#endif
}
#endif

#if HP_DET_EN
//Headphone检测函数
#pragma location="USER_IO_SEG"
void headphone_detect(void)
{
#if USE_SDCLK_SDDET_PHONE_MUX               //SD CLK、SD检测、耳机检测复用
	if(0 == (SD_CHECK_DIR & BIT(SD_CHECK_PIN)))
	{
		return;   //IO输出状态
	}

	if(scan_ctl.sddect_val < 145)

//#elif DAC_BUF_EN             //使用VCM BUF时ADC检测，开内部上拉10K
#elif DAC_BUF_EN             //使用VCM BUF时ADC检测，开内部上拉10K

	if(scan_ctl.hpdect_val < 200)

#else
#if USE_SDCMD_IICCLK_PHONE_MUX              //复用SDCMD/IICCLK进行耳机检测
	if(!(sd_port_sta()))
	{
		return;
	}
	SD_CMD_PU |= BIT(SD_CMD_PIN);           //CMD开上拉10K
	SD_CMD_PU500R_DIS();                    //CMD关上拉500R
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
#endif
	if((HP_CHECK_PORT & BIT(HP_CHECK_PIN)))
#endif
	{
		if(device_insert(DEVICE_HP))
		{
			printf("HP IN\n");
		}
		//mute_enable();
		MUTE_EN();
	}
	else
	{
		if(device_remove(DEVICE_HP))
		{
			printf("HP OUT\n");
			//mute_disable();
			if(sys_ctl.volume !=0)
				UN_MUTE();
		}
	}

#if USE_SDCMD_IICCLK_PHONE_MUX
	SD_CMD_PU &= ~BIT(SD_CMD_PIN);          //CMD关上拉10K
	SD_CMD_PU500R_EN();                     //CMD开上拉500R
#endif

#if 0
#if (MUTE_TYPE != MUTE_NO)
	if(device_is_online(DEVICE_HP))
	{
		MUTE_EN();
	}
	else if(device_remove(DEVICE_HP))
	{
		if((!b_dac.mute_en)
#if DAC_POWER_SPEED
		        && (dac_init_ctl.sta == DAC_INIT_END)
#endif
		  )
		{
			UN_MUTE();
		}
	}
#endif
#endif
}
#endif

#if DCIN_DET_EN
//dcin检测函数
#pragma location="USER_IO_SEG"
void dcin_detect(void)
{
	static u8 cnt_in = 0,cnt_out = 0;
	if((DCIN_CHECK_PORT & BIT(DCIN_CHECK_PIN)))
	{
		cnt_out = 0;
		if(cnt_in < 20)
		{
			cnt_in++;
			poweroff_ctl.dc_in_sta = DC_UNSTABLE;
		}
		else if(cnt_in == 20)
		{
			cnt_in = 30;
			poweroff_ctl.dc_in_sta = DC_IN;
			printf("dc in...\n");
		}
	}
	else
	{
		cnt_in = 0;
		if(cnt_out < 20)
		{
			cnt_out++;
			poweroff_ctl.dc_in_sta = DC_UNSTABLE;
		}
		else if(cnt_out == 20)
		{
			cnt_out = 30;
			poweroff_ctl.dc_in_sta = DC_OUT;
			printf("dc out...\n");
		}
	}
}
#endif

#pragma location="USER_IO_SEG"
void mute_init(void)
{
	MUTE_PORT_OUTPUT;
	EN_MUTE();
}


//开MUTE函数, 停止声音输出
#pragma location="USER_IO_SEG"
void MUTE_EN(void)
{
#if (MUTE_TYPE != MUTE_NO)
	EN_MUTE();
#endif
	//printf("mute\n");
}


//关MUTE函数, 输出声音
#pragma location="USER_IO_SEG"
void UN_MUTE(void)
{
#if (MUTE_TYPE != MUTE_NO)
	DIS_MUTE();
#endif
	//printf("mute\n");
}

//开MUTE函数, 停止声音输出
#pragma location="USER_IO_SEG"
void mute_enable(void)
{
	if(!b_dac.mute_en)
	{
		b_dac.mute_en = 1;
		mute_status_change = true;
	}
#if 0
#if (MUTE_TYPE != MUTE_NO)
	//EN_MUTE();
	MUTE_PORT_LOW;
#endif
#endif
	//printf("mute\n");
}


//关MUTE函数, 让声音出来
#pragma location="USER_IO_SEG"
void mute_disable(void)
{
	if(b_dac.mute_en)
	{
		b_dac.mute_en = 0;
		mute_status_change = true;
	}
#if 0
#if (MUTE_TYPE != MUTE_NO)
	DIS_MUTE();
#endif
#endif
	//printf("unmute\n");
}

#pragma location="USER_IO_SEG"
void mute_ctl_auto(void)
{	// running by 5mSec
	static u8 cnt = 0;

	if(mute_status_change)
	{
		if(b_dac.mute_en)
		{
			MUTE_EN();
			mute_status_change = false;
		}
		else
		{
			if(cnt == 0)
			{
				UN_MUTE();
				mute_status_change = false;
			}
			else
			{
				cnt--;
			}
		}
	}
	else
	{
		cnt = 0;
	}
}


#pragma location="USER_IO_SEG"
void io_output_clk_start(void)
{
#if (FM_CLOCK == USE_IO_32KHZ_CLK)
	CLKCON2 &= ~BIT(7);      //RTCC clock source select XOSC
	//CLKCON2 |= BIT(7);       //RTCC clock source select RC32K
	CLKCON3 |= BIT(6);       //output clk 32.768KHz

#elif (FM_CLOCK == USE_IO_12MHZ_CLK)
	CLKCON3 &= ~(BIT(7) | BIT(6));
#endif

	AIPCON8 &= ~(BIT(2) | BIT(1) | BIT(0));
	AIPCON8 |= FM_CLK_IO;

#if (FM_CLK_IO == CLK_P16)
	P1DIR &= ~BIT(6);
#elif (FM_CLK_IO == CLK_P15)
	P1DIR &= ~BIT(5);
#elif (FM_CLK_IO == CLK_P06)
	P0DIR &= ~BIT(6);
#elif (FM_CLK_IO == CLK_P32)
	P3DIR &= ~BIT(2);
#elif (FM_CLK_IO == CLK_P33)
	P3DIR &= ~BIT(3);
#elif (FM_CLK_IO == CLK_P34)
	P3DIR &= ~BIT(4);
#elif (FM_CLK_IO == CLK_P35)
	P3DIR &= ~BIT(5);
#else
#error "请选择提供时钟的IO"
#endif
}

#pragma location="USER_IO_SEG"
void io_output_clk_stop(void)
{
#if (FM_CLK_IO == CLK_P16)
	P1DIR |= BIT(6);
#elif (FM_CLK_IO == CLK_P15)
	P1DIR |= BIT(5);
#elif (FM_CLK_IO == CLK_P06)
	P0DIR |= BIT(6);
#elif (FM_CLK_IO == CLK_P32)
	P3DIR |= BIT(2);
#elif (FM_CLK_IO == CLK_P33)
	P3DIR |= BIT(3);
#elif (FM_CLK_IO == CLK_P34)
	P3DIR |= BIT(4);
#elif (FM_CLK_IO == CLK_P35)
	P3DIR |= BIT(5);
#endif
}
