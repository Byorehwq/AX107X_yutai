/*****************************************************************************
 * Module    : Config
 * File      : config.c
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : �û������ļ���������Ŀ¼չ�����ɱ༭��Ӧ������
 *****************************************************************************/
#include "config.h"

/*****************************************************************************
��ע�����
AX207X����:
    AX2070(LQFP48);
    AX2073(SOP16);
    AX2073B(SOP16);
AX107X����:
    AX1070(LQFP48);
    AX1071(SOP28);
    AX1073(SOP16);
    AX1073B(SOP16);
    AX1076(SSOP24);

�������ú�DAC�����ڲ�ͬ��װ���в�𣬾������£�
AX2070:
    1.DAC��ʼ��ѡ���VCM:    #define DAC_INIT_TYPE     DAC_VCM_WITH_CAP
    2.VDDHP��VDDIO�ֿ�:        #define VDDHP_VDDIO_SHT   0

AX2073:
    1.ϵͳ����ѡ���޾���   #define SYS_CRY           CRY_RC
    2.DAC��ʼ��ѡ���VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    3.VDDHP��VDDIO��һ��:      #define VDDHP_VDDIO_SHT   1

AX2073B:
    1.ϵͳ����ѡ�񣬵�PIN����#define CRY_PIN_NUM       CRY_1PIN
    2.DAC��ʼ��ѡ���VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    3.VDDHP��VDDIO��һ��:      #define VDDHP_VDDIO_SHT   1

AX2075:
    1.ϵͳ����ѡ���޾���   #define SYS_CRY           CRY_RC
    2.DAC��ʼ��ѡ���VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    3.VDDHP��VDDIO��һ��:      #define VDDHP_VDDIO_SHT   1

AX1070:
    1.ʹ��multi-bit DAC:       #define USE_MULTI_BIT_DAC 1
    2.DAC��ʼ��ѡ���VCM:    #define DAC_INIT_TYPE     DAC_VCM_WITH_CAP
    3.VDDHP��VDDIO�ֿ�:        #define VDDHP_VDDIO_SHT   0

AX1071:
    1.ϵͳ����ѡ�񣬵�PIN����#define CRY_PIN_NUM       CRY_1PIN
    2.ʹ��multi-bit DAC:       #define USE_MULTI_BIT_DAC 1
    3.DAC��ʼ��ѡ���VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    4.VDDHP��VDDIO�ֿ�:        #define VDDHP_VDDIO_SHT   0

AX1073:
    1.ϵͳ����ѡ���޾���   #define SYS_CRY           CRY_RC
    2.ʹ��multi-bit DAC:       #define USE_MULTI_BIT_DAC 1
    3.DAC��ʼ��ѡ���VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    4.VDDHP��VDDIO��һ��:      #define VDDHP_VDDIO_SHT   1

AX1073B:
    1.ϵͳ����ѡ�񣬵�PIN����#define CRY_PIN_NUM       CRY_1PIN
    2.ʹ��multi-bit DAC:       #define USE_MULTI_BIT_DAC 1
    3.DAC��ʼ��ѡ���VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    4.VDDHP��VDDIO��һ��:      #define VDDHP_VDDIO_SHT   1

AX1076:
    1.ϵͳ����ѡ�񣬵�PIN����#define CRY_PIN_NUM       CRY_1PIN
    2.ʹ��multi-bit DAC:       #define USE_MULTI_BIT_DAC 1
    3.DAC��ʼ��ѡ���VCM:    #define DAC_INIT_TYPE     DAC_VCM_NO_CAP
    4.VDDHP��VDDIO��һ��:      #define VDDHP_VDDIO_SHT   1

 *****************************************************************************/
