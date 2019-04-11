/*****************************************************************************
 * Module    : Task
 * File      : task_music.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ���ֲ�����������
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
	u8 mode;                        //�����л�ģʽ
#endif
	u8 poweron;                     //�ϵ��־���������䲥���豸
} type_t_msc;

extern type_t_msc t_msc;

void task_music_display(void);
bool task_music_scan_music(void);
bool task_music_scan_voice(void);
void task_music_set_a(void);
void task_music_repeat_play(void);
void task_music_break_save(bool flag);  //����ϵ�
void task_music_break_read(void);  //��ȡ�ϵ�
void task_music_break_init(void);  //�����������������ϵ㲥��

#endif
