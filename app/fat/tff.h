

#ifndef _FATFS
#define _FATFS

#include "include.h"
//#include "typedef.h"
#include "diskio.h"
//#include "string.h"
//#include "stdio.h"


#define _MCU_ENDIAN     1

#define _FS_READONLY    0
/* Setting _FS_READONLY to 1 defines read only configuration. This removes
/  writing functions, f_write, f_sync, f_unlink, f_mkdir, f_chmod, f_rename,
/  f_truncate and useless f_getfree. */

#define _FS_MINIMIZE    0
/* The _FS_MINIMIZE option defines minimization level to remove some functions.
/  0: Full function.
/  1: f_stat, f_getfree, f_unlink, f_mkdir, f_chmod, f_truncate and f_rename are removed.
/  2: f_opendir and f_readdir are removed in addition to level 1.
/  3: f_lseek is removed in addition to level 2. */

#define _USE_MKDIR      1
/* When _USE_MKDIR is set to 1 and _FS_READONLY is set to 0, f_mkdir function is
/  enabled. */

#define _USE_DELETE     1
/* When _USE_DELETE is set to 1 and _FS_READONLY is set to 0, f_delete function is
/  enabled. */

#define _USE_FSINFO     0
/* To enable FSInfo support on FAT32 volume, set _USE_FSINFO to 1. */

#define _USE_SJIS       1
/* When _USE_SJIS is set to 1, Shift-JIS code transparency is enabled, otherwise
/  only US-ASCII(7bit) code can be accepted as file/directory name. */

#define _USE_NTFLAG     1
/* When _USE_NTFLAG is set to 1, upper/lower case of the file name is preserved.
/  Note that the files are always accessed in case insensitive. */

#define _FAT32          1
/* To enable FAT32 support in addition of FAT12/16, set _FAT32 to 1. */

#define _EXFAT          1
/* To enable EXFAT support. */

/* File system object structure */
typedef struct
{
	u16     n_rootdir;          /* Number of root directory entries */
	u16     s_size;             /* sector size*/
	//u32     winsect;            /* Current sector appearing in the win[] */
	u32     fatbase;            /* FAT表的起始位置，单位为512Byte */
	u32     dirbase;            /* Root directory start clust(for fat12/fat16, is start sector) */
	u32     database;           /* 存放数据起始，单位为512Byte */
	u32     sects_fat;          /* Sectors per fat */
	u32     max_clust;          /* Maximum cluster# + 1 */
	u8      csize_shift;        /* 记录csize的移位 */
	u8      n512sec_shift;      /* 记录n512per_sect的移位 */

#if !_FS_READONLY
	u32     free_fatbase;       /* 空闲FAT表起始位置 */
	u32     last_clust;         /* Last allocated cluster */
	u16     limit_clust;        /* 预分配簇大小限制 (number of clust) */
#if _USE_FSINFO
	u32     free_clust;         /* Number of free clusters */
	DWORD   fsi_sector;         /* fsinfo sector */
	BYTE    fsi_flag;           /* fsinfo dirty flag (1:must be written back) */
	BYTE    pad2;
#endif
#endif

	u8      fs_type;            /* FAT sub type */
	//u32     csize;              /* Number of sectors per cluster,int for exfat */
	u8      n_fats;             /* Number of FAT copies */
	u8      winflag;            /* win[] dirty flag (1:must be written back) */

} type_file_system;

//将fat_fs部份放入pdata
typedef struct
{
	u32     clust_mask;         //这个必须放在第一个，汇编时需要调用
	u8      n512clust_shift;    //记录csize * n512per_sect的移位和

	bool (*disk_read)(void *, u32);
	bool (*disk_write)(void *, u32);

	u8     *win;                /* Disk access window for Directory/FAT/File */
	u8     *win_back;

	u8      mp3_input;              //控制Input Ouput的大小,1-3
} type_file_system_hook;

