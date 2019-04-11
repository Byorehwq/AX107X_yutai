/*****************************************************************************
 * Module    : User
 * File      : file_filter.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 文件扫描过滤器
 *****************************************************************************/
#ifndef FILE_FILTER_H
#define FILE_FILTER_H

#define SCAN_SUB_FOLDER     0x01        //是否需要扫描子目录
#define SCAN_SPEED          0x20        //使用搜文件加速

bool file_filter_music(void);           //音乐播放的文件过滤器
bool dir_filter_music(void);            //音乐播放的目录过滤器
#if MPX_SUPPORT
extern bool f_Encrypt; 	// 1: is Encrypt MP3, 0: is normal MP3
#endif
#endif
