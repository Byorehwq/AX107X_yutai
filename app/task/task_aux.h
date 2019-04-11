/*****************************************************************************
 * Module    : Task
 * File      : task_aux.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : AUX任务相关定义
 *****************************************************************************/
#ifndef TASK_AUX_H
#define TASK_AUX_H

#if AUX_EQ_EN
#define AUX_ADC_SAMPLE          0x7F
#define AUX_EQ_SAMPLE           (AUX_ADC_SAMPLE + 1)

#if (AUADC_LINEIN == AUADC_CH_AUX_LR)
#define AUX_EQ_INPUT_ADDR   AUX_EQ_SAMPLE*2
#else
#define AUX_EQ_INPUT_ADDR   AUX_EQ_SAMPLE
#endif
#else
#define AUX_ADC_SAMPLE          0x78
#endif

void task_aux_display(void);
void auxeq_dac_out(u16 addr, u16 samples);
void task_aux_eq_process(void);
#endif
