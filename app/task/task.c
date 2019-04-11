/*****************************************************************************
* Module    : Task
* File      : task.c
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : 任务控制器
*****************************************************************************/
#include "include.h"
#include "user_key.h"
u8 device_chg = 0;
#define device_music	0
#define device_flash	1

bool Timer1_flag = false;   //设置定时标志位

u8 Timer_flag = 0;

bool mute_status_change = false;
u16 main_loop_count;
bool f_Clear_WDT;       // 1: set by timer interrupt for clear WDT, 0: clear by main program
extern u8 t_pwn_duty1, t_pwn_duty2;   // Duty range from 0x00 to 0xFF

IAR_DATA_A type_task_ctl task_ctl;

//清除、还原默认的设置
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
	b_dac.dac_stop = 0;                 //使切换模式，以及假关机能解除静音
}

//执行任务
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
			if(sys_ctl.irtc_poweron == 0)    //当不是第一次上电时，插上电源进入关机模式
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
/*播放OTP里的音乐,选到别的设备去,
因为播放OTP音乐的时候，音乐初始化里会对当前设备(USB)状态做些处理，
所以播放OTP里音乐之前需选到另外的设备去,播放完了之后恢复到原来的设备状态即可，
以防止影响当前设备的一些信息(如激活状态等)，目前是新增了一个设备"DEVICE_OTP"*/
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
		default:                                              //退出无效的菜单
            //printf("...\n");
			task_exit();
			break;
		}
	}
}

//检查，并加载设备
#pragma location="TASK_SEG"
u8 device_change(void)
{
	if(!b_sys.dev_change)
	{
		return DEVICE_IDLE;             //设备正常
	}

	b_sys.dev_change = 0;
	return device_change_check();
}

//退出任务，返回到其它任务去
#pragma location="TASK_SEG"
static void task_exit(void)
{
	//如果没有主菜单，则切换到下一个任务
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
		if((SD1_CHECK_PORT & BIT(SD1_CHECK_PIN)))		  //SD卡插入检测IO会被拉低
		{	// IO 为 High, 无插入内置 SD卡
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
		{	// IO 为 Low, 有插入内置 SD卡
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
		task_ctl.work_sta = TASK_MUSIC;         //切到空闲模式
#endif 
        //uart_putc('+');
		break;
	}
}

#pragma location="TASK_SEG"
void deal_vol_msg(void)
{
	showvol();
	//使用VOL+,VOL-能解除静音
//	if(b_dac.mute_en)
//	{
//		user_set_mute();
//	}
#if(TASK_AUX_EN || TASK_FM_EN)
	//设置amux音量
	amux_volume_set();
#endif
}

u8 power_key_cnt;

//所有任务的公共消息处理函数
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
#if 1	// 2015/09/08 Merlin 使用Volum控制功能,
		//音量减、音量加 (Prev)
	//case KL_PREV:	// 改为音量加调整 Merlin 2015/09/08
	case KH_PREV:	// 改为音量加调整 Merlin 2015/09/08
	case KH_PREV_VOL_DOWN:
			user_set_volume(sys_ctl.volume - 1);
			deal_vol_msg();
			if(sys_ctl.volume == 0)
				MUTE_EN();
		printf("v-\n");
		break;
	//case KL_NEXT:	// 改为音量减调整 Merlin 2015/09/08
	case KH_NEXT:	// 改为音量减调整 Merlin 2015/09/08
	case KH_NEXT_VOL_UP:
			user_set_volume(sys_ctl.volume + 1);
			deal_vol_msg();
			if(scan_ctl.hpdect_val < 200)	//耳机没有插入
			//if(HP_CHECK_PORT & BIT(HP_CHECK_PIN))	//耳机没有插入
			{
				if(sys_ctl.volume >= 1)
					UN_MUTE();
			}
		printf("v+\n");
		break;
#endif

		//按模式键进行切换
	case KU_MODE:
	case KL_MODE:
	case KL_PLAY_AND_MODE:
		task_ctl.work_sta = TASK_EXIT;
		break;

		//静音
	case KU_MUTE:
		user_set_mute();
		break;

		//关机
#if TASK_POWEROFF_EN
	case KL_POWER:
	case KLU_POWER:
		task_ctl.work_sta = TASK_POWEROFF;
		sys_ctl.user_power_down_flag = 1;
		break;
#endif

		//软关机
#if SOFT_POWEROFF_EN
	case KL_SOFT_POWER:
		//printf("--");
		task_ctl.work_sta = TASK_POWEROFF;
		sys_ctl.user_power_down_flag = 1;
		break;
#endif
		//500ms消息
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

//所有任务的公共事件处理
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
	if(device_need_activate(DEVICE_PC))      //PC检测
	{
		if(device_activate(DEVICE_PC))
		{
			task_ctl.work_sta = TASK_USBDEV;
			return;
		}
	}
#endif

#if AUX_DET_EN
	if(device_need_activate(DEVICE_LINEIN))     //LineIn检测
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
	if(device_need_activate(DEVICE_MIC))     //Mic检测
	{
		if(device_activate(DEVICE_MIC))
		{
#if KARAOKE_EN
			if(task_ctl.work_sta == TASK_MUSIC)
			{
				mic_enable();
				b_sys.karaoke_flag = 1;
				b_dac.sound_en = 0;                //在MIC打开时不开动态降噪
				msc_ctl.mscfade_en = 0;            //音乐播放模式的切换不需要淡入淡出
#if DAC_FADE_EN
				dac_fade_in(1);                    //在音乐暂停后插入mic需要重新淡入
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
	if(device_need_activate(DEVICE_HP))     //HeadPhone检测
	{
		if(device_activate(DEVICE_HP))
		{
		}
	}
#endif

	if(enable == 1)
	{
		//有几个模式不需要在这里进行扫描
		u8 f_change = device_change();
		if(DEVICE_CHANGE == f_change)
		{
			task_ctl.work_sta = TASK_MUSIC;     //任意状态下，插入设备，进入音乐播放模式
		}
		else if(DEVICE_REMOVE == f_change)
		{
			fs_sel_new();                       //拔出设备，切换到另外一个
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

