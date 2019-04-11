/*****************************************************************************
 * Module    : Task
 * File      : task_lowpower.c
 * Author    :
 * Email     :
 * Function  : 低功耗模式
 *****************************************************************************/
#include "include.h"
#include "task_lowpower.h"
#include "kedir.h"

#if TASK_LOWPOWER_EN

//用户选择：
/***********************
    1.选择组0/1/2/3；
    2.选择IO;
    3.选择上升/下降沿；
***********************/
#define USER_WKUP_PIN        WKUP_PIN_2        //选择组0/1/2/3；
#define USER_WKUP_PORT       WKUP_PIN2_P10     //选择IO
#define USER_WKUP_EDGE       WKUP_EDGE_FALLING //选择上升/下降沿

#pragma location="WKUP_SEG"
void pwkedge_config(void)
{
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)       //上升沿
	PWKEDGE = 0x00;
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)    //下降沿
	PWKEDGE = 0x00 | BIT(USER_WKUP_PIN);
#endif
}

#pragma location="WKUP_SEG"
void pin0_p00_config(void)
{
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	PDN0 |= BIT(0);
	PUP0 &= ~BIT(0);
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	PDN0 &= ~BIT(0);
	PUP0 |= BIT(0);
#endif
}

#pragma location="WKUP_SEG"
void pin0_3in1_config(void)
{
#if (KEDIR_PORT == KEDIR_P10)
	P1DIR |= BIT(0);            //P10
	PIE0 &= ~(BIT(5));          //P10
#elif (KEDIR_PORT == KEDIR_P03)
	P0DIR |= BIT(3);            //P03
	PIE0 &= ~(BIT(3));          //P03
#elif (KEDIR_PORT == KEDIR_P02)
	P0DIR |= BIT(2);            //P02
	PIE0 &= ~(BIT(2));          //P02
#elif (KEDIR_PORT == KEDIR_P27)
	P2DIR |= BIT(7);            //P27
	PIE0 &= ~(BIT(0));          //P27
#endif

	LEDCON &= ~BIT(2);          //2.87V reference voltage
	KEDIR_LED_OFF();

#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	KEDIR_ENABLE_TOP();         //33K pull down resistance enable
#ifdef AX207X_TAG
	if(DCN == 0x00)
	{
		KEDIR_DISABLE_BIG_1();  //22k pull up resistance enable
		KEDIR_DISABLE_TINY_1(); //2.2k pull up resistance disable
	}
	else
#endif
	{
		KEDIR_DISABLE_BIG();
		KEDIR_DISABLE_TINY();
	}
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	KEDIR_DISABLE_TOP();           //33K pull down resistance disable
#ifdef AX207X_TAG
	if(DCN == 0x00)
	{
		KEDIR_ENABLE_BIG_1();      //22k pull up resistance enable
		KEDIR_DISABLE_TINY_1();    //2.2k pull up resistance disable
	}
	else
#endif
	{
		KEDIR_ENABLE_BIG();
		KEDIR_DISABLE_TINY();
	}
#endif
}

#pragma location="WKUP_SEG"
void pin0_config(void)
{
	LEDCON |= (USER_WKUP_PORT << 1);

#if (USER_WKUP_PORT == WKUP_PIN0_P00)
	pin0_p00_config();
#elif (USER_WKUP_PORT == WKUP_PIN0_3IN1)
	pin0_3in1_config();
#endif
}

#pragma location="WKUP_SEG"
void pin1_p01_config(void)
{
	PIE0 |= BIT(1);
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	PDN0 |= BIT(1);
	PUP0 &= ~BIT(1);
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	PDN0 &= ~BIT(1);
	PUP0 |= BIT(1);
#endif
}

#pragma location="WKUP_SEG"
void pin1_dp1dm1_config(void)
{
	AIPCON7 |= BIT(2);
	AIPCON7 &= ~BIT(1);
	USBCON0 = 0xf0;
	USBCON1 = 0x1f;
	USBCON2 = 0x04;
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	USBDPDM = 0x0A; //DP 15K下拉，DM 15K下拉
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	USBDPDM = 0x05; //DP 1.5K上拉，DM 120K上拉
#endif
}

