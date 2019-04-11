/*****************************************************************************
 * Module    : Spi
 * File      : spi.c
 * Author    :
 * Email     :
 * Function  :
 *****************************************************************************/
#include "include.h"
#include "spi.h"

type_spi_ctl spi_ctl;

extern void spi_cs_enable(void);
extern void spi_cs_disable(void);
extern u8 spi_read_byte(void);
extern void spi_write_byte(u8 data);

#pragma location="SPI_SEG"
void spi_init(void)
{
#if !SOFT_SPI
	PCON1 &= ~BIT(2);       //SPI clock en
	SPI0CON |= BIT(1);      //master,tx,clock signal stay at 1 when idle

	SPIBAUD = 0;            //12M
	SPI0CON |= BIT(0);      //SPI EN
	SPI0BUF = 0xFF;         //clear pnd
#endif

	//spi_ctl.data_start = SPI_REC_START_ADDR;
	//spi_ctl.data_end = SPI_REC_END_ADDR;
}

//小延时
#pragma location="SPI_SEG"
void spi_delay(void)
{
	u8 i;
	for(i=0; i!=10; i++)
	{
		asm("nop");
	}
}

//SPI总线上发送一Byte
#pragma location="SPI_SEG"
void spi_write_byte_hard(u8 data)
{
	SPI0CON &= ~BIT(5);     //TX
	SPI0BUF = data;
	while(!(SPI0CON & 0x80));
}

//SPI读取一Byte
#pragma location="SPI_SEG"
u8 spi_read_byte_hard(void)
{
	SPI0CON |= BIT(5);      //RX
	SPI0BUF = 0xff;
	while(!(SPI0CON & 0x80));
	return SPI0BUF;
}

//打开SPIFlash的写使能
#pragma location="SPI_SEG"
void spi_write_en(void)
{
	spi_cs_enable();
	spi_write_byte(F_WREN);
	spi_cs_disable();
	spi_delay();
}

//地址，对齐到256byte
#pragma location="SPI_SEG"
void spi_send_addr(u16 addr)
{
	spi_write_byte(addr >> 8);
	spi_write_byte(addr);
	spi_write_byte(0);
}

//发送SPIFlash地址,对齐512字节
#pragma location="SPI_SEG"
void spi_send_addr_2(u32 addr)
{
	spi_write_byte(addr >> 16);
	spi_write_byte(addr >> 8);
	spi_write_byte(addr);
}

//读取状态寄存器
#pragma location="SPI_SEG"
u8 spi_read_ssr(void)
{
	u8 cmd;
	spi_cs_enable();
	spi_write_byte(F_RDSR);

	cmd = spi_read_byte();

	spi_cs_disable();
	return cmd;
}

//写取状态寄存器
#pragma location="SPI_SEG"
void spi_write_ssr(void)
{
	spi_cs_enable();
	spi_write_byte(F_WRSR);
	spi_write_byte(0);
	spi_cs_disable();
}


//SPI等待Busy操作
#pragma location="SPI_SEG"
void spi_wait_busy(void)
{
	u8 SPI_wait_timeout;
	
	SPI_wait_timeout = 15;
	do
	{
		WATCHDOG_CLR();				// 防止SPI Wait导致看门狗复位
		spi_delay();
		if(SPI_wait_timeout)
			SPI_wait_timeout--;
		else
		{	// Merlin 增加 2015/06/30
			u_spi.spi_online = 0;	// 因为超时, 所以停用SPI Flash的操作
			break;
		}
	}
	while(spi_read_ssr() & 0x01);
}

//SPI擦除,地址对齐到256Byte
#pragma location="SPI_SEG"
void spi_sector_erase(u32 addr)
{
	spi_wait_busy();
	spi_write_en();
	spi_cs_enable();
	spi_write_byte(F_ERASSEC);
	spi_send_addr_2(addr);
	spi_cs_disable();
}

#pragma location="SPI_SEG"
void spi_bulk_erase(void)
{
	spi_wait_busy();
	while(!(spi_read_ssr() & SSR_WEL))
	{
		spi_write_en();
	}
	spi_cs_enable();
	spi_write_byte(0xC7);

	spi_cs_disable();
	spi_wait_busy();
}

#pragma location="SPI_SEG"
void spi_write_hard(u8 *buf, u32 addr,u16 len)
{
	u16 ram_addr = (u16)buf;
	spi_wait_busy();
	spi_write_en();
	spi_cs_enable();
	spi_write_byte(F_PROG);
	spi_send_addr_2(addr);

	SPI0CON &= ~BIT(5);       //Set SPI TX
	SPIDMAPTRH = ram_addr >> 8;
	SPIDMAPTRL = ram_addr;
	SPIDMACNT = len/2 - 1;	        //256Byte
	while(!(SPI0CON & 0x80));

	spi_cs_disable();
}

//SPI DMA读接口
#pragma location="SPI_SEG"
void spi_dma_kick(void *ram_addr, u16 len)
{
	SPI0CON |= BIT(5);	            //RX
	SPIDMAPTRH = (u8)((u16)ram_addr >> 8);
	SPIDMAPTRL = (u8)(u16)ram_addr;
	SPIDMACNT = (len >> 1) - 1;
	while(!(SPI0CON & 0x80));
}

//SPI读接口
#pragma location="SPI_SEG"
void spi_read_hard(u8 *buf, u32 addr, u16 len)
{
	char *cbuf = (void *)buf;
	u16 kick_len;

	spi_wait_busy();
	spi_cs_enable();
	spi_write_byte(F_READ);
	spi_send_addr_2(addr);

	//先对齐RAM地址
	while(((u16)cbuf & 0x3) && len)
	{
		*cbuf++ = spi_read_byte();
		len--;
	}
	//DMA
	while(len >= 4)
	{
		if(len >= 512)
		{
			kick_len = 512;
		}
		else
		{
			kick_len = (len >> 2) << 2;
		}
		spi_dma_kick(cbuf, kick_len);
		cbuf += kick_len;
		len -= kick_len;
	}
	//读取剩余Byte
	while(len)
	{
		*cbuf++ = spi_read_byte();
		len--;
	}
	spi_cs_disable();
}

//读8bit接口
#pragma location="SPI_SEG"
u8 spi_read8(u32 flash_addr)
{
	u8 temp;
	spi_read(&temp, flash_addr, 1);
	return temp;
}

//读16bit接口
#pragma location="SPI_SEG"
u16 spi_read16(u32 flash_addr)
{
	u16 temp;
	spi_read((u8 *)&temp, flash_addr, 2);
	return temp;
}

//读32bit接口
#pragma location="SPI_SEG"
u32 spi_read32(u32 flash_addr)
{
	u32 temp;
	spi_read((u8 *)&temp, flash_addr, 4);
	return temp;
}

