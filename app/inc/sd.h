#ifndef _SD_H_
#define _SD_H_
#include "diskio.h"
#define SDMMC_INSERT		0

typedef enum
{
    DISONLINE,  //SD��������
    UNACTIVE,  //SD���ղ���,δ���г�ʼ��
    ACTIVED,  //SD����ʼ���ɹ�
    INVALID,  //SD����Ч
} TypeCardSta;

typedef enum
{
    OPT_NONE,
    OPT_READ,
    OPT_WRITE,
} TypeOpt;

typedef enum
{
    UNUSABLE,  //��֧��
    SD_VER_1,  //SD 1.0
    SD_VER_2,  //SD 2.0
    MMC_TYPE,  //MMC
} cardType;

__root void SdInsert(void);  //SD������ʱ���øú���,�ⲿ����Ҫ����������¼SD�����������
__root void SdRemove(void);  //SD���Ƴ�ʱ���øú���,�ⲿ����Ҫ����������¼SD�����������
__root void SdSetStatus(TypeCardSta tmpSta);
__root bool sd_read(void *buf,u32 lba,u8 offset);
__root bool SdWrite(void *Buffer,u32 Lba);
__root void SdVarInit(void) @ "SD_BANK";
#endif
