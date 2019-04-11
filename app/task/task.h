/*****************************************************************************
* Module    : Task
* File      : task.h
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : 任务控制器的相关定义
*****************************************************************************/
#ifndef TASK_H
#define TASK_H

#include "task_alarm.h"
#include "task_rtc.h"
#include "task_music.h"
#include "task_speaker.h"
#include "task_aux.h"
#include "task_fm.h"
#include "task_usbdev.h"
#include "task_poweroff.h"
#include "task_fmam_detect.h"

//任务的编号
enum
{
    TASK_MUSIC      = 0,
    TASK_AUX,
    TASK_FM,
    TASK_RTC,
    TASK_SPEAKER,
    TASK_USBDEV,
    TASK_FLASH,
    TASK_ALARM,
    TASK_IDLE,
    TASK_POWEROFF,
    TASK_LOWPOWER,
    TASK_FMAM_DETECT, 
#if TASK_TIPS_EN
	TASK_TIPS,
#endif
    TASK_EXIT,
};


typedef struct
{
	u8 work_sta;                //任务状态
	u8 last_work;               //上一个有效有任务状态
	u8 pre_sta;                 //用于mic检测、linein检测,退出后回到前一个任务状态
} type_task_ctl;

extern IAR_DATA_A type_task_ctl task_ctl;
u8 device_change(void);

extern void run_task(void);
extern void task_music(void);
extern void task_aux(void);
extern void task_rtc(void);
extern void task_fm(void);
extern void task_poweroff(void);
extern void task_speaker(void);
extern void task_idle(void);
extern void task_exit(void);
extern void task_usbdev(void);
extern void task_alarm(void);
extern void task_lowpower(void);
extern void task_flash_music(void);

void task_battery_check(void);
void deal_msg(u8 msg);
void comm_event(u8 enable);
void deal_vol_msg(void);
extern u8 device_chg;
extern bool f_Clear_WDT;       // 1: set by timer interrupt for clear WDT, 0: clear by main program
extern bool Timer1_flag;   //设置定时

extern u8 Timer_flag;
extern u16 main_loop_count;
#define Cnt_Init    0xffff
#define device_music	0
#define device_flash	1
extern bool mute_status_change;
#endif
