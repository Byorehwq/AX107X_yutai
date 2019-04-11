/*****************************************************************************
 * Module    : Task
 * File      : task_speaker.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ������������ض���
 *****************************************************************************/
#ifndef TASK_SPEAKER_H
#define TASK_SPEAKER_H

#if MIX_DEEP_EN
#define ECHO_MAX            15          //���켶��
#define ECHO_STEP           0x144       //ÿ������ļ��
#define ECHO_SAMPLE         72          //ÿ�μ����������
#define MIX_ADC_SAMPLE      0x8F        //ADC��������

#define ECHO_BUF_START_0    0x0800
#define ECHO_BUF_END_0      0x0F98
#define ECHO_BUF_START_1    0x4200
#define ECHO_BUF_END_1      0x54FC

#else
#define ECHO_MAX            10
#define ECHO_SAMPLE         64
#define ECHO_STEP           0x200
#define MIX_ADC_SAMPLE      0x7F

#define ECHO_BUF_START_0    0x0800
#define ECHO_BUF_END_0      0x1000
#define ECHO_BUF_START_1    0x4200
#define ECHO_BUF_END_1      0x5600
#endif

#if LIMIT_MIX_NOISE_EN
#define LIMIT_VALUE         0x04        //����������ֵ
#endif

void mix_proc_deep(u16 samples);
void mix_proc_fix(u16 samples, u8 type);
void task_speaker_display(void);
s8 lim_val(s16 *adr, u16 len, u8 type);

#endif
