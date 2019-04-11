/*****************************************************************************
 * Module    : API
 * File      : api_spi.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : SPIFlash 的操作接口
 *****************************************************************************/
#ifndef API_SPI_H
#define API_SPI_H

__near_func void spi_read(void *buf, u32 addr, u16 len);      //SPIFlash 读接口
__near_func u8 spi_read8(u32 flashaddress);                   //读1Byte
__near_func u16 spi_read16(u32 flashaddress);                 //按小端读2Byte
__near_func u32 spi_read32(u32 flashaddress);                 //按小端读4Byte
void spi_write(void *buf, u16 addr);
void spi_erase(u16 addr);

void spi_param_factory(void);                                   //恢复出厂设置

u16 spi_get_datalen(void);                                      //获取DATA区大小
__near_func bool spi_read_data(void *buf, u16 addr);            //DATA区读接口，addr = 0 ~ datalen
bool spi_write_data(void *buf, u16 addr);                       //DATA区写接口
bool spi_erase_data(u16 addr);                                  //DATA区擦除接口
#endif
