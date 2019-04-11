/*****************************************************************************
 * Module    : MAIN
 * File      : main.c
 * Author    : jingfa
 * Email     :
 * Function  : 主流程
 *****************************************************************************/
#include "include.h"
#include "user_io.h"
#include "user_key.h"


extern u8 SPI_wait_timeout;
extern u8 vol_less_power_warning_flag;//低电压提示后关机标志
extern u8 vol_less_33V_flag;         //电压3.3v标志
extern u8 vol_less_34V_flag;         //电压3.4v标志
extern bool vol_less_34V_blink; //低于3.3v屏闪
extern bool Timer1_flag;

extern u8 power_key_cnt;
extern u8 count_led_off;
u8 vcmbuf_flag = 1;
#define TIMER_15	15
#define TIMER_30	30
#define TIMER_60	60
#define TIMER_7h	420

//1ms定时器延时
#pragma location="SYS_DELAY"
void delay_1ms(u8 cnt)
{
#ifdef AX207X_TAG
	if(sys_fix.usb_init_fix)
	{
		sys_fix.usb_init_fix = 0;
		return;
	}
#endif

	/*IE_EA和IE_TM2中断,都已打开时，使用外部代码；
	  IE_EA或IE_TM2中断,没打开时，使用maskrom中代码;
	*/
	if((IE0 & BIT(7)) && (IE0 & BIT(3)))
	{
		sys_ctl.delay_cnt = cnt + 1;
		while(sys_ctl.delay_cnt);
	}
	else
	{
		delay_m(cnt);
	}
}

u8 t_pwn_duty1, t_pwn_duty2;	// Duty range from 0x00 to 0xFF

#pragma optimize=speed high
#pragma vector = VECTOR_OFFSET(TIMER3_INT)
//#pragma register_bank=3
#pragma location="INTERRUPT"
__interrupt void timer3_isr(void)		// 122us
{
	ISR_ENTER();
	TMR3CON &= ~BIT(7);    //clear pending

	static __data u8 PWM_ACC_0, PWM_ACC_1;

	// LED 0
	PWM_ACC_0 += t_pwn_duty1;   //PWM_Duty_R;	// PWM_ACC_R = PWM_ACC_R + PWM_Duty_R; CY is Carry
	MOV_Carry_To_LED_O();
	//asm("MOV	P4.5 , C");

	// LED 1
	PWM_ACC_1 += t_pwn_duty2;  //PWM_Duty_G;	// PWM_ACC_R = PWM_ACC_R + PWM_Duty_R; CY is Carry
	MOV_Carry_To_LED_T();
	//asm("MOV	P4.5 , C");



	ISR_EXIT();

	// move to here for speed up
	//asm("POP PSW");
	//asm("POP A");
	//asm("RETI");
}

void no_used_code(void)
{
	LED0_PORT |= BIT(LED0_PIN);
	LED1_PORT |= BIT(LED1_PIN);
}


