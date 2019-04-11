/*****************************************************************************
 * Module    : Task
 * File      : task_fm.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 收音机任务流程
 *****************************************************************************/
#include "include.h"
#include "fm_fade.h"

#if TASK_FM_EN

IAR_XDATA_A type_fm_ctl fm_ctl;


//根据ch频道获取FM频点
#pragma location="TASK_FM_SEG"
static void fm_get_freq(void)
{
	select_irom2();
	fm_ctl.fm_freq = fm_get_diff(fm_ctl.ch_cur, fm_ctl.ch_buf, CH_BUF_MAX);
	restore_mem();

#if FM_SEEK_50K_STEP
	if(fm_ctl.fm_freq <= ((FM_MAX_FREQ - FM_MIN_FREQ)/5))
	{
		fm_ctl.fm_freq = fm_ctl.fm_freq*5+FM_MIN_FREQ;
#else
	if(fm_ctl.fm_freq <= (FM_MAX_FREQ - FM_MIN_FREQ))
	{
		fm_ctl.fm_freq += FM_MIN_FREQ;
#endif
	}
	else
	{
		fm_ctl.ch_total = 1;
		fm_ctl.ch_cur = 1;
		fm_ctl.ch_buf[0] = 0x01;
		fm_ctl.fm_freq = FM_MIN_FREQ;
	}
}


//FM任务初始化
#pragma location="TASK_FM_SEG"
static void task_fm_enter(void)
{
	if(b_sys.modeswitch_flag)
	{
		b_sys.modeswitch_flag = 0;
		delay_5ms(100); //这里延迟500MS，防止卡检测不在线了，但卡没有移走（即CMD与DATA没有断开）
	}

	SPMODE &= ~0x0F;

#if ((FM_CLOCK == USE_IO_32KHZ_CLK) || (FM_CLOCK == USE_IO_12MHZ_CLK))
	io_output_clk_start();
#elif ((FM_CLOCK == USE_SD_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
	sdclk_output_clk_start();
#endif

#if ((FM_CLOCK == USE_SD_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK) || FM_CLK_IO_MUX_SDCLK)
	fm_ctl.fm_mode_flag = 1;
#endif

#if AMUX_FM_EN
#if((defined AX107X_TAG) && (AMUX_FM == AMUX_CH_MM))
	mic_enable();
	DACACON6 = (DACACON6 & 0x83) | (GAIN_0DB <<2);
#else

#if((defined AX107X_TAG)\
    && ((AMUX_FM == AMUX_CH_0M)||(AMUX_FM == AMUX_CH_2M)||(AMUX_FM == AMUX_CH_4M)))
	DACACON6 |= BIT(0);//打开MIC用作LINE功能
#endif

	amux_init(AMUX_FM);                   //初始化FM的AMUX
#if ENERGY_DISP_EN
	energy_init();
#endif
#endif  //((defined AX107X_TAG) && (AMUX_FM == AMUX_CH_MM))
#endif

	fm_rx_init();
	delay_5ms(5);
	fm_rx_set_freq(fm_ctl.fm_freq);
	mute_disable();
#if AMUX_FM_WITH_NO_CAP
	user_change_volume(VOLUME_MAX);
	dac_trim_process();
#endif
#if AMUX_FM_EN
#if AMUX_FM_WITH_NO_CAP
	fm_rx_set_vol(sys_ctl.volume);
#else
	fm_rx_set_vol(VOLUME_MAX);            //FM音量设置最大
#endif
#if DAC_FADE_EN
#if KEY_VOICE_EN
	dac_fade_in(1);
#else
	dac_fade_in(0);
#endif
#if AMUX_FM_WITH_NO_CAP
	delay_5ms(60);
	trim_B_to_A();
#endif
#else
#if KEY_VOICE_EN
	b_dac.digvol_en = 1;
#else
	b_dac.digvol_en = 0;
#endif
	dac_set_volume_direct(sys_ctl.volume);
#endif
#if AMUX_FM_WITH_NO_CAP
	DACACON4 |= BIT(5);
	delay_5ms(2);
#endif
#if (!KEY_VOICE_EN)
	DAC_ANL_DIS();
#endif
#else   //AMUX_FM_EN
#if (!KEY_VOICE_EN)
	DACACON2 &= ~BIT(0);                  //disable dac hp
	DAC_ANL_DIS();                        //disable dac
#endif
	fm_rx_set_vol(sys_ctl.volume);
#endif

#if (DISP_TYPE==DISP_LED)
	ocx.led_sta = LED_STA_ON;
#endif
}

