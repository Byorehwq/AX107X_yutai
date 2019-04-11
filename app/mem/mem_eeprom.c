/*****************************************************************************
 * Module    : mem
 * File      : mem_eeprom.c
 * Author    :
 * Email     :
 * Function  : EEPROM记忆任务流程
 *****************************************************************************/
#include "include.h"
#include "mem_eeprom.h"

#if (SYS_MEM == MEM_E2PROM)

#pragma location="MEMEEP"
bool eeprom_check(void)
{
	u8 temp;

	temp = 0xAA;
	i2c_write(126, &temp , 1);
	delay_5ms(2);
	temp = i2c_read(126);

	if(temp != 0xAA)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


#pragma location="MEMEEP"
void eeprom_read_multi(u8 addr, u8 *value, u8 num)
{
	for(u8 i = 0; i < num; i++)
	{
		*(value + i) = eeprom_read(addr + i);
	}
}

//使用该函数写eeprom可以避免跨页问题，但占用空间
#pragma location="MEMEEP"
void eeprom_write_multi(u8 addr, u8 *buf, u8 num)
{
	u8 page, cnt;

	cnt = (addr%8 >num)? num : addr%8;
	if(cnt)
	{
		i2c_write(addr, buf, cnt);
		delay_hook(2);          //跨页时要延时2ms
		addr += cnt;
		buf += cnt;
		num -= cnt;
	}

	page = num/8;
	cnt = num%8;

	while(page--)
	{
		i2c_write(addr, buf, 8);
		delay_hook(2);          //跨页时要延时2ms
		addr += 8;
		buf += 8;
	}

	if(cnt)
	{
		i2c_write(addr, buf, cnt);
		delay_hook(2);          //写完要延时2ms
	}
}

#pragma location="MEMEEP"
void write_volume_eeprom(void)
{
	u8 volume = sys_ctl.volume;
	eeprom_write(PARAM_VOLUME,&volume,1);
}

#pragma location="MEMEEP"
void read_volume_eeprom(void)
{
	sys_ctl.volume = eeprom_read(PARAM_VOLUME);
}

#pragma location="MEMEEP"
void write_num_eeprom(void)
{
	eeprom_write((mem_ctl.dev * EEPROM_OFFSET + PARAM_MUSIC_NUM_SD), (u8 *)&u_msc.music_num, 2);
}

#pragma location="MEMEEP"
void read_num_eeprom(void)
{
	eeprom_read_multi((mem_ctl.dev * EEPROM_OFFSET + PARAM_MUSIC_NUM_SD), (u8 *)&u_msc.music_num, 2);
}

#pragma location="MEMEEP"
void write_device_eeprom(void)
{
	eeprom_write(PARAM_ACTIVE_DEVICE,&t_fat.dev_sel,1);
}

#pragma location="MEMEEP"
void read_device_eeprom(void)
{
	sys_ctl.dev_save = eeprom_read(PARAM_ACTIVE_DEVICE);
}

#if USE_BREAK_POINT
#pragma location="MEMEEP"
void write_music_break_eeprom(void)
{
	u8 addr;

	addr = PARAM_MUSIC_POS_SD + mem_ctl.dev * EEPROM_OFFSET;

	eeprom_write(addr,(u8 *)&u_msc.break_point.pos,4);
	eeprom_write(addr+4,(u8 *)&u_msc.break_point.frame,4);
	eeprom_write(addr+8, (u8 *)&u_msc.filename_checksum,2);
}

#pragma location="MEMEEP"
void read_music_break_eeprom(void)
{
	u8 addr;

	addr = PARAM_MUSIC_POS_SD + mem_ctl.dev * EEPROM_OFFSET;

	eeprom_read_multi(addr,(u8 *)&u_msc.break_point.pos,4);
	eeprom_read_multi(addr+4,(u8 *)&u_msc.break_point.frame,4);
	eeprom_read_multi(addr+8, (u8 *)&u_msc.filename_checksum,2);
}
#endif

#pragma location="MEMEEP"
void write_total_channel_eeprom(void)
{
	eeprom_write(EEPROM_FM_TOTAL_CHANNEL,&fm_ctl.ch_total,1);
}

#pragma location="MEMEEP"
void write_cur_channel_eeprom(void)
{
	eeprom_write(EEPROM_CUR_CHANNEL,&fm_ctl.ch_cur,1);

	u8 freq = fm_ctl.fm_freq - FM_MIN_FREQ;
	eeprom_write(PARAM_FREQUENCY,&freq,1);
}

#pragma location="MEMEEP"
void write_fm_info_eeprom(u8 byte_value)
{
	eeprom_write(EEPROM_FREQUENCY_BUF+byte_value,&fm_ctl.ch_buf[byte_value],1);
}

#pragma location="MEMEEP"
void read_fm_info_eeprom(void)
{
	/*for(u8 i=0; i < CH_BUF_MAX;i++) {
	    fm_ctl.ch_buf[i] = eeprom_read(EEPROM_FREQUENCY_BUF+i);
	}*/
	eeprom_read_multi(EEPROM_FREQUENCY_BUF, fm_ctl.ch_buf, CH_BUF_MAX);

	fm_ctl.ch_total = eeprom_read(EEPROM_FM_TOTAL_CHANNEL);
	fm_ctl.ch_cur = eeprom_read(EEPROM_CUR_CHANNEL);
	fm_ctl.fm_freq = eeprom_read(PARAM_FREQUENCY);
}
#endif
