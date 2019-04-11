/*****************************************************************************
* Module    : Task
* File      : task.c
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : ���������
*****************************************************************************/
#include "include.h"
#include "user_key.h"
u8 device_chg = 0;
#define device_music	0
#define device_flash	1

bool Timer1_flag = false;   //���ö�ʱ��־λ

u8 Timer_flag = 0;

bool mute_status_change = false;
u16 main_loop_count;
bool f_Clear_WDT;       // 1: set by timer interrupt for clear WDT, 0: clear by main program
extern u8 t_pwn_duty1, t_pwn_duty2;   // Duty range from 0x00 to 0xFF

IAR_DATA_A type_task_ctl task_ctl;

//�������ԭĬ�ϵ�����
#pragma location="TASK_SEG"
static void task_clear(void)
{
#if ( ((DISP_TYPE != DISP_LED)&&(DISP_TYPE != DISP_NO)) || ((DISP_TYPE == DISP_LED)&&(USE_IR_NUM)) )
	ocx_clear();
#endif
#if DAC_FADE_EN
	dac_fade_wait();
#endif
	mute_enable();
	b_dac.dac_stop = 0;                 //ʹ�л�ģʽ���Լ��ٹػ��ܽ������
}

//ִ������
#pragma location="TASK_SEG"
void run_task(void)
{
#if TASK_TIPS_EN
	u8 fs_cur_dev_bak=DEVICE_INVALID;
#endif

	//printf("run_task\n");
	//task_ctl.work_sta = TASK_MUSIC;
    //task_ctl.work_sta = TASK_FLASH;

	while(1)
	{
		task_clear();
		if((task_ctl.work_sta < TASK_EXIT)
#if ALARM_EN
		        && (task_ctl.work_sta != TASK_ALARM)
#endif
#if TASK_POWEROFF_EN
		        && (task_ctl.work_sta != TASK_POWEROFF)
#endif
		  )
		{
			/*printf("sys_ctl.irtc_poweron: %d\n", sys_ctl.irtc_poweron);
			if(sys_ctl.irtc_poweron == 0)    //�����ǵ�һ���ϵ�ʱ�����ϵ�Դ����ػ�ģʽ
		    {
		       task_ctl.work_sta = TASK_POWEROFF;
		       sys_ctl.irtc_poweron = 1;
			   uart_putc('*');
			}*/
			task_ctl.last_work = task_ctl.work_sta;
		}

		//printf("Y:%d,%d,%d\n",u_spi.spi_online,fs_cur_dev(),t_fat.dev_new);
        //printf("work_sta0:%d\n",task_ctl.work_sta);
		switch(task_ctl.work_sta)
		{
/*
		case TASK_MUSIC:
			task_music();
			//printf("work_sta1:%d\n",task_ctl.work_sta);
			break;
*/ 
#if TASK_AUX_EN
		case TASK_AUX:
			task_aux();
			break;
#endif

#if ALARM_EN
		case TASK_ALARM:
			task_alarm();
			break;
#endif

#if TASK_SPEAKER_EN
		case TASK_SPEAKER:
			task_speaker();
			break;
#endif

#if TASK_RTC_EN
		case TASK_RTC:
			task_rtc();
			break;
#endif

#if TASK_FM_EN
		case TASK_FM:
			task_fm();
			break;
#endif

#if TASK_USBDEV_EN
		case TASK_USBDEV:
			task_usbdev();
			break;
#endif

#if TASK_FLASH_EN
		case TASK_FLASH:
			task_flash_music();
			break;
#endif

#if TASK_POWEROFF_EN
		case TASK_POWEROFF:
			task_poweroff();
			break;
#endif

#if TASK_LOWPOWER_EN
		case TASK_LOWPOWER:
			task_lowpower();
			break;
#endif

#if TASK_IDLE_EN
		case TASK_IDLE:
			task_idle();
			break;
#endif

#if TASK_FMAM_DETECT_EN
		case TASK_FMAM_DETECT:
			task_fmam_detect();
			break;
#endif

#if TASK_TIPS_EN
		case TASK_TIPS:
/*����OTP�������,ѡ������豸ȥ,
��Ϊ����OTP���ֵ�ʱ�����ֳ�ʼ�����Ե�ǰ�豸(USB)״̬��Щ����
���Բ���OTP������֮ǰ��ѡ��������豸ȥ,��������֮��ָ���ԭ�����豸״̬���ɣ�
�Է�ֹӰ�쵱ǰ�豸��һЩ��Ϣ(�缤��״̬��)��Ŀǰ��������һ���豸"DEVICE_OTP"*/
			sys_ctl.prv_volume = sys_ctl.volume;
			user_set_volume(VOLUME_MAX);
			fs_cur_dev_bak=fs_cur_dev();
			fs_sel(DEVICE_OTP);
			startmusic_play();
			fs_sel(fs_cur_dev_bak);
			task_ctl.work_sta=task_ctl.pre_sta;
			user_set_volume(sys_ctl.prv_volume);
			break;
#endif
			//case TASK_EXIT:
		default:                                              //�˳���Ч�Ĳ˵�
            //printf("...\n");
			task_exit();
			break;
		}
	}
}