//FM任务退出
#pragma location="TASK_FM_SEG"
static void task_fm_exit(void)
{
#if AMUX_FM_EN
#if DAC_FADE_EN
#if AMUX_FM_WITH_NO_CAP
	fm_dac_fade_out();
#else
	dac_fade_out();
	dac_fade_wait();
#endif
#endif
#if AMUX_FM_WITH_NO_CAP
	delay_5ms(2);
	trim_B_to_A();
	dac_ctl.anl_vol = tbl_anlvol[sys_ctl.volume];   //设置当前模拟音量级数
#endif
#if (!KEY_VOICE_EN)
	DAC_ANL_EN();
#endif
#if ENERGY_DISP_EN
	energy_stop();
#endif

#if((defined AX107X_TAG) && (AMUX_LINEIN == AMUX_CH_MM))
	mic_disable();
#else
#if((defined AX107X_TAG) && ((AMUX_LINEIN == AMUX_CH_0M)||(AMUX_LINEIN == AMUX_CH_2M)||(AMUX_LINEIN == AMUX_CH_4M)))
	DACACON6 &= ~BIT(0);//关闭MIC用作LINE功能
#endif
	amux_exit();
#endif

#else  //AMUX_FM_EN
#if (!KEY_VOICE_EN)
	DACACON2 |= BIT(0);                  //enable dac hp
	DAC_ANL_EN();
#endif
	dac_ctl.anl_vol = tbl_anlvol[sys_ctl.volume];   //设置当前模拟音量级数
#endif /*END AMUX_FM_EN*/

#if ((FM_CLOCK == USE_IO_32KHZ_CLK) || (FM_CLOCK == USE_IO_12MHZ_CLK))
	io_output_clk_stop();
#elif ((FM_CLOCK == USE_SD_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
	sdclk_output_clk_stop();
	fm_ctl.fm_mode_flag = 0;
#endif

	fm_rx_off();
#if ((FM_CLOCK == USE_SD_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
	SDCON0 &= ~BIT(1);                 //disable clk keeping out
#endif

#if FM_CLK_IO_MUX_SDCLK
	printf("clk_io_mux_sdclk\n");
	if(device_is_online(DEVICE_SDMMC))
	{
		SDCON0 = 0x01;
	}
	sd_port_init(0);
	fm_ctl.fm_mode_flag = 0;
#endif

	SPMODE |= 0x0F;
}

//FM任务事件
#pragma location="TASK_FM_SEG"
static void task_fm_event(void)
{
	comm_event(2);                          //U盘、SD卡插入，不激活

#if USE_IR_NUM
	if(ocx.event == 1)
	{
		ocx.event = 0;
		if((ocx.number > 0) && (ocx.number <= fm_ctl.ch_total))
		{
			fm_ctl.ch_cur = ocx.number;
			fm_get_freq();
			fm_rx_set_freq(fm_ctl.fm_freq);
			mem_write_cur_channel();
		}
#if FM_SEEK_50K_STEP
		else if((ocx.number >= FM_MIN_FREQ/10) && (ocx.number <= FM_MAX_FREQ/10))
		{
			fm_ctl.fm_freq = ocx.number;
			fm_rx_set_freq(fm_ctl.fm_freq*10);
		}
#else
		else if((ocx.number >= FM_MIN_FREQ) && (ocx.number <= FM_MAX_FREQ))
		{
			fm_ctl.fm_freq = ocx.number;
			fm_rx_set_freq(fm_ctl.fm_freq);
			mem_write_cur_channel();
		}
#endif

	}
#endif
}

