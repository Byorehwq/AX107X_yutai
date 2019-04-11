/*****************************************************************************
 * Module    : mem
 * File      : mem_device.c
 * Author    :
 * Email     :
 * Function  : DEVICE������������
 *****************************************************************************/
#include "include.h"
#include "mem_device.h"

#if (SYS_MEM == MEM_DEVICE)

//������Ϣ�洢��U��/SD���е���ʼ��ַ
#define DEV_START_ADDR  0x1C0

u32 mem_sect;

/*----------------------------------------------------------------------------*/
/**@brief  ����豸DBR������
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="MEMDEVICE"
void dev_check_dbr(void)
{
	if(fat_fs.s_size != 512)
	{
		mem_ctl.dev_mem_ready = 0;
		//printf("fs_read_only\n");
		return;
	}

	if(fat_fs.fs_type == FS_EXFAT)
	{
		mem_sect = fat_fs.database - LD_DWORD(&fs_hook.win[BPB_ClusterHeapOffset]);
	}
	else
	{
		mem_sect = fat_fs.fatbase - LD_WORD(&fs_hook.win[BPB_RsvdSecCnt]);
	}

	if(LD_WORD(&fs_hook.win[BPB_RsvdSecCnt]) > 1)
	{
		mem_sect += 1;
	}
	if(fat_fs.fs_type == FS_EXFAT)
	{
		if(fs_hook.win[BPB_FatOffset] != 24)
		{
			mem_sect += 24;
		}
	}

	mem_ctl.dev_mem_ready = 1;
}

/*----------------------------------------------------------------------------*/
/**@brief  ���豸��ȡ��ʼ����Ϣ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="MEMDEVICE"
void dev_read_info(void)
{
	if(!mem_ctl.dev_mem_ready)
	{
		//printf("dev_mem_err\n");
		return;
	}

	if(fs_hook.disk_read(fs_hook.win, mem_sect) == RES_OK)
	{
#if TASK_FM_EN
		fm_ctl.fm_freq = fs_hook.win[DEV_START_ADDR + PARAM_FREQUENCY];
#endif
		sys_ctl.volume = fs_hook.win[DEV_START_ADDR + PARAM_VOLUME];
	}
}

/*----------------------------------------------------------------------------*/
/**@brief  ���豸��ȡ��Ŀ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="MEMDEVICE"
void dev_read_music_num(void)
{
	if(!mem_ctl.dev_mem_ready)
	{
		//printf("dev_mem_err\n");
		return;
	}

	if(fs_hook.disk_read(fs_hook.win, mem_sect) == RES_OK)
	{
		u_msc.music_num = fs_hook.win[DEV_START_ADDR + PARAM_MUSIC_NUM + 1];
		u_msc.music_num <<=8;
		u_msc.music_num += fs_hook.win[DEV_START_ADDR + PARAM_MUSIC_NUM];
	}
	else
	{
		u_msc.music_num = 1;
	}
}

/*----------------------------------------------------------------------------*/
/**@brief  д��Ϣ���豸
   @param ��
   @return ��
   @note �����������
*/
/*----------------------------------------------------------------------------*/
#pragma location="MEMDEVICE"
void dev_write_info(void)
{
	if((!mem_ctl.dev_mem_ready) || (fs_read_only()))
	{
		//printf("dev_mem_err\n");
		return;
	}
	bool ie_soft = IE_SINT;
	IE_SINT = 0;
	if(fs_hook.disk_read(fs_hook.win, mem_sect) == RES_OK)
	{
#if TASK_FM_EN
		fs_hook.win[DEV_START_ADDR + PARAM_FREQUENCY] = fm_ctl.fm_freq - FM_MIN_FREQ;
#endif
		fs_hook.win[DEV_START_ADDR + PARAM_VOLUME] = sys_ctl.volume;
		if(task_ctl.work_sta == TASK_MUSIC)
		{
			fs_hook.win[DEV_START_ADDR + PARAM_MUSIC_NUM] = (u8)u_msc.music_num;
			fs_hook.win[DEV_START_ADDR + PARAM_MUSIC_NUM + 1] = (u8)(u_msc.music_num>>8);
		}
		fs_hook.disk_write(fs_hook.win, mem_sect);
	}

	IE_SINT = ie_soft;
}

#endif

