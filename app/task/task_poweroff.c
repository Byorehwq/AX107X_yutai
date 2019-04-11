/*****************************************************************************
 * Module    : Task
 * File      : task_poweroff.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 关机任务流程
 *****************************************************************************/
#include "include.h"

#if TASK_POWEROFF_EN

void task_poweroff_display(void);

#if SOFT_POWEROFF_EN

type_poweroff_ctl poweroff_ctl;

__root void osc_delay(u16 cnt);
#pragma location="TASK_POWEROFF_SEG"
void soft_poweroff(void)
{
	while(1)
	{
		irtc_write_cmd(PWR_WR, 0x01);//diable VDDCORE
		WDTCON = 0x3d;
		osc_delay(0x2000);             //delay 100ms
	}
}

#pragma location="TASK_POWEROFF_SEG"
void soft_poweroff_enter(void)
{
	if((sys_ctl.bat_sta == VOL_INIT)               //未完成电压检测
//	        || (sys_ctl.bat_sta == VOL_GREATER_44V)     //电压高于4.4V时不能软关机，否则会损坏芯片
#if DCIN_DET_EN
	        || (poweroff_ctl.dc_in_sta == DC_IN)        //充电时不进软关机
#endif
	  )
	{
		return;
	}
	

	printf("soft_powerdown\n");
	
	#if (SYS_CRY == CRY_RC)          //建荣提供修改   2017/05/12
        irtc_write_cmd(CFG_WR,0x0c);//
    #else
    #if (CRY_PIN_NUM == CRY_1PIN)
            irtc_write_cmd(CFG_WR,0xac);//enable 32Khz,choose 1pin osc
    #else
            irtc_write_cmd(CFG_WR,0x8c);//enable 32Khz
    #endif
    #endif
	
//#if (CRY_PIN_NUM == CRY_1PIN)
//	irtc_write_cmd(CFG_WR,0xac);//enable 32Khz,choose 1pin osc
//#else
//	irtc_write_cmd(CFG_WR,0x8c);//enable 32Khz
//#endif

	CLKCON |= BIT(4);           //enable rc2m
	IE_EA =0;                   //disable IE

	//打开2mA和8mA负载，以加快VDDCORE放电
	AIPCON8 |= BIT(6) | BIT(5);

	//打开P2内部500R上拉电阻且输出0，以加快VDDCORE放电
	//【注意】请根据外部电路情况配置合适的IO状态
	disp_port_init();

	PUP2 = 0xff;
	PUP2L = 0xff;
	P2 = 0x00;
	P2DIR = 0x00;


	/*把函数hold_func拷贝到iram*/
	void (*funpt)();
	funpt = (void ( *)())0x0000;
	u8 const   __code *otp_tbl;
	u8 *ram_tbl;
	u8 i;
	otp_tbl = (u8 const __code *)((u16)soft_poweroff);
	ram_tbl = (u8 *)0x0000;
	for(i = 0; i < 20; i++)
	{
		*ram_tbl = *otp_tbl;
		ram_tbl++;
		otp_tbl++;
	}

	CLKCON = (CLKCON & 0xfc) | BIT(0);  //sysclk select rc2m
	CLKCON1 = (CLKCON1 & 0xf3);         //sysclk no div

	(*funpt)();
}

#pragma location="TASK_POWEROFF_SEG"
void wko_init(void)
{
    u8 wkocon_val = irtc_read_cmd(WKO_RD);
    #if (SYS_CRY == CRY_RC)
            CLKCON2 |= BIT(7);
            wkocon_val |= BIT(4) | BIT(3) | BIT(2);
            wkocon_val &= ~BIT(6);
    #else
            wkocon_val |= BIT(6) | BIT(4) | BIT(3) | BIT(2);
                //bit6-FITEN:WKO 1ms filter enable
                //bit4-WKOEN:WKO PIN enable
                //bit3-WKOUTEN:WKO PIN output enable
                //bit2-WKOINEN:WKO PIN input enable
    #endif
    
    irtc_write_cmd(WKO_WR,wkocon_val);
    
    #if SRAM_DEBUG
    	//【注意】在SRAM板上调试时，请使用电池供电，不要连接5V电源
    	irtc_write_cmd(VCL_WR,0x06);//使能HVD并把HVD级别设置为4.2v(防止连接5V电源时使用软关机把芯片烧掉)
    #endif

}


/* 
void wko_init(void)
{
	u8 wkocon_val = irtc_read_cmd(WKO_RD);
	wkocon_val |= BIT(6) | BIT(4) | BIT(3) | BIT(2);
	//bit6-FITEN:WKO 1ms filter enable
	//bit4-WKOEN:WKO PIN enable
	//bit3-WKOUTEN:WKO PIN output enable
	//bit2-WKOINEN:WKO PIN input enable
	wkocon_val &= ~BIT(7);
	//bit7-WKPIN_STA:wake up pin output 0,打开内部下拉电阻
	irtc_write_cmd(WKO_WR,wkocon_val);

#if SRAM_DEBUG
	//【注意】在SRAM板上调试时，请使用电池供电，不要连接5V电源
	irtc_write_cmd(VCL_WR,0x06);//使能HVD并把HVD级别设置为4.2v(防止连接5V电源时使用软关机把芯片烧掉)
#endif
}
  */


