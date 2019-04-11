/*****************************************************************************
 * Module    : mem
 * File      : mem.h
 * Author    :
 * Email     :
 * Function  : ������������
 *****************************************************************************/
#ifndef __MEM_H__
#define __MEM_H__

#include "mem_rtcram.h"
#include "mem_internal.h"
#include "mem_eeprom.h"
#include "mem_device.h"

typedef struct
{
	u8 dev;               //���ŵ��豸
	u8 change_flag;       //���ݸı��ʶ
	u8 delay_time;       //д������Ϣ����ʱ����
#if (SYS_MEM == MEM_INTERNAL)
	bool internal_sta;    //���ü���״̬
	u8 prev_ch_number;    //���ü��䣬FM�Զ���̨ʱ��¼��һ����̨��ch_buf[]�е����
#if USE_BREAK_POINT
	u8 erase_sta;         //������ʶ 0x00-0x04����Ҫ���������� 0x05���Ѳ�����ɣ� 0xFF����Ҫ���²���
	u8 erase_time;        //�������豸��2.5S����
	u8 read_dev;          //��������Ϣʱ�Ĳ����豸
#endif
#endif
#if (SYS_MEM == MEM_RTCRAM)
	bool rtcram_poweron;  //irtc�ϵ��ʶ
#endif
#if (SYS_MEM == MEM_DEVICE)
	u8 dev_mem_ready;     //�����豸�Ƿ�����
	u8 dev_mem_new;       //���µļ����豸
#endif
} type_mem_ctl;

extern type_mem_ctl mem_ctl;

#define PARAM_VOLUME           0x00   //1byte
#define PARAM_FREQUENCY        0x01   //1byte
#define PARAM_ACTIVE_DEVICE    0x02   //1byte
#define PARAM_MUSIC_NUM        0x03   //2byte, ����д��U/SD

//ע�⣺EEPROM��дʱ���ܿ�ҳ
#define EEPROM_OFFSET          12     //pos+frame+name+num, ��12byte
#define PARAM_MUSIC_POS_SD     0x08   //4byte
#define PARAM_MUSIC_FRAME_SD   0x0c   //4byte
#define PARAM_MUSIC_NAME_SD    0x10   //2byte
#define PARAM_MUSIC_NUM_SD     0x12   //2byte

#define PARAM_MUSIC_POS_SD1    0x14   //4byte
#define PARAM_MUSIC_FRAME_SD1  0x18   //4byte
#define PARAM_MUSIC_NAME_SD1   0x1c   //2byte
#define PARAM_MUSIC_NUM_SD1    0x1e   //2byte

#define PARAM_MUSIC_POS_USB    0x20   //4byte
#define PARAM_MUSIC_FRAME_USB  0x24   //4byte
#define PARAM_MUSIC_NAME_USB   0x26   //2byte
#define PARAM_MUSIC_NUM_USB    0x28   //2byte

#define PARAM_MUSIC_POS_USB1   0x2A   //4byte
#define PARAM_MUSIC_FRAME_USB1 0x2E   //4byte
#define PARAM_MUSIC_NAME_USB1  0x32   //2byte
#define PARAM_MUSIC_NUM_USB1   0x34   //2byte
//                                �ϼ� 52byte

#define EEPROM_FM_TOTAL_CHANNEL 0x36   //1byte             //��̨����
#define EEPROM_CUR_CHANNEL      0x37   //1byte             //��ǰ��̨���
#define EEPROM_FREQUENCY_BUF    0x38   //26byte(0x38-0x51) //�Զ���̨����ĸ���̨


#define MEM_DELAY              0xFF    //255*10ms=2.55s

/*
MEM_VOL_CHANGE
MEM_FRE_CHANGE
MEM_NUM_CHANGE
*/
#define MEM_UNCHANGE  0x00
#define MEM_VOL_BIT   1    //����
#define MEM_DEV_BIT   2    //�豸
#define MEM_FRE_BIT   3    //Ƶ��
#define MEM_NUM_BIT   4    //��Ŀ



void dev_check_dbr(void);
void mem_check(void);
void mem_event(void);
void mem_read_init_info(void);
void mem_read_music_num(void);
void rtcram_write_sys_info(void);
void mem_write_file_change(void);
void mem_write_device(void);
void mem_read_music_break(void);
void mem_write_music_break(void);
void internal_write_sys_info(void);
void eep_write_sys_info(void);
bool device_mount(void);
void mem_write_fm(void);
void mem_write_channel(void);
void mem_write_cur_channel(void);
void mem_write_frequency(void);
void mem_fmbuf_clr(void);
void mem_time_set(void);
void var_set_default(void);

#if (SYS_MEM == MEM_RTCRAM)
#define mem_read_music_break()    read_music_break_rtcram()
#define mem_write_music_break()   write_music_break_rtcram()
#define write_fm_info(n)          write_fm_info_rtcram(n)
#elif (SYS_MEM == MEM_INTERNAL)
#define mem_read_music_break()    read_music_break_internal()
#define mem_write_music_break()   write_music_break_internal()
#define write_fm_info(n)          write_fm_info_internal(n)
#elif (SYS_MEM == MEM_E2PROM)
#define mem_read_music_break()    read_music_break_eeprom()
#define mem_write_music_break()   write_music_break_eeprom()
#define write_fm_info(n)          write_fm_info_eeprom(n)
#elif (SYS_MEM == MEM_DEVICE)
#define mem_read_music_break()
#define mem_write_music_break()
#define write_fm_info(n)
#endif


#endif
