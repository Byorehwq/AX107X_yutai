/*--------------------------------------------------------------------------*/
/**@file Si484x.C
   @brief FM模块Si484x驱动
   @details FM模块Si484x的驱动函数
   @author LZZ
   @date 2010-08-17
   @note 本驱动支持模块，支持32K共晶振,支持P05输出时钟,三种方式时钟的程序一样
         通过识别ID来区别各种版本
*/
/*----------------------------------------------------------------------------*/

#include "include.h"
#include "Si484x.h"


#if FM_SI484X_EN

u8 band_index=12;
u8 tuner_mode;
u8 state_machine;
u8 fm_bass_treble=0;
u8 am_bass_treble=0;
u8 max_vol=0;
u8 si484x_dat[11];
u8 si484x_readdata[4];

void FM_Si484x_Write_2(u8 size);
void FM_Si484x_Read_2(u8 size);

#define Si484X_RD_CTRL   0x0021
#define Si484X_WR_CTRL   0x0020
#pragma constseg = Si484x_RX_TABLE

const u8 max_vol_list[9] = {59,60,61,62,63,62,61,60,59};
#pragma constseg=default
#if USE_PORT_INTERRUPT
/*----------------------------------------------------------------------------*/
/***@brief   PORT外部中断的初始化
    @details
    @param 无
    @return 无
    @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="START_UP"
__near_func void port_init(void)
{
	IE1 &=~BIT(2);
	PWKEN &=~0x01;  //enable PORT(P04) Interrupt
	asm("nop");
	asm("nop");
	PWKEDGE= 0x00;
	asm("nop");
	asm("nop");
	P0DIR |=BIT(4);    //set ingput
	PIE0 |= (1<<4);
	IE1 |= (1<<2);     //
}
/*----------------------------------------------------------------------------*/
/***@brief   PORT外部中断服务函数
    @details
    @param 无
    @return 无
    @note
*/
/*----------------------------------------------------------------------------*/
#pragma vector = VECTOR_OFFSET(PORT_INT)
#pragma register_bank = 1
__interrupt __near_func void isr_port(void)
{
	ISR_ENTER();
	if(PWKEN & BIT(4))   //PORT 中断
	{
		PWKEN &=~BIT(4);
		if(state_machine & SM_RADIO_READY)
		{
			flag_tuner_irq = 1;
		}
	}
	ISR_EXIT();
}
#endif
#if USE_CAPTURE_MODE
/*----------------------------------------------------------------------------*/
/**@brief timer0 初始化函数
   @details:主要用于SI系列的收音芯片IRQ信号的Capture
   @param 无
   @return 无
   @note : timer0对应Capture的功能的GPIO是P05
*/
/*----------------------------------------------------------------------------*/
void timer0_init()
{
	IE1 &=~BIT(5);
	TMR0CON = 0x37;
	PIE0 |= (1<<5);   //digital input
	P0DIR |= BIT(5);  //Si484xIRQ
	IE1 |= BIT(5);
}
/*----------------------------------------------------------------------------*/
/**@brief timer0 中断服务函数
   @details:主要用于SI系列的收音芯片IRQ信号的Capture
   @param 无
   @return 无
   @note : timer0对应Capture的功能的GPIO是P05
*/
/*----------------------------------------------------------------------------*/
#pragma vector = VECTOR_OFFSET(TIMER0_INT)
#pragma register_bank = 1
__interrupt __near_func void isr_timer0(void)
{
	ISR_ENTER();
	if(TMR0CON & 0x80)	//CAPTURE
	{
		TMR0CON &= ~BIT(7);
		if(state_machine & SM_RADIO_READY)
		{
			flag_tuner_irq = 1;
		}
	}
	ISR_EXIT();
}
#endif