//��飬�������豸
#pragma location="TASK_SEG"
u8 device_change(void)
{
	if(!b_sys.dev_change)
	{
		return DEVICE_IDLE;             //�豸����
	}

	b_sys.dev_change = 0;
	return device_change_check();
}

//�˳����񣬷��ص���������ȥ
#pragma location="TASK_SEG"
static void task_exit(void)
{
	//���û�����˵������л�����һ������
	switch(task_ctl.last_work)
	{
	case TASK_MUSIC:
#if TASK_FM_EN
		task_ctl.work_sta = TASK_FM;
		break;

	case TASK_FM:
#endif
#if TASK_SPEAKER_EN
		task_ctl.work_sta = TASK_SPEAKER;
		break;

	case TASK_SPEAKER:
#endif
#if TASK_AUX_EN
		task_ctl.work_sta = TASK_AUX;
		break;

	case TASK_AUX:
#endif
#if TASK_RTC_EN
		task_ctl.work_sta = TASK_RTC;
		break;

	case TASK_RTC:
#endif
#if TASK_USBDEV_EN
		task_ctl.work_sta = TASK_USBDEV;
		break;

	case TASK_USBDEV:
#endif
#if TASK_FLASH_EN
	#if 1
		if(u_spi.spi_online)  
		{  
			//uart_putc('B');
			task_ctl.work_sta = TASK_FLASH;}
		else
			task_ctl.work_sta = TASK_MUSIC;
	#else
		if((SD1_CHECK_PORT & BIT(SD1_CHECK_PIN)))		  //SD��������IO�ᱻ����
		{	// IO Ϊ High, �޲������� SD��
			spi_sd_mux_enter();
			spi_port_init();
			u_spi.spi_online = spi_flash_init();
			spi_sd_mux_exit();
			sd_port_init(1);
			if(u_spi.spi_online)
			{
				//uart_putc('A');
				task_ctl.work_sta = TASK_FLASH;}
			else
			{
				task_ctl.work_sta = TASK_MUSIC;
			}
		}
		else
		{	// IO Ϊ Low, �в������� SD��
			task_ctl.work_sta = TASK_MUSIC;
		}
	#endif
		break;

	case TASK_FLASH:
#endif
		task_ctl.work_sta = TASK_MUSIC;
		break;

	default:
#if TASK_IDLE_EN
		task_ctl.work_sta = TASK_MUSIC;         //�е�����ģʽ
#endif 
        //uart_putc('+');
		break;
	}
}

#pragma location="TASK_SEG"
void deal_vol_msg(void)
{
	showvol();
	//ʹ��VOL+,VOL-�ܽ������
//	if(b_dac.mute_en)
//	{
//		user_set_mute();
//	}
#if(TASK_AUX_EN || TASK_FM_EN)
	//����amux����
	amux_volume_set();
#endif
}

u8 power_key_cnt;

