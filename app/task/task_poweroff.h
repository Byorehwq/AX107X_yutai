/*****************************************************************************
 * Module    : Task
 * File      : task_poweroff.h
 * Author    :
 * Email     :
 * Function  : 关机任务流程
 *****************************************************************************/
#ifndef TASK_POWEROFF_H
#define TASK_POWEROFF_H

void wko_init(void);
void soft_poweroff_init(void);

#if SOFT_POWEROFF_EN

typedef struct
{
	volatile u8 power_key_sta;   //power按键状态
#if DCIN_DET_EN
	volatile u8 dc_in_sta;       //DCIN状态
#endif
	u8 wait_powerkey_up;         //用于上电时power按键状态的区分
	u8 flag;                     //用于区分是从初始化中关机，还是从其它模式中关机

} type_poweroff_ctl;

extern type_poweroff_ctl poweroff_ctl;

//充电线状态
enum
{
    DC_INIT = 0,       //初始状态
    DC_UNSTABLE,       //未稳定状态
    DC_IN,             //插入状态
    DC_OUT,            //拔出状态
};

//power按键状态
enum
{
    POWERKEY_INIT = 0, //初始状态
    POWERKEY_DOWN,     //按下状态
    POWERKEY_UP,       //放开状态
};

#endif

#endif
