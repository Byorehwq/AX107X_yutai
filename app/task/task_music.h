/*****************************************************************************
 * Module    : Task
 * File      : task_music.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 音乐播放任务流程
 *****************************************************************************/
#ifndef TASK_MUSIC_H
#define TASK_MUSIC_H

enum
{
    ITEM_LOOP_ALL ,
    ITEM_LOOP_RAMDOM,
    ITEM_LOOP_SINGLE,
    ITEM_LOOP_FOLDER,
};

typedef struct
{
#if MUSIC_SWITCH_DEV
	u8 mode;                        //用来切换模式
#endif
	u8 poweron;                     //上电标志，用来记忆播放设备
} type_t_msc;

extern type_t_msc t_msc;

void task_music_display(void);
bool task_music_scan_music(void);
bool task_music_scan_voice(void);
void task_music_set_a(void);
void task_music_repeat_play(void);
void task_music_break_save(bool flag);  //保存断点
void task_music_break_read(void);  //读取断点
void task_music_break_init(void);  //进入音乐任务跳至断点播放

#endif
