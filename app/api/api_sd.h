/*****************************************************************************
 * Module    : API
 * File      : api_sd.h
 * Author    : jingfa
 * Email     :
 * Function  : SD����ؽӿ�
 *****************************************************************************/
#ifndef API_SD_H
#define API_SD_H

bool sd_read(void *buf, u32 lba);       //SD��һ����
bool sd_write(void *buf, u32 lba);      //SDдһ����
void sd_set_rate(u8 rate);              //����SD����BAUD
bool sd_stop(bool type);                //SDֹͣ
void set_sd_idle_state(void);

#endif
