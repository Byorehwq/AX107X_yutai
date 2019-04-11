/*****************************************************************************
 * Module    : Spi
 * File      : spi.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : SPIFLASH的相关定义
 *****************************************************************************/
#ifndef SPI_H
#define SPI_H

//Spi flash command define
#define		F_READ		    0x03
#define         F_DUAL_READ         0x3B
#define		F_PROG		    0x02
#define		F_RDSR		    0x05
#define		F_WRSR		    0x01
#define		F_WREN		    0x06
#define		F_WRDIS		    0x04
#define		F_RDID		    0x90
#define		F_ERASBLK	    0xD8
#define         F_ERASSEC           0x20

#define     READ_10                                 0x28
#define     READ_TOC                                0x43
#define     INQUIRY                                 0x12
#define     TEST_UNIT_READY                         0x00
#define     PREVENT_ALLOW_MEDIUM_REMOVAL            0x1E
#define     READ_CD_ROM_CAPACITY                    0x25

#define SSR_BUSY              (1<<0)
#define SSR_WEL               (1<<1)
#define SSR_BP0               (1<<2)
#define SSR_BP1               (1<<3)
#define SSR_AAI               (1<<6)
#define SSR_BPL               (1<<7)

typedef struct
{
	u16 data_start;             //录音起始地址
	u16 data_end;               //录音结束地址
} type_spi_ctl;

typedef struct
{
	u32 addr;                   //资源首地址
	u32 len;                    //资源大小
	u32 pos;
} type_flash_music;



extern type_flash_music flash_music;
extern type_spi_ctl spi_ctl;

void spi_init(void);
void spi_delay(void);
void spi_write_byte_hard(u8 data);
u8 spi_read_byte_hard(void);
void spi_write_en(void);
void spi_send_addr(u16 addr);
void spi_send_addr_2(u32 addr);
u8 spi_read_ssr(void);
void spi_write_ssr(void);
void spi_wait_busy(void);
void spi_sector_erase(u32 addr);
void spi_bulk_erase(void);
void spi_write(u8 *buf, u32 addr,u16 len);
void spi_dma_kick(void *ram_addr, u16 len);
void spi_read(u8 *buf, u32 addr, u16 len);
u8 spi_read8(u32 flash_addr);
u16 spi_read16(u32 flash_addr);
u32 spi_read32(u32 flash_addr);




#endif