/* Directory object structure */
typedef struct
{
	/* DIR的sclust至cindex的位置尽量不要改变，否则要相应修改OTP */
	/* next_dir_entry汇编中有调用cindex，请不要改变该结构体！！！*/
//    u8      fattrib;            /* Attribute */
	u32     sclust;             /* Start cluster */
	u32     clust;              /* Current cluster */
	u32     sect;               /* Current sector */
	u32     cindex;             /* Current index */

//    FATFS*  fs;                 /* Pointer to the owner file system object */
//    u8      fname[8+3+1];       /* Name (8+3 format) */
//    u16     file_total;         /* file counte in dir */
//    u8      dir_deep;           /* dir deepth */
//    u32     last_sect;          /* more than one dentry & long file name reserve，写长文件名用，保留 */

} type_dir;

/* File object structure */
typedef struct
{
	u8      fattrib;            /* attributes and NoFatChain */
	u32     sclust;             /* File start cluster */
	u32     cur_clust;          /* Current cluster */
	u32     cur_sect;           /* Current sector(512 byte) */
	u8      ser_clust;          /* 连续簇的大小 */
	u32     fptr;               /* File R/W pointer */
	u32     fsize;              /* File size */
	s32     seek;               /* seek block(512 bytes) length */
	u8      seek_flag;          /* seek快速判断标志 */
	u8      fname[8+3+1];       /* Name (8+3 format) */

	type_dir     dir;                /* 文件所在目录的信息 */
	u8      flag;               /* File status flags */

	//u32     csect;              /* Sector address in the cluster，以前用于f_write */

#if _FS_READONLY == 0
	u16     remain_clust;       /* 预分配簇剩余个数 */
	u32     end_clust;          /* 预分配时文件结束簇 */
	u32     total_clust;        /* 文件总簇数 */

	u32     dir_sect;           /* Sector containing the directory entry */
	u8     *dir_ptr;            /* Ponter to the directory entry in the window */
#endif
} type_file;


typedef enum
{
    F_SEEK_ORG = 0,
    F_SEEK_CUR

} SEEK_MODE;


/* File function return code (FRESULT) */
typedef enum
{
    FR_OK = 0,              // 0
    FR_NOT_READY,           // 1
    FR_NO_FILE,             // 2
    FR_NO_PATH,             // 3
    FR_INVALID_NAME,        // 4
    FR_INVALID_DRIVE,       // 5
    FR_DENIED,              // 6
    FR_EXIST,               // 7
    FR_RW_ERROR,            // 8
    FR_WRITE_PROTECTED,     // 9
    FR_NOT_ENABLED,         // 10
    FR_NO_FILESYSTEM,       // 11
    FR_INVALID_OBJECT,      // 12
    FR_DEEP_LIMIT,          // 13
    FR_END_PATH,            // 14
    FR_FILE_LIMIT,          // 15
    FR_END_FILE,            // 16
    FR_LFN_ERR,             // 17
    FR_MKFS_ABORTED,        // 18

    DIR_DELETE,             // 19
    DIR_END,                // 20
    DIR_ERROR,              // 21
} FRESULT;

/*-----------------------------------------------------*/
/* Tiny-FatFs module application interface             */
bool move_window_hook(u32 sector);
bool f_move_window_hook(u32 __pdata *sector);
u32 get_cluster_hook(u8 opt, u32 clust);
u32 clust2sect(u32 clust);
BOOL put_cluster_hook(u32 clust, u32 val);
u32 get_fattime_hook(void);
u8 check_fs_hook(u32 sect);
BOOL remove_chain_hook(u32 clust, u32 exfat_end);
u32 create_chain_hook(u32 clust);
u32 get_free_clust_hook(u32 scl);
char make_dirfile_hook(const char **path,char *dirname);
u8 check_fs_hook(u32 sect);
bool forward_chain_hook(void);

