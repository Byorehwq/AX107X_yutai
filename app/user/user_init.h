/*****************************************************************************
 * Module    : user
 * File      : user_init.h
 * Author    :
 * Email     :
 * Function  : �û��ļ�
 *****************************************************************************/
#ifndef USER_INIT_H
#define USER_INIT_H

void system_init(void);
void libvar_init(void);
void delay_m(u8 cnt);
bool rtcram_poweron_check(void);
extern u8 rtc_flag;

#endif


