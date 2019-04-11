/*****************************************************************************
 * Module    : Module
 * File      : clock.c
 * Author    : jingfa
 * Email     :
 * Function  : ϵͳʱ�ӵ��������
 *****************************************************************************/
#include "include.h"

/*
1��rc2m�ϵ�ʱʹ�ܲ������ʵ��ƫ���Ϊ���Ľϵͣ�������ΪWDTʱ��Դ������ʱ��ʱ���رգ�
   ������PLL��Ƶǰ���������е�rc2m��
2����ʹ��xosc����Ϊʹ����ʱ�ϳ���Լ500ms�����ϵ�ʱʹ��һ�Σ���ʱ��ʱ���رգ�
3��rc10m�ϵ�ʱ����ΪPLLʱ��Դ����ʼ��DAC��Ȼ�����ʱ��ѡ�񣬿��Զ�̬���أ�
4��ϵͳֻ��ʹ��һ��xosc������ѡ��xoscʱ���Ͱ�����xosc�ص���
*/

void Pllset(void);
u16 cal_rc2m(void);                         //����rc2mʵ��Ƶ�ʣ�ϵͳʱ�ӱ���Ϊ24M
void pll_clk_x32k(void);                    //ʹ��xosc32k��Ϊpllʱ��Դ
void pll_clk_x4m(void);                     //ʹ��xosc4m��Ϊpllʱ��Դ
void pll_clk_x12m(void);                    //ʹ��xosc12m��Ϊpllʱ��Դ
void pll_clk_x24m(void);                    //ʹ��xosc24m��Ϊpllʱ��Դ
void pll_clk_rc10m(void);                   //ʹ��rc12m��Ϊpllʱ��Դ
void sys_clk_pll(u8 sys_div);               //ʹ��pll��Ϊϵͳʱ��
void pll_clk_rc10m_sram(u8 pll_intl,u8 pll_fracl,u8 pll_frach);

IAR_CONST u8 pll_v_s[] = {0, 1, 1, 2};

//���ϵͳʱ��
#pragma location="CLK_SEG"
void sysclk_output(void)
{
	CLKCON3 |= BIT(7);
	CLKCON3 &= ~BIT(6);
	AIPCON8 |= (1<<1) | (1<<0);
	AIPCON8 &= ~(1<<3);
	P0DIR &= ~(1<<6);
}

//���RTCʱ��
#pragma location="CLK_SEG"
void rtcclk_output(void)
{
	CLKCON2 |= BIT(7);
	CLKCON3 |= BIT(6);
	AIPCON8 |= BIT(1) | BIT(0); //clock output IO select P06
	P0DIR &= ~BIT(6);
}

//���PLL�ο�ʱ��(�羧��ʱ�ӡ�RCʱ��)
#pragma location="CLK_SEG"
void pllrefclk_output(void)
{
	CLKCON4 |= BIT(4);          //pll test signal enable
	CLKCON2 &= ~BIT(2);         //output pll reference clock
	AIPCON8 |= BIT(1) | BIT(0); //clock output IO select P06
	P0DIR &= ~BIT(6);
}

