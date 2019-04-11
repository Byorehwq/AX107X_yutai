/*****************************************************************************
 * Module    : API
 * File      : api_music.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : 音乐播放API接口
 *****************************************************************************/

#ifndef API_MUSIC_H
#define API_MUSIC_H

typedef struct
{
	u32 frame;
	u32 pos;
} type_music_point;

typedef struct
{
	type_music_point pa;
	type_music_point pb;
} type_music_point_ab;

typedef struct
{
	u8 min;
	u8 sec;
} type_play_time;

typedef struct
{
	u8 type;                                            //音乐格式
	u8 status;
	u32 cur_mtime;                                      //当前播放时间，单位为0.1s
	u8 repeat;                                          //重复次数

	type_music_point_ab point_ab;                       //AB点
	u16 all_time;

	u8 err_cnt;                                         //错误次数管理

	u8 mscfade_en;             //1:音乐播放模式的切换需要淡入淡出，0：不需要淡入淡出
} type_msc_ctl;

enum
{
    MUSIC_FAST_FORWORD,
    MUSIC_FAST_BACKWORD,
};

enum
{
    TYPE_NONE,             //不支持的格式
    TYPE_MP3,
    //TYPE_WMA,
    TYPE_WAV,
};


extern type_msc_ctl msc_ctl;




/*****************************************************************************
 * Module    : 音乐播放相关
 *****************************************************************************/

#define REPEAT_LOOP 0xff                                //无限循环复读
#define REPEAT_END  0xfe                                //复读一次后停止。用于跟读

u8 music_init(void);                                    //初始化音乐解码
u8 music_init_mp3_only(void);                           //初始化音乐解码，只初始化MP3格式

bool mp3_init(void);                                    //初始化MP3解码
bool mp3_init_spi(void);                                //初始化MP3解码的SPIFlash精简定制版
void mp3_synth_check(void);
void mp3_set_jump(void);
void wav_set_jump(void);

bool music_repeat_hook(void);
void msc_mp3_decode(void);                              //MP3解码，软中断调用
void msc_wav_decode(void);                              //WAV解码，软中断调用
void bt_decode(void);                                   //BT解码，软中断调用
void music_decode(void);                                //音频解码，软中断调用

void music_stop(void);                                  //停止音乐播放
void music_event(void);                                 //解码需要的事件
void music_play(void);                                  //开始播放
void music_pause(void);                                 //暂停
bool music_start_pause(void);                           //播放/暂停
void music_fast(bool direct, u8 sec);                   //快进/快退. direct = 0为快进，1为快退
void music_fast_end(void);                              //快进/快退结束，恢复播放
void music_get_point(type_music_point *point);          //获取当前播放位置
void music_set_a_point(type_music_point *point);        //设置A点，以便复读时间计算
//AB复读。repeat为复读次数。当repeat = REPEAT_LOOP，无限复读。为REPEAT_END时，复读一次停止
void music_repeat(u8 repeat, type_music_point_ab *pab);
void music_repeat_clear(void);                          //清除当前的复读状态，继续正常播放
void music_jump(type_music_point *point);               //跳至某点进行播放
void music_eq_init_m(void);                             //EQ系数初始化
void music_eq_set_m(u8 eq_num);                         //EQ渐变
void music_eq_setd_m(u8 eq_num);                        //直接设置EQ
void music_eq_init(void);                               //EQ系数初始化
void music_eq_set(u8 eq_num);                           //EQ渐变
void music_eq_setd(u8 eq_num);                          //直接设置EQ
u16 music_filename_check(void);                         //获取文件名checksum

//频率线初始化选择
enum
{
    FREQ_DAC,                   //计算DAC的频率线
    FREQ_ADC,                   //计算ADC的频率线
};
void music_freq_init(u8 freq_type);                     //初始化频率线, freq_type参考频率线初始化选择枚举
bool music_freq_get(void);                              //获取频率线
void music_freq_kick(void);                             //Kick下一次

//播放状态
enum
{
    STATUS_PLAY_STOP,                       //完全停止
    STATUS_PLAY_STOPPING,                   //正在停止
    STATUS_PLAY_PLAYING,                    //正在播放
    STATUS_PLAY_PAUSE,                      //暂停中
    STATUS_PLAY_FAST,                       //快进快退中
    STATUS_PLAY_CONTINUE,                   //等待再次KICK继续播放
};

u8 music_get_status(void);                              //获取音乐的播放状态
bool music_is_stop(void);                               //音乐是否已停止

u32 music_calc_mtime(u32 frame_count);
u32 music_get_mtime(void);
u32 music_point_a_mtime(void);
void spi_music_wait(void);                              //语音播放等待函数
void mp3_res_play(u8 music_name);                       //选择语言菜单播放
void mp3_res_play_wait(u8 music_name);                  //选择语言菜单播放,并等待播放完毕

#ifdef AX207X_TAG
void sound_ctl_init(u8 is_cnt, u32 is_pow, u8 silence_cnt, u32 silence_pow);
u8 sound_detect(u32 v_pow);
u32 voice_power(void *buf, u16 len);                    //获取一包声音的平均能量值
#else
void sound_ctl_init(u8 is_cnt, u8 is_pow, u8 silence_cnt, u8 silence_pow);
u8 sound_detect(u8 v_pow);
#endif
void sound_ctl_clr(void);
u8 sound_maxpow(void *ptr, u8 len);                     //获取一包声音的最大音量

#endif
