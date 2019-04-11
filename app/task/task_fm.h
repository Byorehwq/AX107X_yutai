/*****************************************************************************
 * Module    : Task
 * File      : task_fm.h
 * Author    :
 * Email     :
 * Function  : FM任务相关定义
 *****************************************************************************/
#ifndef TASK_FM_H
#define TASK_FM_H

extern IAR_CONST u8 tbl_anlvol[VOLUME_MAX+1];

u16 fm_get_diff(u8 ch_cur, u8 *chbuf, u8 len);
void task_fm_display(void);

#endif
