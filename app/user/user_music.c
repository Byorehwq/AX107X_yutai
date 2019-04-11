/*****************************************************************************
 * Module    : User
 * File      : user_music.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户音乐播放控制
 *****************************************************************************/
#include "include.h"

type_user_music u_msc;
IAR_BDATA_A type_user_music_flag b_msc;


//音乐解码
#pragma location="MUSIC_SEG"
void music_decode(void)
{
	if(msc_ctl.status == STATUS_PLAY_PLAYING || msc_ctl.status == STATUS_PLAY_FAST)
	{
#if AB_REPEAT_EN
		if(msc_ctl.status == STATUS_PLAY_PLAYING)
		{
			if(!music_repeat_hook())
			{
				return;
			}
		}
#endif

#if WAV_SUPPORT
		switch(msc_ctl.type)
		{
		case TYPE_MP3:
			msc_mp3_decode();
			break;

		case TYPE_WAV:
			msc_wav_decode();
			break;
		default:
			break;
		}
#else
		msc_mp3_decode();
#endif
	}
}

#pragma location="MUSIC_SEG"
void music_set_jmp(void)
{
#if WAV_SUPPORT
	if(msc_ctl.type == TYPE_WAV)
	{
		wav_set_jump();
	}
	else
	{
		mp3_set_jump();
	}
#else
	mp3_set_jump();
#endif
}

bool is_wma_file(void);
u8 *decoder_peek(void);

#if !WAV_SUPPORT
IAR_CONST u8 wav_header[4] =
{
	'R', 'I', 'F', 'F',
};
#endif

#pragma location="MUSIC_SEG"
bool is_other_file(void)
{
	if(is_wma_file())
	{
		return true;    //WMA
	}
#if !WAV_SUPPORT
	else if(0 == smemcmp_c(decoder_peek(), wav_header, 4))
	{
		return true;    //WAV
	}
#endif
	return false;
}

/*----------------------------------------------------------------------------*/
/**@brief  解码软中断
   @param  无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma optimize=speed high
#pragma vector = VECTOR_OFFSET(SOFT_INT)		//soft int for decode
#pragma register_bank=1
#pragma location="INTERRUPT"
__interrupt void soft_isr(void)
{
	ISR_ENTER();

	CLR_SOFT_INT();
	WATCHDOG_CLR();
	music_decode();

	ISR_EXIT();
}

/*----------------------------------------------------------------------------*/
/**@brief  MP3 解码中断
   @param  无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma optimize=speed high
#pragma vector=VECTOR_OFFSET(AUDIO_INT)
#pragma register_bank=1
#pragma location="MUSIC_SEG"
__interrupt void mp3_isr(void)
{
	ISR_ENTER();

#if WAV_SUPPORT
	if(msc_ctl.type == TYPE_MP3)
	{
		//MP3硬件中断
		mp3_synth_check();
		KICK_SOFT_INT();
	}
	else if(msc_ctl.type == TYPE_WAV)
	{
		//WAV DAC缺数中断
		AUCON8 &= ~BIT(2);
		KICK_SOFT_INT();
	}
#else
	//MP3硬件中断
	mp3_synth_check();
	KICK_SOFT_INT();
#endif

	ISR_EXIT();
}

#pragma location="TASK_MUSIC_SEG"
bool device_switch(void)
{
	if(fs_cur_dev() == DEVICE_SDMMC1)	// 内插卡
	{
		sd_stop(1);
		if(device_activate(DEVICE_UDISK))
		{
			t_fat.dev_new = DEVICE_UDISK;
			return true;
		}
#if UDISK_DOUBLE_EN
		else  if(device_activate(DEVICE_UDISK1))
		{
			t_fat.dev_new = DEVICE_UDISK1;
			return true;
		}
#endif
		else  if(device_is_actived(DEVICE_SDMMC))	// 外接卡
		{
			t_fat.dev_new = DEVICE_SDMMC;
			return true;
		}
	}
	else if(fs_cur_dev() == DEVICE_UDISK)
	{
#if UDISK_DOUBLE_EN
		if(device_activate(DEVICE_UDISK1))
		{
			t_fat.dev_new = DEVICE_UDISK1;
			return true;
		}
		else
#endif
			if(device_is_actived(DEVICE_SDMMC))
			{

				t_fat.dev_new = DEVICE_SDMMC;
				return true;
			}
			else  if(device_is_actived(DEVICE_SDMMC1))
			{

				t_fat.dev_new = DEVICE_SDMMC1;
				return true;
			}
	}
#if UDISK_DOUBLE_EN
	else if(fs_cur_dev() == DEVICE_UDISK1)
	{
		if(device_is_actived(DEVICE_SDMMC))
		{
			t_fat.dev_new = DEVICE_SDMMC;
			return true;
		}
		else if(device_is_actived(DEVICE_SDMMC1))
		{
			t_fat.dev_new = DEVICE_SDMMC1;
			return true;
		}
		else if(device_activate(DEVICE_UDISK))
		{
			t_fat.dev_new = DEVICE_UDISK;
			return true;
		}
	}
#endif
	else if(fs_cur_dev() == DEVICE_SDMMC)	// 外接卡
	{
		sd_stop(1);
		if(device_is_actived(DEVICE_SDMMC1))	// 内接卡
		{
			t_fat.dev_new = DEVICE_SDMMC1;
			return true;
		}
		else if(device_activate(DEVICE_UDISK))
		{
			t_fat.dev_new = DEVICE_UDISK;
			printf("UDISK NEXT \n");
			return true;
		}
#if UDISK_DOUBLE_EN
		else if(device_activate(DEVICE_UDISK1))
		{
			t_fat.dev_new = DEVICE_UDISK1;
			return true;
		}
#endif
	}
	return false;
}

#pragma location="TASK_MUSIC_SEG"
bool device_mount(void)
{
	fs_sel_new();
#if (SYS_MEM == MEM_DEVICE)
	mem_ctl.dev_mem_new = 0;                //挂载后，清除新记忆设备标志
#endif
	if(fs_need_mounted())
	{
		if(fs_init())
		{
			mem_ctl.dev = fs_cur_dev();

#if (SYS_MEM == MEM_DEVICE)
			dev_check_dbr();                //记忆到设备时，检查是否可用
#else
			mem_write_device();             //保存当前播放设备
#endif
		}
	}
	if(!fs_is_mounted())
	{
		return false;
	}
	if(!device_is_actived(fs_cur_dev()))
	{
		return false;
	}
	return true;
}

#pragma location="USER_MUSIC_SEG"
bool user_music_enter(void)
{
	device_change();

	if(t_msc.poweron)
	{
		t_msc.poweron = 0;
		if(sys_ctl.dev_save < MEDIA_DEVICE_NUM)
		{
			t_fat.dev_new = sys_ctl.dev_save;
		}
	}

	if(device_mount())                      //挂载文件系统
	{
		return true;
	}

//	if(device_switch() && device_mount())   //选择下一设备挂载文件系统
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
	return true;
}

//dir = 0: inc, dir = 1: dec
#pragma location="MUSIC_SEG"
u16 loop_auto(bool dir, u16 cur, u16 min, u16 max)
{
	if(dir)
	{
		if(cur > max || cur <= min)
		{
			return max;
		}
		return (cur - 1);
	}
	else
	{
		if(cur >= max || cur < min)
		{
			return min;
		}
		return (cur + 1);
	}
}
