/*****************************************************************************
 * Module    : mem
 * File      : mem_rtcram.c
 * Author    :
 * Email     :
 * Function  : RTCRAM记忆任务流程
 *****************************************************************************/
#include "include.h"
#include "mem_rtcram.h"

#if (SYS_MEM == MEM_RTCRAM)

#pragma location="MEMRTCRAM"
bool rtcram_poweron_check(void)
{
	if(mem_ctl.rtcram_poweron)
	{
		printf("FPON\n");
		mem_ctl.rtcram_poweron = 0;//RTC掉电后使用默认值
		var_set_default();
		return 0;
	}
	return 1;
}

#pragma location="MEMRTCRAM"
void write_playmode_rtcram(void)
{
	rtcram_write_byte(RTCRAM_PLAY_MODE, sys_ctl.mode_play);
}

#pragma location="MEMRTCRAM"
void write_volume_rtcram(void)
{
	rtcram_write_byte(RTCRAM_VOLUME, sys_ctl.volume);
}

#pragma location="MEMRTCRAM"
void read_volume_rtcram(void)
{
	sys_ctl.volume = irtc_read_ram(RTCRAM_VOLUME);
}

#pragma location="MEMRTCRAM"
void write_num_rtcram(void)
{
#if UDISK_DOUBLE_EN
	if(mem_ctl.dev != DEVICE_UDISK1)
	{
		irtc_write_ram((mem_ctl.dev * RTCRAM_OFFSET + RTCRAM_MUSIC_POS_SD + 8), (u8 *)&u_msc.music_num, 2);
	}
	else
	{
		irtc_write_ram(RTCRAM_MUSIC_NUM_USB1, (u8 *)&u_msc.music_num, 2);
	}
#else
/*	if(mem_ctl.dev == DEVICE_UDISK1)	// Merlin 停用
	{
		irtc_write_ram(RTCRAM_MUSIC_POS_USB, (u8 *)&u_msc.music_num, 2);
	}
	else */
	{
		irtc_write_ram((mem_ctl.dev * RTCRAM_OFFSET + RTCRAM_MUSIC_NUM_SD), (u8 *)&u_msc.music_num, 2);
	}
#endif
}

#pragma location="MEMRTCRAM"
void read_num_rtcram(void)
{
#if UDISK_DOUBLE_EN
	if(mem_ctl.dev != DEVICE_UDISK1)
	{
		rtcram_read_multi((mem_ctl.dev * RTCRAM_OFFSET + RTCRAM_MUSIC_POS_SD + 8), (u8 *)&u_msc.music_num, 2);
	}
	else
	{
		rtcram_read_multi(RTCRAM_MUSIC_NUM_USB1, (u8 *)&u_msc.music_num, 2);
	}
#else
/*	if(mem_ctl.dev == DEVICE_UDISK1)	// Merlin 停用
	{
		rtcram_read_multi(RTCRAM_MUSIC_POS_USB, (u8 *)&u_msc.music_num, 2);
	}
	else */
	{
		rtcram_read_multi((mem_ctl.dev * RTCRAM_OFFSET + RTCRAM_MUSIC_NUM_SD), (u8 *)&u_msc.music_num, 2);
	}
#endif
}

#if TASK_FLASH_EN
#pragma location="MEMRTCRAM"
void write_flash_num_rtcram(void)
{
//	printf("write=%02x num=%04x\n", RTCRAM_MUSIC_NUM_SD1, u_msc.music_num);
	irtc_write_ram(RTCRAM_MUSIC_NUM_SD1, (u8 *)&u_msc.music_num, 2);
}
	
#pragma location="MEMRTCRAM"
void read_flash_num_rtcram(void)
{
	rtcram_read_multi(RTCRAM_MUSIC_NUM_SD1, (u8 *)&u_msc.music_num, 2);
//	printf("read=%02x num=%04x\n", RTCRAM_MUSIC_NUM_SD1, u_msc.music_num);
}
#endif


#pragma location="MEMRTCRAM"
void write_device_rtcram(void)
{
	rtcram_write_byte(RTCRAM_ACTIVE_DEVICE, sys_ctl.dev_save);
}

#pragma location="MEMRTCRAM"
void read_device_rtcram(void)
{
	sys_ctl.dev_save = irtc_read_ram(RTCRAM_ACTIVE_DEVICE);
}

