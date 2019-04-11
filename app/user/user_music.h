/*****************************************************************************
 * Module    : User
 * File      : user_music.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户音乐播放控制定义
 *****************************************************************************/
#ifndef USER_MUSIC_H
#define USER_MUSIC_H


typedef struct type_user_music_
{
	u16 music_num;                          //播放曲目编号, from 1 to N when SD/USB.  from 0 to flash_music_total-1 when used SPI Flash
	u16 music_num_last;						// 上一曲歌曲曲目号   20180314
	u16 music_total;                        //曲目总数
	u8 music_type;                          //音频格式
	type_play_time cur_time;                //当前播放时间
#if AB_REPEAT_EN
	type_music_point_ab point;              //记录AB点
	u8 repeat;                              //AB复读，0为不复读，1为设置A点，2为设置B点
#endif
#if USE_BREAK_POINT
	//别动！断点信息必须和checksum挨着放，保存与载入时，直接按块操作 ======
	type_music_point break_point;           //记录断点信息
	u16 filename_checksum;                  //记录歌曲名checksum
	//======
#endif
	u16 dir_num;                            //文件夹编号
	u16 dir_total;                          //文件夹总数
} type_user_music;

typedef struct type_user_music_flag_
{
	u8 file_change :    1;                  //文件改变，需要打开新文件
	u8 pause :          1;                  //是否处于暂停状态。主要为了方便一些条件判断
	u8 file_prev_next : 1;                  //文件方向，0为next，1为prev
#if USE_BREAK_POINT
	u8 break_init :     1;                  //播放设备改变，需要读出播放断点信息
#endif
#if REPEAT_AS_ONE
	u8 repeat_as_one_flag : 	1;
#endif
	u8 stop       :     1;                  //红外模式下，stop按键的标志位
} type_user_music_flag;

extern type_user_music u_msc;
extern IAR_BDATA_A type_user_music_flag b_msc;

bool user_music_enter(void);

bool device_switch(void);
bool device_mount(void);

u16 loop_auto(bool dir, u16 cur, u16 min, u16 max);
#endif
