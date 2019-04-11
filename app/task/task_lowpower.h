/*****************************************************************************
 * Module    : Task
 * File      : task_lowpower.h
 * Author    :
 * Email     :
 * Function  : 低功耗模式
 *****************************************************************************/
#ifndef TASK_LOWPOWER_H
#define TASK_LOWPOWER_H


/*****************************************************************************/
//用户不要更改这个范围内的定义

//wakeup pin:
#define WKUP_PIN_0        0 //P00/3in1(P02/P03/P10/P27)
#define WKUP_PIN_1        1 //P01/DP1/DM1
#define WKUP_PIN_2        2 //DP2/DM2/P02/P10/P12/P14/P11/P27
#define WKUP_PIN_3        3 //P03/SDCLK(P06/P13/P16/P42)

//wakeup pin0(LEDCON[1]):
#define WKUP_PIN0_P00     0 //wkup pin0 select p00
#define WKUP_PIN0_3IN1    1 //wkup pin0 select 3in1

//wakeup pin1(AIPCON6[5:3]):
#define WKUP_PIN1_P01     0 //wkup pin1 select P01
#define WKUP_PIN1_DM1     1 //wkup pin1 select DM1
#define WKUP_PIN1_DP1     2 //wkup pin1 select DP1

//wakeup pin2(AIPCON6[5:3]):
#define WKUP_PIN2_P02     0 //wkup pin2 select P02
#define WKUP_PIN2_P15     1 //wkup pin2 select P15
#define WKUP_PIN2_DM2     1 //wkup pin2 select DM2
#define WKUP_PIN2_DP2     2 //wkup pin2 select DP2
#define WKUP_PIN2_P10     3 //wkup pin2 select P10
#define WKUP_PIN2_P27     4 //wkup pin2 select P27
#define WKUP_PIN2_P12     5 //wkup pin2 select P12
#define WKUP_PIN2_P14     6 //wkup pin2 select P14
#define WKUP_PIN2_P11     7 //wkup pin2 select P11

//wakeup pin3(AIPCON6[2]):
#define WKUP_PIN3_P03     0 //wkup pin3 select P03
#define WKUP_PIN3_SDCLK   1 //wkup pin3 select SDCLK,chosen by SD_IOMAP(P06/P13/P16/P42)

//port wakeup edge:
#define WKUP_EDGE_RISING  0 //rising edge
#define WKUP_EDGE_FALLING 1 //falling edge

//用户不要更改这个范围内的定义
/*****************************************************************************/

#endif