/*----------------------------------------------------------------------------*/
/**@brief  IIC初始化复位
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void i2c_reset(void)@"Si484x_RX"
{
	IIC_DATA_HIGH();
	IIC_CLK_HIGH();
	SI484X_RST_LOW();
	delay_5ms(1);
	SI484X_RST_HIGH();
	delay_5ms(1);
}
/*----------------------------------------------------------------------------*/
/**@brief  复位SI芯片
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void Si484x_Reset(void)@"Si484x_RX"
{
	SI484X_RST_HIGH();
	delay_5ms(1);
	SI484X_RST_LOW();
	IIC_DATA_LOW();
	IIC_CLK_LOW();
	delay_5ms(1);
	IIC_CLK_HIGH();
	SI484X_RST_HIGH();
	IIC_DATA_HIGH();
}

/****************************************************************************
 * this function set the bass/treble
 * **************************************************************************/
/*----------------------------------------------------------------------------*/
/**@brief  获取SI芯片的高、低音输出幅度
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void atdd_set_bass_treble(u8 tone)@"Si484x_RX"
{
	set_property(0x4002,tone);
#if 0//FM_AUDIO_MODE == 3
	if(tuner_mode == FM)
	{
		volume= atdd_get_actual_vol();
		max_vol = max_vol_list[tone];
	}
#endif
}
/*----------------------------------------------------------------------------*/
/**@brief  获取SI芯片的去加重时间
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void atdd_set_deemphasis(u8 deemphasis)@"Si484x_RX"
{
	set_property(0x1100,deemphasis);
}
/*----------------------------------------------------------------------------*/
/**@brief  获取SI芯片的单声道调制的RSSI门限
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void atdd_set_rssi_blend_mono(u8 blend_mono)@"Si484x_RX"
{
	set_property(0x1801,blend_mono);
}
/*----------------------------------------------------------------------------*/
/**@brief  获取SI芯片的立体声混合的RSSI门限
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void atdd_set_rssi_blend_stereo(u8 blend_stereo)@"Si484x_RX"
{
	set_property(0x1800,blend_stereo);
}

/*----------------------------------------------------------------------------*/
/**@brief  获取SI芯片的立体声分离度
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void atdd_set_stereo_separation(u8 separation)@"Si484x_RX"
{
	set_property(0x1207,separation);
}

/*----------------------------------------------------------------------------*/
/**@brief  获取SI芯片的实际的音量　
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#ifdef  enable_get_actual_vol_command
#ifndef enable_get_property_command
#define enable_get_property_command
#endif
u8 atdd_get_actual_vol()@"Si484x_RX"
{
	u8 actual_vol;
	actual_vol = (u8) get_property(0x4003);
	return actual_vol;
}
#endif
/*----------------------------------------------------------------------------*/
/**@brief  设置SI芯片的Volume　
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void atdd_set_volume(u8 vol)@"Si484x_RX"
{
	set_property(0x4000,vol);
	//set_property(0x4001,0);
}


#define ADJPT_STERRO_DISABLE 0
#define FM_ADJPT_ATTENUATION_DISABLE 0
#define FM_AUDIO_MODE 3
#define FM_FORCE_MONO 0
//1:de-emphasis 50us
//2:de-emphasis 75us
#define FM_DE_EMPHASIS 2
#define FM_BLEND_MONO  8
#define FM_BLEND_STEREO 49
#define FM_SEPARATION 0x9F
/*----------------------------------------------------------------------------*/
/**@brief  配置FM波段的一些参数　
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void config_fm()@"Si484x_RX1"
{
	if(!flag_tuner_pri)
	{
		atdd_set_deemphasis(FM_DE_EMPHASIS);
		atdd_set_rssi_blend_mono(FM_BLEND_MONO);
		atdd_set_rssi_blend_stereo(FM_BLEND_STEREO);
		atdd_set_stereo_separation(FM_SEPARATION);
	}
	atdd_audio_mode(ADJPT_STERRO_DISABLE,FM_ADJPT_ATTENUATION_DISABLE,FM_FORCE_MONO,FM_AUDIO_MODE);
//#if  FM_AUDIO_MODE != 1
	atdd_set_volume(SI484X_OUT_VOL);
//#endif
}
/***************************************************
 * config am band,initial volume and bass/treble
 * *************************************************/
