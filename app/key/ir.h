/*****************************************************************************
* Module    : Module
* File      : ir.h
* Author    : jingfa
* Email     :
* Function  : 红外驱动相关
*****************************************************************************/
#ifndef _IR_H_
#define _IR_H_


#define USERCODE_FF00      0xFF00
#define USERCODE_BF00      0xBF00
#define USERCODE_FD02      0xFD02
#define USERCODE_FE01      0xFE01

#define IR_TIMEOUT_CNT     80 //ir超时后的标志值

extern volatile __data u8 ir_state;
extern volatile __data u8 ir_cnt;
extern volatile __data u8 repeat_cnt;
extern volatile __data u16 ir_data;
extern volatile __data u16 ir_code;
//extern volatile bool ir_key_sta;

void ir_init(void);
u8 ir_process(void);
void ir_cap_isr(void);

#endif
