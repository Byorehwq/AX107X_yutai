/*****************************************************************************
 * Module    : User
 * File      : file_filter.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �ļ�ɨ�������
 *****************************************************************************/
#ifndef FILE_FILTER_H
#define FILE_FILTER_H

#define SCAN_SUB_FOLDER     0x01        //�Ƿ���Ҫɨ����Ŀ¼
#define SCAN_SPEED          0x20        //ʹ�����ļ�����

bool file_filter_music(void);           //���ֲ��ŵ��ļ�������
bool dir_filter_music(void);            //���ֲ��ŵ�Ŀ¼������
#if MPX_SUPPORT
extern bool f_Encrypt; 	// 1: is Encrypt MP3, 0: is normal MP3
#endif
#endif
