/*****************************************************************************
 * Module    : Config
 * File      : config.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : �����ļ�
 *****************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#include "cfg_def.h"

/*****************************************************************************
 * SDK�汾   :
 *****************************************************************************/


/*****************************************************************************
 * Module    : �û�ѡ��
 *****************************************************************************/
//����ǰ���Ķ��ļ�config.c�С�ע�����

#ifdef AX207X_TAG
#define USER_SEL            USER_AX2070_DEMO    //�û�ѡ��
#endif

#ifdef AX107X_TAG
#define USER_SEL            USER_AX1070_DEMO    //�û�ѡ�� (�����û�ʹ�õ��� AX1071)
#endif


//�����û��Զ�������
#if (USER_SEL == USER_TEST)
#include "config_test.h"                //���԰�
#elif (USER_SEL == USER_AX2070_DEMO)
#include "config_ax2070_demo.h"         //AX2070 DEMO��
#elif (USER_SEL == USER_AX2073_DEMO)
#include "config_ax2073_demo.h"         //AX2073 DEMO��?
#elif (USER_SEL == USER_AX1070_DEMO)
#include "config_ax1070_demo.h"         //AX1070 DEMO��
#elif (USER_SEL == USER_AX1071_DEMO)
#include "config_ax1071_demo.h"         //AX1071 DEMO��?
#elif (USER_SEL == USER_AX1073_DEMO)
#include "config_ax1073_demo.h"         //AX1073 DEMO��?
#elif (USER_SEL == USER_AX1076_DEMO)
#include "config_ax1076_demo.h"         //AX1076 DEMO��
#endif


#include "config_default.h"                 //Ĭ������


#endif
