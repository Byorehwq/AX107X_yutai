/*****************************************************************************
 * Module    : user
 * File      : user_spi.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户的SPI操作
 *****************************************************************************/
#include "include.h"
#include "spi.h"

IAR_XDATA_A type_user_spi u_spi;
u8 flash_chipnum;                      //flash序号
extern u8 dec_bs[512+6];               //DECbuf，用于暂存flash数据
extern  u8 flash_music_total_tbl[7];
extern u16 get_dirstart(void);

IAR_CONST u8 ReadIDCMD[4] =
{
	0x9F,0x90,0xAB,0x15               //读取flashID命令
};

//SPI IO初始化
#pragma location="USER_SPI_SEG"
void spi_port_init(void)
{
#if !SOFT_SPI    
	SPI_PORT_SEL();
#endif
    

#if (USE_SD_CLK_DET && USE_SDCMD_IIC_SPIDAT)
	SPI_CLK_DIR |= BIT(SPI_CLK_PIN);
#else
	SPI_CLK_DIR &= ~BIT(SPI_CLK_PIN);       //CLK
#endif
	SPI_DO_DIR &= ~BIT(SPI_DO_PIN);         //DO
	SPI_CS_DIR &= ~BIT(SPI_CS_PIN);         //CS
#if (SPI_MODE_SEL == SPI_THREE_WIRES_MODE)
	SPI_DI_DIR |=  BIT(SPI_DI_PIN);         //DI
#endif

#if !SOFT_SPI
	spi_init();
#endif
}

#pragma location="USER_SPI_SEG"
void spi_sd_mux_enter(void)
{
	SDCON0 = 0;
}


#pragma location="USER_SPI_SEG"
void spi_sd_mux_exit(void)
{
	if(device_is_online(DEVICE_SDMMC)||device_is_online(DEVICE_SDMMC1))
	{
		SDCON0 = 1;
	}
}

#pragma location="USER_SPI_SEG"
void spi_cs_enable(void)
{
	switch(flash_chipnum)
	{
	case 0x01:
		SPI_CS_PORT &= ~BIT(SPI_CS_PIN);
		break;
	default:
		break;
	}
}

#pragma location="USER_SPI_SEG"
void spi_cs_disable(void)
{
	switch(flash_chipnum)
	{
	case 0x01:
		SPI_CS_PORT |= BIT(SPI_CS_PIN);
		break;
	default:
		break;
	}
}
//软件SPI写入一Byte
#pragma location="USER_SPI_SEG"
void spi_write_byte_soft(u8 data)
{
	u8 i;
	if(SPI_MODE_SEL == SPI_THREE_WIRES_MODE)
	{
		for(i=0; i<8; i++)
		{
			SPI_CLK_PORT &= ~BIT(SPI_CLK_PIN);
			if(data & 0x80)
			{
				SPI_DO_PORT |= BIT(SPI_DO_PIN);
			}
			else
			{
				SPI_DO_PORT &= ~BIT(SPI_DO_PIN);
			}
			SPI_CLK_PORT |= BIT(SPI_CLK_PIN);//上升沿
			data <<= 1;
		}
	}
	else
	{
		SPI_2Wire_TX();
		for(i=0; i<8; i++)
		{
			SPI_CLK_PORT &= ~BIT(SPI_CLK_PIN);
		#if 1
			asm("MOV	A,R1");
			asm("MOV	C,ACC.7");
			asm("MOV	P1.3,C");
			asm("CLR	C");
			asm("RLC	A");
			asm("MOV	R1,A");
		#else
			if(data & 0x80)
			{
				SPI_DO_PORT |= BIT(SPI_DO_PIN);
			}
			else
			{
				SPI_DO_PORT &= ~BIT(SPI_DO_PIN);
			}
		#endif
			SPI_CLK_PORT |= BIT(SPI_CLK_PIN);//上升沿
			data <<= 1;
		}
	}
}

//软件SPI读取一Byte
#pragma location="USER_SPI_SEG"
u8 spi_read_byte_soft(void)
{
	u8 ch,i;
	ch = 0;
#if (SPI_MODE_SEL == SPI_THREE_WIRES_MODE)
	for(i=0; i<8; i++)
	{
		ch <<= 1;
		SPI_CLK_PORT |= BIT(SPI_CLK_PIN);//下降沿
		asm("nop");
		asm("nop");
		asm("nop");
		SPI_CLK_PORT &= ~BIT(SPI_CLK_PIN);
		asm("nop");
		if(SPI_DI_PORT & BIT(SPI_DI_PIN))
		{
			ch |= 0x01;
		}
		else
		{
			ch &= ~0x01;
		}
	}
#else
	SPI_2Wire_RX();
	for(i=0; i<8; i++)
	{
		SPI_CLK_PORT |= BIT(SPI_CLK_PIN);//下降沿
		ch <<= 1;
	//	asm("nop");
	//	asm("nop");
	//	asm("nop");
		SPI_CLK_PORT &= ~BIT(SPI_CLK_PIN);
		asm("nop");
		if(SPI_DO_PORT & BIT(SPI_DO_PIN))
		{
			ch |= 0x01;
		}
		else
		{
	//		ch &= ~0x01;
		}
	}
#endif
	return ch;
}
#pragma location="USER_SPI_SEG"
u8 spi_read_byte(void)
{
	u8 temp;
#if SOFT_SPI
	temp = spi_read_byte_soft();
#else
	temp = spi_read_byte_hard();
#endif
	return temp;
}

