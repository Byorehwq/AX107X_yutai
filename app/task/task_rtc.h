/*****************************************************************************
 * Module    : Task
 * File      : task_rtc.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 音乐播放任务流程
 *****************************************************************************/
#ifndef TASK_RTC_H
#define TASK_RTC_H

#define SECONDS_OF_A_DAY    86400//60*60*24 = 86400 一天的秒数

typedef struct
{
	u8 select;  //0=time, 1=alarm
	u8 setting;
	u8 index;
} type_task_rtc;

#endif
