/*****************************************************************************
 * Module    : Module
 * File      : clock.c
 * Author    : jingfa
 * Email     :
 * Function  : 系统时钟的相关配置
 *****************************************************************************/
#ifndef __CLOCK_H__
#define __CLOCK_H__


//系统时钟配置
enum
{
    SYS_24M,        //PLL=48M, SYS=24M
    SYS_48M,        //PLL=48M, SYS=48M
    SYS_RC_1M,      //RC=1M  , SYS=1M, 关PLL, 关RC12M, 低功耗模式
    SYS_RC_2M,      //RC=2M  , SYS=2M, 关PLL, 关RC12M, 低功耗模式
#if (FM_CLOCK == USE_SD_IO_32KHZ_CLK)
    SYS_12M,
#endif
    SYS_INVALID,
};

void sysclk_set(void);                  //上电系统时钟初始化，使用RC10M作为时钟源
bool auto_check_x32k(void);             //自动检测是否有晶振，需要先使用RC10M作为时钟源
void sys_pll_xosc(void);                //系统时钟从RC10M切换到晶振
u8 get_sys_clk(void);                   //获取当前系统时钟模式
void set_sys_clk(u8 mode);              //设置当前系统时钟模式

#endif

