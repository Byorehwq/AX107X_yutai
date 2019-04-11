/*****************************************************************************
 * Module    : COMMON
 * File      : macro.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : 常用的宏定义
 *****************************************************************************/
#ifndef MACRO_H
#define MACRO_H

#define BIT(n)                  (1<<(n))
#define BYTE0(n)                ((unsigned char)(n))
#define BYTE1(n)                ((unsigned char)((n)>>8))
#define BYTE2(n)                ((unsigned char)(((unsigned short)((n)>>8))>>8))
#define BYTE3(n)                ((unsigned char)(((unsigned short)((n)>>16))>>8))

//进入中断服务程序，备份，并设置DPCON
#define ISR_ENTER()             asm("push 0x8b"); \
                                asm("push 0x86"); asm("anl 0x86, #0xc0"); \
                                asm("push 0x84"); \
                                asm("push 0x85");

//出中断服务程序，还原
#define ISR_EXIT()              asm("pop 0x85"); \
                                asm("pop 0x84"); \
                                asm("pop 0x86"); \
                                asm("pop 0x8b");

//memory切换
#define SELECT_IROM1()          {MEMCON &= 0xEE;}
#define SELECT_IROM2()          {MEMCON &= 0xEF; MEMCON |= 0x01;}
#define SELECT_IROM3()          {MEMCON |= 0x10;}
#define SELECT_OTP()            {MEMCON &= ~0x02;}
#define SELECT_DECRAM()         {MEMCON |= 0x02;}


#define select_irom1()          {asm("push 0x8b"); asm("anl 0x8b, #0xee");}
#define select_irom2()          {asm("push 0x8b"); asm("anl 0x8b, #0xef");asm("orl 0x8b, #0x01");}
#define select_irom3()          {asm("push 0x8b"); asm("orl 0x8b, #0x10");}
#define restore_mem()           {asm("pop 0x8b");}

//看门狗
#define WATCHDOG_INIT()         WDTCON = 0x1d           //看门狗时间：2s
#define WATCHDOG_CLR()          WDTCON |= BIT(5)        //清看门狗
#define WATCHDOG_KILL()         WDTCON = 0x00           //关看门狗

#define AT(seg) @ #seg

__sfr __no_init volatile unsigned short DPTR1 @ 0x84;

#define IAR_ROOT __root
#define IAR_CONST const __code
//xdata absolute
#define IAR_XDATA_A __root __no_init __xdata
//pdata absolute
#define IAR_PDATA_A __root __no_init __pdata
//idata absolute
#define IAR_IDATA_A __root __no_init __idata
//data absolute
#define IAR_DATA_A __root __no_init __data
//bit absolute
#define IAR_BIT_A __root __no_init __bit
//bdata absolute
#define IAR_BDATA_A __root __no_init __bdata


//软中断
#define CLR_SOFT_INT() SPMODE &= ~BIT(7);
#define KICK_SOFT_INT() do{SPMODE |= BIT(7);asm("nop");}while(0)

//大小端控制访问
#define LOAD_U32B(p) load_u32b(p)
#define LOAD_U32(p)  (*(u32 __xdata*)(p))
#define LOAD_U16B(p) load_u16b(p)
#define LOAD_U16(p)  (*(u16 __xdata*)(p))

#define U32_ACCESS(p) (*(u32 __xdata*)(p))
#define U16_ACCESS(p) (*(u16 __xdata*)(p))

#define SAVE_U32B(p, num) { \
    ((u8 __xdata*)p)[0] = BYTE3(num); \
    ((u8 __xdata*)p)[1] = BYTE2(num); \
    ((u8 __xdata*)p)[2] = BYTE1(num); \
    ((u8 __xdata*)p)[3] = BYTE0(num); \
}
//重定义系统函数
#define memset my_memset
#define memcpy my_memcpy
#define memcmp smemcmp

#if PRINTF_EN
#define printf(...) my_printf(__VA_ARGS__)
#else
#define printf(...)
#endif

#endif
