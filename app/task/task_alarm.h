/*--------------------------------------------------------------------------*/
/***@file task_alarm.h
    @brief ���Ӵ���ӿں���
    @details
    @author
    @date
    @note
*/
/*----------------------------------------------------------------------------*/
#ifndef _TASK_ALARM_H_
#define _TASK_ALARM_H_


#define ALARM_RING_CYCLE          80     //ms   ����ʱ��
#define ALARM_SILENCE_CYCLE_A     80     //ms   ��ͣʱ��
#define ALARM_SILENCE_CYCLE_B     200    //ms   ��ͣʱ��


typedef struct
{
	u8 vol_tmp;
	u8 alarm_cnt;
	u8 volatile delay_10ms_cnt;    //10ms����
	u8 volatile delay_1s_cnt;      //1s����
} type_task_alarm;

extern type_task_alarm t_alarm;
void task_alarm_check(void);

#endif

