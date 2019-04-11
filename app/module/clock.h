/*****************************************************************************
 * Module    : Module
 * File      : clock.c
 * Author    : jingfa
 * Email     :
 * Function  : ϵͳʱ�ӵ��������
 *****************************************************************************/
#ifndef __CLOCK_H__
#define __CLOCK_H__


//ϵͳʱ������
enum
{
    SYS_24M,        //PLL=48M, SYS=24M
    SYS_48M,        //PLL=48M, SYS=48M
    SYS_RC_1M,      //RC=1M  , SYS=1M, ��PLL, ��RC12M, �͹���ģʽ
    SYS_RC_2M,      //RC=2M  , SYS=2M, ��PLL, ��RC12M, �͹���ģʽ
#if (FM_CLOCK == USE_SD_IO_32KHZ_CLK)
    SYS_12M,
#endif
    SYS_INVALID,
};

void sysclk_set(void);                  //�ϵ�ϵͳʱ�ӳ�ʼ����ʹ��RC10M��Ϊʱ��Դ
bool auto_check_x32k(void);             //�Զ�����Ƿ��о�����Ҫ��ʹ��RC10M��Ϊʱ��Դ
void sys_pll_xosc(void);                //ϵͳʱ�Ӵ�RC10M�л�������
u8 get_sys_clk(void);                   //��ȡ��ǰϵͳʱ��ģʽ
void set_sys_clk(u8 mode);              //���õ�ǰϵͳʱ��ģʽ

#endif

