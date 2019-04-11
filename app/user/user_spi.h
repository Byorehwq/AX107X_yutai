/*****************************************************************************
 * Module    : User
 * File      : user_spi.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : SPI FLASH的相关定义
 *****************************************************************************/
#ifndef _USER_SPI_H_
#define _USER_SPI_H_

/**************************************************
SPI_PORT  AIPCON8[4]   SPI0CON[3]   DO   CLK    DI
   0        0              0       P16   P15   P17
   1        0              1       P13   P12   P14
   2        1              /       P11   P10    /
***************************************************/

//SPI Flash模式选择
#define SPI_TWO_WIRES_MODE              0           //SPI 2线模式
#define SPI_THREE_WIRES_MODE            1           //SPI 3线模式


#define SPI_MODE_SEL                    SPI_TWO_WIRES_MODE   //选择两线/三线SPI
#define SOFT_SPI                        0                    //0为选择硬件SPI，1为选择软件SPI
#define SPI_PC_DOWNLOAD                 0                    //flash烧写方式的选择，1为使用PC下载，0为选择烧录器烧写

#if !SOFT_SPI
#define SPI_PORT                        0                    //选择一组SPIIO，0~2，选择硬件SPI时选择
#endif

#if SOFT_SPI
//SPI两线模式
#define SPI_CLK_PORT					P1
#define SPI_CLK_DIR						P1DIR
#define SPI_CLK_PU						PUP1
#define SPI_CLK_PIN						2   //2

#define SPI_CS_PORT						P1
#define SPI_CS_DIR						P1DIR
#define SPI_CS_PIN						4

#define SPI_DO_PORT						P1
#define SPI_DO_DIR						P1DIR
#define SPI_DO_PIN						3

#define SPI_2Wire_RX()					SPI_DO_DIR |=  (1<<SPI_DO_PIN)
#define SPI_2Wire_TX()					SPI_DO_DIR &= ~(1<<SPI_DO_PIN)

#if (SPI_MODE_SEL == SPI_THREE_WIRES_MODE)
#define SPI_DI_PORT                    P1
#define SPI_DI_DIR                     P1DIR
#define SPI_DI_PIN			           7
#endif

#else
//SPI CS控制IO
#define SPI_CS_PORT                     P1
#define SPI_CS_DIR                      P1DIR
#define SPI_CS_PIN                      7

//SPI两线模式
#if (SPI_PORT == 0)
#define SPI_PORT_SEL()              {AIPCON8 &= ~BIT(4);SPI0CON = 0x10;}

#define SPI_DO_PORT                 P1
#define SPI_DO_DIR                  P1DIR
#define SPI_DO_PU                   PUP1
#define SPI_DO_PIN			        6

#define SPI_CLK_PORT                P1
#define SPI_CLK_DIR                 P1DIR
#define SPI_CLK_PU                  PUP1
#define SPI_CLK_PIN			        5

#elif (SPI_PORT == 1)
#define SPI_PORT_SEL()              {AIPCON8 &= ~BIT(4);SPI0CON = 0x18;}

#define SPI_DO_PORT                 P1
#define SPI_DO_DIR                  P1DIR
#define SPI_DO_PU                     PUP1
#define SPI_DO_PIN			        3

#define SPI_CLK_PORT                P1
#define SPI_CLK_DIR                 P1DIR
#define SPI_CLK_PU                  PUP1
#define SPI_CLK_PIN			        2

#elif (SPI_PORT == 2)
#define SPI_PORT_SEL()              {AIPCON8 |= BIT(4);SPI0CON = 0x10;}

#define SPI_DO_PORT                 P1
#define SPI_DO_DIR                  P1DIR
#define SPI_DO_PIN			        1

#define SPI_CLK_PORT                P1
#define SPI_CLK_DIR                 P1DIR
#define SPI_CLK_PIN			        0

#else
#error "请选择一组SPIIO";
#endif  //SPI_PORT

#define SPI_2Wire_RX()                  {SPI_DO_DIR |=  (1<<SPI_DO_PIN);SPI0CON |= BIT(5); }
#define SPI_2Wire_TX()                  {SPI_DO_DIR &= ~(1<<SPI_DO_PIN);SPI0CON &= ~BIT(5);}

//SPI三线模式
#if (SPI_MODE_SEL == SPI_THREE_WIRES_MODE)
#if (SPI_PORT == 0)
#define SPI_PORT_SEL()              SPI0CON &= ~BIT(4)

#define SPI_DI_PORT                 P1
#define SPI_DI_DIR                  P1DIR
#define SPI_DI_PIN			        7

#elif (SPI_PORT == 1)
#define SPI_PORT_SEL()              SPI0CON &= ~BIT(4)

#define SPI_DI_PORT                 P1
#define SPI_DI_DIR                  P1DIR
#define SPI_DI_PIN			        4

#elif (SPI_PORT == 2)
#error "此组不支持SPI三线模式";
#endif
#endif

#endif

typedef struct
{
	u8  spi_music_flag;                 //SPI播放标志
	u8  spi_online;	                    //SPI FLASH是否在线，TRUE是表示在线
	u8  spi_play_mode;                  //SPI播放模式
} type_user_spi;


enum
{
    ALL_DIR,                            //全部文件夹循环
    SINGLE_DIR,                         //单文件夹循环
};

extern IAR_XDATA_A type_user_spi u_spi;

void spi_init(void);
void spi_delay(void);
void spi_wait_busy(void);
void spi_write_ssr(void);
void spi_bulk_erase(void);
void spi_write_en(void);
void spi_sector_erase(u32 addr);
void spi_send_addr(u16 addr);
void spi_send_addr_2(u32 addr);
void spi_write_byte_hard(u8 data);
void spi_write_hard(u8 *buf, u32 addr,u16 len);
void spi_read_hard(u8 *buf, u32 addr, u16 len);
void spi_dma_kick(void *ram_addr, u16 len);

u8 spi_read_ssr(void);
u8 spi_read_byte_hard(void);
u8 spi_read8(u32 flash_addr);
u16 spi_read16(u32 flash_addr);
u32 spi_read32(u32 flash_addr);


void spi_mp3_index_read_program(u8 music_name);
void spi_mp3_index_read_pc(u8 music_name);
void spi_wav_index_read(void);
bool spi_mp3flash_read(void *buf);
bool spi_mp3flash_seek(u8 mode, u32 offset);


void spi_port_init(void);
void spi_cs_enable(void);
void spi_cs_disable(void);
void spi_write_byte_soft(u8 data);
void spi_write_byte(u8 data);
void spi_write_soft(u8 *buf, u32 addr,u16 len);
void spi_read_soft(u8 *buf, u32 addr, u16 len);
void spi_write(u8 *buf, u32 addr,u16 len);
void spi_read(u8 *buf, u32 addr, u16 len);
void spi_music_wait(void);
void mp3_res_play(u8 music_name);
void mp3_res_play_wait(u8 music_name);
void spi_mp3_index_read(u8 music_name);

bool spi_flash_init(void);
bool spi_sector_write(u32 sect,void *buf);
bool spi_sector_read(u32 sect,void *buf);
bool usb_read1(void *buf, u32 lba);

u8 spi_read_byte_soft(void);
u8 spi_read_byte(void);
u32 spi_readID(u8 bCmd);

void spi_sd_mux_enter(void);
void spi_sd_mux_exit(void);

#define sd_sector_read     sd_read
#define sd_sector_write    sd_write

#define usb_sector_read    usb_read1
#define usb_sector_write   usb_write

#endif