//��������Ĺ�����Ϣ������
#pragma location="TASK_SEG"
void deal_msg(u8 msg)
{
	
	switch(msg)
	{
#if Emcode_Enable
	case KL_TIMER:
		scan_ctl.disp_clock_set_flag = 1;
		scan_ctl.count_KU_TIMER = 0;
		//ocx.timeh = 0;
		//ocx.timel = 0;
	break;
	
	case KU_TIMER:
		scan_ctl.count_KU_TIMER++;
		if(scan_ctl.count_KU_TIMER >= 2)
		{
			scan_ctl.count_KU_TIMER = 0;
			scan_ctl.disp_clock_set_flag = 0;
			disp.display_led_flag = 0;
		}
	break;

	case KL_LIGHT:
		scan_ctl.disp_light_set_flag = 1;
		if(t_pwn_duty1 == 0x00)
		{
			t_pwn_duty1 = LED_LIGHT1;
			t_pwn_duty2 = LED_LIGHT1;
		}
	break;

	case KU_LIGHT:
		scan_ctl.disp_light_set_flag = 0;
	break;

	case KU_LED_OFF:
		t_pwn_duty1 = 0x00;
		t_pwn_duty2 = 0x00;
		scan_ctl.disp_light_set_flag = 0;
	break;
#endif

#if MUSIC_PLAYMODE
		case KU_PLAYMODE:
#if (PLAYMODE_NUM == PLAYMODE_FOUR)
		if(sys_ctl.mode_play < ITEM_LOOP_FOLDER)
		{
			sys_ctl.mode_play++;
		}
		else
		{
			sys_ctl.mode_play = 0;
		}

#else
		if(sys_ctl.mode_play == ITEM_LOOP_ALL)
		{
			sys_ctl.mode_play = ITEM_LOOP_SINGLE;
			printf("singal\n");
		}
		else
		{
			sys_ctl.mode_play = ITEM_LOOP_ALL;
			printf("loop\n");
		}
#endif
		irtc_write_ram(RTCRAM_PLAY_MODE, (u8 *)&sys_ctl.mode_play, 1);
		show_playmode();
#if TASK_TIPS_EN
		task_ctl.work_sta = TASK_TIPS;
#endif
		break;
#endif
#if 1	// 2015/09/08 Merlin ʹ��Volum���ƹ���,
		//�������������� (Prev)
	//case KL_PREV:	// ��Ϊ�����ӵ��� Merlin 2015/09/08
	case KH_PREV:	// ��Ϊ�����ӵ��� Merlin 2015/09/08
	case KH_PREV_VOL_DOWN:
			user_set_volume(sys_ctl.volume - 1);
			deal_vol_msg();
			if(sys_ctl.volume == 0)
				MUTE_EN();
		printf("v-\n");
		break;
	//case KL_NEXT:	// ��Ϊ���������� Merlin 2015/09/08
	case KH_NEXT:	// ��Ϊ���������� Merlin 2015/09/08
	case KH_NEXT_VOL_UP:
			user_set_volume(sys_ctl.volume + 1);
			deal_vol_msg();
			if(scan_ctl.hpdect_val < 200)	//����û�в���
			//if(HP_CHECK_PORT & BIT(HP_CHECK_PIN))	//����û�в���
			{
				if(sys_ctl.volume >= 1)
					UN_MUTE();
			}
		printf("v+\n");
		break;
#endif

		//��ģʽ�������л�
	case KU_MODE:
	case KL_MODE:
	case KL_PLAY_AND_MODE:
		task_ctl.work_sta = TASK_EXIT;
		break;

		//����
	case KU_MUTE:
		user_set_mute();
		break;

		//�ػ�
#if TASK_POWEROFF_EN
	case KL_POWER:
	case KLU_POWER:
		task_ctl.work_sta = TASK_POWEROFF;
		sys_ctl.user_power_down_flag = 1;
		break;
#endif

		//��ػ�
#if SOFT_POWEROFF_EN
	case KL_SOFT_POWER:
		//printf("--");
		task_ctl.work_sta = TASK_POWEROFF;
		sys_ctl.user_power_down_flag = 1;
		break;
#endif
		//500ms��Ϣ
#if ALARM_EN
	case QSYSTEM_500MS:
		task_alarm_check();
		break;
#endif
	}
}

