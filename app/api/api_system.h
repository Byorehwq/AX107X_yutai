/*****************************************************************************
 * Module    : API
 * File      : api.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : API系统函数
 *****************************************************************************/
#ifndef API_SYSTEM_H
#define API_SYSTEM_H

typedef struct
{
	u8 delay_sys;                   //根据系统时钟来控制小的软件延时
	u8 delay_cnt;                   //延时控制
	u8 clk_mode;                    //时钟模式
} type_sys_var;

extern volatile IAR_DATA_A type_sys_var sys;

typedef struct
{
	u8 usb_init_fix;
} type_sys_fix;
extern IAR_XDATA_A type_sys_fix sys_fix;

/* uart.c */
__root void uart_init(void);
__root void uart_putc(u8 ch);
__root void uart_putchar(u8 ch);

/* printf.c */
__root void my_printf(const char __code *format, ...);
__root void printf_array(const void *buff, u16 len);

/* system.c & system_asm.s51 */
__root void delay_5ms(u8 x);
void delay_nop(u8 cnt);

__root void dummy_void(void);
__root void dummy_u8(u8 ch);

__root void my_memcpy(void *dst, void *src, u16 size);
__root void smemcpy(void *des, void *src, u8 len);
__root void memcpy_c(void *dest, const void __code *src, u16 len);
__root void smemcpy_c(void *dst, const void __code *src, u8 len);

__root void my_memset(void *buf, u8 ch , u16 cnt);
__root void smemset_i(void __idata *buf, u8 ch, u8 cnt);

__root u8 smemcmp(void *des, void *src, u8 cnt);
__root u8 smemcmp_c(void *dst, const void __code *src, u8 cnt);

__root u32 swap_u32(u32 val);
__root u32 load_u32b(void __xdata *ptr);
__root u32 load_u16b(void __xdata *ptr);

__root void delay_hook(u8 cnt);
void delay_1ms(u8 cnt);     //1ms 定时器延时
__root void clear_u32(u32 *var);

__root void timer2_init(void);
__root char *string_tolow(char *str, u8 len);
void rand_init(u16 seed);
#ifdef AX207X_TAG
u16 rand(u16 max);
#else
u16 rand(u16 max,u16 cur);
#endif

/* msg */
__root void msg_init(u8 __pdata *msg_pool, u8 depth);
__root void flush_msg(void);
__root void put_msg(u8 msg);
__root u8 get_msg(void);


#endif
