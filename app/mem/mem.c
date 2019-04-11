/*****************************************************************************
 * Module    : mem
 * File      : mem.c
 * Author    :
 * Email     :
 * Function  : ������������
 *****************************************************************************/
#include "include.h"
#include "mem_rtcram.h"
#include "mem_internal.h"
#include "mem_eeprom.h"
#include "mem_device.h"

type_mem_ctl mem_ctl;


#pragma location="MEM"
void mem_event(void)
{
	if(mem_ctl.delay_time == 1)
	{
		//��ʱ��
		mem_ctl.delay_time = 0;

#if (SYS_MEM == MEM_RTCRAM)
		rtcram_write_sys_info();

#elif (SYS_MEM == MEM_INTERNAL)
		internal_write_sys_info();

#elif (SYS_MEM == MEM_E2PROM)
		eep_write_sys_info();

#elif (SYS_MEM == MEM_DEVICE)
		dev_write_info();
#endif
	}

#if (SYS_MEM == MEM_INTERNAL)
#if USE_BREAK_POINT
	if(task_ctl.work_sta == TASK_MUSIC)
	{
		if(mem_ctl.erase_time == 1)
		{
			mem_ctl.erase_time = 0;
			if(mem_ctl.erase_sta == NEED_RE_ERASE)
			{
				mem_ctl.erase_sta = BEING_ERASE;
			}
		}

		erase_music_break_internal();
	}
	else
	{
		mem_ctl.erase_time = 0;
	}
#endif
#endif

	mem_ctl.dev = fs_cur_dev();
}

#if ((SYS_MEM == MEM_RTCRAM) || (SYS_MEM == MEM_INTERNAL))
#pragma location="MEM"
void var_set_default(void)
{
	sys_ctl.volume = VOLUME_MAX/2+9;;		// Merlin 2015/06/06
	sys_ctl.dev_save = DEVICE_SDMMC;	// Merlin 2015/06/29 �⿨����,��Ϊϵͳ������л��豸����

#if USE_BREAK_POINT
	u_msc.music_num = 0;
	mem_ctl.dev = 1; task_music_break_save(0); write_num_rtcram();
	mem_ctl.dev = 0; task_music_break_save(0); write_num_rtcram();
	
#endif

#if TASK_FM_EN
	fm_ctl.ch_total = 1;
	fm_ctl.ch_cur = 1;
	fm_ctl.fm_freq = FM_MIN_FREQ;
	mem_fmbuf_clr();
	fm_ctl.ch_buf[0] = 0x01;
#if ((SYS_MEM == MEM_RTCRAM) || (SYS_MEM == MEM_E2PROM))
	write_fm_info(0);
#endif
#endif

#if (SYS_MEM == MEM_RTCRAM)    //��һ���ϵ��ʼ��RTCRAM
	write_volume_rtcram();
	write_device_rtcram();
	sys_ctl.mode_play = ITEM_LOOP_SINGLE; //�ϵ粥��ģʽΪ����ѭ��
	write_playmode_rtcram();
#if TASK_FM_EN
	write_total_channel_rtcram();
	write_cur_channel_rtcram();
#endif
#endif


}
#endif

