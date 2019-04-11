/*****************************************************************************
 * Module    : mem
 * File      : mem_rtcram.h
 * Author    :
 * Email     :
 * Function  : RTCRAM记忆任务流程
 *****************************************************************************/
#ifndef __MEM_RTCRAM_H__
#define __MEM_RTCRAM_H__

//RTCRAM中用于记忆的地址:
#define RTCRAM_OFFSET               10     //pos+frame+num, 共10byte

#define RTCRAM_MUSIC_POS_SD         0x00   //4byte	// 外接SD卡
#define RTCRAM_MUSIC_FRAME_SD       0x04   //4byte
#define RTCRAM_MUSIC_NUM_SD         0x08   //2byte

#define RTCRAM_MUSIC_POS_SD1        0x0A   //4byte	// 内接SD卡 或 SPI-Flash (2选1)
#define RTCRAM_MUSIC_FRAME_SD1      0x0E   //4byte
#define RTCRAM_MUSIC_NUM_SD1        0x12   //2byte

#define RTCRAM_MUSIC_POS_USB        0x14   //4byte
#define RTCRAM_MUSIC_FRAME_USB      0x18   //4byte
#define RTCRAM_MUSIC_NUM_USB        0x1C   //2byte

#define RTCRAM_MUSIC_NUM_USB1       0x1E   //2byte

#define RTCRAM_ACTIVE_DEVICE        0x20   //1byte
#define RTCRAM_VOLUME               0x21   //1byte
#define RTCRAM_PLAY_MODE			0x22   //1byte	// 播放模式 ALL / ONE

//#define RTCRAM_FREQUENCY            0x22   //1byte             //当前FM频率
//#define RTCRAM_FM_TOTAL_CHANNEL     0x23   //1byte             //电台总数
//#define RTCRAM_CUR_CHANNEL          0x24   //1byte             //当前电台序号
//#define RTCRAM_FREQUENCY_BUF        0x25   //26byte(0x25-0x3F) //自动搜台保存的各电台

bool rtcram_poweron_check(void);

// 各个目录的曲目位置记忆
//#define RTCRAM_FLASH_OFFSET       sizeof(u16)
//#define RTCRAM_VOICE            0x22   //2byte	, 取消各个目录的曲目记忆, 因此保存到 RTCRAM_MUSIC_POS_SD1
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