/*----------------------------------------------------------------------------*/
/**@brief  配置AM波段的一些参数
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void config_am()@"Si484x_RX1"
{
	//atdd_set_bass_treble(am_bass_treble);
	atdd_set_volume(SI484X_OUT_VOL);
}
/***************************************************
 * config sw band,initial volume ,bass/treble and tuning preference
 * a
 * *************************************************/
/*----------------------------------------------------------------------------*/
/**@brief   配置SW波段的一些参数
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#define SW_ADJPT_ATTENUATION_DISABLE 0
void config_sw()@"Si484x_RX1"
{
	//atdd_set_bass_treble(am_bass_treble);
	atdd_set_volume(SI484X_OUT_VOL);
	//atdd_audio_mode(0,SW_ADJPT_ATTENUATION_DISABLE,0,0);
}

/*----------------------------------------------------------------------------*/
/**@brief  设置SI芯片开机,等待晶振起振时间、波段的选择和属性的建立
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void atdd_power_up(u8 xoscen,u8 xowait,u8 band_index,u16 band_bottom,u16 band_top,u8 chspc)@"Si484x_RX1"
{
	u8 len;
	si484x_dat[0] = ATDD_POWER_UP;  //ATDD powerup command
	si484x_dat[1] = band_index;
	if(xoscen)
	{
		si484x_dat[1] |= ATDD_XOSCEN ;
	}
	if(xowait)
	{
		si484x_dat[1] |= ATDD_XOWAIT;
	}
	len = 2;
	if(flag_tuner_pri)
	{
		si484x_dat[2] = band_bottom>>8;
		si484x_dat[3] = band_bottom;
		si484x_dat[4] = band_top>>8;
		si484x_dat[5] = band_top;
		si484x_dat[6] = chspc;
		len = 7;
	}
	FM_Si484x_Write_2(len);
	wait_cts();
	//write_command(len, cmd, 0, NULL);
}

/*******************************************************************************
 * This function set the audio mode,fm force mono,+/-100K-adjance channel attenuation mode
 * and +/-100k-adjance channel stereo status
 * *****************************************************************************/
