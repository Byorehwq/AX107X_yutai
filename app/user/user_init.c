/*****************************************************************************
 * Module    : User
 * File      : user_init.c
 * Author    : jingfa
 * Email     :
 * Function  : 初始化流程
 *****************************************************************************/
#include "include.h"

u8 rtc_flag;

#ifdef UART_DEBUG

void __program_start(void);
void __reset_vector(void)
{
	__program_start();
}

#else //UART_DEBUG

#pragma constseg="CODE_TBL"
__root const u8 __code code_tbl[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#pragma constseg=default

#endif //UART_DEBUG

#define MSG_POOL_DEPTH      16          //消息池深度
IAR_PDATA_A u8 msg_pool[MSG_POOL_DEPTH] AT(MSG_POOL);


/*----------------------------------------------------------------------------*/
/**@brief 系统变量初始化
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma inline=forced
#pragma location="SYS_INIT"
void sysvar_init(void)
{
	libvar_init();
	dac_ctl_init();
#if DAC_POWER_SPEED
	dac_init_ctl.sta = DAC_INIT_IDLE;
#endif
	msg_init(msg_pool, MSG_POOL_DEPTH);
#if ((DISP_TYPE != DISP_LED) && (DISP_TYPE != DISP_NO))
	ocx_clear();
#endif

	b_dac.all = 0;
	b_sys.all = 0;
#if DAC_FADE_EN
	fade_ctl.en = 1;
#else
	fade_ctl.en = 0;
#endif
#if (BATTERY_CHECK || SOFT_POWEROFF_EN)
	sys_ctl.bat_sta = VOL_INIT;
#endif
	sys_ctl.eq_num = 0;
#if TASK_SPEAKER_EN
	sys_ctl.mix_step = ECHO_MAX;
#endif
	t_fat.dev_new = DEVICE_SDMMC1;//DEVICE_INVALID;
	t_fat.dev_sel = DEVICE_SDMMC1;//DEVICE_INVALID;
	task_ctl.last_work = 0;
#if ENERGY_DISP_EN
	ocx.disp_energy = 0;
	auadc_ctl.auadc_process = dummy_void;
#endif
	mem_ctl.delay_time = 0;
#if ((SYS_MEM == MEM_INTERNAL) && USE_BREAK_POINT)
	mem_ctl.erase_time = 0;
#endif
	t_msc.poweron = 0;
#if SOFT_POWEROFF_EN
	poweroff_ctl.power_key_sta = POWERKEY_INIT;
	poweroff_ctl.wait_powerkey_up = 0;
#if DCIN_DET_EN
	poweroff_ctl.dc_in_sta = DC_INIT;
#endif
#endif

#if ((((FM_CLOCK == USE_SD_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK)) && (TASK_FM_EN)) || FM_CLK_IO_MUX_SDCLK)
	fm_ctl.fm_mode_flag = 0;
#endif
#if TASK_FLASH_EN
	u_spi.spi_music_flag = 0;
#endif

	memset(&u_spi,0,sizeof(u_spi));
}


#pragma inline=forced
#pragma location="SYS_INIT"
void timer3_init()		// 1ms timer
{
	IE1 &= ~BIT(4);		// Enable timer3 interrupt
	//asm("ORL 0xB9,#0x10");
	IP1 |= BIT(4);		// Set interrupt level to high level

	// Sysclock 128 div, Time mode, Rising Edge         sysclock   24M 
	TMR3CON =  BIT(4) | BIT(2) | BIT(1)  | BIT(0) ;

	TMR3CNTH = 0;	
	TMR3CNTL = 0;

	TMR3PRH = 0x00;				// 	Timer3 period
	TMR3PRL = 0x09; 			// 	48 uSec = 9*(1/24)*128 (uSec),	1/48= 2.08%

//	TMR3PRL = 0Xbb;   		// 	1ms
//	TMR3PRL = 0x17;     		// 	122 uSec = 23*(1/24)*128 (uSec),	1/48= 2.08%
//	TMR3PRL = 0x12;     		// 	96 uSec = 9*(1/24)*128 (uSec),	1/48= 2.08%
//	TMR3PRL = 0x06;     		//  32 uSec = 6*(1/24)*128 (uSec)		, 1/32= 3.125%

	IE1 |= BIT(4);		// Enable timer3 interrupt
}

#pragma inline=forced
#pragma location="SYS_INIT"
void sd_logic_init(void)
{
	SDCON0 |= BIT(1);   //keeping clk out
	delay_m(1);
	SDCON0 = 0;
}

//判断IRTC是否第一次上电
#pragma location="SYS_INIT"
void irtc_poweron(void)
{
	u8 rtccon_val = irtc_read_cmd(CFG_RD);
	rtc_flag = 0;
	if(rtccon_val & BIT(4))
	{
		rtccon_val &= ~BIT(4);
		irtc_write_cmd(CFG_WR, rtccon_val);
		sys_ctl.irtc_poweron = 1;
		printf("irtc power on\n");

		rtc_flag = 1;
		
#ifdef AX107X_TAG		
       task_ctl.work_sta = TASK_POWEROFF;   //2070需屏蔽，1070、1171打开
#endif
       //sys_ctl.irtc_poweron = 1;
	   uart_putc('*');			
	}
	else
	{
		sys_ctl.irtc_poweron = 0;
	}
	//sys_ctl.irtc_poweron = 1;	// 测试用
}

/*----------------------------------------------------------------------------*/
/**@brief 系统初始化
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="SYS_INIT"
void system_init(void)
{

	sysclk_set();
#ifndef UART_DEBUG
#if PRINTF_EN
	uart_init();        //uart_tx:P35, baud rate:115200
#if SRAM_DEBUG
	DBGCON |= BIT(1);   //SRAM板上使用uart调试专用IO
#endif
#endif
#endif

	printf("start\n");

	sd_logic_init();
	sysvar_init();
	mute_init();           //mute 初始化，P00设为输出口，且输出高电平
	timer2_init();
	ocx.timeh = 0x00;	// 时钟显示初始化
	ocx.timel = 0x00;
	
#if LED_7P7S_SCAN_HALF_MS
	//TMR2PRH = 0x00;
	//TMR2PRL = 0xBA;   // 0x00BA为0.5ms
	TMR2PRH = 0x00;
	TMR2PRL = 0x2f;      // 0x002f为0.125ms
#endif

#if (TASK_RTC_EN || (SYS_MEM == MEM_RTCRAM) || \
    ((SYS_CRY == CRY_32K)&&(CRY_PIN_NUM == CRY_1PIN)) )
	irtc_poweron();
#endif
#if (TASK_RTC_EN || (SYS_MEM == MEM_RTCRAM))
	irtc_init();                //使用IRTC时钟、IRTCRAM记忆需要初始化
#endif

	sys_ctl.alarm_cnt = 0xFF;	// 低压报警计时器初始化为0xFF   
	disp.display_timer = 0x0a;  // 充电期间有按键触发显示定时信息时长   10s
	sys_ctl.bat_charger_cnt = 0xFF;	// 充电计时器初始化为0xFF，即默认不在充电

	port_init();
	timer3_init();
	
	disp_init();
	key_init();
	//while(1);
	
#if USE_TOUCH_KEY
	touch_key_init();           //要放在ir初始化之前
#endif
#if USE_IR
	ir_init();                  //一定要在按键初始化之后
#endif
#if SOFT_POWEROFF_EN
	wko_init();                 //要放在开总中断前
#endif

#ifdef AX107X_TAG
#if 0
	u16 key_dn_cnt = 0;
	u8 key_adcval = 0;
    SOFT_POWER_CTL_INIT();//IRTCWKO_Output_Init();
    SOFT_POWER_CTL_OFF();//IRTCWKO_Output_0();
    while(1){
        delay_1ms(1);
        LED_OFF();
		ocx.led_sta = LED_STA_OFF;
		key_adcval = get_pwrkey_adcval();
	//	printf(" %02x ",key_adcval);
        if((key_adcval>0x0b) && (key_adcval<0x4a)){//if(sys_ctl.bPowerKeydownFlag){//if((key_adcval>0x0b) && (key_adcval>0x4a)){//
            if(key_dn_cnt++ > 600){
                break;
            }
        } else {
	    	key_dn_cnt =0; 
        }
    }
    
    SOFT_POWER_CTL_ON();//IRTCWKO_Output_1();
  //  printf("123456");
#endif
#endif

#if TASK_FLASH_EN
	spi_sd_mux_enter();
	spi_port_init();
	u_spi.spi_online = spi_flash_init();  //系统初始化时已经做好spi_flash_init检测
	//printf("u_spi.spi_online: %d \n",u_spi.spi_online);
	spi_sd_mux_exit();
#endif

	device_init();

	IE_EA = 1;                  //开总中断
#if (SYS_MEM == MEM_RTCRAM)	//开机上电，清掉断点。让关机之前的播放曲目从头开始播放
	mem_ctl.dev = 1; task_music_break_save(0);
	mem_ctl.dev = 0; task_music_break_save(0);
#endif
#if SOFT_POWEROFF_EN
#if (SYS_MEM == MEM_RTCRAM)
	rtcram_poweron_check();
#endif
    //printf("A\n");

	soft_poweroff_init();
#endif
	SOFT_POWER_CTL_INIT();
	SOFT_POWER_CTL_ON();
	dac_init();                 //DAC初始化开始
   	//printf("B\n");
   

#if KEY_VOICE_EN
	key_voice_init();
#endif
    //printf("C\n");

	WATCHDOG_CLR();
#if ((DAC_POWER_SPEED) || (DAC_INIT_TYPE == DAC_SIMPLE))
	delay_5ms(30); 			   //wait for device stable
#endif

#if (SYS_CRY == CRY_AUTO)
	b_sys.xosc_flag = auto_check_x32k();
#endif


   // printf("D\n");
#if DAC_POWER_SPEED
	dac_init_wait();            //等待DAC初始化完成
#endif

#if (SYS_CRY != CRY_RC)
	sys_pll_xosc();             //若有外部晶振，则选用外部晶振
#endif

#if ((FM_CLOCK == SHARE_CRYSTAL_32KHz) && (TASK_FM_EN))
	if(fm_ctl.fm_type != FM_NO)
	{
		irtc_write_cmd(CFG3_WR, 0x06); //共晶振，增加晶振驱动能力
	}
#endif

	for(u8 i=0; i<7; i++)
	{
		//printf(".");
		comm_event(1);          //激活设备
	}

	mem_read_init_info();       //读取记忆信息
	//task_ctl.work_sta = TASK_MUSIC;

#if USE_TOUCH_KEY
	touch_key_start();          //等待系统稳定后打开TouchKey，否则上电时一段时间内按键无效
#endif

#if TASK_USBDEV_EN
	if(device_is_actived(DEVICE_PC))
	{
		task_ctl.work_sta = TASK_USBDEV;
	}
	else
#endif
	ocx.led_sta = LED_STA_ON;
		

#if TASK_FLASH_EN
	if(u_spi.spi_online && (rtc_flag != 1))
	{
		printf("not frist\n");		
		task_ctl.work_sta = TASK_FLASH;
		t_msc.poweron = 1;
			
	}
	else
#endif
	if(device_is_actived(DEVICE_UDISK)
#if UDISK_DOUBLE_EN
	        || device_is_actived(DEVICE_UDISK1)
#endif
	        || device_is_actived(DEVICE_SDMMC) || device_is_actived(DEVICE_SDMMC1))
	{
		//task_ctl.work_sta = TASK_MUSIC;
		t_msc.poweron = 1;
	}
	else
#if MIC_DET_EN
	if(device_is_actived(DEVICE_MIC))
	{
		task_ctl.work_sta = TASK_SPEAKER;
	}
	else
#endif
#if AUX_DET_EN
	if(device_is_actived(DEVICE_LINEIN))
	{
		task_ctl.work_sta = TASK_AUX;
	}
	else
#endif
#if TASK_FM_EN
	if(fm_ctl.fm_type != FM_NO)
	{
		task_ctl.work_sta = TASK_FM;
	}
	else
#endif
	{
		//task_ctl.work_sta = TASK_AUX;
	}
	//sys_ctl.volume = VOLUME_MAX/2+9;
	//sys_ctl.mode_play = ITEM_LOOP_ALL;
	sys_ctl.volume = VOLUME_MAX;    //初始化把音量设置为最大
	user_change_volume(sys_ctl.volume);     //设置系统音量
	user_change_eq(sys_ctl.eq_num);         //设置系统EQ
	printf("sys_ctl.volume: %d\n",sys_ctl.volume);
//	printf("sys_ctl.mode_play =%d\n",sys_ctl.mode_play);
#if PLAY_STARTMUSIC
	//startmusic_play();
#endif
#if USE_KEDIR
	b_sys.kedir_flag = 1;
#endif
}
