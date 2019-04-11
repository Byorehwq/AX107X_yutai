/*****************************************************************************
 * Module    : mem
 * File      : mem_internal.c
 * Author    :
 * Email     :
 * Function  : 内置记忆任务流程
 *****************************************************************************/
#include "include.h"
#include "mem_internal.h"

#if (SYS_MEM == MEM_INTERNAL)

//检查内置记模块是否第一次上电
#pragma location="MEMINT"
bool internal_poweron_check(void)
{
#if USE_BREAK_POINT
	mem_ctl.erase_sta = NEED_RE_ERASE;
#endif
	if(((internal_read_info(INTERNAL_MARK_1)) == 0x55)
	        && ((internal_read_info(INTERNAL_MARK_2)) == 0xAA))
	{
		return 1;
	}
	else
	{
		internal_sector_erase(INTERNAL_MARK_1);  //擦除两byte
		u16 write_value = 0xAA55;
		internal_write_info(INTERNAL_MARK_1, (u8 *)&write_value, 2);

		internal_sector_erase(INTERNAL_NUM_SD_ADDR); //用于记忆曲目地址的字节需要擦除
		internal_sector_erase(INTERNAL_NUM_USB_ADDR);
		//printf("internal first poweron.\n");
		return 0;
	}
}

//标记内置记忆已经损坏
#pragma location="MEMINT"
void internal_is_damage(void)
{
	mem_ctl.internal_sta = false;  //已损坏，不再保存
}

#pragma location="MEMINT"
u8 get_addr_offset(u8 para_tyte)
{
	u16 value;
	u8 addr_offset = 0;

	//para_tyte:  0-SD;1-USB
	value = internal_read_info(para_tyte * 2 + 1);
	value <<= 8;
	value += internal_read_info(para_tyte * 2);

	for(u8 i=0; i<16; i++)
	{
		if(value & BIT(0))
		{
			break;
		}
		else
		{
			value >>= 1;
			addr_offset++;
		}
	}
	if(addr_offset >= INTERNAL_OFFSET_MAX)
	{
		addr_offset = 0;
	}
	//printf("addr offset = %d\n",addr_offset);

	return addr_offset;
}

#pragma location="MEMINT"
u8 addr_offset_increase(u8 para_tyte)
{
	u8 next_addr_offset;
	u16 offset_value = 0xFFFF;

	next_addr_offset = get_addr_offset(para_tyte);
	next_addr_offset++;
	if(next_addr_offset >= INTERNAL_OFFSET_MAX)
	{
		next_addr_offset = 0;
		internal_sector_erase(para_tyte * 2);
	}

	for(u8 i=0; i<next_addr_offset; i++)
	{
		offset_value <<= 1;
	}

	internal_write_info(para_tyte * 2, (u8 *)&offset_value, 2);

	return next_addr_offset;
}

//写入音量值+当前播放设备值,2byte,每1次需要擦除
#pragma location="MEMINT"
void write_volume_device_internal(void)
{
	u16 value_vol_dev;

	value_vol_dev = t_fat.dev_sel;
	value_vol_dev <<= 8;
	value_vol_dev += sys_ctl.volume;

#if MEM_WRITE_COMPARE
	u16 read_value=0;
	internal_read_multi(INTERNAL_VOL_PARA,(u8 *)&read_value,2);  //读出上次值
	if(read_value == value_vol_dev)
	{
		return;                 //如果当前值与上次值相等，就不再写操作
	}
#endif

	internal_sector_erase(INTERNAL_VOL_PARA);
	internal_write_info(INTERNAL_VOL_PARA,(u8 *)&value_vol_dev,2);

#if MEM_WRITE_COMPARE
	internal_read_multi(INTERNAL_VOL_PARA,(u8 *)&read_value,2);  //读出本次写入的值
	if(read_value != value_vol_dev)
	{
		internal_is_damage();
	}
#endif
}

//读出音量值
#pragma location="MEMINT"
void read_volume_internal(void)
{
	sys_ctl.volume = internal_read_info(INTERNAL_VOL_PARA);
}

//读出当前播放设备值
#pragma location="MEMINT"
void read_device_internal(void)
{
	sys_ctl.dev_save = internal_read_info(INTERNAL_DEV_PARA);
}