#pragma location="MEM"
void mem_read_init_info(void)
{
#if (SYS_MEM == MEM_RTCRAM)
	if(!(rtcram_poweron_check()))
	{
		return;
	}

	read_volume_rtcram();

#if 0	// ǿ�����⿨Ϊ����, ���豸���书��
	read_device_rtcram();
#else
	sys_ctl.dev_save = DEVICE_SDMMC;	// Merlin 2015/06/29 �⿨����,��Ϊϵͳ������л��豸����
#endif

#if TASK_FM_EN
	read_fm_info_rtcram();
#endif

#elif (SYS_MEM == MEM_INTERNAL)
	if(!(internal_poweron_check()))  //����ʹ��Ĭ��ֵ
	{
		var_set_default();
		return;
	}

	read_volume_internal();
	read_device_internal();
#if TASK_FM_EN
	read_fm_info_internal();
#endif

#elif (SYS_MEM == MEM_E2PROM)
	read_volume_eeprom();
	read_device_eeprom();
#if TASK_FM_EN
	read_fm_info_eeprom();
#endif

#elif (SYS_MEM == MEM_DEVICE)
	device_mount();
	dev_read_info();
#endif
#if 0
#if 1	// ����ֻʹ�ù̶��ļ���, ��������
	sys_ctl.volume = VOLUME_MAX;	// Merlin 2015/06/06
#else
	if(sys_ctl.volume > VOLUME_MAX)
	{
		sys_ctl.volume = VOLUME_MAX;
	}
	else if(sys_ctl.volume < 5)
	{
		sys_ctl.volume = 10;
	}
#endif
#endif
	if(sys_ctl.dev_save >= MEDIA_DEVICE_NUM)
	{
		sys_ctl.dev_save = DEVICE_INVALID;
	}

	// 2015/06/29 ����ѭ��ģʽ�ļ��书��
	rtcram_read_multi(RTCRAM_PLAY_MODE,(u8 *)&sys_ctl.mode_play, 1);	// ����ѭ��ģʽ, ONE/ALL

	if((sys_ctl.mode_play != ITEM_LOOP_SINGLE) && (sys_ctl.mode_play != ITEM_LOOP_ALL))
	{
		sys_ctl.mode_play = ITEM_LOOP_SINGLE;
	}

#if TASK_FM_EN
	if(fm_ctl.fm_freq > (FM_MAX_FREQ - FM_MIN_FREQ))
	{
		fm_ctl.fm_freq = FM_MIN_FREQ;
	}
	else
	{
		fm_ctl.fm_freq += FM_MIN_FREQ;
	}

	if(fm_ctl.ch_cur > fm_ctl.ch_total || fm_ctl.ch_cur == 0 || fm_ctl.ch_total > 26*8 /*|| fm_ctl.ch_total == 0 �����ж�*/)
	{
		fm_ctl.ch_cur = 1;
		fm_ctl.ch_total = 1;
	}
#endif
}

#if (SYS_MEM == MEM_INTERNAL)
#pragma inline=forced
#pragma location="MEM"
void internal_write_sys_info(void)
{
	if(mem_ctl.change_flag & BIT(MEM_NUM_BIT))
	{
		mem_ctl.change_flag &= ~BIT(MEM_NUM_BIT);
		write_num_internal();
	}
	if((mem_ctl.change_flag & BIT(MEM_VOL_BIT))
	        ||(mem_ctl.change_flag & BIT(MEM_DEV_BIT)))
	{
		mem_ctl.change_flag &= ~(BIT(MEM_VOL_BIT) | BIT(MEM_DEV_BIT));
		write_volume_device_internal();
	}
#if TASK_FM_EN
	if(mem_ctl.change_flag & BIT(MEM_FRE_BIT))
	{
		mem_ctl.change_flag &= ~BIT(MEM_FRE_BIT);
		write_cur_channel_internal();
	}
#endif
}
#endif

#if (SYS_MEM == MEM_E2PROM)
#pragma inline=forced
#pragma location="MEM"
void eep_write_sys_info(void)
{
	if(mem_ctl.change_flag & BIT(MEM_VOL_BIT))
	{
		mem_ctl.change_flag &= ~BIT(MEM_VOL_BIT);
		write_volume_eeprom();
	}
}
#endif

#if (SYS_MEM == MEM_RTCRAM)
#pragma inline=forced
#pragma location="MEM"
void rtcram_write_sys_info(void)
{
	if(mem_ctl.change_flag & BIT(MEM_VOL_BIT))
	{
		mem_ctl.change_flag &= ~BIT(MEM_VOL_BIT);
		write_volume_rtcram();
	}
}
#endif
	
#pragma location="MEM"
void mem_write_file_change(void)
{
#if (SYS_MEM == MEM_RTCRAM)
	write_num_rtcram();                           //�л��ļ��ɹ�������д��Ŀ

#elif (SYS_MEM == MEM_INTERNAL)
	mem_ctl.delay_time = MEM_DELAY;               //internal����ʱ����
	mem_ctl.change_flag |= BIT(MEM_NUM_BIT);

#elif (SYS_MEM == MEM_E2PROM)
	write_num_eeprom();                           //�л��ļ��ɹ�������д��Ŀ

#elif (SYS_MEM == MEM_DEVICE)
	mem_ctl.delay_time = 0;
	dev_write_info();                             //U/SD�洢�����ļ�ʱ����
#endif
}

