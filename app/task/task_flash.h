#ifndef TASK_FLASH_H
#define TASK_FLASH_H

enum
{
    VOICE,              //ÉlNo
    CHILDREN,           //¶løc
    ENGLISH,            //Ó¢Ól
    STORY,              //¹ZZA
    HABIT,              //ŠOønL°¹?    POEM,               //Z«øc
    POEM,               //Z«øc
    JOKE,               //Š¦»°
};


//ŠŽøÄ×ZO´ÖŠø÷øö·ÖAaµÄEsÄæróŠcNRŠŽøÄ´Erz
#define VOICE_TOTAL         7
#define CHILDREN_TOTAL      2
#define ENGLISH_TOTAL       2
#define STORY_TOTAL         1
#define HABIT_TOTAL         4
#define POEM_TOTAL          5
#define JOKE_TOTAL          6

#define Low_Battery_Num     6

#define ALL_MP3_TOTAL       (VOICE_TOTAL+CHILDREN_TOTAL+ENGLISH_TOTAL+STORY_TOTAL+HABIT_TOTAL+POEM_TOTAL+JOKE_TOTAL)

extern u16 flash_music_total;
extern bool device_c;
extern bool alarm_flag;
extern void task_music_cal_time(void);
extern void task_music_display(void);

void task_flash_music_display(void);
u16 get_dirstart(void);
u16 get_dirtotal(void);
bool task_flash_music_init(u16 music_num);
void task_flash_music_prev(void);
void task_flash_music_next(void);
void task_flash_music_sel_mode(void);
void task_flash_music_new(void);
void task_flash_music_auto_next(void);
void task_flash_music_deal_msg(u8 msg);
void task_flash_music_event(void);
void task_flash_music_enter(void);
void task_flash_music_exit(void);
void task_flash_music(void);

#endif