//写入曲目值2byte,需要先擦除
#pragma location="MEMINT"
void write_num_internal(void)
{
	u8 addr;
	u16 num_temp;

	num_temp = u_msc.music_num; //保存当前值

#if MEM_WRITE_COMPARE
	read_num_internal();             //读出上次值
	if(num_temp == u_msc.music_num)
	{
		return;                 //如果当前值与上次值相等，就不再写操作
	}
#endif

	addr = INTERNAL_NUM_SD_PARA + mem_ctl.dev * INTERNAL_OFFSET
	       + addr_offset_increase(mem_ctl.dev) * 2;
	//printf("write num addr = %02x\n",addr);
	internal_sector_erase(addr);
	internal_write_info(addr, (u8 *)&num_temp, 2);

#if MEM_WRITE_COMPARE
	read_num_internal();             //读出本次写入的值
	if(num_temp != u_msc.music_num)
	{
		u_msc.music_num = num_temp;
		internal_is_damage();
	}
#endif
}

//读出曲目值
#pragma location="MEMINT"
void read_num_internal(void)
{
	u8 addr;

	addr = INTERNAL_NUM_SD_PARA + mem_ctl.dev * INTERNAL_OFFSET
	       + get_addr_offset(mem_ctl.dev) * 2;
	//printf("read num addr = %02x\n",addr);

	internal_read_multi(addr,(u8 *)&u_msc.music_num,2);
	//printf("read num = %02x\n",u_msc.music_num);
}

#if USE_BREAK_POINT
//读出音乐播放断点信息
#pragma location="MEMINT"
void read_music_break_internal(void)
{
	u8 addr;

	addr = INTERNAL_POS_SD_PARA + mem_ctl.dev * INTERNAL_OFFSET;

	internal_read_multi(addr,(u8 *)&u_msc.break_point.pos,4);
	internal_read_multi(addr+4,(u8 *)&u_msc.break_point.frame,4);
	internal_read_multi(addr+8,(u8 *)&u_msc.filename_checksum,2);
	//printf("read frame=%08lu pos=%08lu\n",u_msc.break_point.frame,u_msc.break_point.pos);
	//printf("read checksum = %04x\n",u_msc.filename_checksum);

	mem_ctl.read_dev = mem_ctl.dev;
	mem_ctl.erase_time = MEM_DELAY;
}

//写入音乐播放断点信息
#pragma location="MEMINT"
void write_music_break_internal(void)
{
	mem_ctl.erase_time = MEM_DELAY;

	u8 addr = INTERNAL_POS_SD_PARA + mem_ctl.dev * INTERNAL_OFFSET;

	if(mem_ctl.erase_sta < HAVE_ERASED)
	{
		for(u8 i=mem_ctl.erase_sta; i<5; i++)
		{
			mem_ctl.erase_sta++;
			internal_sector_erase(addr + i*2);  //由于连续擦除5个sector地址的时间较长，移到其它处擦除
		}
	}

	if(mem_ctl.erase_sta != HAVE_ERASED)
	{
		return;
	}

	mem_ctl.erase_sta = NEED_RE_ERASE;     //已写标识，需要重新擦除
	//printf("addr = %02x,addr+4=%02x\n",addr,addr+4);
	//printf("write frame=%08lu pos=%08lu\n",u_msc.break_point.frame,u_msc.break_point.pos);
	internal_write_info(addr,(u8 *)&u_msc.break_point.pos,4);
#if MEM_WRITE_COMPARE
	u32 read_value=0;
	internal_read_multi(addr,(u8 *)&read_value,4);
	if(read_value != u_msc.break_point.pos)
	{
		internal_is_damage();
		return;
	}
#endif

	internal_write_info(addr+4,(u8 *)&u_msc.break_point.frame,4);
#if MEM_WRITE_COMPARE
	internal_read_multi(addr+4,(u8 *)&read_value,4);
	if(read_value != u_msc.break_point.frame)
	{
		internal_is_damage();
		return;
	}
#endif

	//printf("write checksum = %04x\n",u_msc.filename_checksum);
	internal_write_info(addr+8, (u8 *)&u_msc.filename_checksum, 2);
#if MEM_WRITE_COMPARE
	internal_read_multi(addr+8,(u8 *)&read_value,2);
	if((u16)read_value != u_msc.filename_checksum)
	{
		internal_is_damage();
		return;
	}
#endif
}