#pragma location="MEM"
void mem_read_music_num(void)
{
#if (SYS_MEM == MEM_RTCRAM)
	read_num_rtcram();

#elif (SYS_MEM == MEM_INTERNAL)
	read_num_internal();

#elif (SYS_MEM == MEM_E2PROM)
	read_num_eeprom();

#elif (SYS_MEM == MEM_DEVICE)
	dev_read_music_num();
#endif

	if((u_msc.music_num == 0) || (u_msc.music_num > (u_msc.music_total - 1)))
	{
		u_msc.music_num = 1;
	}
}

#pragma location="MEM"
void mem_write_device(void)
{

	if(task_ctl.work_sta == TASK_FLASH)
	{
		sys_ctl.dev_save = DEVICE_SDMMC1;
	}
	else
	{
		sys_ctl.dev_save = t_fat.dev_sel;
	}

#if (SYS_MEM == MEM_RTCRAM)
	write_device_rtcram();

#elif (SYS_MEM == MEM_INTERNAL)
	mem_ctl.delay_time = MEM_DELAY;               //internal����ʱ����
	mem_ctl.change_flag |= BIT(MEM_DEV_BIT);

#elif (SYS_MEM == MEM_E2PROM)
	write_device_eeprom();
#endif
}

#if TASK_FM_EN
//д����̨������ǰ��̨��š���ǰƵ��
#pragma location="MEM"
void mem_write_channel(void)
{
#if (SYS_MEM == MEM_RTCRAM)
	write_total_channel_rtcram();
	write_cur_channel_rtcram();

#elif (SYS_MEM == MEM_INTERNAL)
	write_total_channel_internal();
	write_cur_channel_internal();

#elif (SYS_MEM == MEM_E2PROM)
	write_total_channel_eeprom();
	write_cur_channel_eeprom();

#elif (SYS_MEM == MEM_DEVICE)
	mem_ctl.delay_time = 1;

#endif
}

//д��FM��ǰ��̨��ź�Ƶ��
void mem_write_cur_channel(void)
{
#if (SYS_MEM == MEM_RTCRAM)
	write_cur_channel_rtcram();

#elif (SYS_MEM == MEM_INTERNAL)
	mem_ctl.delay_time = MEM_DELAY;               //internal����ʱ����
	mem_ctl.change_flag |= BIT(MEM_FRE_BIT);

#elif (SYS_MEM == MEM_E2PROM)
	write_cur_channel_eeprom();

#elif (SYS_MEM == MEM_DEVICE)
	mem_ctl.delay_time = MEM_DELAY;

#endif
}

//д��FM�Զ���̨��Ϣ
#pragma location="MEM"
void mem_write_fm(void)
{
#if FM_SEEK_50K_STEP
	u16 byte_idx;
	u8 bit_idx;
	byte_idx = (fm_ctl.fm_freq - FM_MIN_FREQ)/5;
	bit_idx = byte_idx % 8;
	byte_idx = byte_idx / 8;

#else
	u8 byte_idx, bit_idx;
	bit_idx = (u8)(fm_ctl.fm_freq - FM_MIN_FREQ);

	byte_idx = bit_idx / 8;
	bit_idx = bit_idx % 8;
#endif
	if(fm_ctl.fm_freq <= FM_MAX_FREQ)
	{
		fm_ctl.ch_buf[byte_idx] |= (1<<bit_idx);
	}
	//printf("ch_buf[%d] = %02x\n",byte_idx,fm_ctl.ch_buf[byte_idx]);

	write_fm_info(byte_idx);
}

//�������ģ�飨�������ü���ģ��Ĳ�дʱ�䳤��������������
#pragma location="MEM"
void mem_fmbuf_clr(void)
{
	memset(fm_ctl.ch_buf,0x00,CH_BUF_MAX);
#if ((SYS_MEM == MEM_RTCRAM) || (SYS_MEM == MEM_E2PROM))
	for(u8 i = 0; i < CH_BUF_MAX; i++)
	{
		write_fm_info(i);
	}
#endif
}
#endif

#pragma location="MEM"
void mem_time_set(void)
{
	if(mem_ctl.delay_time > 1)
	{
		mem_ctl.delay_time--;
	}

#if ((SYS_MEM == MEM_INTERNAL) && USE_BREAK_POINT)
	if((mem_ctl.erase_time > 1) && (mem_ctl.erase_sta == NEED_RE_ERASE))
	{
		mem_ctl.erase_time--;
	}
#endif
}
