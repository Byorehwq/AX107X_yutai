/*****************************************************************************
 * Module    : user
 * File      : user_fat.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 文件系统接口，可以增加读接口的加密算法等
 *****************************************************************************/
#include "include.h"
#define XorValue	0x63		// 使用此值进行XOR运算

//音乐文件读接口
bool music_decoder_read(void *buf)
{
	u8 fr_sta;

	fr_sta = f_read(buf);
#if MPX_SUPPORT
	if(f_Encrypt)
	{
		u8 *buffer;
		register u16 i;
		i = 0;
		buffer = (u8 *) buf;
		do
		{
			*buffer = *buffer ^ XorValue;
			buffer++;
			i++;
		} while(i < 512);
	}
#endif
	if(FR_OK != fr_sta)
    {
#if WAV_SUPPORT
        if((FR_RW_ERROR == fr_sta) && (TYPE_WAV == msc_ctl.type))
        {
            IE_SINT = 0;
            msc_ctl.status = STATUS_PLAY_STOPPING;
        }
#endif
#if REPEAT_AS_ONE
        if((FR_END_FILE == fr_sta))
        {
            m_f_lseek(F_SEEK_ORG, 2);
            //f_read(buf);
            //printf("567");
            return true;
        }
        else
            return false;
#else
        if( (sys_ctl.mode_play == ITEM_LOOP_SINGLE) ||
            (u_msc.music_total == 1))
        {
            m_f_lseek(F_SEEK_ORG, 2);
            //f_read(buf);
            //printf("567");
            return true;
        }
        else
        {
		return false;
        }
#endif

    }
    return true;
}

//音乐文件读接口
bool decoder_read(void *buf)
{
//	printf("buf = %04x\n",buf);
#if PLAY_STARTMUSIC
	if(startmusic_ctl.flag)
	{
		return startmusic_read(buf);
	}
	else
#endif

#if TASK_FLASH_EN
		if(u_spi.spi_music_flag)
		{
			return spi_mp3flash_read(buf);
		}
		else
#endif
		{
			return music_decoder_read(buf);
		}
}

//音乐文件Seek接口
bool decoder_seek(u8 mode, u32 offset)
{
#if PLAY_STARTMUSIC
	if(startmusic_ctl.flag)
	{
		return startmusic_seek();
	}
	else
#endif

#if TASK_FLASH_EN
		if(u_spi.spi_music_flag)
		{
			return spi_mp3flash_seek(mode, offset);
		}
		else
#endif
		{
			return m_f_lseek((SEEK_MODE)mode, offset);
		}
}


#pragma location="FS_SCAN_SEG"
FRESULT fs_scan_files(void)
{
#if FS_SCAN_SPEED_EN
	return fs_scan_files_speed();
#else
	return fs_scan_files_no_speed();
#endif
}