/*----------------------------------------------------------------------------*/
/**@brief  得到设置音频模式
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void atdd_audio_mode(u8 adj_st,u8 adj_attn,u8 force_mono,u8 audio_mode)@"Si484x_RX"
{
	si484x_dat[0] = ATDD_AUDIO_MODE;
	si484x_dat[1] = audio_mode;
	if(adj_st)
	{
		si484x_dat[1] |= ADJPT_STEO;
	}
	if(adj_attn)
	{
		si484x_dat[1] |= ADJPT_ATTN;
	}
	if(force_mono)
	{
		si484x_dat[1] |= FM_MONO;
	}
	FM_Si484x_Write_2(2);
	wait_cts();
	//write_command(2, cmd, 0, NULL);
}
/*----------------------------------------------------------------------------*/
/**@brief  得到SI芯片当前调谐频率、波段等等芯片的状态
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void atdd_get_status(void)@"Si484x_RX"
{
	//get atdd status and analyze them
	si484x_dat[0] = ATDD_GET_STATUS;
	FM_Si484x_Write_2(1);
	wait_cts();
	FM_Si484x_Read_2(4);
	//write_command(1,&cmd,4,rspbuf);
}
/*----------------------------------------------------------------------------*/
/**@brief  设置SI芯片的属性值
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#if FM_AUDIO_MODE == 3
#ifndef enable_get_actual_vol_command
#define enable_get_actual_vol_command
#endif
#endif
void set_property(u16 property_id,u16 property_value)@"Si484x_RX1"
{
	si484x_dat[0] = 0x12;
	si484x_dat[1] = 0;
	si484x_dat[2] = (u8)(property_id >> 8);
	si484x_dat[3] = (u8)(property_id);
	si484x_dat[4] = (u8)(property_value >> 8);
	si484x_dat[5] = (u8)(property_value);
	FM_Si484x_Write_2(6);
	wait_cts();
	//write_command(6, cmd, 0, NULL);
}
/*----------------------------------------------------------------------------*/
/**@brief  获取SI芯片的属性值
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#ifdef  enable_get_property_command
u16 get_property(u16 property_id)@"Si484x_RX1"
{
	si484x_dat[0] = 0x13;
	si484x_dat[1] = 0;
	si484x_dat[2] = (u8)(property_id >> 8);
	si484x_dat[3] = (u8)(property_id);
	FM_Si484x_Write_2(4);
	wait_cts();
	FM_Si484x_Read_2(4);
	//write_command(4, cmd,4, rsp);
	return (u16)(si484x_readdata[2]<<8+si484x_readdata[3]);
}
#endif

/*----------------------------------------------------------------------------*/
/**@brief   退出FM模式,关掉SI芯片
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#ifdef  enable_power_down_command
void atdd_power_down(void)@"Si484x_RX"
{
	si484x_dat[0] = ATDD_POWER_DOWN;
	FM_Si484x_Write_2(1);
	wait_cts();
	//write_command(1,&cmd,0,NULL);
}
#endif
/*----------------------------------------------------------------------------*/
/**@brief FM模块Si484x初始化
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void si484x_init(void)@"Si484x_RX"
{
	RESTSI484XDIROUT();
	Si484x_Reset();
	flag_tuner_irq = 0;
	state_machine = SM_RADIO_RESET;
	i2c_reset();
	delay_5ms(20);  //延时100MS
	Si484x_Reset();
}
/*----------------------------------------------------------------------------*/
/***@brief    配置芯片的工作波段
    @details
    @param
    @return 无
    @note ：在使用外部晶振时SM_RADIO_RESET之后
            必须延时100ms等待晶振稳定
*/
/*----------------------------------------------------------------------------*/
void config_tune(void)@"Si484x_RX"
{
	if(tuner_mode == FM)
	{
		atdd_power_up(XOSCEN,XOWAIT_LONG,band_index,FM_BAND_BOTTOM,FM_BAND_TOP,FM_BAND_SPACING);

		if(state_machine == SM_RADIO_RESET)
		{
			delay_5ms(20);
		}
		config_fm();
	}
	if(tuner_mode == AM)
	{
		atdd_power_up(XOSCEN,XOWAIT_LONG,21,AM_BAND_BOTTOM,AM_BAND_TOP,AM_BAND_SPACING);
		if(state_machine == SM_RADIO_RESET)
		{
			delay_5ms(20);
		}
		config_am();
	}
	if(tuner_mode == SW)
	{
		atdd_power_up(XOSCEN,XOWAIT_LONG,band_index,SW_BAND_BOTTOM,SW_BAND_TOP,SW_BAND_SPACING);
		if(state_machine == SM_RADIO_RESET)
		{
			delay_5ms(20);
		}
		config_sw();
	}
}
/*----------------------------------------------------------------------------*/
/***@brief    查询SI的芯片的状态，包括当前工作的频点、电台的质量等信息
    @details
    @param
    @return 无
    @note ：
*/
/*----------------------------------------------------------------------------*/
void parse_atdd_status(void)@"Si484x_RX"
{
	atdd_get_status();
	if(si484x_readdata[0] & INFORDY)
	{
		flag_host_band = si484x_readdata[0] & HOSTBAND;
		flag_tuner_pri = si484x_readdata[0] & TUNERPRI;
		if(!flag_host_band) //SI芯片自己控制波段
		{
			band_index = si484x_readdata[1] & 0x3F;
			tuner_mode = si484x_readdata[1] >> 6;
		}

		if(state_machine == SM_RADIO_POWERUP)
		{
			state_machine = SM_RADIO_READY;
		}
		if(state_machine == SM_RADIO_READY)
		{

			flag_station = 0;
			if(si484x_readdata[0] & STATION)
			{
				flag_station = 1;   //电台的质量
			}
#if 0
			flag_stereo = 0;
			if(si484x_readdata[0] & STEREO)
			{
				flag_stereo = 1;   //是否是立体声
			}
#endif
			if(tuner_mode==TUNER_MODE_SW)
			{
				fm_freq=(((si484x_readdata[2]>>4)&0x07)*10+(si484x_readdata[2]&0x0f))*100+((si484x_readdata[3]>>4)&0x0f)*10+(si484x_readdata[3]&0x0f);
				cur_menu = MENU_FM_FREQ;   //SW波段的频率菜单显示
				disp_menu(cur_menu);
			}
			else if(tuner_mode==TUNER_MODE_FM)
			{
				fm_freq=(((si484x_readdata[2]>>4)&0x0f)*10+(si484x_readdata[2]&0x0f))*100+((si484x_readdata[3]>>4)&0x0f)*10+(si484x_readdata[3]&0x0f);
				cur_menu = MENU_FM_FREQ;   //FM波段的频率菜单显示
				disp_menu(cur_menu);
			}
			else
			{
				fm_freq=(((si484x_readdata[2]>>4)&0x0f)*10+(si484x_readdata[2]&0x0f))*100+((si484x_readdata[3]>>4)&0x0f)*10+(si484x_readdata[3]&0x0f);
				cur_menu = MENU_FM_FREQ;   //AM波段的频率菜单显示
				disp_menu(cur_menu);
			}
		}
	}
	if(si484x_readdata[0] & HOSTRST)
	{
		i2c_reset();
		state_machine = SM_RADIO_RESET;
	}
	if(si484x_readdata[0] & HOSTPWRUP)//||(si484x_readdata[0] == HOSTERR))
	{
		config_tune();       //可以选择配FM、AM、SW
		state_machine = SM_RADIO_POWERUP;
	}
}
/*----------------------------------------------------------------------------*/
/**@brief 通过iic读取SI状态的一些参数
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void FM_Si484x_Read_2(u8 size)@"Si484x_RX"

{
	u16 i;

	fm_i2c_start();
	fm_i2c_send_byte(0x23);
	fm_i2c_recv_ack();

	for(i=0; i<size-1; i++)
	{
		si484x_readdata[i] = fm_i2c_read_byte();
		fm_i2c_send_ack();
	}

	si484x_readdata[i] = fm_i2c_read_byte();
	fm_i2c_send_nack();

	fm_i2c_stop();
}
/*----------------------------------------------------------------------------*/
/**@brief 通过IIC向SI芯片写CMD
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void FM_Si484x_Write_2(u8 size)@"Si484x_RX"
{
	u8 i;

	fm_i2c_start();
	fm_i2c_send_byte(0x22);
	fm_i2c_recv_ack();

	for(i=0; i<size; i++)
	{
		fm_i2c_send_byte(si484x_dat[i]);
		fm_i2c_recv_ack();
	}

	fm_i2c_stop();
}
/*----------------------------------------------------------------------------*/
/**@brief SI等待可以发送下一个命令
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void wait_cts(void)@"Si484x_RX"
{
	u8 i=10;
	do
	{
		delay_5ms(1);
		FM_Si484x_Read_2(1);
	}
	while(!(si484x_readdata[0] & 0x80) && i--);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块Si484x关闭
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void Si484x_off(void)@"Si484x_RX"
{
	//Si484x_Powerdown();
#ifdef  enable_power_down_command
	atdd_power_down();
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块Si484x设置音量
   @param vol 音量值
   @return 无
   @note 音量范围0~15
*/
/*----------------------------------------------------------------------------*/
void Si484x_set_vol(u8 vol)@"Si484x_RX"
{

}

#endif
