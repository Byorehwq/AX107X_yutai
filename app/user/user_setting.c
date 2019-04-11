/*****************************************************************************
 * Module    : User
 * File      : user_setting.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 系统参数设置
 *****************************************************************************/
#include "include.h"

type_sys_ctl sys_ctl;
IAR_BDATA_A type_sys_flag b_sys;

#define EQ0(n) ((u8)(((n) + (16 - 3)) * 8))
#define EQ1(n) ((u8)(((n) + (16 - 4)) * 8))
#define EQ2(n) EQ1(n)
#define EQ3(n) EQ1(n)
#define EQ4(n) EQ1(n)
#define EQ5(n) EQ1(n)
#define EQ6(n) EQ1(n)
#define EQ7(n) EQ1(n)
#define EQ8(n) ((u8)(((n) + (16 - 2)) * 8))

#pragma constseg="EQ_TBL"
//五种EQ效果DB值，每个值的范围为 -12 ~ 12 db
IAR_CONST u8 tbl_eq_db[6][9] =
{
	{EQ0(0),  EQ1(0),  EQ2(0),  EQ3(0),  EQ4(0),  EQ5(0),  EQ6(0), EQ7(0), EQ8(0)},             //0db,不要修改此项
	{EQ0(5),  EQ1(2),  EQ2(1),  EQ3(-6),  EQ4(-6),  EQ5(-4),  EQ6(0), EQ7(1), EQ8(2)},          //POP
	{EQ0(0),  EQ1(4),  EQ2(7),  EQ3(-4),  EQ4(-4),  EQ5(-2),  EQ6(3), EQ7(7), EQ8(7)},          //ROCK
	{EQ0(-3),  EQ1(-3),  EQ2(-3),  EQ3(7),  EQ4(7),  EQ5(7),  EQ6(-1), EQ7(0), EQ8(1)},         //JAZZ
	{EQ0(9),  EQ1(9),  EQ2(8),  EQ3(-2),  EQ4(-2),  EQ5(-2),  EQ6(1), EQ7(2), EQ8(3)},          //CLASSIC
	//{EQ0(-4),  EQ1(-3),  EQ2(-2),  EQ3(0),  EQ4(-3),  EQ5(-3),  EQ6(6), EQ7(8), EQ8(8)},      //COUNTRY
	{EQ0(9),  EQ1(9),  EQ2(9),  EQ3(9),  EQ4(9),  EQ5(9),  EQ6(9), EQ7(9), EQ8(9)}
	
};

//EQ滤波器系数表,请使用EQ调试工具 EQGAIN.exe
IAR_CONST u8 eq_coeff[54] =
{
	0x00, 0x34, 0x7F, 0x98, 0xc0, 0x67,
	0x00, 0x68, 0x7f, 0x2d, 0xc0, 0xcf,
	0x00, 0xce, 0x7e, 0x54, 0xc1, 0x9c,
	0x01, 0x96, 0x7c, 0x94, 0xc3, 0x2d,
	0x03, 0x1a, 0x78, 0xd7, 0xc6, 0x34,
	0x06, 0x03, 0x70, 0x4f, 0xcc, 0x06,
	0x0a, 0x00, 0x5e, 0xda, 0xd3, 0xff,
	0x11, 0xf1, 0x32, 0x87, 0xe3, 0xe2,
	0x19, 0xa8, 0xe3, 0x7a, 0xf3, 0x50
};
#pragma constseg=default

//设置当前播放音量
#pragma location="USER_SETTING_SEG"
void user_change_volume(u8 volume)
{
	if(volume <= VOLUME_MAX)
	{
#if DAC_FADE_EN
		dac_set_volume(volume);
#else
		dac_set_volume_direct(volume);
#endif
	}
}

//保存并设置系统音量
#pragma location="USER_SETTING_SEG"
bool user_set_volume(u8 volume)
{
	if(volume == sys_ctl.volume)
	{
		return false;                             //没有更改
	}
	user_change_volume(volume);
	if(volume <= VOLUME_MAX)
	{
		sys_ctl.volume = volume;

		mem_ctl.delay_time = MEM_DELAY;
		mem_ctl.change_flag |= BIT(MEM_VOL_BIT);
	}
	return true;
}

