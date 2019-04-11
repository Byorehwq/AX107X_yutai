/*****************************************************************************
 * Module    : mem
 * File      : mem_internal.h
 * Author    :
 * Email     :
 * Function  : ���ü�����������
 *****************************************************************************/
#ifndef __MEM_INT_H__
#define __MEM_INT_H__

bool internal_poweron_check(void);
u8 internal_read_info(u8 addr);
void internal_write_info(u8 addr, u8 *value, u8 num);
void internal_sector_erase(u8 addr);
void internal_read_multi(u8 addr, u8 *value, u8 num);
void write_volume_device_internal(void);
void read_volume_internal(void);
void read_device_internal(void);
void read_frequency_internal(void);
void write_num_internal(void);
void read_num_internal(void);
void read_music_break_internal(void);
void write_music_break_internal(void);
void write_fmscan_internal(u8 byte_value);
void write_total_channel_internal(void);
void write_cur_channel_internal(void);
void write_fm_info_internal(u8 byte_value);
void read_fm_info_internal(void);
void erase_music_break_internal(void);

/*
		        ��¼������Ϣ	��¼��ַ �ϼ�
        ����+�豸	1byte	        0	 1
        FMƵ��		1byte	        0	 1
SD	SD��Ŀ	        2byte*12��	2byte	 26
	�ļ�ָ��	4	        0	 4
	֡��	        4	        0	 4
	�ļ���	        2	        0	 2
U	U����Ŀ	        2byte*12��	2byte	 26
	�ļ�ָ��	4	        0	 4
	֡��	        4	        0	 4
	�ļ���	        2	        0	 2
				                 110
*/


//���µ�ַ���ڼ�¼������Ϣ
#define INTERNAL_OFFSET_MAX      12


//���µ�ַ���ڼ�¼��ǰ��ַ
#define INTERNAL_NUM_SD_ADDR      0x00    //2byte:0x00-0x01
#define INTERNAL_NUM_USB_ADDR     0x02    //2byte:0x02-0x03

#define INTERNAL_VOL_PARA         0x04    //1byte:0x04       //�������豸2������һ��д����
#define INTERNAL_DEV_PARA         0x05    //1byte:0x05

#if FM_SEEK_50K_STEP
#define INTERNAL_OFFSET           30      //��Ŀ+ָ��+֡��+�ļ��� ��30byte

#define INTERNAL_NUM_SD_PARA      0x06    //2byte*10=20byte:0x06-0x19
#define INTERNAL_POS_SD_PARA      0x1a    //4byte:0x1a-0x1c
#define INTERNAL_FRAME_SD_PARA    0x1e    //4byte:0x1e-0x21
#define INTERNAL_NAME_SD_PARA     0x22    //2byte:0x22-0x23

#define INTERNAL_NUM_USB_PARA     0x24     //2byte*10=20byte:0x24-0x37
#define INTERNAL_POS_USB_PARA     0x38     //4byte:0x38-0x3b
#define INTERNAL_FRAME_USB_PARA   0x3c     //4byte:0x3c-0x3f
#define INTERNAL_NAME_USB_PARA    0x40     //2byte:0x40-0x41

#define INTERNAL_CUR_CHANNEL      0x42   //1byte             //��ǰ��̨���
#define INTERNAL_FM_TOTAL_CHANNEL 0x43   //1byte             //��̨����
#define INTERNAL_FREQUENCY        0x44   //2byte:0x44-0x45   //��ǰFMƵ��

#define INTERNAL_FREQUENCY_BUF    0x46   //52byte(0x46-0x70) //�Զ���̨����ĸ���̨

#define INTERNAL_MARK_1           0x7E
#define INTERNAL_MARK_2           0x7F   //(���ü���ģ�鹲128byte,��ַ��ΧΪ0x00-0x7F)

#else
#define INTERNAL_OFFSET           34      //��Ŀ+ָ��+֡��+�ļ��� ��34byte

#define INTERNAL_NUM_SD_PARA      0x06    //2byte*12=24byte:0x06-0x1D
#define INTERNAL_POS_SD_PARA      0x1E    //4byte:0x1E-0x21
#define INTERNAL_FRAME_SD_PARA    0x22    //4byte:0x22-0x25
#define INTERNAL_NAME_SD_PARA     0x26    //2byte:0x26-0x27

#define INTERNAL_NUM_USB_PARA     0x28     //2byte*12=24byte:0x28-0x3F
#define INTERNAL_POS_USB_PARA     0x40     //4byte:0x40-0x43
#define INTERNAL_FRAME_USB_PARA   0x44     //4byte:0x44-0x47
#define INTERNAL_NAME_USB_PARA    0x48     //2byte:0x48-0x49

#define INTERNAL_CUR_CHANNEL      0x4A   //1byte             //��ǰ��̨���
#define INTERNAL_FREQUENCY        0x4B   //1byte             //��ǰFMƵ��
#define INTERNAL_FM_TOTAL_CHANNEL 0x4C   //1byte             //��̨����

#define INTERNAL_FREQUENCY_BUF    0x4E   //26byte(0x4E-0x67) //�Զ���̨����ĸ���̨

#define INTERNAL_MARK_1           0x7E
#define INTERNAL_MARK_2           0x7F   //(���ü���ģ�鹲128byte,��ַ��ΧΪ0x00-0x7F)
#endif

enum
{
    BEING_ERASE   = 0x00,   //���ڲ�����
    HAVE_ERASED   = 0x05,   //����ɲ���������д
    NEED_RE_ERASE = 0xFF,   //��д����Ҫ���²���
};


#endif