//擦除用于存放断点信息的地址
#pragma location="MEMINT"
void erase_music_break_internal(void)
{
	if((mem_ctl.erase_sta < HAVE_ERASED) && (mem_ctl.read_dev == mem_ctl.dev))
	{
		u8 addr = INTERNAL_POS_SD_PARA + mem_ctl.dev * INTERNAL_OFFSET;

		internal_sector_erase(addr + mem_ctl.erase_sta*2);
		mem_ctl.erase_sta++;
	}
}
#endif

#if TASK_FM_EN
#pragma location="MEMINT"
void write_fm_info_internal(u8 byte_value)
{
	u8 start_addr;
	u8 erase_addr;

	byte_value = byte_value >> 1;

	if((mem_ctl.prev_ch_number) && (byte_value)
	        && (mem_ctl.prev_ch_number != byte_value))
	{
		start_addr = mem_ctl.prev_ch_number+1;
	}
	else
	{
		start_addr = mem_ctl.prev_ch_number;
	}
	//printf("start addr = %d\n",start_addr);

	for(u8 i=start_addr*2; i<(byte_value*2+1); i=i+2)
	{

		erase_addr = INTERNAL_FREQUENCY_BUF + i;

		//printf("erase add = %02x\n",erase_addr);
		internal_sector_erase(erase_addr);

		//printf("write add = %02x\n",erase_addr);
		internal_write_info(erase_addr,&fm_ctl.ch_buf[i],1);

		//printf("write add = %02x\n",erase_addr +1);
		internal_write_info(erase_addr + 1,&fm_ctl.ch_buf[i+1],1);
	}

	mem_ctl.prev_ch_number = byte_value;
}

#pragma location="MEMINT"
void write_cur_channel_internal(void)
{
	internal_sector_erase(INTERNAL_CUR_CHANNEL);
	internal_write_info(INTERNAL_CUR_CHANNEL,&fm_ctl.ch_cur,1);
#if FM_SEEK_50K_STEP
	internal_write_info(INTERNAL_FM_TOTAL_CHANNEL,&fm_ctl.ch_total,1);
	internal_sector_erase(INTERNAL_FREQUENCY);
	u8 freq_high,freq_low;
	u16 freq = (fm_ctl.fm_freq - FM_MIN_FREQ)/5;
	freq_high = (u8)(freq>>8);
	freq_low = (u8)freq;
	internal_write_info(INTERNAL_FREQUENCY,&freq_high,1);
	internal_write_info(INTERNAL_FREQUENCY+1,&freq_low,1);
#else
	u8 freq = fm_ctl.fm_freq - FM_MIN_FREQ;
	internal_write_info(INTERNAL_FREQUENCY,&freq,1);
#endif

}

#pragma location="MEMINT"
void write_total_channel_internal(void)
{
	internal_sector_erase(INTERNAL_FM_TOTAL_CHANNEL);
	internal_write_info(INTERNAL_FM_TOTAL_CHANNEL,&fm_ctl.ch_total,1);
}

#pragma location="MEMINT"
void read_fm_info_internal(void)
{

	for(u8 i=0; i<CH_BUF_MAX; i++)
	{
		fm_ctl.ch_buf[i] = internal_read_info(INTERNAL_FREQUENCY_BUF+i);
		//printf("buf[%d] = %02x\n",i,fm_ctl.ch_buf[i]);
	}

	fm_ctl.ch_total = internal_read_info(INTERNAL_FM_TOTAL_CHANNEL);
	fm_ctl.ch_cur = internal_read_info(INTERNAL_CUR_CHANNEL);
#if FM_SEEK_50K_STEP
	fm_ctl.fm_freq = 5*(internal_read_info(INTERNAL_FREQUENCY)+internal_read_info(INTERNAL_FREQUENCY+1));
#else
	fm_ctl.fm_freq = internal_read_info(INTERNAL_FREQUENCY);
#endif
	//printf("total = %d\n",fm_ctl.ch_total);
}
#endif

#endif
