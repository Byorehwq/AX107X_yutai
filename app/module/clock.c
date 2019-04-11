/*****************************************************************************
 * Module    : Module
 * File      : clock.c
 * Author    : jingfa
 * Email     :
 * Function  : 系统时钟的相关配置
 *****************************************************************************/
#include "include.h"

/*
1、rc2m上电时使能并计算出实际偏差，因为功耗较低，而且作为WDT时钟源，在切时钟时不关闭，
   所以切PLL分频前，可以先切到rc2m；
2、若使用xosc，因为使能延时较长（约500ms），上电时使能一次，切时钟时不关闭；
3、rc10m上电时用作为PLL时钟源，初始化DAC，然后根据时钟选择，可以动态开关；
4、系统只会使用一种xosc，所以选定xosc时，就把其他xosc关掉；
*/

void Pllset(void);
u16 cal_rc2m(void);                         //计算rc2m实际频率，系统时钟必须为24M
void pll_clk_x32k(void);                    //使用xosc32k作为pll时钟源
void pll_clk_x4m(void);                     //使用xosc4m作为pll时钟源
void pll_clk_x12m(void);                    //使用xosc12m作为pll时钟源
void pll_clk_x24m(void);                    //使用xosc24m作为pll时钟源
void pll_clk_rc10m(void);                   //使用rc12m作为pll时钟源
void sys_clk_pll(u8 sys_div);               //使用pll作为系统时钟
void pll_clk_rc10m_sram(u8 pll_intl,u8 pll_fracl,u8 pll_frach);

IAR_CONST u8 pll_v_s[] = {0, 1, 1, 2};

//输出系统时钟
#pragma location="CLK_SEG"
void sysclk_output(void)
{
	CLKCON3 |= BIT(7);
	CLKCON3 &= ~BIT(6);
	AIPCON8 |= (1<<1) | (1<<0);
	AIPCON8 &= ~(1<<3);
	P0DIR &= ~(1<<6);
}

//输出RTC时钟
#pragma location="CLK_SEG"
void rtcclk_output(void)
{
	CLKCON2 |= BIT(7);
	CLKCON3 |= BIT(6);
	AIPCON8 |= BIT(1) | BIT(0); //clock output IO select P06
	P0DIR &= ~BIT(6);
}

//输出PLL参考时钟(如晶振时钟、RC时钟)
#pragma location="CLK_SEG"
void pllrefclk_output(void)
{
	CLKCON4 |= BIT(4);          //pll test signal enable
	CLKCON2 &= ~BIT(2);         //output pll reference clock
	AIPCON8 |= BIT(1) | BIT(0); //clock output IO select P06
	P0DIR &= ~BIT(6);
}