#pragma location="TASK_SEG"
void device_check_stop(u8 dev_num)
{
	if(fs_cur_dev() == dev_num)
	{
		music_stop();
	}
}

//��������Ĺ����¼�����
#pragma location="TASK_SEG"
void comm_event(u8 enable)
{
    if(f_Clear_WDT)
    {
		f_Clear_WDT = false;
		WATCHDOG_CLR();
    }
	mem_event();
	
	//ocx.timeh = 0x0B;
	//ocx.timel = 0x3B;

    IE_EA = 0;                  // Disable interrupt
    main_loop_count++;
    IE_EA = 1;                  // Enable interrupt


	if(sys_ctl.alarm_cnt == 1)
	{	
		task_ctl.last_work = task_ctl.work_sta;
		task_ctl.work_sta = TASK_FLASH;
	}


#if TASK_USBDEV_EN
	if(device_need_activate(DEVICE_PC))      //PC���
	{
		if(device_activate(DEVICE_PC))
		{
			task_ctl.work_sta = TASK_USBDEV;
			return;
		}
	}
#endif

#if AUX_DET_EN
	if(device_need_activate(DEVICE_LINEIN))     //LineIn���
	{
		if(device_activate(DEVICE_LINEIN))
		{
			task_ctl.pre_sta = task_ctl.work_sta;
			task_ctl.work_sta = TASK_AUX;
			return;
		}
	}
#endif

#if MIC_DET_EN
	if(device_need_activate(DEVICE_MIC))     //Mic���
	{
		if(device_activate(DEVICE_MIC))
		{
#if KARAOKE_EN
			if(task_ctl.work_sta == TASK_MUSIC)
			{
				mic_enable();
				b_sys.karaoke_flag = 1;
				b_dac.sound_en = 0;                //��MIC��ʱ������̬����
				msc_ctl.mscfade_en = 0;            //���ֲ���ģʽ���л�����Ҫ���뵭��
#if DAC_FADE_EN
				dac_fade_in(1);                    //��������ͣ�����mic��Ҫ���µ���
#endif
			}
#else
			task_ctl.pre_sta = task_ctl.work_sta;
			task_ctl.work_sta = TASK_SPEAKER;
#endif
			return;
		}
	}
#if KARAOKE_EN
	if((!device_activate(DEVICE_MIC)) && (task_ctl.work_sta == TASK_MUSIC))
	{
		mic_disable();
		if(b_sys.karaoke_flag)
		{
			b_sys.karaoke_flag = 0;
			b_dac.sound_en = 1;
			msc_ctl.mscfade_en = 1;
		}
	}
#endif
#endif

#if HP_DET_EN
	if(device_need_activate(DEVICE_HP))     //HeadPhone���
	{
		if(device_activate(DEVICE_HP))
		{
		}
	}
#endif

	if(enable == 1)
	{
		//�м���ģʽ����Ҫ���������ɨ��
		u8 f_change = device_change();
		if(DEVICE_CHANGE == f_change)
		{
			task_ctl.work_sta = TASK_MUSIC;     //����״̬�£������豸���������ֲ���ģʽ
		}
		else if(DEVICE_REMOVE == f_change)
		{
			fs_sel_new();                       //�γ��豸���л�������һ��
		}
	}
	else if(enable == 2)
	{    //printf("ppp\n");
		if((b_sys.dev_change) &&
		        (device_need_activate(DEVICE_SDMMC)||device_need_activate(DEVICE_UDISK))||\
		        (device_need_activate(DEVICE_SDMMC1)
#if UDISK_DOUBLE_EN
		         || device_need_activate(DEVICE_UDISK1)
#endif
		        )
		  )
		{
			task_ctl.work_sta = TASK_MUSIC; 
		}
	}
}