#pragma location="WKUP_SEG"
void pin1_config(void)
{
	AIPCON6 = (AIPCON6 & (~0x38)) | (USER_WKUP_PORT << 3); //wakeup pin2(AIPCON6[5:3])

#if (USER_WKUP_PORT == WKUP_PIN1_P01)
	pin1_p01_config();
#elif ((USER_WKUP_PORT == WKUP_PIN1_DP1) || (USER_WKUP_PORT == WKUP_PIN1_DM1))
	pin1_dp1dm1_config();
#endif
}

#pragma location="WKUP_SEG"
void pin2_p02_config(void)
{
	PIE0 |= BIT(2);
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	PDN0 |= BIT(2);
	PUP0 &= ~BIT(2);
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	PDN0 &= ~BIT(2);
	PUP0 |= BIT(2);
#endif
}

#pragma location="WKUP_SEG"
void pin2_p15_config(void)
{
	PIE0 |= BIT(6);
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	PDN1 |= BIT(5);
	PUP1 &= ~BIT(5);
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	PDN1 &= ~BIT(5);
	PUP1 |= BIT(5);
#endif
}

#pragma location="WKUP_SEG"
void pin2_p10_config(void)
{
	PIE0 |= BIT(5);
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	PDN1 |= BIT(0);
	PUP1 &= ~BIT(0);
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	PDN1 &= ~BIT(0);
	PUP1 |= BIT(0);
#endif
}

#pragma location="WKUP_SEG"
void pin2_p27_config(void)
{
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	PDN2 |= BIT(7);
	PUP2 &= ~BIT(7);
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	PDN2 &= ~BIT(7);
	PUP2 |= BIT(7);
#endif
}

#pragma location="WKUP_SEG"
void pin2_p12_config(void)
{
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	PDN1 |= BIT(2);
	PUP1 &= ~BIT(2);
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	PDN1 &= ~BIT(2);
	PUP1 |= BIT(2);
#endif
}

#pragma location="WKUP_SEG"
void pin2_p14_config(void)
{
	PIE0 |= BIT(4);
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	PDN1 |= BIT(4);
	PUP1 &= ~BIT(4);
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	PDN1 &= ~BIT(4);
	PUP1 |= BIT(4);
#endif
}

#pragma location="WKUP_SEG"
void pin2_p11_config(void)
{
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	PDN1 |= BIT(1);
	PUP1 &= ~BIT(1);
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	PDN1 &= ~BIT(1);
	PUP1 |= BIT(1);
#endif
}

#pragma location="WKUP_SEG"
void pin2_dp2dm2_config(void)
{
	AIPCON7 &= ~(BIT(2) | BIT(0));
	USBCON0 = 0xf0;
	USBCON1 = 0x0f;
	USBCON2 = 0x04;
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	USBDPDM = 0x0A; //DP 15K下拉，DM 15K下拉
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	USBDPDM = 0x05; //DP 1.5K上拉，DM 120K上拉
#endif
}

#pragma location="WKUP_SEG"
void pin2_config(void)
{
	AIPCON6 = (AIPCON6 & (~0x38)) | (USER_WKUP_PORT << 3); //wakeup pin2(AIPCON6[5:3])

#if (USER_WKUP_PORT == WKUP_PIN2_P02)
	pin2_p02_config();
#elif (USER_WKUP_PORT == WKUP_PIN2_P15)
	pin2_p15_config();
#elif ((USER_WKUP_PORT == WKUP_PIN2_DM2) || (USER_WKUP_PORT == WKUP_PIN2_DP2))
	pin2_dp2dm2_config();
#elif (USER_WKUP_PORT == WKUP_PIN2_P10)
	pin2_p10_config();
#elif (USER_WKUP_PORT == WKUP_PIN2_P27)
	pin2_p27_config();
#elif (USER_WKUP_PORT == WKUP_PIN2_P12)
	pin2_p12_config();
#elif (USER_WKUP_PORT == WKUP_PIN2_P14)
	pin2_p14_config();
#elif (USER_WKUP_PORT == WKUP_PIN2_P11)
	pin2_p11_config();
#endif
}


#pragma location="WKUP_SEG"
void pin3_p03_config(void)
{
	PIE0 |= BIT(3);
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
	PDN0 |= BIT(3);
	PUP0 &= ~BIT(3);
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
	PDN0 &= ~BIT(3);
	PUP0 |= BIT(3);
#endif
}