#pragma location="TASK_POWEROFF_SEG"
void soft_poweroff_init(void)
{
	u8 poweron_cnt;

	//printf("Pw:%d\n",poweroff_ctl.power_key_sta);
	//poweroff_ctl.power_key_sta = 1;   //  强制开启电源   20170617
	while(poweroff_ctl.power_key_sta == POWERKEY_INIT)     //等待确认power key的状态
	{
		WATCHDOG_CLR();
	}
#if DCIN_DET_EN
	while((poweroff_ctl.dc_in_sta == DC_INIT)
	        || (poweroff_ctl.dc_in_sta == DC_UNSTABLE))              //等待确认DC_IN充电线的状态
	{
		WATCHDOG_CLR();
	}
#endif

#if POWERON_LONG_KEY
	poweron_cnt = 250;    //100*5ms = 500ms,长按POWER键开机    长按1.25s开机 
#else
	poweron_cnt = 10;     //10*5ms = 50ms,短按POWER键开机
#endif

	uart_putc('A');

	for(u8 i = 0; i < poweron_cnt; i++)
	{
		//if((poweroff_ctl.power_key_sta == POWERKEY_DOWN)   //power key按下或DC IN
		if(1   //power key按下或DC IN
#if DCIN_DET_EN
		        &&(poweroff_ctl.dc_in_sta == DC_OUT)
#endif
		  )         //充电线拔出状态
		{
			delay_5ms(1);
			WATCHDOG_CLR();
			//uart_putc('<');
		}
		else
		{
			//printf("bat_sta: %d ",sys_ctl.bat_charger_sta);
			if(sys_ctl.bat_charger_sta == 1)
			{
				uart_putc('a');
				while(sys_ctl.bat_charger_sta == 1)     //等待确认power key的状态
				{
					//uart_putc('b');
					WATCHDOG_CLR();
					if((get_msg() == KL_POWER) || (get_msg() == KH_POWER))
					{
						WDTCON = 0x18;  //watchdog 2ms后reset
						printf("charge_power_on\n");
	                    while(1);
					}
				}
			}
			uart_putc('+');
			poweroff_ctl.flag = 0;        //用于区分是从初始化中关机，还是从其它模式中关机
			task_ctl.work_sta = TASK_POWEROFF;
			task_poweroff();              //继续关机
			break;
		}
	}

	poweroff_ctl.flag = 1;                //用于区分是从初始化中关机，还是从其它模式中关机

	printf("power on...\n");
	//showhello();	
}
#endif

//任务事件
#pragma location="TASK_POWEROFF_SEG"
void task_poweroff_event(void)
{
	WATCHDOG_CLR();

	mem_event();

	//printf("s_");
	//delay_5ms(200);          //延时2s期间会输出低电平
	//delay_5ms(200);
	//delay_5ms(200);
	

#if SOFT_POWEROFF_EN
	if(poweroff_ctl.power_key_sta == POWERKEY_UP)
	{
		//printf("e_");
		soft_poweroff_enter();
	}
#endif
}

//任务消息处理
#pragma location="TASK_POWEROFF_SEG"
void task_poweroff_deal_msg(u8 msg)
{
	
	switch(msg)
	{
	case KL_POWER:
#if SOFT_POWEROFF_EN
#if POWERON_LONG_KEY
	case KL_SOFT_POWER:    //长按开机
#else
	case KU_SOFT_POWER:    //短按开机
#endif
#endif
        printf("Again on\n");
		task_ctl.work_sta = task_ctl.last_work;
		break;
	default:
		break;
	}
	
}

/*****************************************************************************
 * Module    : 关机任务入口
 *****************************************************************************/
//任务初始化
#pragma location="TASK_POWEROFF_SEG"
void task_poweroff_enter(void)
{
	PCON0 |= BIT(2);
#if (DISP_TYPE==DISP_LED)
	ocx.led_sta = LED_STA_ON;
#endif

#if ALARM_WAKEUP_EN
	u8 wkocon_val = irtc_read_cmd(WKO_RD);
	wkocon_val &= ~BIT(7);
	wkocon_val |= BIT(5);  //enable alarm output update wko output
	irtc_write_cmd(WKO_WR, wkocon_val);
#endif

#if SOFT_POWEROFF_EN
	if(poweroff_ctl.flag)
	{
#endif
		showoff();
        //uart_putc('+');
		//delay_5ms(100);   // 取消延时，缩短关机等待时间  20170726  //延时中可看出MUTE输出为高电平，查找关机ber声产生原因
		//uart_putc('-');
		WATCHDOG_CLR();
		
#if SOFT_POWEROFF_EN
	}
	else
	{
		disp_clear();
	}
#endif
//if(sys_ctl.user_power_down_flag == 1)//关机键按下，断电关机   //  重新把mute拉低，导致关机有ber声音  20170705   wq 
//	SOFT_POWER_CTL_OFF();             

}

//任务退出
#pragma location="TASK_POWEROFF_SEG"
void task_poweroff_exit(void)
{
}

//任务主流程
#pragma location="TASK_POWEROFF_SEG"
void task_poweroff(void)
{
	printf("task_poweroff\n");
	task_poweroff_enter();
	while(task_ctl.work_sta == TASK_POWEROFF)
	{
		task_poweroff_deal_msg(get_msg());
		task_poweroff_display();
		task_poweroff_event();
	}
	task_poweroff_exit();
}
#endif


