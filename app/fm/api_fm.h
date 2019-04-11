/*--------------------------------------------------------------------------*/
/***@file fm_api.c
    @brief 操作fm模块
    @details
    @author
    @date
    @note
*/
/*----------------------------------------------------------------------------*/
#ifndef _API_FM_H_
#define _API_FM_H_

#include "typedef.h"
#include "api_system.h"

//支持的FM模块列表
#define FM_NO                           0
#define FM_RDA5807                      1
#define FM_BK1080                       2
#define FM_QN8035                       3
#define FM_QN8065                       4
#define FM_KT0830                       5
#define FM_SI484X                       6           //未调试
#define FM_AR1010                       7           //未调试
#define FM_AR1019                       8           //未调试
#define FM_CL6017S                      9           //未调试
#define FM_CL6017G                      10          //未调试
#define FM_RTC6218                      11

#if FM_SEEK_50K_STEP
#define CH_BUF_MAX                      52
#else
#define CH_BUF_MAX                      26
#endif

typedef struct
{
	u8  ch_buf[CH_BUF_MAX];
#if ((FM_CLOCK == USE_SD_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK) || FM_CLK_IO_MUX_SDCLK)
	bool fm_mode_flag;
#endif
	u8  ch_total;
	u8  ch_cur;
	u16 fm_freq;
	u8  fm_type;
#if FM_QN8035_EN
	u8  init_sta;    //初始化状态
#endif
} type_fm_ctl;

extern type_fm_ctl fm_ctl;

#define DELAY_MS(x)			delay_hook(x)           //1ms

#if FM_SEEK_50K_STEP
#define FM_MIN_FREQ			8730					//<最小FM频道
#define FM_MAX_FREQ			10800					//<最大FM频道
#else
#define FM_MIN_FREQ			875						//<最小FM频道
#define FM_MAX_FREQ			1080					//<最大FM频道
#endif

//操作函数接口
extern u8   fm_read_id(void);
extern void fm_rx_init(void);
extern u8   fm_rx_seek(u16 freq);
extern void fm_rx_set_freq(u16 freq);
extern void fm_rx_off(void);
extern void fm_rx_set_vol(u8 vol);
extern void fm_init_process(void);
#endif
