/*****************************************************************************
 * Module    : Task
 * File      : task_rtc.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ���ֲ�����������
 *****************************************************************************/
#ifndef TASK_RTC_H
#define TASK_RTC_H

#define SECONDS_OF_A_DAY    86400//60*60*24 = 86400 һ�������

typedef struct
{
	u8 select;  //0=time, 1=alarm
	u8 setting;
	u8 index;
} type_task_rtc;

#endif
