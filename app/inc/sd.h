#ifndef _SD_H_
#define _SD_H_
#include "diskio.h"
#define SDMMC_INSERT		0

typedef enum
{
    DISONLINE,  //SD卡不在线
    UNACTIVE,  //SD卡刚插入,未进行初始化
    ACTIVED,  //SD卡初始化成功
    INVALID,  //SD卡无效
} TypeCardSta;

typedef enum
{
    OPT_NONE,
    OPT_READ,
    OPT_WRITE,
} TypeOpt;

typedef enum
{
    UNUSABLE,  //不支持
    SD_VER_1,  //SD 1.0
    SD_VER_2,  //SD 2.0
    MMC_TYPE,  //MMC
} cardType;

__root void SdInsert(void);  //SD卡插入时调用该函数,外部不需要其它变量记录SD卡的在线情况
__root void SdRemove(void);  //SD卡移除时调用该函数,外部不需要其它变量记录SD卡的在线情况
__root void SdSetStatus(TypeCardSta tmpSta);
__root bool sd_read(void *buf,u32 lba,u8 offset);
__root bool SdWrite(void *Buffer,u32 Lba);
__root void SdVarInit(void) @ "SD_BANK";
#endif
