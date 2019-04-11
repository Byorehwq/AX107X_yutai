/*****************************************************************************
 * Module    : Task
 * File      : task_poweroff.h
 * Author    :
 * Email     :
 * Function  : �ػ���������
 *****************************************************************************/
#ifndef TASK_POWEROFF_H
#define TASK_POWEROFF_H

void wko_init(void);
void soft_poweroff_init(void);

#if SOFT_POWEROFF_EN

typedef struct
{
	volatile u8 power_key_sta;   //power����״̬
#if DCIN_DET_EN
	volatile u8 dc_in_sta;       //DCIN״̬
#endif
	u8 wait_powerkey_up;         //�����ϵ�ʱpower����״̬������
	u8 flag;                     //���������Ǵӳ�ʼ���йػ������Ǵ�����ģʽ�йػ�

} type_poweroff_ctl;

extern type_poweroff_ctl poweroff_ctl;

//�����״̬
enum
{
    DC_INIT = 0,       //��ʼ״̬
    DC_UNSTABLE,       //δ�ȶ�״̬
    DC_IN,             //����״̬
    DC_OUT,            //�γ�״̬
};

//power����״̬
enum
{
    POWERKEY_INIT = 0, //��ʼ״̬
    POWERKEY_DOWN,     //����״̬
    POWERKEY_UP,       //�ſ�״̬
};

#endif

#endif
