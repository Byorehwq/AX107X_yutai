/*****************************************************************************
 * Module    : User
 * File      : file_filter.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 文件扫描过滤器
 *****************************************************************************/
#include "include.h"

#if WAV_SUPPORT
	#if MPX_SUPPORT
		#define MUSIC_EXT_NUM 3     //一共搜索3个扩展名
		#pragma constseg="FILE_FILTER_CONST"
		IAR_CONST char tbl_music_ext[MUSIC_EXT_NUM][4] = {"mp3", "mpx", "wav"};
		bool f_Encrypt; 	// 1: is Encrypt MP3, 0: is normal MP3
	#else
#define MUSIC_EXT_NUM 2     //一共搜索2个扩展名
#pragma constseg="FILE_FILTER_CONST"
IAR_CONST char tbl_music_ext[MUSIC_EXT_NUM][4] = {"mp3", "wav"};
	#endif
#else
	#if MPX_SUPPORT
		#define MUSIC_EXT_NUM 2 	//一共搜索2个扩展名
		#pragma constseg="FILE_FILTER_CONST"
		IAR_CONST char tbl_music_ext[MUSIC_EXT_NUM][4] = {"mp3", "mpx"};
		bool f_Encrypt; 	// 1: is Encrypt MP3, 0: is normal MP3
	#else
#define MUSIC_EXT_NUM 1     //一共搜索1个扩展名
#pragma constseg="FILE_FILTER_CONST"
IAR_CONST char tbl_music_ext[MUSIC_EXT_NUM][4] = {"mp3"};
	#endif
#endif
#pragma constseg=default

#pragma location="FILE_FILTER_SEG"
u8 char2value(unsigned char c)
{
	u8 value;
	value = c - '0';
	return (value>9)?0:value;
}

//判断文件是否为需要查找的文件
#pragma location="FILE_FILTER_SEG"
bool file_is_target_number(void)
{
	if(t_fat.file_number == t_fat.file_count)
	{
		return true;
	}
	return false;
}

#if MPX_SUPPORT
#pragma location="FILE_FILTER_SEG"
bool file_is_Encrypt_music(char *ext_name)
{
	f_Encrypt = false;
	if(	(ext_name[0] == tbl_music_ext[1][0]) &&
		(ext_name[1] == tbl_music_ext[1][1]) &&
		(ext_name[2] == tbl_music_ext[1][2]))
	{
		f_Encrypt = true;
	}
	return f_Encrypt;
}
#endif
#pragma location="FILE_FILTER_SEG"
bool file_is_music(char *ext_name)
{
#if MPX_SUPPORT
	file_is_Encrypt_music(ext_name);
#endif
	return f_check_ext(ext_name, tbl_music_ext, MUSIC_EXT_NUM);
}

//音乐播放的文件过滤器
#pragma location="FILE_FILTER_SEG"
bool file_filter_music(void)
{
	if(!f_attr_is_file())
	{
		return false;
	}

	char ext_name[3];
	get_ext_name(ext_name);
	//在EXFAT中，后缀名可能为小写
	if(f_check_ext(ext_name, tbl_music_ext, MUSIC_EXT_NUM))     //检测是否为音乐文件
	{

#if DIR_SWITCH_EN
		/* 此段统计文件夹编号，不用时可屏蔽 */
		if(t_fat.file_count == t_fat.t_item.before_files)
		{
			t_fat.dir_count++;
			if(t_fat.dir_number == t_fat.dir_count)
			{
				t_fat.file_number = t_fat.t_item.before_files+1;
			}
		}
#endif
#if MPX_SUPPORT
		file_is_Encrypt_music(ext_name);
#endif
		t_fat.file_count++;
#if FS_SCAN_SPEED_EN
		fs_save_dpath();
#endif
		if(t_fat.file_number == t_fat.file_count)               //检测是否找到目标曲目文件
		{
			return true;
		}
	}
	return false;
}

//音乐播放的目录过滤器
#pragma location="FILE_FILTER_SEG"
bool dir_filter_music(void)
{
	if(!f_attr_is_dir())
	{
		return false;
	}
	return true;
}

#pragma location="FILE_FILTER_SEG"
u8 get_scan_dept(void)
{
	return t_fat.t_item.dir_depth;
}


