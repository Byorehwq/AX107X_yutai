/*****************************************************************************
 * Module    : mem
 * File      : mem_rtcram.h
 * Author    :
 * Email     :
 * Function  : RTCRAM������������
 *****************************************************************************/
#ifndef __MEM_RTCRAM_H__
#define __MEM_RTCRAM_H__

//RTCRAM�����ڼ���ĵ�ַ:
#define RTCRAM_OFFSET               10     //pos+frame+num, ��10byte

#define RTCRAM_MUSIC_POS_SD         0x00   //4byte	// ���SD��
#define RTCRAM_MUSIC_FRAME_SD       0x04   //4byte
#define RTCRAM_MUSIC_NUM_SD         0x08   //2byte

#define RTCRAM_MUSIC_POS_SD1        0x0A   //4byte	// �ڽ�SD�� �� SPI-Flash (2ѡ1)
#define RTCRAM_MUSIC_FRAME_SD1      0x0E   //4byte
#define RTCRAM_MUSIC_NUM_SD1        0x12   //2byte

#define RTCRAM_MUSIC_POS_USB        0x14   //4byte
#define RTCRAM_MUSIC_FRAME_USB      0x18   //4byte
#define RTCRAM_MUSIC_NUM_USB        0x1C   //2byte

#define RTCRAM_MUSIC_NUM_USB1       0x1E   //2byte

#define RTCRAM_ACTIVE_DEVICE        0x20   //1byte
#define RTCRAM_VOLUME               0x21   //1byte
#define RTCRAM_PLAY_MODE			0x22   //1byte	// ����ģʽ ALL / ONE

//#define RTCRAM_FREQUENCY            0x22   //1byte             //��ǰFMƵ��
//#define RTCRAM_FM_TOTAL_CHANNEL     0x23   //1byte             //��̨����
//#define RTCRAM_CUR_CHANNEL          0x24   //1byte             //��ǰ��̨���
//#define RTCRAM_FREQUENCY_BUF        0x25   //26byte(0x25-0x3F) //�Զ���̨����ĸ���̨

bool rtcram_poweron_check(void);

// ����Ŀ¼����Ŀλ�ü���
//#define RTCRAM_FLASH_OFFSET       sizeof(u16)
//#define RTCRAM_VOICE            0x22   //2byte	, ȡ������Ŀ¼����Ŀ����, ��˱��浽 RTCRAM_MUSIC_POS_SD1
//#define RTCRAM_CHILDREN         0x24   //2byte
//#define RTCRAM_ENGLISH          0x26   //2byte
//#define RTCRAM_STORY            0x28   //2byte
//#define RTCRAM_HABIT            0x2A   //2byte
//#define RTCRAM_POEM             0x2C   //2byte
//#define RTCRAM_JOKE             0x2E   //2byte


bool rtcram_poweron_check(void);
void rtcram_read_multi(u8 addr, u8 *value, u8 num);
void write_volume_rtcram(void);
void read_volume_rtcram(void);
void write_num_rtcram(void);
void read_num_rtcram(void);
void write_device_rtcram(void);
void read_device_rtcram(void);
void write_music_break_rtcram(void);
void read_music_break_rtcram(void);
void write_fm_info_rtcram(u8 byte_value);
void read_fm_info_rtcram(void);
void write_total_channel_rtcram(void);
void write_cur_channel_rtcram(void);
void write_playmode_rtcram(void);

#if TASK_FLASH_EN
extern void write_flash_num_rtcram(void);
extern void read_flash_num_rtcram(void);
#endif

#endif
