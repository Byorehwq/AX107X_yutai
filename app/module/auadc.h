#ifndef _AUADC_H_
#define _AUADC_H_

//auadc channel
#define AUADC_CH_MIC            0                   //MIC通道
#define AUADC_CH_AUX_L          1                   //左声道
#define AUADC_CH_AUX_R          2                   //右声道
#define AUADC_CH_AUX_LR         3                   //双声道
#define AUADC_CH_AUX_ML         (1|AUADC_BIT_MONO)  //左声道单进单出
#define AUADC_CH_AUX_MR         (2|AUADC_BIT_MONO)  //右声道单进单出

//auadc samplerate
#define AUADC_SPR_8K            0
#define AUADC_SPR_16K           BIT(0)
#define AUADC_SPR_32K           BIT(1)

//auadc filter
#define AUADC_FS160K_FP5750     0
#define AUADC_FS160K_FP2910     BIT(0)
#define AUADC_FS128K_FP5750     BIT(1)
#define AUADC_FS128K_FP2810     (BIT(1)|BIT(0))
#define AUADC_FS160K_FP12K      BIT(2)
#define AUADC_FS128K_FP12K      (BIT(2)|BIT(0))




typedef struct
{
	u8 channel;         //声道数，见宏AUADC_CH_XXX
	u8 sample_rate;     //采样率
	u8 filter;          //滤波器
	u8 buf_addr;        //以128字节对齐
	u16 samples;        //样点数
	void (*auadc_process)(void);
	u8 discard_cnt;
} type_auadc_ctl;

extern type_auadc_ctl auadc_ctl;
extern IAR_CONST type_auadc_ctl auadc_ch_sel[];

void auadc_init(u8 ch_sel);
void auadc_start(void);
void auadc_stop(void);

#endif //_AUADC_H_
