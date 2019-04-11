/*****************************************************************************
 * Module    : API
 * File      : api_filesystem.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : 文件系统API接口
 *****************************************************************************/
#ifndef API_FILESYSTEM_H
#define API_FILESYSTEM_H

#include "tff.h"
#include "include.h"

#define DIR_DEPTH           9           //支持根目录+8层子目录

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
	u8 dir_depth;           //文件夹深度
	u16 file_start;         //起始文件编号
	u16 dir_start;          //起始文件夹编号
	u16 before_files;       //该文件夹前有多少文件，用于文件夹循环
	u16 win_cnt;            //读扇区次数
	u16 dir_wincnt;         //记录本层目录的wincnt值，文件夹加速时使用，不参与copy
	u16 file_tmp_start;     //临时点起始文件编号，不参与copy
} type_point_item;


//文件系统结构体
typedef struct
{

	type_point_item t_item;

	u16 dir_count;                      //文件夹计数
	u16 dir_number;                     //目标文件夹编号
	u16 file_count;                     //文件计数
	u16 file_number;                    //目标文件编号
	//u16 dir_wincnt;                     //记录本层目录的wincnt值
	u32 dir_sclust;                     //当前目录的起始位置，用于返回目录起始

	u8 dev_sel;                         //当彰选择的设备
	u8 dev_new;                         //下一个需要选择的设备
	u8 dev_test_ready;                  //设备(U盘)是否需要test_ready
	u8 mounted;                         //文件系统已初始化完成
	u8 file_ok;                         //文件可读写
	u8 read_only;                       //只读
	u8 scan_mode;                       //扫描模式
	bool (*file_filter)(void);          //文件过滤器
	bool (*dir_filter)(void);           //目录过滤器
} type_fat;


extern type_fat t_fat;


bool fs_is_mounted(void);                   //判断文件系统是否已经挂载
bool fs_need_mounted(void);                 //判断文件系统是否需要挂载
bool fs_read_only(void);                    //判断文件系统是否只读
void fs_save_dpath(void);                   //保存文件搜索信息
bool fs_init(void);                         //文件系统挂载初始化
void fs_unmount(void);                      //卸载文件系统
bool fs_sel(u8 dev);                        //选择设备
void fs_sel_new(void);                      //选择刚刚插入的设备
u8 fs_cur_dev(void);                        //返回文件系统当前挂载的设备

u16 f_gettotal_no_speed(void);              //获取总文件数
u16 f_gettotal_speed(void);                 //使用文件搜索回事获取文件总数
u16 f_get_dirtotal(void);                   //获取当前文件夹下总文件数
u16 f_get_dirstart(void);
bool f_open_speed(u16 file_num);            //根据文件编号打开一个文件
bool f_open_no_speed(u16 file_num);         //根据文件编号打开一个文件
bool f_open(u16 file_num);                  //根据文件编号打开一个文件
void f_delete(void);                        //删除当前打开的文件
bool f_open_dir(u16 dir_num);               //根据文件编号打开一个文件
bool f_open_dir_speed(u16 dir_num);         //根据文件夹编号打开一个文件夹
bool f_open_dir_no_speed(u16 dir_num);      //根据文件夹编号打开一个文件夹

void get_ext_name(char *ext_name);
u8 get_scan_dept(void);
bool f_attr_is_dir(void);
bool f_attr_is_file(void);
bool f_check_ext(char *ext_name, char IAR_CONST(*extlist)[4], u8 extnum);       //检查扩展名

FRESULT fs_scan_files_no_speed(void);
FRESULT fs_scan_files_speed(void);

//配置扫描过滤器
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
