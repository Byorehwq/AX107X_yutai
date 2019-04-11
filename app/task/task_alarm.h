/*--------------------------------------------------------------------------*/
/***@file task_alarm.h
    @brief 闹钟处理接口函数
    @details
    @author
    @date
    @note
*/
/*----------------------------------------------------------------------------*/
#ifndef _TASK_ALARM_H_
#define _TASK_ALARM_H_


#define ALARM_RING_CYCLE          80     //ms   短响时间
#define ALARM_SILENCE_CYCLE_A     80     //ms   短停时间
#define ALARM_SILENCE_CYCLE_B     200    //ms   长停时间


typedef struct
{
	u8 vol_tmp;
	u8 alarm_cnt;
	u8 volatile delay_10ms_cnt;    //10ms计数
	u8 volatile delay_1s_cnt;      //1s计数
} type_task_alarm;

extern type_task_alarm t_alarm;
void task_alarm_check(void);

#endif

