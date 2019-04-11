/*****************************************************************************
 * Module    : API
 * File      : api_music.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : ���ֲ���API�ӿ�
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
	u8 type;                                            //���ָ�ʽ
	u8 status;
	u32 cur_mtime;                                      //��ǰ����ʱ�䣬��λΪ0.1s
	u8 repeat;                                          //�ظ�����

	type_music_point_ab point_ab;                       //AB��
	u16 all_time;

	u8 err_cnt;                                         //�����������

	u8 mscfade_en;             //1:���ֲ���ģʽ���л���Ҫ���뵭����0������Ҫ���뵭��
} type_msc_ctl;

enum
{
    MUSIC_FAST_FORWORD,
    MUSIC_FAST_BACKWORD,
};

enum
{
    TYPE_NONE,             //��֧�ֵĸ�ʽ
    TYPE_MP3,
    //TYPE_WMA,
    TYPE_WAV,
};


extern type_msc_ctl msc_ctl;




/*****************************************************************************
 * Module    : ���ֲ������
 *****************************************************************************/

#define REPEAT_LOOP 0xff                                //����ѭ������
#define REPEAT_END  0xfe                                //����һ�κ�ֹͣ�����ڸ���

u8 music_init(void);                                    //��ʼ�����ֽ���
u8 music_init_mp3_only(void);                           //��ʼ�����ֽ��룬ֻ��ʼ��MP3��ʽ

bool mp3_init(void);                                    //��ʼ��MP3����
bool mp3_init_spi(void);                                //��ʼ��MP3�����SPIFlash�����ư�
void mp3_synth_check(void);
void mp3_set_jump(void);
void wav_set_jump(void);

bool music_repeat_hook(void);
void msc_mp3_decode(void);                              //MP3���룬���жϵ���
void msc_wav_decode(void);                              //WAV���룬���жϵ���
void bt_decode(void);                                   //BT���룬���жϵ���
void music_decode(void);                                //��Ƶ���룬���жϵ���

void music_stop(void);                                  //ֹͣ���ֲ���
void music_event(void);                                 //������Ҫ���¼�
void music_play(void);                                  //��ʼ����
void music_pause(void);                                 //��ͣ
bool music_start_pause(void);                           //����/��ͣ
void music_fast(bool direct, u8 sec);                   //���/����. direct = 0Ϊ�����1Ϊ����
void music_fast_end(void);                              //���/���˽������ָ�����
void music_get_point(type_music_point *point);          //��ȡ��ǰ����λ��
void music_set_a_point(type_music_point *point);        //����A�㣬�Ա㸴��ʱ�����
//AB������repeatΪ������������repeat = REPEAT_LOOP�����޸�����ΪREPEAT_ENDʱ������һ��ֹͣ
void music_repeat(u8 repeat, type_music_point_ab *pab);
void music_repeat_clear(void);                          //�����ǰ�ĸ���״̬��������������
void music_jump(type_music_point *point);               //����ĳ����в���
void music_eq_init_m(void);                             //EQϵ����ʼ��
void music_eq_set_m(u8 eq_num);                         //EQ����
void music_eq_setd_m(u8 eq_num);                        //ֱ������EQ
void music_eq_init(void);                               //EQϵ����ʼ��
void music_eq_set(u8 eq_num);                           //EQ����
void music_eq_setd(u8 eq_num);                          //ֱ������EQ
u16 music_filename_check(void);                         //��ȡ�ļ���checksum

//Ƶ���߳�ʼ��ѡ��
enum
{
    FREQ_DAC,                   //����DAC��Ƶ����
    FREQ_ADC,                   //����ADC��Ƶ����
};
void music_freq_init(u8 freq_type);                     //��ʼ��Ƶ����, freq_type�ο�Ƶ���߳�ʼ��ѡ��ö��
bool music_freq_get(void);                              //��ȡƵ����
void music_freq_kick(void);                             //Kick��һ��

//����״̬
enum
{
    STATUS_PLAY_STOP,                       //��ȫֹͣ
    STATUS_PLAY_STOPPING,                   //����ֹͣ
    STATUS_PLAY_PLAYING,                    //���ڲ���
    STATUS_PLAY_PAUSE,                      //��ͣ��
    STATUS_PLAY_FAST,                       //���������
    STATUS_PLAY_CONTINUE,                   //�ȴ��ٴ�KICK��������
};

u8 music_get_status(void);                              //��ȡ���ֵĲ���״̬
bool music_is_stop(void);                               //�����Ƿ���ֹͣ

u32 music_calc_mtime(u32 frame_count);
u32 music_get_mtime(void);
u32 music_point_a_mtime(void);
void spi_music_wait(void);                              //�������ŵȴ�����
void mp3_res_play(u8 music_name);                       //ѡ�����Բ˵�����
void mp3_res_play_wait(u8 music_name);                  //ѡ�����Բ˵�����,���ȴ��������

#ifdef AX207X_TAG
void sound_ctl_init(u8 is_cnt, u32 is_pow, u8 silence_cnt, u32 silence_pow);
u8 sound_detect(u32 v_pow);
u32 voice_power(void *buf, u16 len);                    //��ȡһ��������ƽ������ֵ
#else
void sound_ctl_init(u8 is_cnt, u8 is_pow, u8 silence_cnt, u8 silence_pow);
u8 sound_detect(u8 v_pow);
#endif
void sound_ctl_clr(void);
u8 sound_maxpow(void *ptr, u8 len);                     //��ȡһ���������������

#endif
