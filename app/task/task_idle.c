/*****************************************************************************
 * Module    : Task
 * File      : task_idle.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ��������
 *****************************************************************************/
#include "include.h"

u8 count_led_off;   //15min led off


#if TASK_IDLE_EN

void task_idle_display(void);

//�����ʼ��
#pragma location="TASK_IDLE_SEG"
static void task_idle_enter(void)
{
	count_led_off = 0;
#if (DISP_TYPE==DISP_LED)
	ocx.led_sta = LED_STA_ON;
#endif

#if KEY_VOICE_EN
	AMUX_AIN_EN();
	mute_disable();
#endif
}

//�����˳�
#pragma location="TASK_IDLE_SEG"
static void task_idle_exit(void)
{
}

//������Ϣ����
#pragma location="TASK_IDLE_SEG"
static void task_idle_deal_msg(u8 msg)
{
	switch(msg)
	{
	/*	
	case KU_VOL_UP:
	case KH_VOL_UP:
	case KU_VOL_DOWN:
	case KH_VOL_DOWN:
		break;
	*/

	case T_KEY_NUM_0:
	case T_KEY_NUM_1:
	case T_KEY_NUM_2:
	case T_KEY_NUM_3:
	case T_KEY_NUM_4:
	case T_KEY_NUM_5:
		printf("Again\n");
		Timer1_flag = false;
		task_ctl.work_sta = TASK_FLASH;
		break;

    //�ػ�
#if TASK_POWEROFF_EN
	case KL_POWER:
	case KLU_POWER:
		task_ctl.work_sta = TASK_POWEROFF;
		sys_ctl.user_power_down_flag = 1;
		break;
#endif

	default:
		deal_msg(msg);
		break;
	}
}

//����������
#pragma location="TASK_IDLE_SEG"
void task_idle(void)
{
	printf("task_idle\n");
	task_idle_enter();
	while(task_ctl.work_sta == TASK_IDLE)
	{
		comm_event(1);
		task_idle_deal_msg(get_msg());
		task_idle_display();
	}
	task_idle_exit();
}

#endif