/*----------------------------------------------------------------------------*/
/**@brief ϵͳʱ�ӳ�ʼ��
   @param ��
   @return ��
   @note �ϵ�Ĭ��PLLʹ��RC10M��SYS=PLL=24M
*/
/*----------------------------------------------------------------------------*/
#pragma location="CLK_SEG"
void sysclk_set(void)
{
	IRTCON1 |= BIT(2);          //IRTCC POR reset system enable(VDDRTCͨ������̽ӵ��ؿɸ�λоƬ)

	RC_CON |= BIT(5)|BIT(4);                //trim_clk tie to 0

	/*
	    sysclk_output();            //���ϵͳʱ��
	    rtcclk_output();            //���RTCʱ��
	    pllrefclk_output();         //���PLL�ο�ʱ��(�羧��ʱ�ӡ�RCʱ��)
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
	sys.delay_sys = 48;                         //�ϵ�Ĭ����24M
	sys.clk_mode = SYS_24M;
#else   //UART_DEBUG
#if ((SYS_CRY == CRY_32K) || (SYS_CRY == CRY_AUTO))
	irtc_write_cmd(CFG3_WR, 0x07);              //drive sel �ӿ�xosc����ʱ�䵫�����ӹ��� �������Ҫ�������Ļ�Ĭ��ֵ�Խ�ʡ����
	CLKCON |= BIT(7)|BIT(4);                    //enable xosc32k, rc2m
	CLKCON &= ~(BIT(6)|BIT(2));                 //disable xosc12m, xosc select 32k
#elif ((SYS_CRY == CRY_24M) || (SYS_CRY == CRY_12M) || (SYS_CRY == CRY_4M))
	CLKCON |= BIT(6)|BIT(4)|BIT(2);             //enable xosc12m, rc2m, xosc select 12m
	CLKCON &= ~BIT(7);                          //disable xosc32k
#endif
	sys.delay_sys = 5;                          //�ϵ�Ĭ����512k

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
	sys_ctl.rc2m = cal_rc2m();                  //����rc2mʵ��Ƶ��
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief �ⲿ�����ʼ��
   @param ��
   @return ��
   @note �о���ʱ��ϵͳʱ����RC+PLL�л���XOSC+PLL���޾���ʱ�����л�
*/
/*----------------------------------------------------------------------------*/
#pragma location="CLK_SEG"
void sys_pll_xosc(void)
{
#ifndef UART_DEBUG

#if (SYS_CRY == CRY_AUTO)
	irtc_write_cmd(CFG3_WR, 0x04); //��Ϊ����Ҫһ��ʱ�� ������ڴ˴���xosc�������Ļ�Ĭ��ֵ�Խ�ʡ����
	if(!b_sys.xosc_flag)
	{
		return;                    //�Զ��ж�ģʽ���޾��񣬲��л�
	}
	pll_clk_x32k();
#elif (SYS_CRY == CRY_32K)
#if (CRY_PIN_NUM == CRY_1PIN)    //���ž���������
	if(sys_ctl.irtc_poweron)
	{
		delay_5ms(70);
		WATCHDOG_CLR();
	}
#endif
	irtc_write_cmd(CFG3_WR, 0x04); //��Ϊ����Ҫһ��ʱ�� ������ڴ˴���xosc�������Ļ�Ĭ��ֵ�Խ�ʡ����
	pll_clk_x32k();
#elif (SYS_CRY == CRY_4M)
	pll_clk_x4m();
#elif (SYS_CRY == CRY_12M)
	pll_clk_x12m();
#elif (SYS_CRY == CRY_24M)
	pll_clk_x24m();
#elif (SYS_CRY == CRY_RC)
#else
#error "��ѡ��һ��ʱ��Դ"
#endif
	sys_clk_pll(1);

	CLKCON &= ~BIT(5);                  //disable rc12m
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief �л�ϵͳʱ��
   @param ��
   @return ��
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
	while((UARTCON & BIT(4)) && (UARTSTA & BIT(4)) == 0);           //�ȴ����ڴ�ӡ���
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

#if WAV_SUPPORT                 //Ŀǰֻ��WAV�õ���48M
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
		TMR2CON1 = (TMR2CON1 & 0xf8);       //��ʱ��
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
		        TMR2CON1 = (TMR2CON1 & 0xf8);       //��ʱ��
		#if PRINTF_EN
		        UARTBAUD = sys_ctl.rc2m*10/1152;    //UART
		        UARTBAUDH = 0;
		#endif
		        sd_set_rate(0);                     //sd
		        IE_EA = tmp_ea;
		        return;
		*/
	}

	//����pll
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
#error "��ѡ��һ��ʱ��Դ"
#endif
	}

	//����ϵͳʱ��
	sys_clk_pll(sys_v);

#ifdef AX107X_TAG
#if WAV_SUPPORT     //Ŀǰֻ��WAV�õ���48M
	if(mode != SYS_48M)
	{
		MEMCON &= ~BIT(7);       //otp_clk = sys_clk
	}
#endif
#endif

	//����ģ��ʱ��
	sys_v += pll_v_s[pll_v & 0x03];
#if LED_7P7S_SCAN_HALF_MS
	TMR2PRH = 0x00;
	TMR2PRL = 0x2f;      // 0x005dΪ0.125ms
	//TMR2PRH = 0x00;
	//TMR2PRL = 0xBA;      // 0x00BAΪ0.5ms
#else
	TMR2PRH = 0x01;
	TMR2PRL = 0x77;      // 0x0752Ϊ5ms, 0x177Ϊ1ms
#endif
	TMR1CON1 = (TMR1CON1 & 0xf8) | (7-sys_v);   //��ʱ��
	TMR2CON1 = (TMR2CON1 & 0xf8) | (7-sys_v);   //��ʱ��
#if PRINTF_EN
	UARTBAUD = 0x33 >> sys_v;                   //UART
	UARTBAUDH = 0x70;
#endif
	sd_set_rate(0);                             //sd, ϵͳ��12Mʱ��6M������Ϊ12M

	IE_EA = tmp_ea;
}

//��ȡϵͳʱ��
#pragma location="CLK_SEG"
u8 get_sys_clk(void)
{
	return sys.clk_mode;
}
