/*****************************************************************************
 * Module    : Task
 * File      : task_fmam_detect.c
 * Author    :
 * Email     :
 * Function  : fm/am频率检测任务流程
 *****************************************************************************/
#include "include.h"

#if TASK_FMAM_DETECT_EN

IAR_XDATA_A type_fmam_ctl fmam_ctl;

/****************************************************
    AM frequency band: 500K ~ 31MHz
    FM frequency band: 60M  ~ 130MHz
****************************************************/


#pragma location="TASK_FMAM_DETECT_SEG"
static void task_fmam_detect_enter(void)
{
	PCON2 &= ~BIT(5);                       //FMAM clock en
	PIE1 &= ~BIT(6);                        //P35 digital input disabled
	P3DIR |= BIT(5);

	FMAMTPR = 0x00;                         //8ms * 24000 - 191488 = 512(0x200)
	//FMAMCON0[5:4] = 2b'10

	FMAMCON0 = BIT(6)|BIT(5)|BIT(1)|BIT(0); //bit6-CHSEL:fm and am channel effect
	//bit5 bit4-:FMAMTPRH
	//bit1-AMIN_EN:am input enable
	//bit0-FMIN_EN:fm input enable
	FMAMCON2 |= BIT(1);                     //bit1-FMAMCNT_CLR:fmam count clean zero
	FMAMCON1 = BIT(1) | BIT(0);             //bit1-FM_SEL:fm frequency select fm
	//bit0-FMAM_EN:fmam frequency detect enable
}

#pragma location="TASK_FMAM_DETECT_SEG"
static void task_fmam_detect_exit(void)
{
	FMAMCON0 = 0;
	FMAMCON1 = 0;
}

#pragma location="TASK_FMAM_DETECT_SEG"
static u16 get_freq_value(void)
{
	u16 freq;
	while(!(FMAMCON1&BIT(3)))       //wait fmam detect finish
	{
		WATCHDOG_CLR();
	}
	FMAMCON1 &= ~BIT(3);
	freq = FMAMCNTH;
	freq <<= 8;
	freq += FMAMCNTL;
	return freq;
}

#pragma location="TASK_FMAM_DETECT_SEG"
static void get_fmam_freq(void)
{
	u32 freq;

	FMAMCON2 |= BIT(1);             /* 先清计数器 */
	FMAMCON1 = BIT(1)|BIT(0);       /* 切成FM通道，同时清Pending */
	delay_5ms(20);
	freq = get_freq_value();
	printf("[%ld]", freq);
	if(freq > (59000>>2))
	{
		freq <<= 2;
		fmam_ctl.freq = freq;
		printf("------fm freq = %ld\n", fmam_ctl.freq);
		return;
	}

	FMAMCON2 |= BIT(1);             /* 先清计数器 */
	FMAMCON1 = BIT(0);              /* 切成AM通道，同时清Pending */
	delay_5ms(20);
	fmam_ctl.freq = get_freq_value();
	printf("am freq = %ld\n", fmam_ctl.freq);
}

#pragma location="TASK_FMAM_DETECT_SEG"
static void task_fmam_detect_event(void)
{
	WATCHDOG_CLR();
	get_fmam_freq();
}

#pragma location="TASK_FMAM_DETECT_SEG"
static void task_fmam_detect_deal_msg(u8 msg)
{
}

#pragma location="TASK_FMAM_DETECT_SEG"
static void task_fmam_detect_display(void)
{
}

#pragma location="TASK_FMAM_DETECT_SEG"
void task_fmam_detect(void)
{
	printf("task_fmam_detect.\n");
	task_fmam_detect_enter();
	while(task_ctl.work_sta == TASK_FMAM_DETECT)
	{
		task_fmam_detect_event();
		task_fmam_detect_deal_msg(get_msg());
		task_fmam_detect_display();
	}
	task_fmam_detect_exit();
}

#endif