FRESULT validate(void);
FRESULT sync_hook(void);
FRESULT m_f_sync(void);
FRESULT m_f_mount(void);
FRESULT trace_path_hook(type_dir __pdata *dj, char *fn, const char *path, u8 **dir);
FRESULT reserve_direntry_hook(type_dir __pdata *dj, u8 **dir);
FRESULT m_f_open(type_dir __pdata *dj, const char *path, u8 mode);
FRESULT m_f_open_exist(void);
FRESULT f_read(void *buff);
FRESULT f_write(void *buff);
FRESULT f_close(void);
FRESULT m_f_lseek(SEEK_MODE type, u32 ofs);
FRESULT m_f_rootdir(type_dir __pdata *dj);
FRESULT m_f_readdir(type_dir __pdata *dj);
FRESULT m_f_mkdir(const char *path, type_dir __pdata *dj);
FRESULT m_f_delete(type_dir __pdata *dj);
//FRESULT m_f_getlongname(type_dir *dj, u8 *fname, u8 *longName_buff, s16 length);
//FRESULT m_f_getfree(const char *drv, u32 *nclust, FATFS **fatfs);
//FRESULT m_f_unlink(const char *path);
void m_f_opendir(type_dir __pdata *dj);
bool next_dir_entry_hook(type_dir __pdata *dj);
//u8 prv_dir_entry(type_dir *dj);
void mkitem_fat_hook(u8 *dir, char *fn, u8 fattrib, u32 sclust, u32 fsize);
void mkitem_exfat_hook(u8 *dir, char *fn, u8 fattrib, u32 sclust, u32 fsize);
u16 NameHash(u8 *fileName, u8 fileNameLen);
u16 EntrySetChecksum(u8 *dir, u8 NumberOfBytes);

void save_clust_hook(void);
void load_clust_hook(void);

extern type_file_system fat_fs;
extern IAR_PDATA_A type_file_system_hook fs_hook;
extern IAR_PDATA_A type_file fat_file;
extern IAR_PDATA_A type_dir file_dir;

/* File access control and file status flags (type_file.flag) */
#define FA_READ                 0x01
#define FA_OPEN_EXISTING        0x00
#if _FS_READONLY == 0
#define FA_WRITE                0x02
#define FA_CREATE_NEW           0x04
#define FA_CREATE_ALWAYS        0x08
#define FA_OPEN_ALWAYS          0x10
#define FA__WRITTEN             0x20
#define FA__DIRTY               0x40
#endif
#define FA__ERROR               0x80


/* FAT sub type (FATFS.fs_type) */
#define FS_FAT12    1
#define FS_FAT16    2
#define FS_FAT32    3
#define FS_EXFAT    4


/* File attribute bits for directory entry */
#define AM_RDO    	0x01	/* Read only */
#define AM_HID    	0x02    /* Hidden */
#define AM_SYS    	0x04    /* System */
#define AM_VOL    	0x08    /* Volume label */
#define AM_LFN    	0x0F    /* LFN entry */
#define AM_DIR    	0x10    /* Directory */
#define AM_ARC    	0x20    /* Archive */
#define AM_NFC    	0x80    /* NoFatChain */

/* Offset of FAT structure members */
#define BS_jmpBoot              0
#define BS_OEMName              3
#define BPB_BytsPerSec          11
#define BPB_SecPerClus          13
#define BPB_RsvdSecCnt          14
#define BPB_NumFATs             16
#define BPB_RootEntCnt          17
#define BPB_TotSec16            19
#define BPB_Media               21
#define BPB_FATSz16             22
#define BPB_SecPerTrk           24
#define BPB_NumHeads            26
#define BPB_HiddSec             28
#define BPB_TotSec32            32
#define BS_55AA                 510

#define BS_DrvNum               36
#define BS_BootSig              38
#define BS_VolID                39
#define BS_VolLab               43
#define BS_FilSysType           54