//FM自动搜台
#pragma location="TASK_FM_SEG"
static void fm_scan(void)
{
	u8 seek_sta;

	fm_ctl.ch_total = 0;
	fm_ctl.ch_cur = 0;
	fm_ctl.fm_freq = FM_MIN_FREQ;             //从87.5开始搜台
#if (SYS_MEM == MEM_INTERNAL)
	mem_ctl.prev_ch_number = 0;
#endif
#if AMUX_FM_EN
#if DAC_FADE_EN
#if AMUX_FM_WITH_NO_CAP
	fm_dac_fade_out();
#else
	dac_fade_out();
	dac_fade_wait();
#endif
#endif
#endif
	fm_rx_set_vol(0);
	mem_fmbuf_clr();
#if (DISP_TYPE==DISP_LED)
	ocx.led_sta = LED_STA_FAST_GLITTER;
#endif

	//printf("fm_scan: %d\n", fm_ctl.fm_freq);
	while(1)
	{
		//printf(".");
		WATCHDOG_CLR();
		seek_sta = fm_rx_seek(fm_ctl.fm_freq);
		if(seek_sta > 0)        //收到一个台
		{
#if ((FM_SEEK_50K_STEP) && (FM_RTC6218_EN))
			if((fm_ctl.fm_type == FM_RTC6218) && (seek_sta == 2))
			{
				fm_ctl.fm_freq += 5;         //RTC6218返回2代表+50KHz的频点
			}
#endif
			fm_rx_set_freq(fm_ctl.fm_freq);
#if AMUX_FM_EN
#if AMUX_FM_WITH_NO_CAP
			fm_rx_set_vol(sys_ctl.volume);
#else
			fm_rx_set_vol(VOLUME_MAX);
#endif
#if DAC_FADE_EN
#if AMUX_FM_WITH_NO_CAP
			fm_dac_fade_in();
#else
			dac_fade_in(0);
#endif
#endif
#else
			fm_rx_set_vol(sys_ctl.volume);
#endif

			fm_ctl.ch_cur++;
			fm_ctl.ch_total++;

			showch(fm_ctl.ch_cur);
			//printf("\nch%d, freq=%d\n", fm_ctl.ch_cur, fm_ctl.fm_freq);

			mem_write_fm();     //保存频道
			delay_5ms(200);             //放一段時間
#if AMUX_FM_EN
#if DAC_FADE_EN
#if AMUX_FM_WITH_NO_CAP
			fm_dac_fade_out();
#else
			dac_fade_out();
			dac_fade_wait();
#endif
#endif
#endif
			fm_rx_set_vol(0);
		}
#if FM_SEEK_50K_STEP
		fm_ctl.fm_freq += 5;
#else
		fm_ctl.fm_freq++;
#endif
		if(fm_ctl.fm_freq > FM_MAX_FREQ)
		{
			break;
		}

		task_fm_display();

		if(get_msg() == KU_PLAY)
		{
			goto _exit_fm_scan;
		}

#if FM_CLK_IO_MUX_SDCLK //复用SD_CLK的IO输出时钟给FM
		if((b_sys.dev_change) && device_need_activate(DEVICE_SDMMC))    //SD卡插入不激活
		{
			task_ctl.work_sta = TASK_MUSIC;
		}
		else
#endif
		{
			comm_event(2);                          //调用公共事件
		}

		if(task_ctl.work_sta != TASK_FM)
		{
#if ((FM_CLOCK == USE_SD_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
			SDBAUD = 182;
			goto _exit_fm_scan_1;
#else
			goto _exit_fm_scan;
#endif
		}
	}

_exit_fm_scan:

	//选取第一个频道开始播放
	//printf("\nscan_end: %d\n", fm_ctl.fm_freq);

	mem_write_fm();         //保存频道

	if(!fm_ctl.ch_total)
	{
		fm_ctl.ch_total = 1;    //未搜到台时将总台数设为1
	}
	fm_ctl.ch_cur = 1;
	fm_get_freq();

#if (DISP_TYPE==DISP_LED)
	ocx.led_sta = LED_STA_ON;
#endif
	showch(fm_ctl.ch_cur);
	//printf("sel_ch%d, freq=%d total_ch=%d\n", fm_ctl.ch_cur, fm_ctl.fm_freq, fm_ctl.ch_total);

	fm_rx_set_freq(fm_ctl.fm_freq);

#if ((FM_CLOCK == USE_SD_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
_exit_fm_scan_1:
#endif

#if AMUX_FM_EN
#if AMUX_FM_WITH_NO_CAP
	fm_rx_set_vol(sys_ctl.volume);
#else
	fm_rx_set_vol(VOLUME_MAX);
#endif
#if DAC_FADE_EN
#if AMUX_FM_WITH_NO_CAP
	fm_dac_fade_in();
#else
	dac_fade_in(0);
#endif
#endif
#else
	fm_rx_set_vol(sys_ctl.volume);
#endif
	mem_write_channel();      //保存总台数、当前电台序号、当前频率
}

