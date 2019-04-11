/*****************************************************************************
 * Module    : API
 * File      : api_filesystem.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : �ļ�ϵͳAPI�ӿ�
 *****************************************************************************/
#ifndef API_FILESYSTEM_H
#define API_FILESYSTEM_H

#include "tff.h"
#include "include.h"

#define DIR_DEPTH           9           //֧�ָ�Ŀ¼+8����Ŀ¼

enum
{
    SEEK_ORG = 0,
    SEEK_CUR
};

typedef struct
{
	u32     sclust;             /* Start cluster */
	u32     clust;              /* Current cluster */
	u32     sect;               /* Current sector */
	u32     cindex;             /* Current index */
	u32     pindex;             /* pre index */
} DIR_PATH;

typedef struct
{
	u8 dir_depth;           //�ļ������
	u16 file_start;         //��ʼ�ļ����
	u16 dir_start;          //��ʼ�ļ��б��
	u16 before_files;       //���ļ���ǰ�ж����ļ��������ļ���ѭ��
	u16 win_cnt;            //����������
	u16 dir_wincnt;         //��¼����Ŀ¼��wincntֵ���ļ��м���ʱʹ�ã�������copy
	u16 file_tmp_start;     //��ʱ����ʼ�ļ���ţ�������copy
} type_point_item;


//�ļ�ϵͳ�ṹ��
typedef struct
{

	type_point_item t_item;

	u16 dir_count;                      //�ļ��м���
	u16 dir_number;                     //Ŀ���ļ��б��
	u16 file_count;                     //�ļ�����
	u16 file_number;                    //Ŀ���ļ����
	//u16 dir_wincnt;                     //��¼����Ŀ¼��wincntֵ
	u32 dir_sclust;                     //��ǰĿ¼����ʼλ�ã����ڷ���Ŀ¼��ʼ

	u8 dev_sel;                         //����ѡ����豸
	u8 dev_new;                         //��һ����Ҫѡ����豸
	u8 dev_test_ready;                  //�豸(U��)�Ƿ���Ҫtest_ready
	u8 mounted;                         //�ļ�ϵͳ�ѳ�ʼ�����
	u8 file_ok;                         //�ļ��ɶ�д
	u8 read_only;                       //ֻ��
	u8 scan_mode;                       //ɨ��ģʽ
	bool (*file_filter)(void);          //�ļ�������
	bool (*dir_filter)(void);           //Ŀ¼������
} type_fat;


extern type_fat t_fat;


bool fs_is_mounted(void);                   //�ж��ļ�ϵͳ�Ƿ��Ѿ�����
bool fs_need_mounted(void);                 //�ж��ļ�ϵͳ�Ƿ���Ҫ����
bool fs_read_only(void);                    //�ж��ļ�ϵͳ�Ƿ�ֻ��
void fs_save_dpath(void);                   //�����ļ�������Ϣ
bool fs_init(void);                         //�ļ�ϵͳ���س�ʼ��
void fs_unmount(void);                      //ж���ļ�ϵͳ
bool fs_sel(u8 dev);                        //ѡ���豸
void fs_sel_new(void);                      //ѡ��ող�����豸
u8 fs_cur_dev(void);                        //�����ļ�ϵͳ��ǰ���ص��豸

u16 f_gettotal_no_speed(void);              //��ȡ���ļ���
u16 f_gettotal_speed(void);                 //ʹ���ļ��������»�ȡ�ļ�����
u16 f_get_dirtotal(void);                   //��ȡ��ǰ�ļ��������ļ���
u16 f_get_dirstart(void);
bool f_open_speed(u16 file_num);            //�����ļ���Ŵ�һ���ļ�
bool f_open_no_speed(u16 file_num);         //�����ļ���Ŵ�һ���ļ�
bool f_open(u16 file_num);                  //�����ļ���Ŵ�һ���ļ�
void f_delete(void);                        //ɾ����ǰ�򿪵��ļ�
bool f_open_dir(u16 dir_num);               //�����ļ���Ŵ�һ���ļ�
bool f_open_dir_speed(u16 dir_num);         //�����ļ��б�Ŵ�һ���ļ���
bool f_open_dir_no_speed(u16 dir_num);      //�����ļ��б�Ŵ�һ���ļ���

void get_ext_name(char *ext_name);
u8 get_scan_dept(void);
bool f_attr_is_dir(void);
bool f_attr_is_file(void);
bool f_check_ext(char *ext_name, char IAR_CONST(*extlist)[4], u8 extnum);       //�����չ��

FRESULT fs_scan_files_no_speed(void);
FRESULT fs_scan_files_speed(void);

//����ɨ�������
void f_scan_set(u8 scan_mode, bool (*file_filter)(void), bool (*dir_filter)(void));

u16 f_gettotal(void);

#if FS_SCAN_SPEED_EN
#define f_gettotal()        f_gettotal_speed()
#define f_open_dir(n)       f_open_dir_speed(n)
#define f_open(n)           f_open_speed(n)
#else
#define f_gettotal()        f_gettotal_no_speed()
#define f_open_dir(n)       f_open_dir_no_speed(n)
#define f_open(n)           f_open_no_speed(n)
#endif

#endif