#pragma location="USER_SPI_SEG"
void spi_write_byte(u8 data)
{
#if SOFT_SPI
	spi_write_byte_soft(data);
#else
	spi_write_byte_hard(data);
#endif
}

#pragma location="USER_SPI_SEG"
void spi_write_soft(u8 *buf, u32 addr,u16 len)
{
#if (USE_SD_CLK_DET && USE_SDCMD_IIC_SPIDAT)
	SPI_CLK_DIR &= ~BIT(SPI_CLK_PIN);
	SPI_CLK_PU &= ~BIT(SPI_CLK_PIN);          //关闭上拉
#endif
	spi_wait_busy();
	spi_write_en();
	spi_cs_enable();
	spi_write_byte(0x02);
	spi_send_addr_2(addr);
	for(u16 i = 0; i < len; i++)
	{
		spi_write_byte(*(buf+i));          //发送数据
	}
	spi_cs_disable();
#if (USE_SD_CLK_DET && USE_SDCMD_IIC_SPIDAT)
	SPI_CLK_DIR |= BIT(SPI_CLK_PIN);
	SPI_CLK_PU  |= BIT(SPI_CLK_PIN);          //打开上拉
#endif
}

#pragma location="USER_SPI_SEG"
void spi_read_soft(u8 *buf, u32 addr, u16 len)
{
#if (USE_SD_CLK_DET && USE_SDCMD_IIC_SPIDAT)
	SPI_CLK_DIR &= ~BIT(SPI_CLK_PIN);
	SPI_CLK_PU &= ~BIT(SPI_CLK_PIN);          //关闭上拉
#endif

	spi_wait_busy();
	spi_cs_enable();
	spi_write_byte(0x03);
	spi_send_addr_2(addr);
	for(u16 i = 0; i < len; i++)
	{
		*(buf + i) = spi_read_byte();
	}
	spi_cs_disable();
#if (USE_SD_CLK_DET && USE_SDCMD_IIC_SPIDAT)
	SPI_CLK_DIR |= BIT(SPI_CLK_PIN);
	SPI_CLK_PU  |= BIT(SPI_CLK_PIN);          //打开上拉
#endif
}

#pragma location="USER_SPI_SEG"
void spi_write(u8 *buf, u32 addr,u16 len)
{
#if SOFT_SPI
	spi_write_soft(buf, addr,len);
#else
	spi_write_hard(buf, addr,len);
#endif
}


#pragma location="USER_SPI_SEG"
void spi_read(u8 *buf, u32 addr, u16 len)
{
#if 0	// dump index read
	//if(addr < 0x400L)
	{
	//	printf("\n%04X,%03X_",(u16)addr, (u16)flash_music.pos);
		printf("\n%04X_", (u16)flash_music.pos);
	}
#endif

#if SOFT_SPI
	spi_read_soft(buf, addr,len);
#else
	spi_read_hard(buf, addr,len);
#endif

#if 0
	if((sys_ctl.mode_play == ITEM_LOOP_SINGLE) &&
		(flash_music.len == flash_music.pos+2) &&
		(music_get_status() == STATUS_PLAY_PLAYING)
		)
		flash_music.pos = 0;
#endif
}

#pragma location="USER_SPI_SEG"
u32 spi_readID(u8 bCmd)
{
	u8 cmd[4],i;
	u32 dwFlashID = 0;
	spi_cs_enable();
	//printf("bCmd = 0x%x!!!\n", bCmd );
	if((bCmd == 0x90) || (bCmd == 0xAB))
	{
		spi_write_byte(bCmd);
		spi_write_byte(0x00);
		spi_write_byte(0x00);
		spi_write_byte(0x00);
	}
	else
	{
		spi_write_byte(bCmd);
	}

	for(i=0; i<4; i++)
	{
		cmd[i] = spi_read_byte();
		dwFlashID = (dwFlashID<<8) + cmd[i];
	}
	spi_cs_disable();
	//printf("CUS spi_readID:[0x%8lx]\n",dwFlashID);
	return dwFlashID;
}