#define BPB_FATSz32             36
#define BPB_ExtFlags            40
#define BPB_FSVer               42
#define BPB_RootClus            44
#define BPB_FSInfo              48
#define BPB_BkBootSec           50
#define BS_DrvNum32             64
#define BS_BootSig32            66
#define BS_VolID32              67
#define BS_VolLab32             71
#define BS_FilSysType32         82
// exFAT的“FAT”字样在偏移5
#define BS_FileSysTypeEX        5
#define BPB_FatOffset           80
#define BPB_FatLength           84
#define BPB_ClusterHeapOffset   88
#define BPB_ClusterCount        92
#define BPB_FirstClusterOfRootDirectory     96
#define BPB_VolumeFlags                     106
#define BPB_BytesPerSectorShift             108
#define BPB_SectorsPerClusterShift          109
#define BPB_NumberOfFats                    110



#define FSI_LeadSig             0
#define FSI_StrucSig            484
#define FSI_Free_Count          488
#define FSI_Nxt_Free            492

#define MBR_Table               446

// FAT目录项偏移
#define DIR_Name                0
#define DIR_Attr                11
#define DIR_NTres               12
#define DIR_Checksum            13
#define DIR_CrtTime             14
#define DIR_CrtDate             16
#define DIR_FstClusHI           20
#define DIR_WrtTime             22
#define DIR_WrtDate             24
#define DIR_FstClusLO           26
#define DIR_FileSize            28

// exFAT目录项及偏移
#define DIR_Empty                       0x00
#define DIR_Delete                      0x05
#define DIR_FileEntryType               0x85
#define DIR_BitmapEntryType             0x81
#define DIR_UPCASEEntryType             0x82
#define DIR_StreamExtensionEntryType    0xC0
#define DIR_FileNameEntryType           0xC1

#define DIR_85_SecondaryCount           1
#define DIR_85_SetCheckSum              2
#define DIR_85_FileAttributes           4

#define DIR_C0_GeneralSecondaryFlags    1
#define DIR_C0_NameLength               3
#define DIR_C0_NameHash                 4
#define DIR_C0_ValidDataLength          8
#define DIR_C0_FirstCluster             20
#define DIR_C0_DataLength               24



/* Multi-u8 u16 access macros  */

//#if _MCU_ENDIAN == 1            // Use u16 access

#define    LD_WORD(ptr)        (u16)(*(u16*)(u8*)(ptr))
#define    LD_DWORD(ptr)        (u32)(*(u32*)(u8*)(ptr))
#define    ST_WORD(ptr,val)    *(u16*)(u8*)(ptr)=(u16)(val)
#define    ST_DWORD(ptr,val)    *(u32*)(u8*)(ptr)=(u32)(val)
/*
#elif _MCU_ENDIAN == 2            // Use u8-by-u8 access
#define    LD_WORD(ptr)        (u16)(((u16)*(volatile u8*)((ptr)+1)<<8)|(u16)*(volatile u8*)(ptr))
#define    LD_DWORD(ptr)        (u32)(((u32)*(volatile u8*)((ptr)+3)<<24)|((u32)*(volatile u8*)((ptr)+2)<<16)|((u16)*(volatile u8*)((ptr)+1)<<8)|*(volatile u8*)(ptr))
#define    ST_WORD(ptr,val)    *(volatile u8*)(ptr)=(u8)(val); *(volatile u8*)((ptr)+1)=(u8)((u16)(val)>>8)
#define    ST_DWORD(ptr,val)    *(volatile u8*)(ptr)=(u8)(val); *(volatile u8*)((ptr)+1)=(u8)((u16)(val)>>8); *(volatile u8*)((ptr)+2)=(u8)((u32)(val)>>16); *(volatile u8*)((ptr)+3)=(u8)((u32)(val)>>24)
#else
#error Do not forget to set _MCU_ENDIAN properly!
#endif
*/


/* Definitions corresponds to multiple sector size (not tested) */
#define    SS()             (fat_fs.s_size)
#define    CLUST_SHIFT()    (fs_hook.n512clust_shift + 9)   //簇转换为字节的移位
#define    CLUST_SIZE()     (fs_hook.clust_mask + 1)        //每簇字节数

#endif /* _FATFS */
