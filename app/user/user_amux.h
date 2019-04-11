/*****************************************************************************
 * Module    : User
 * File      : user_amux.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : AMUX的相关定义
 *****************************************************************************/
#ifndef USER_AMUX_H
#define USER_AMUX_H


#include "amux.h"
#include "auadc.h"


//ADC和AMUX通道选择
enum
{
    CH_SEL_LINEIN = 0,     //linein模式
    CH_SEL_FM,             //fm模式
    CH_SEL_SPK,            //speaker模式
    CH_SEL_USBMIC,         //usbmic模式
};

#define ADC_BUF_ADDR        0x4000
#define FILTER_ADDR         0x1460


//KARAOKE MIC gain sel
#define USER_MIC_GAIN       GAIN_30DB

#define AMUX_AIN_EN()       DACACON1 |=  BIT(0)      //enable AMUX AIN
#define AMUX_AIN_DIS()      DACACON1 &= ~BIT(0)      //disable AMUX AIN

#define DAC_ANL_EN()        DACACON1 |=  BIT(6)      //打开模拟dac
#define DAC_ANL_DIS()       DACACON1 &= ~BIT(6)      //关闭模拟dac

void amux_volume_set(void);
void mic_enable(void);
void mic_disable(void);

void energy_auadc_init(void);
void energy_init(void);
void energy_start(void);
void energy_close(void);
void energy_stop(void);
void energy_calc_process(void);
void energy_calc(const u16 __code *tbl);
extern IAR_CONST u16 tbl_energy_music[12];
extern IAR_CONST u16 tbl_energy_amux[12];
extern IAR_XDATA_A volatile u8 energy_cnt;
extern IAR_XDATA_A volatile u8 energy_flag;
#endif
