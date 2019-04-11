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

//С��ʱ
#pragma location="SPI_SEG"
void spi_delay(void)
{
	u8 i;
	for(i=0; i!=10; i++)
	{
		asm("nop");
	}
}

//SPI�����Ϸ���һByte
#pragma location="SPI_SEG"
void spi_write_byte_hard(u8 data)
{
	SPI0CON &= ~BIT(5);     //TX
	SPI0BUF = data;
	while(!(SPI0CON & 0x80));
}

//SPI��ȡһByte
#pragma location="SPI_SEG"
u8 spi_read_byte_hard(void)
{
	SPI0CON |= BIT(5);      //RX
	SPI0BUF = 0xff;
	while(!(SPI0CON & 0x80));
	return SPI0BUF;
}

//��SPIFlash��дʹ��
#pragma location="SPI_SEG"
void spi_write_en(void)
{
	spi_cs_enable();
	spi_write_byte(F_WREN);
	spi_cs_disable();
	spi_delay();
}

//��ַ�����뵽256byte
#pragma location="SPI_SEG"
void spi_send_addr(u16 addr)
{
	spi_write_byte(addr >> 8);
	spi_write_byte(addr);
	spi_write_byte(0);
}

//����SPIFlash��ַ,����512�ֽ�
#pragma location="SPI_SEG"
void spi_send_addr_2(u32 addr)
{
	spi_write_byte(addr >> 16);
	spi_write_byte(addr >> 8);
	spi_write_byte(addr);
}

//��ȡ״̬�Ĵ���
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

//дȡ״̬�Ĵ���
#pragma location="SPI_SEG"
void spi_write_ssr(void)
{
	spi_cs_enable();
	spi_write_byte(F_WRSR);
	spi_write_byte(0);
	spi_cs_disable();
}


//SPI�ȴ�Busy����
#pragma location="SPI_SEG"
void spi_wait_busy(void)
{
	u8 SPI_wait_timeout;
	
	SPI_wait_timeout = 15;
	do
	{
		WATCHDOG_CLR();				// ��ֹSPI Wait���¿��Ź���λ
		spi_delay();
		if(SPI_wait_timeout)
			SPI_wait_timeout--;
		else
		{	// Merlin ���� 2015/06/30
			u_spi.spi_online = 0;	// ��Ϊ��ʱ, ����ͣ��SPI Flash�Ĳ���
			break;
		}
	}
	while(spi_read_ssr() & 0x01);
}

//SPI����,��ַ���뵽256Byte
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

//SPI DMA���ӿ�
#pragma location="SPI_SEG"
void spi_dma_kick(void *ram_addr, u16 len)
{
	SPI0CON |= BIT(5);	            //RX
	SPIDMAPTRH = (u8)((u16)ram_addr >> 8);
	SPIDMAPTRL = (u8)(u16)ram_addr;
	SPIDMACNT = (len >> 1) - 1;
	while(!(SPI0CON & 0x80));
}

//SPI���ӿ�
#pragma location="SPI_SEG"
void spi_read_hard(u8 *buf, u32 addr, u16 len)
{
	char *cbuf = (void *)buf;
	u16 kick_len;

	spi_wait_busy();
	spi_cs_enable();
	spi_write_byte(F_READ);
	spi_send_addr_2(addr);

	//�ȶ���RAM��ַ
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
	//��ȡʣ��Byte
	while(len)
	{
		*cbuf++ = spi_read_byte();
		len--;
	}
	spi_cs_disable();
}

//��8bit�ӿ�
#pragma location="SPI_SEG"
u8 spi_read8(u32 flash_addr)
{
	u8 temp;
	spi_read(&temp, flash_addr, 1);
	return temp;
}

//��16bit�ӿ�
#pragma location="SPI_SEG"
u16 spi_read16(u32 flash_addr)
{
	u16 temp;
	spi_read((u8 *)&temp, flash_addr, 2);
	return temp;
}

//��32bit�ӿ�
#pragma location="SPI_SEG"
u32 spi_read32(u32 flash_addr)
{
	u32 temp;
	spi_read((u8 *)&temp, flash_addr, 4);
	return temp;
}