#if USE_BREAK_POINT
#pragma location="MEMRTCRAM"
void write_music_break_rtcram(void)
{
	u8 addr;
#if UDISK_DOUBLE_EN
#if SD_DOUBLE_EN
	if(mem_ctl.dev != DEVICE_UDISK1)             //RTCRAM空间不够，暂不保存UDISK1播放断点
	{
		addr = RTCRAM_MUSIC_POS_SD + mem_ctl.dev * RTCRAM_OFFSET;
		irtc_write_ram(addr,(u8 *)&u_msc.break_point, 8);
	}
#else
	if(mem_ctl.dev != DEVICE_UDISK1)
	{
		addr = RTCRAM_MUSIC_POS_SD + mem_ctl.dev * RTCRAM_OFFSET;
	}
	else
	{
		addr = RTCRAM_MUSIC_POS_SD1;            //无双卡功能时，复用SD1的保存地址
	}
	irtc_write_ram(addr,(u8 *)&u_msc.break_point, 8);
#endif
#else
	if(mem_ctl.dev == DEVICE_UDISK1)
	{
		addr = RTCRAM_MUSIC_POS_USB;
	}
	else
	{
		addr = RTCRAM_MUSIC_POS_SD + mem_ctl.dev * RTCRAM_OFFSET;
	}
	irtc_write_ram(addr,(u8 *)&u_msc.break_point, 8);
#endif
}

#pragma location="MEMRTCRAM"
void read_music_break_rtcram(void)
{
	u8 addr;
#if UDISK_DOUBLE_EN
#if SD_DOUBLE_EN
	if(mem_ctl.dev != DEVICE_UDISK1)
	{
		addr = RTCRAM_MUSIC_POS_SD + mem_ctl.dev * RTCRAM_OFFSET;
		rtcram_read_multi(addr,(u8 *)&u_msc.break_point, 8);
	}
	else
	{
		u_msc.break_point.pos = 0;               //RTCRAM空间不够，暂不保存UDISK1播放断点
		u_msc.break_point.frame = 0;
	}
	//printf("read pos:%ld,frame:%ld\n",u_msc.break_point.pos,u_msc.break_point.frame);
#else
	if(mem_ctl.dev != DEVICE_UDISK1)
	{
		addr = RTCRAM_MUSIC_POS_SD + mem_ctl.dev * RTCRAM_OFFSET;
	}
	else
	{
		addr = RTCRAM_MUSIC_POS_SD1;           //无双卡功能时，复用SD1的保存地址
	}
	rtcram_read_multi(addr,(u8 *)&u_msc.break_point, 8);
#endif
#else
	if(mem_ctl.dev == DEVICE_UDISK1)
	{
		addr = RTCRAM_MUSIC_POS_USB;
	}
	else
	{
		addr = RTCRAM_MUSIC_POS_SD + mem_ctl.dev * RTCRAM_OFFSET;
	}
	rtcram_read_multi(addr,(u8 *)&u_msc.break_point, 8);
#endif
}
#endif /*END USE_BREAK_POINT*/


#if TASK_FM_EN
#pragma location="MEMRTCRAM"
void write_total_channel_rtcram(void)
{
	rtcram_write_byte(RTCRAM_FM_TOTAL_CHANNEL, fm_ctl.ch_total);
}

#pragma location="MEMRTCRAM"
void write_cur_channel_rtcram(void)
{
	rtcram_write_byte(RTCRAM_CUR_CHANNEL, fm_ctl.ch_cur);

	rtcram_write_byte(RTCRAM_FREQUENCY, fm_ctl.fm_freq - FM_MIN_FREQ);
}

#pragma location="MEMRTCRAM"
void write_fm_info_rtcram(u8 byte_value)
{
	rtcram_write_byte(RTCRAM_FREQUENCY_BUF+byte_value, fm_ctl.ch_buf[byte_value]);
}

#pragma location="MEMRTCRAM"
void read_fm_info_rtcram(void)
{
	rtcram_read_multi(RTCRAM_FREQUENCY_BUF, fm_ctl.ch_buf, CH_BUF_MAX);

	fm_ctl.ch_total = irtc_read_ram(RTCRAM_FM_TOTAL_CHANNEL);
	fm_ctl.ch_cur = irtc_read_ram(RTCRAM_CUR_CHANNEL);
	fm_ctl.fm_freq = irtc_read_ram(RTCRAM_FREQUENCY);
}
#endif

#endif