#pragma location="USER_SPI_SEG"
bool spi_flash_init(void)
{
#if (USE_SD_CLK_DET && USE_SDCMD_IIC_SPIDAT)
	SPI_CLK_DIR &= ~BIT(SPI_CLK_PIN);
	SPI_CLK_PU  &= ~BIT(SPI_CLK_PIN);          //关闭上拉
#endif
	u8 i;
	u32 FlashID;
	bool ret_state;

	/*if(!(SD1_CHECK_PORT & BIT(SD1_CHECK_PIN)))        //SD卡插入检测IO会被拉低
	{	// 有 SD卡时无SPI-Flash
		printf("Have SD Card\n");
		return false;
	}*/

#if (SPI_MODE_SEL == SPI_TWO_WIRES_MODE)
	SPI_2Wire_RX();
#endif
	flash_chipnum = 0x01;               //默认为第一块flash
	for(ret_state = false, i=0; i<4; i++)
	{
		FlashID = spi_readID(ReadIDCMD[i]);
		//printf("ID:%xH,%08lxH\n",ReadIDCMD[i],FlashID);   //查看是否检查到flash
		if((FlashID != 0xFFFFFFFF) && (FlashID != 0x0) && ((FlashID & 0x00FFFFFF) != 0x00FFFFFF))
		{
#if (USE_SD_CLK_DET && USE_SDCMD_IIC_SPIDAT)
			SPI_CLK_DIR |= BIT(SPI_CLK_PIN);
			SPI_CLK_PU  |= BIT(SPI_CLK_PIN);          //关闭上拉
#endif
			ret_state = true;
			break;
		}
	}
	if(ret_state == false)
		printf("Error\n");
#if (USE_SD_CLK_DET && USE_SDCMD_IIC_SPIDAT)
	SPI_CLK_DIR |= BIT(SPI_CLK_PIN);
	SPI_CLK_PU  |= BIT(SPI_CLK_PIN);	//关闭上拉
#endif
	return ret_state;
}

//spi写入一sector数据
#pragma location="USER_SPI_SEG"
bool spi_sector_write(u32 sect,void *buf)//指定扇区，写512byte数据
{
	u32 i,temp1,temp2,addr;
	u8 *temp3;
	u8 *buf_addr;

	buf_addr = (u8 *)buf;

	temp1 = sect/8;
	temp2 = sect;
	temp2 = temp2 * 0x200UL;

	addr = temp1 * 0x1000UL;
	if(addr > 8 * 1024 * 1024UL)     //超过SPI Flash的容量
	{
		return false;
	}
	spi_sector_erase(addr);

	for(i = 0; i < 2; i++)
	{
		temp3 = buf_addr + (i * 0x100);
		spi_write(temp3,temp2,256);
		temp2 = temp2 + 0x100;
	}
	return true;
}


//spi读取一sector数据
#pragma location="USER_SPI_SEG"
bool spi_sector_read(u32 sect,void *buf)
{
	u32 addr = sect * 0x200UL;
	if(addr > 8 * 1024 * 1024UL)           //超过SPI Flash的容量
	{
		printf("return\n");
		return false;
	}
	
	spi_read(buf,addr,512);

	return true;
}


//等待播放
#pragma location="USER_SPI_SEG"
void spi_music_wait(void)
{
	while(music_get_status()> STATUS_PLAY_STOPPING)
	{
		WATCHDOG_CLR();
		music_event();
	}
	music_stop();
	u_spi.spi_music_flag = 0;
}

//SPI MP3 语音播报
#pragma location="USER_SPI_SEG"
void mp3_res_play(u8 music_name)
{

	music_stop();
	u_spi.spi_music_flag = 1;
	spi_mp3_index_read(music_name);//读取Flash音乐索引信息
	music_init();
	music_play();
#if LED_EN
	ocx.led_sta = LED_STA_FAST_GLITTER;
#endif
}

//等待SPI MP3 语音播放完成
// music_name from 0 to N
#pragma location="USER_SPI_SEG"
void mp3_res_play_wait(u8 music_name)
{
	u8 music_num;
	music_num = (u8)get_dirstart();
	printf("res cur num:%d\n",music_num);
	mp3_res_play(music_num);
	spi_music_wait();
}


// music_name from 0 to N
#pragma location="USER_SPI_SEG"
void spi_mp3_index_read(u8 music_name)
{
#if SPI_PC_DOWNLOAD
	spi_mp3_index_read_pc(music_name);
#else
	spi_mp3_index_read_program(music_name);
#endif
}


extern bool usb_host_mass_read_do(void *buf, u32 lba, u8 offset);
extern void usb_host_set_fail(void);
bool usb_read1(void *buf, u32 lba)
{
	u8 offset = 0;
	bool res = usb_host_mass_read_do(buf, lba, offset);
	if(!res)
	{
		usb_host_set_fail();
	}
	return !res;
}