#pragma location="WKUP_SEG"
void pin3_sdclk_config(void)
{
#if (USER_WKUP_EDGE == WKUP_EDGE_RISING)
#if (SD_PORT == 0)
	PDN1 |= BIT(6);
	PUP1 &= ~BIT(6);
#elif (SD_PORT == 1)
	PDN1 |= BIT(3);
	PUP1 &= ~BIT(3);
#elif (SD_PORT == 2)
	PDN4 |= BIT(2);
	PUP4 &= ~BIT(2);
#elif (SD_PORT == 3)
	PDN0 |= BIT(6);
	PUP0 &= ~BIT(6);
#endif
#elif (USER_WKUP_EDGE == WKUP_EDGE_FALLING)
#if (SD_PORT == 0)
	PDN1 &= ~BIT(6);
	PUP1 |= BIT(6);
#elif (SD_PORT == 1)
	PDN1 &= ~BIT(3);
	PUP1 |= BIT(3);
#elif (SD_PORT == 2)
	PDN4 &= ~BIT(2);
	PUP4 |= BIT(2);
#elif (SD_PORT == 3)
	PDN0 &= ~BIT(6);
	PUP0 |= BIT(6);
#endif
#endif
}

#pragma location="WKUP_SEG"
void pin3_config(void)
{
	AIPCON6 = (AIPCON6 & (~0x04)) | (USER_WKUP_PORT << 2); //wakeup pin3(AIPCON6[2])

#if (USER_WKUP_PIN3_PORT == WKUP_PIN3_P03)
	pin3_p03_config();
#elif (USER_WKUP_PIN3_PORT == WKUP_PIN3_SDCLK)
	pin3_sdclk_config();
#endif
}

//任务初始化
#pragma location="TASK_LOWPOWER_SEG"
static void task_lowpower_enter(void)
{
	printf("enter lowpower.\n");

	WATCHDOG_KILL();
	IE_EA = 0;

	ADCCON = 0;
	UARTCON = 0;
	TMR2CON0 = 0;                       //disable timer2

	USBDPDM = 0;                        //disable usb pu/pd
	USBCON0 = 0;
	USBCON1 = 0;
	AIPCON7 |= 0x03;                    //P4 as gpio

	disp_clear();
	DACACON2 &= ~(BIT(4)|BIT(1)|BIT(0));//disable en_vcmbuf, en_lpf, en_hp
	DACACON1 &= ~BIT(6);                //disable dac_anl
	DACACON1 &= ~(BIT(3)|BIT(2));       //disable dacbias, adckey/pll/rc/touch key biasing
	DACACON2 &= ~(BIT(6)|BIT(5));

	//IO,请根据实际硬件情况配置
	P0DIR = 0xff;
	P1DIR = 0xff;
	P2DIR = 0xff;
	P3DIR = 0xff;
	P4DIR = 0xff;

	PUP2L = 0x00;
	PDN0 = 0xff;
	PDN1 = 0xff;
	PDN2 = 0xff;
	PDN3 = 0xff;
	PDN4 = 0xff;
	PUP0 = 0x00;
	PUP1 = 0x00;
	PUP3 = 0x00;
	PUP4 = 0x00;
	AIPCON5 &= ~(BIT(4) | BIT(3));  //bit4-P17 500R; bit3-P15 500R

	//唤醒设置
	pwkedge_config();
#if (USER_WKUP_PIN == WKUP_PIN_0)
	pin0_config();
#elif (USER_WKUP_PIN == WKUP_PIN_1)
	pin1_config();
#elif (USER_WKUP_PIN == WKUP_PIN_2)
	pin2_config();
#elif (USER_WKUP_PIN == WKUP_PIN_3)
	pin3_config();
#endif
	delay_1ms(10);
	PWKEN = 0x0F & (~BIT(USER_WKUP_PIN)); //enable PWKEN0/PWKEN1/PWKEN2/PWKEN3 wakeup

	set_sys_clk(SYS_RC_1M);

	PCON0 |= BIT(1);
}

//任务退出
#pragma location="TASK_LOWPOWER_SEG"
static void task_lowpower_exit(void)
{
	WDTCON = 0x18;           //watchdog 2ms后reset
	while(1);
}

//任务主流程
#pragma location="TASK_LOWPOWER_SEG"
void task_lowpower(void)
{
	printf("task_lowpower\n");
	task_lowpower_enter();
	task_lowpower_exit();
}

#endif