/*----------------------------------------------------------------------------*/
/**@brief  timer2中断
   @param  无
   @return 无
   @note   1ms中断
*/
/*----------------------------------------------------------------------------*/
#pragma vector = VECTOR_OFFSET(TIMER2_INT)
#pragma register_bank=2
#pragma location="INTERRUPT"
__interrupt void timer2_isr(void)
{
	static u8 count_1s = 0;
	ISR_ENTER();
	TMR2CON1 &= ~BIT(7);    //clear pending

	static u8 __data counter_5ms, counter_1ms, counter_1s, counter_1min;
	static u8 WDT_time;		// count by 10mSec, to make 1.5Sec for f_Clear_WDT
//	static u16 bat_disp_cnt;			// 低压报警显示计数 20180327
//	static u16 char_disp_cnt;		// 电池充电显示计数	20180327

	if(scan_ctl.disp_volume_flag == true)
	{
		disp_vol();
	}else if(scan_ctl.disp_timer_flag == true)
	{
		disp_timer();
	}else if(scan_ctl.disp_song_flag == true)
	{
		disp_song();	
	}else{
		disp_clock();						// 时钟显示
	}
	
#if Emcode_Enable
	KEY_Whire1();		// 编码开关的分析
#endif

#if LED_7P7S_SCAN_HALF_MS
	static u8 __data counter_half_ms;
	static u8 __data counter_half_half_ms;
	counter_half_half_ms++;
	disp_scan();
	if((counter_half_half_ms%4) == 0)
	{
		counter_half_ms++;
		counter_half_half_ms = 0;
	}
	else
	{
		goto timer2_exit;
	}
	if((counter_half_ms%2) == 0)
	{
		counter_1ms++;
		counter_half_ms = 0;
	}
	else
	{
		goto timer2_exit;
	}
#else
	counter_1ms++;
	disp_scan();
#endif

	if(sys_ctl.delay_cnt > 0)
	{
		sys_ctl.delay_cnt--;
	}

	if((counter_1ms%3) == 0)
	{

#if DAC_POWER_SPEED
		if(dac_init_ctl.sta < DAC_INIT_END)
		{
			dac_init_process();
		}
#endif
	}
#if DAC_FADE_EN
	if((counter_1ms & 0x01) == 0)
	{
		dac_fade_auto();
		//dac_fade_auto();
	}
#endif

	/*5ms  定时****************************************************/
	if((counter_1ms%5) == 0)
	{
		//scan_ctl.disp_scan_flag = 1;
		if(sys.delay_cnt > 0)
		{
			sys.delay_cnt--;
		}
#if ( ((DISP_TYPE != DISP_LED)&&(DISP_TYPE != DISP_NO)) || ((DISP_TYPE == DISP_LED)&&(USE_IR_NUM)) )
		ocx_delay();
#endif
		//sd_detect();        //sd检测, 外接SD卡检测
#if SD_DOUBLE_EN
		sd1_detect();       //sd1检测, 内接SD卡检测
#endif
		//usb_detect();       //usb检测
#if (AUX_DET_EN && !USE_7P7S_LED_AUX_MUX)
		aux_detect();       //lienin检测
#endif
#if MIC_DET_EN
		mic_detect();       //mic检测
#endif
#if HP_DET_EN
		headphone_detect(); //headphone检测
#endif
#if DCIN_DET_EN
		dcin_detect();
#endif
		key_scan();         // 按键扫描
		
#if Emcode_Enable
	KEY_Whire2();		// 编码开关的分析
#endif

#if 0// DAC_FADE_EN
		dac_fade_auto();	// 由 5mSec 改至 2mSec
#endif
		mute_ctl_auto();    //5ms中断里控制MUTE
#if KEY_VOICE_EN
		user_chk_kv();
#endif

		counter_5ms++;
		/*10ms 定时****************************************************/
		if((counter_5ms%2) == 0x00)
		{
			counter_1ms = 0;

            #if USB_HID_SUPPORT
            			usb_hid_delay();
            			if(sys_ctl.hid_vol_delay_cnt > 0)
            			{
            				sys_ctl.hid_vol_delay_cnt--;
            			}
            #endif
            #if USB_MASS_SUPPORT
            			usb_mass_delay();
            #endif
            #if ALARM_EN
            			t_alarm.delay_10ms_cnt++;
            #endif
            #if TASK_FM_EN
            			fm_init_process();
            #endif
            			mem_time_set();
            if(++WDT_time > 150)
			{	// 1500mSec to Make Clear WDT flag
				WDT_time = 0;
				f_Clear_WDT = true;
			}
		}
//#endif
		/*0.5s   定时****************************************************/
		if((counter_5ms%100)==0)
		{
			if(disp.display_dp_flag == 1) 				// 时钟显示时冒号闪烁
			{
				disp.display_dp_flag = 0;
			}else
			{
				disp.display_dp_flag = 1;
			}
			
			if(scan_ctl.disp_clock_set_flag == 1)		// 设置时钟时数码管闪烁  
			{
				if(disp.display_led_flag == 1)
				{
					disp.display_led_flag = 0;
				}else
				{
					disp.display_led_flag = 1;
				}
			}
		}

		/*1s   定时****************************************************/
		if(counter_5ms == 200)
		{
			counter_5ms = 0;//此处必须清零，重新循环
		//	printf("b_msc.pause=%d\n",b_msc.pause);
					
			if(vol_less_33V_flag == 1)
			{
				count_1s++;
				if(count_1s == 4)//3.1v定时4s后关机
				{
					vol_less_power_warning_flag = 1;
					count_1s=0;
				}	
			}
			else
			{
				count_1s =0;
			}

			if((sys_ctl.bat_charger_cnt != 0xFF)&&(sys_ctl.bat_charger_cnt != 0x00))
			{
				sys_ctl.bat_charger_cnt--;
			}


			if((sys_ctl.alarm_cnt != 0xFF)&&(sys_ctl.alarm_cnt != 0x00))
			{
				sys_ctl.alarm_cnt--;
			}
			//printf("%d_%d ",sys_ctl.bat_charger_sta,sys_ctl.bat_charger_cnt);
			//printf("%d_",sys_ctl.alarm_cnt);

			if(scan_ctl.disp_cnt > 0)
			{
				scan_ctl.disp_cnt--;
			}
			if(scan_ctl.disp_cnt == 0)
			{
				scan_ctl.disp_volume_flag = false;
				scan_ctl.disp_timer_flag = false;
				scan_ctl.disp_song_flag = false;
				scan_ctl.disp_cnt = 0;
			}

			//printf("%d_",scan_ctl.disp_cnt);
				
            #if ALARM_EN
            			t_alarm.delay_1s_cnt++;
            #endif
            #if USE_TOUCH_KEY
			
            			touch_key_rest();
            #endif
			    			
        	counter_1s++;
            /*1min   定时****************************************************/
		    if((counter_1s%60) == 0x00)
	        {
				counter_1min++;
				if(ocx.timel < 59)
				{
					ocx.timel += 1;
				}else
				{
					ocx.timel = 0;
					if(ocx.timeh < 23){
						ocx.timeh += 1;
					}else{
						ocx.timeh = 0;
					}
				}
				//printf("1min\n"); 

                counter_1s = 0;  //清零

				if(scan_ctl.timer_flag != 0)            //睡眠仪 产品模式:15min 30min 60min定时 测试模式:1min 2min 3min
				{
					scan_ctl.count_1min++;
					if((scan_ctl.timer_flag == 1) && (scan_ctl.count_1min == TIMER_15))
					//if((scan_ctl.timer_flag == 1) && (scan_ctl.count_1min == 1))
					{
						Timer1_flag = true;
					}else if((scan_ctl.timer_flag == 2) && (scan_ctl.count_1min == TIMER_30))
					{
						Timer1_flag = true;
					}else if((scan_ctl.timer_flag == 3) && (scan_ctl.count_1min == TIMER_60))
					{
						Timer1_flag = true;
					}else if((scan_ctl.timer_flag == 4) && (scan_ctl.count_1min == TIMER_7h))
					{
						Timer1_flag = true;
					}
					printf("min: %d\n", scan_ctl.count_1min);
					counter_1s = 0;
				}

				

	        }
            
			
        }
	}
#if LED_7P7S_SCAN_HALF_MS
timer2_exit:
#endif

	ISR_EXIT();
}

/*----------------------------------------------------------------------------*/
/**@brief 主流程
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="SYS_INIT"
void main(void)
{
	/*
	【注意】为避免新定义的变量在初始化操作前被使用而产生问题，
	系统启动后，已将data区0x1308-0x13FF的248byte(除R0-R7的8byte),
	和xdata区的0x0000-0x0FFF的4096byte全部清0。
	*/
	WATCHDOG_CLR();                 //系统上电后默认打开Watchdog，定时2048ms。
	system_init();                  //系统初始化
	run_task();                     //运行任务
	while(1);
}
