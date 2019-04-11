/*****************************************************************************
 * Module    : MAIN
 * File      : main.c
 * Author    : jingfa
 * Email     :
 * Function  : ������
 *****************************************************************************/
#include "include.h"
#include "user_io.h"
#include "user_key.h"


extern u8 SPI_wait_timeout;
extern u8 vol_less_power_warning_flag;//�͵�ѹ��ʾ��ػ���־
extern u8 vol_less_33V_flag;         //��ѹ3.3v��־
extern u8 vol_less_34V_flag;         //��ѹ3.4v��־
extern bool vol_less_34V_blink; //����3.3v����
extern bool Timer1_flag;

extern u8 power_key_cnt;
extern u8 count_led_off;
u8 vcmbuf_flag = 1;
#define TIMER_15	15
#define TIMER_30	30
#define TIMER_60	60
#define TIMER_7h	420

//1ms��ʱ����ʱ
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

	/*IE_EA��IE_TM2�ж�,���Ѵ�ʱ��ʹ���ⲿ���룻
	  IE_EA��IE_TM2�ж�,û��ʱ��ʹ��maskrom�д���;
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
/**@brief  timer2�ж�
   @param  ��
   @return ��
   @note   1ms�ж�
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
//	static u16 bat_disp_cnt;			// ��ѹ������ʾ���� 20180327
//	static u16 char_disp_cnt;		// ��س����ʾ����	20180327

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
		disp_clock();						// ʱ����ʾ
	}
	
#if Emcode_Enable
	KEY_Whire1();		// ���뿪�صķ���
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

	/*5ms  ��ʱ****************************************************/
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
		//sd_detect();        //sd���, ���SD�����
#if SD_DOUBLE_EN
		sd1_detect();       //sd1���, �ڽ�SD�����
#endif
		//usb_detect();       //usb���
#if (AUX_DET_EN && !USE_7P7S_LED_AUX_MUX)
		aux_detect();       //lienin���
#endif
#if MIC_DET_EN
		mic_detect();       //mic���
#endif
#if HP_DET_EN
		headphone_detect(); //headphone���
#endif
#if DCIN_DET_EN
		dcin_detect();
#endif
		key_scan();         // ����ɨ��
		
#if Emcode_Enable
	KEY_Whire2();		// ���뿪�صķ���
#endif

#if 0// DAC_FADE_EN
		dac_fade_auto();	// �� 5mSec ���� 2mSec
#endif
		mute_ctl_auto();    //5ms�ж������MUTE
#if KEY_VOICE_EN
		user_chk_kv();
#endif

		counter_5ms++;
		/*10ms ��ʱ****************************************************/
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
		/*0.5s   ��ʱ****************************************************/
		if((counter_5ms%100)==0)
		{
			if(disp.display_dp_flag == 1) 				// ʱ����ʾʱð����˸
			{
				disp.display_dp_flag = 0;
			}else
			{
				disp.display_dp_flag = 1;
			}
			
			if(scan_ctl.disp_clock_set_flag == 1)		// ����ʱ��ʱ�������˸  
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

		/*1s   ��ʱ****************************************************/
		if(counter_5ms == 200)
		{
			counter_5ms = 0;//�˴��������㣬����ѭ��
		//	printf("b_msc.pause=%d\n",b_msc.pause);
					
			if(vol_less_33V_flag == 1)
			{
				count_1s++;
				if(count_1s == 4)//3.1v��ʱ4s��ػ�
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
            /*1min   ��ʱ****************************************************/
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

                counter_1s = 0;  //����

				if(scan_ctl.timer_flag != 0)            //˯���� ��Ʒģʽ:15min 30min 60min��ʱ ����ģʽ:1min 2min 3min
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
/**@brief ������
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="SYS_INIT"
void main(void)
{
	/*
	��ע�⡿Ϊ�����¶���ı����ڳ�ʼ������ǰ��ʹ�ö��������⣬
	ϵͳ�������ѽ�data��0x1308-0x13FF��248byte(��R0-R7��8byte),
	��xdata����0x0000-0x0FFF��4096byteȫ����0��
	*/
	WATCHDOG_CLR();                 //ϵͳ�ϵ��Ĭ�ϴ�Watchdog����ʱ2048ms��
	system_init();                  //ϵͳ��ʼ��
	run_task();                     //��������
	while(1);
}
