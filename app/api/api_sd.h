/*****************************************************************************
 * Module    : API
 * File      : api_sd.h
 * Author    : jingfa
 * Email     :
 * Function  : SD卡相关接口
 *****************************************************************************/
#ifndef API_SD_H
#define API_SD_H

bool sd_read(void *buf, u32 lba);       //SD读一扇区
bool sd_write(void *buf, u32 lba);      //SD写一扇区
void sd_set_rate(u8 rate);              //设置SD卡的BAUD
bool sd_stop(bool type);                //SD停止
void set_sd_idle_state(void);

#endif
