/*****************************************************************************
 * Module    : mem
 * File      : mem_eeprom.h
 * Author    :
 * Email     :
 * Function  : EEPROM记忆任务流程
 *****************************************************************************/
#ifndef __EEPROM_MEMORY_H__
#define __EEPROM_MEMORY_H__

#define eeprom_read             i2c_read
//eeprom写时要注意跨页问题，且写完要延时5ms以上
#define eeprom_write(a, b, c)    {i2c_write(a, b, c); delay_hook(6);}

bool eeprom_check(void);
void eeprom_read_multi(u8 addr, u8 *value, u8 num);
void eeprom_write_multi(u8 addr, u8 *buf, u8 num);
void write_volume_eeprom(void);
void read_volume_eeprom(void);
void write_num_eeprom(void);
void read_num_eeprom(void);
void write_device_eeprom(void);
void read_device_eeprom(void);
void write_music_break_eeprom(void);
void read_music_break_eeprom(void);
void write_total_channel_eeprom(void);
void write_cur_channel_eeprom(void);
void write_fm_info_eeprom(u8 byte_value);
void read_fm_info_eeprom(void);

#endif