/*----------------------------------------------------------------------------*/
/**@brief 系统时钟初始化
   @param 无
   @return 无
   @note 上电默认PLL使用RC10M，SYS=PLL=24M
*/
/*----------------------------------------------------------------------------*/
#pragma location="CLK_SEG"
void sysclk_set(void)
{
	IRTCON1 |= BIT(2);          //IRTCC POR reset system enable(VDDRTC通过电阻短接到地可复位芯片)

	RC_CON |= BIT(5)|BIT(4);                //trim_clk tie to 0

	/*
	    sysclk_output();            //输出系统时钟
	    rtcclk_output();            //输出RTC时钟
	    pllrefclk_output();         //输出PLL参考时钟(如晶振时钟、RC时钟)
	*/

#if (CRY_PIN_NUM == CRY_1PIN)
	CLKCON3 |= BIT(5);          //xosc select one pin mode
#endif
#ifdef UART_DEBUG
	DPCON &= 0x3F;
	DPCON |= BIT(6);
#ifdef AX207X_TAG
	select_irom1();
	Pllset();
	restore_mem();
#endif
	sys.delay_sys = 48;                         //上电默认是24M
	sys.clk_mode = SYS_24M;
#else   //UART_DEBUG
#if ((SYS_CRY == CRY_32K) || (SYS_CRY == CRY_AUTO))
	irtc_write_cmd(CFG3_WR, 0x07);              //drive sel 加快xosc起振时间但会增加功耗 起振完后要把驱动改回默认值以节省功耗
	CLKCON |= BIT(7)|BIT(4);                    //enable xosc32k, rc2m
	CLKCON &= ~(BIT(6)|BIT(2));                 //disable xosc12m, xosc select 32k
#elif ((SYS_CRY == CRY_24M) || (SYS_CRY == CRY_12M) || (SYS_CRY == CRY_4M))
	CLKCON |= BIT(6)|BIT(4)|BIT(2);             //enable xosc12m, rc2m, xosc select 12m
	CLKCON &= ~BIT(7);                          //disable xosc32k
#endif
	sys.delay_sys = 5;                          //上电默认是512k

#if SRAM_DEBUG
	pll_clk_rc10m_sram(PLL_INTL,PLL_FRACL,PLL_FRACH);
#else
	pll_clk_rc10m();
#endif

	sys_clk_pll(1);
	sys.delay_sys = 48;
	sys.clk_mode = SYS_24M;
#endif

#if TASK_LOWPOWER_EN
	sys_ctl.rc2m = cal_rc2m();                  //计算rc2m实际频率
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief 外部晶振初始化
   @param 无
   @return 无
   @note 有晶振时，系统时钟由RC+PLL切换到XOSC+PLL；无晶振时，不切换
*/
/*----------------------------------------------------------------------------*/
#pragma location="CLK_SEG"
void sys_pll_xosc(void)
{
#ifndef UART_DEBUG

#if (SYS_CRY == CRY_AUTO)
	irtc_write_cmd(CFG3_WR, 0x04); //因为起振要一段时间 起振后在此处把xosc的驱动改回默认值以节省功耗
	if(!b_sys.xosc_flag)
	{
		return;                    //自动判断模式，无晶振，不切换
	}
	pll_clk_x32k();
#elif (SYS_CRY == CRY_32K)
#if (CRY_PIN_NUM == CRY_1PIN)    //单脚晶振起振慢
	if(sys_ctl.irtc_poweron)
	{
		delay_5ms(70);
		WATCHDOG_CLR();
	}
#endif
	irtc_write_cmd(CFG3_WR, 0x04); //因为起振要一段时间 起振后在此处把xosc的驱动改回默认值以节省功耗
	pll_clk_x32k();
#elif (SYS_CRY == CRY_4M)
	pll_clk_x4m();
#elif (SYS_CRY == CRY_12M)
	pll_clk_x12m();
#elif (SYS_CRY == CRY_24M)
	pll_clk_x24m();
#elif (SYS_CRY == CRY_RC)
#else
#error "请选择一种时钟源"
#endif
	sys_clk_pll(1);

	CLKCON &= ~BIT(5);                  //disable rc12m
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief 切换系统时钟
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="SET_SYS_CLK_SEG"
void set_sys_clk(u8 mode)
{
	u8 pll_v, sys_v;
	if(sys.clk_mode == mode)
	{
		return;
	}
	sys.clk_mode = mode;
#if 0// PRINTF_EN
	while((UARTCON & BIT(4)) && (UARTSTA & BIT(4)) == 0);           //等待串口打印完毕
#endif

	bool tmp_ea = IE_EA;
	IE_EA = 0;

	switch(mode)
	{
#if (FM_CLOCK == USE_SD_IO_32KHZ_CLK)
	case SYS_12M:
		pll_v = 0;              //PLL = 48M
		sys_v = 2;              //SYS = 12M
		break;
#endif
	case SYS_24M:
		pll_v = 0;              //PLL = 48M
		sys_v = 1;              //SYS = 24M
		break;

#if WAV_SUPPORT                 //目前只有WAV用到了48M
	case SYS_48M:
		pll_v = 0;              //PLL = 48M
		sys_v = 0;              //SYS = 48M
#ifdef AX107X_TAG
		MEMCON |= BIT(7);       //otp_clk = sys_clk/2
#endif
		break;
#endif

#if TASK_LOWPOWER_EN
	case SYS_RC_1M:
		CLKCON |= BIT(4);                   //enable rc2m
		delay_1ms(1);                       //delay >2us ????
		CLKCON = (CLKCON & 0xfc) | BIT(0);  //sysclk select rc2m
		CLKCON1 = (CLKCON1 & 0xf3);         //sysclk no div
		CLKCON &= ~BIT(5);                  //disable rc12m
		CLKCON3 &= ~(BIT(0)|BIT(4));        //disable pll, rc_sys_clk select rc2m
		CLKCON2 = (CLKCON2 & 0xfc) | BIT(0);//rc_sys_clk div 4
		CLKCON &= ~BIT(3);                  //mux_clk select rc_sys_clk
		CLKCON = (CLKCON & 0xfc);           //sysclk select mux_clk

		TMR2PRH = (u8)(sys_ctl.rc2m>>9);
		TMR2PRL = (u8)(sys_ctl.rc2m>>1);
		TMR2CON1 = (TMR2CON1 & 0xf8);       //定时器
#if PRINTF_EN
		UARTBAUD = sys_ctl.rc2m*10/2/1152;  //UART
		UARTBAUDH = 0;
#endif
		sd_set_rate(0);                     //sd
		IE_EA = tmp_ea;
		return;
#endif

		/*
		    case SYS_RC_2M:
		        CLKCON |= BIT(4);                   //enable rc2m
		        delay_1ms(1);                       //delay >2us ????
		        CLKCON = (CLKCON & 0xfc) | BIT(0);  //sysclk select rc2m
		        CLKCON1 = (CLKCON1 & 0xf3);         //sysclk no div
		        CLKCON &= ~BIT(5);                  //disable rc12m
		        CLKCON3 &= ~BIT(0);                 //disable pll

		        TMR2PRH = (u8)(sys_ctl.rc2m>>8);
		        TMR2PRL = (u8)(sys_ctl.rc2m);
		        TMR2CON1 = (TMR2CON1 & 0xf8);       //定时器
		#if PRINTF_EN
		        UARTBAUD = sys_ctl.rc2m*10/1152;    //UART
		        UARTBAUDH = 0;
		#endif
		        sd_set_rate(0);                     //sd
		        IE_EA = tmp_ea;
		        return;
		*/
	}

	//设置pll
	if((CLKCON3 & BIT(0)) == 0)
	{
#if (SYS_CRY == CRY_AUTO)
		if(b_sys.xosc_flag)
		{
			pll_clk_x32k();
		}
		else
		{
#if SRAM_DEBUG
			pll_clk_rc10m_sram(PLL_INTL,PLL_FRACL,PLL_FRACH);
#else
			pll_clk_rc10m();
#endif
		}
#elif (SYS_CRY == CRY_32K)
		pll_clk_x32k();
#elif (SYS_CRY == CRY_4M)
		pll_clk_x4m();
#elif (SYS_CRY == CRY_12M)
		pll_clk_x12m();
#elif (SYS_CRY == CRY_24M)
		pll_clk_x24m();
#elif (SYS_CRY == CRY_RC)
#else
#error "请选择一种时钟源"
#endif
	}

	//设置系统时钟
	sys_clk_pll(sys_v);

#ifdef AX107X_TAG
#if WAV_SUPPORT     //目前只有WAV用到了48M
	if(mode != SYS_48M)
	{
		MEMCON &= ~BIT(7);       //otp_clk = sys_clk
	}
#endif
#endif

	//设置模块时钟
	sys_v += pll_v_s[pll_v & 0x03];
#if LED_7P7S_SCAN_HALF_MS
	TMR2PRH = 0x00;
	TMR2PRL = 0x2f;      // 0x005d为0.125ms
	//TMR2PRH = 0x00;
	//TMR2PRL = 0xBA;      // 0x00BA为0.5ms
#else
	TMR2PRH = 0x01;
	TMR2PRL = 0x77;      // 0x0752为5ms, 0x177为1ms
#endif
	TMR1CON1 = (TMR1CON1 & 0xf8) | (7-sys_v);   //定时器
	TMR2CON1 = (TMR2CON1 & 0xf8) | (7-sys_v);   //定时器
#if PRINTF_EN
	UARTBAUD = 0x33 >> sys_v;                   //UART
	UARTBAUDH = 0x70;
#endif
	sd_set_rate(0);                             //sd, 系统在12M时跑6M，其余为12M

	IE_EA = tmp_ea;
}

//获取系统时钟
#pragma location="CLK_SEG"
u8 get_sys_clk(void)
{
	return sys.clk_mode;
}