#pragma location="TASK_FM_SEG"
bool user_set_fm_volume(u8 volume)
{
	if(volume == sys_ctl.volume)
	{
		return false;                             //没有更改
	}
	if(volume <= VOLUME_MAX)
	{
		fm_rx_set_vol(volume);
		sys_ctl.volume = volume;
#if !AMUX_FM_WITH_NO_CAP
		user_change_volume(sys_ctl.volume);
#endif
		mem_ctl.delay_time = MEM_DELAY;
		mem_ctl.change_flag |= BIT(MEM_VOL_BIT);
	}
	if(b_dac.mute_en)
	{
		mute_disable();
	}
	return true;
}

//设置当前EQ
#pragma location="USER_SETTING_SEG"
u8 user_change_eq(u8 eq_num)
{
#if MUSIC_EQ
	if(eq_num > EQ_NUM_MAX)
	{
		eq_num = 0;
	}
	if(eq_num == sys_ctl.eq_num)
	{
		return eq_num;                             //没有更改
	}
#endif
#if EQ_FADE_EN
#if EQ_MODIFY_DB_EN
	music_eq_set(eq_num);
#else
	music_eq_set_m(eq_num);
#endif
#else
#if EQ_MODIFY_DB_EN
	music_eq_setd(eq_num);
#else
	music_eq_setd_m(eq_num);
#endif
#endif
	return eq_num;
}

//保存并设置EQ
#pragma location="USER_SETTING_SEG"
bool user_set_eq(u8 eq_num)
{
	u8 num_eq;
	num_eq = user_change_eq(eq_num);
	if(num_eq <= EQ_NUM_MAX)
	{
		sys_ctl.eq_num = num_eq;
	}
	return true;
}

//设置MUTE
#pragma location="USER_SETTING_SEG"
void user_set_mute(void)
{
	if(b_dac.mute_en)
	{
		mute_disable();
		if(!b_dac.dac_stop || !b_dac.digvol_en)     //DAC暂停时，解mute不进行淡入
		{
#if DAC_FADE_EN
			dac_fade_in(b_dac.digvol_en);
#endif
		}
	}
	else
	{
#if DAC_FADE_EN
		dac_fade_out();
		mute_enable();
#endif
	}
}

#if KEY_VOICE_EN
#pragma location="USER_SETTING_SEG"
void user_kick_kv(void)
{
	if(!b_dac.mute_en && sys_ctl.volume != 0)
	{
		if(!dac_fade_busy())
		{
			dac_fade_in_auto();
		}
		b_dac.key_voice = 1;
	}
}

#pragma location="USER_SETTING_SEG"
void user_chk_kv(void)
{
	if(b_dac.key_voice)
	{
		if(!dac_fade_busy())
		{
			b_dac.key_voice = 0;
			if(!(KVCCON & BIT(0)))
			{
				KVCCON |= BIT(0);    //key voice enable
			}
		}
	}
	else
	{
		if(!(KVCCON & BIT(0)))
		{
			if(b_dac.mute_en || b_dac.dac_stop || (b_dac.sound_flag && !b_dac.sound_sta))
			{
				if(dac_fade_out_auto())
				{
				}
			}
		}
	}
}
#endif

#pragma location="USER_SETTING_SEG"
void sdclk_output_clk_start(void)
{
	PCON1 &= ~BIT(3);                           //sdc clk enable
#if (FM_CLOCK == USE_SD_IO_12MHZ_CLK)
	SDBAUD = 0;                                 //SD_CLK = 12MHz
#elif (FM_CLOCK == USE_SD_IO_32KHZ_CLK)
	set_sys_clk(SYS_12M);
	SDBAUD = 182;                               //SD_CLK = 32768Hz
#endif
	SDCON0 |= (BIT(0) | BIT(1));                //Enable SDC & Keep Clk Outing
}

#pragma location="USER_SETTING_SEG"
void sdclk_output_clk_stop(void)
{
	SDCON0 = 0;
#if (FM_CLOCK == USE_SD_IO_32KHZ_CLK)
	set_sys_clk(SYS_24M);
#endif
}

