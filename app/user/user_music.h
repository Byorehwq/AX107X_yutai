/*****************************************************************************
 * Module    : User
 * File      : user_music.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �û����ֲ��ſ��ƶ���
 *****************************************************************************/
#ifndef USER_MUSIC_H
#define USER_MUSIC_H


typedef struct type_user_music_
{
	u16 music_num;                          //������Ŀ���, from 1 to N when SD/USB.  from 0 to flash_music_total-1 when used SPI Flash
	u16 music_num_last;						// ��һ��������Ŀ��   20180314
	u16 music_total;                        //��Ŀ����
	u8 music_type;                          //��Ƶ��ʽ
	type_play_time cur_time;                //��ǰ����ʱ��
#if AB_REPEAT_EN
	type_music_point_ab point;              //��¼AB��
	u8 repeat;                              //AB������0Ϊ��������1Ϊ����A�㣬2Ϊ����B��
#endif
#if USE_BREAK_POINT
	//�𶯣��ϵ���Ϣ�����checksum���ŷţ�����������ʱ��ֱ�Ӱ������ ======
	type_music_point break_point;           //��¼�ϵ���Ϣ
	u16 filename_checksum;                  //��¼������checksum
	//======
#endif
	u16 dir_num;                            //�ļ��б��
	u16 dir_total;                          //�ļ�������
} type_user_music;

typedef struct type_user_music_flag_
{
	u8 file_change :    1;                  //�ļ��ı䣬��Ҫ�����ļ�
	u8 pause :          1;                  //�Ƿ�����ͣ״̬����ҪΪ�˷���һЩ�����ж�
	u8 file_prev_next : 1;                  //�ļ�����0Ϊnext��1Ϊprev
#if USE_BREAK_POINT
	u8 break_init :     1;                  //�����豸�ı䣬��Ҫ�������Ŷϵ���Ϣ
#endif
#if REPEAT_AS_ONE
	u8 repeat_as_one_flag : 	1;
#endif
	u8 stop       :     1;                  //����ģʽ�£�stop�����ı�־λ
} type_user_music_flag;

extern type_user_music u_msc;
extern IAR_BDATA_A type_user_music_flag b_msc;

bool user_music_enter(void);

bool device_switch(void);
bool device_mount(void);

u16 loop_auto(bool dir, u16 cur, u16 min, u16 max);
#endif