//FM任务消息处理
#pragma location="TASK_FM_SEG"
static void task_fm_deal_msg(u8 msg)
{
#if FM_SEMIAUTOMATIC_SEEK
	bool seek_flag = 0;
	u16 seek_count = 0;
	u8 seek_sta;
#endif

	switch(msg)
	{
		//自动搜台
	case KU_PLAY:
	case KU_PLAY_AND_MODE:
		fm_scan();
		break;

		//下一个频道、上一个频道
	case KU_NEXT:
	case KU_PLUS:
		fm_ctl.ch_cur += 2;
	case KU_PREV:
	case KU_MINUS:
		fm_ctl.ch_cur--;
		if(fm_ctl.ch_cur > fm_ctl.ch_total)    //范围:1--fm_ctl.ch_total
		{
			fm_ctl.ch_cur = 1;
		}
		else if(fm_ctl.ch_cur == 0)
		{
			fm_ctl.ch_cur = fm_ctl.ch_total;
		}
		fm_get_freq();
#if AMUX_FM_EN
#if DAC_FADE_EN
#if AMUX_FM_WITH_NO_CAP
		fm_dac_fade_out();
#else
		dac_fade_out();
		dac_fade_wait();
#endif
#endif
#endif
		fm_rx_set_vol(0);
		fm_rx_set_freq(fm_ctl.fm_freq);
#if AMUX_FM_EN
#if AMUX_FM_WITH_NO_CAP
		fm_rx_set_vol(sys_ctl.volume);
#else
		fm_rx_set_vol(VOLUME_MAX);
#endif
#if DAC_FADE_EN
#if AMUX_FM_WITH_NO_CAP
		fm_dac_fade_in();
#else
		dac_fade_in(0);
#endif
#endif
#else
		fm_rx_set_vol(sys_ctl.volume);
#endif
		showch(fm_ctl.ch_cur);
		//printf("\nch%d, freq=%d\n", fm_ctl.ch_cur, fm_ctl.fm_freq);
		mem_write_cur_channel();        //保存当前电台序号和频率
		break;

#if FM_SEMIAUTOMATIC_SEEK
	case KL_NEXT:
		seek_flag = 1;
	case KL_PREV:
#if AMUX_FM_EN
#if DAC_FADE_EN
#if AMUX_FM_WITH_NO_CAP
		fm_dac_fade_out();
#else
		dac_fade_out();
		dac_fade_wait();
#endif
#endif
#endif
		fm_rx_set_vol(0);
#if (DISP_TYPE==DISP_LED)
		ocx.led_sta = LED_STA_FAST_GLITTER;
#endif
		while(1)
		{
			WATCHDOG_CLR();
#if FM_SEEK_50K_STEP
			if(seek_flag)
			{
				fm_ctl.fm_freq += 5;
			}
			else
			{
#if FM_RTC6218_EN
				if((fm_ctl.fm_type == FM_RTC6218))
				{
					fm_ctl.fm_freq -= 10;
				}
				else
#endif
				{
					fm_ctl.fm_freq -= 5;
				}
			}
#else
			if(seek_flag)
			{
				fm_ctl.fm_freq++;
			}
			else
			{
				fm_ctl.fm_freq--;
			}
#endif
			if(fm_ctl.fm_freq > FM_MAX_FREQ)
			{
				fm_ctl.fm_freq = FM_MIN_FREQ;
			}
			else if(fm_ctl.fm_freq < FM_MIN_FREQ)
			{
				fm_ctl.fm_freq = FM_MAX_FREQ;
			}

			task_fm_display();

			seek_sta = fm_rx_seek(fm_ctl.fm_freq);
			if(seek_sta > 0)
			{
#if ((FM_SEEK_50K_STEP) && (FM_RTC6218_EN))
				if((fm_ctl.fm_type == FM_RTC6218) && (seek_sta == 2))
				{
					fm_ctl.fm_freq += 5;         //RTC6218返回2代表+50KHz的频点
				}
#endif
				break;
			}
			seek_count++;
			if(seek_count == (FM_MAX_FREQ - FM_MIN_FREQ))
			{
				break;
			}
		}
#if AMUX_FM_EN
#if AMUX_FM_WITH_NO_CAP
		fm_rx_set_vol(sys_ctl.volume);
#else
		fm_rx_set_vol(VOLUME_MAX);
#endif
#if DAC_FADE_EN
#if AMUX_FM_WITH_NO_CAP
		fm_dac_fade_in();
#else
		dac_fade_in(0);
#endif
#endif
#else
		fm_rx_set_vol(sys_ctl.volume);
#endif
#if (DISP_TYPE==DISP_LED)
		ocx.led_sta = LED_STA_ON;
#endif
		mem_write_cur_channel();
		break;

#endif

#if USE_IR_NUM
	case T_KEY_NUM_0:
	case T_KEY_NUM_1:
	case T_KEY_NUM_2:
	case T_KEY_NUM_3:
	case T_KEY_NUM_4:
	case T_KEY_NUM_5:
	case T_KEY_NUM_6:
	case T_KEY_NUM_7:
	case T_KEY_NUM_8:
	case T_KEY_NUM_9:
		shownumsel(msg - T_KEY_NUM_0);
		break;
#endif
#if ENERGY_DISP_EN
	case KL_PLAYMODE:
		if(!ocx.disp_energy)
		{
			energy_start();
		}
		else
		{
			energy_close();
		}
		disp.update = 1;
		break;
#endif

#if (!AMUX_FM_EN)||(AMUX_FM_WITH_NO_CAP)
	case KU_VOL_DOWN:
	case KH_VOL_DOWN:
	case KH_MINUS:
		user_set_fm_volume(sys_ctl.volume - 1);
		showvol();
		break;
	case KU_VOL_UP:
	case KH_VOL_UP:
	case KH_PLUS:
		user_set_fm_volume(sys_ctl.volume + 1);
		showvol();
		break;
#endif

	default:
		deal_msg(msg);
		break;
	}
}

//FM任务主流程
#pragma location="TASK_FM_SEG"
void task_fm(void)
{
	printf("task_fm\n");

	if(fm_ctl.fm_type == FM_NO)
	{
		printf("no fm module\n");
		task_ctl.work_sta = TASK_EXIT;
		return;
	}

	task_fm_enter();
	while(task_ctl.work_sta == TASK_FM)
	{
		task_fm_event();
		task_fm_deal_msg(get_msg());
		task_fm_display();
	}
	task_fm_exit();
}
#endif

