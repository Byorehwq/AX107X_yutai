#ifndef _AUADC_H_
#define _AUADC_H_

//auadc channel
#define AUADC_CH_MIC            0                   //MICͨ��
#define AUADC_CH_AUX_L          1                   //������
#define AUADC_CH_AUX_R          2                   //������
#define AUADC_CH_AUX_LR         3                   //˫����
#define AUADC_CH_AUX_ML         (1|AUADC_BIT_MONO)  //��������������
#define AUADC_CH_AUX_MR         (2|AUADC_BIT_MONO)  //��������������

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
	u8 channel;         //������������AUADC_CH_XXX
	u8 sample_rate;     //������
	u8 filter;          //�˲���
	u8 buf_addr;        //��128�ֽڶ���
	u16 samples;        //������
	void (*auadc_process)(void);
	u8 discard_cnt;
} type_auadc_ctl;

extern type_auadc_ctl auadc_ctl;
extern IAR_CONST type_auadc_ctl auadc_ch_sel[];

void auadc_init(u8 ch_sel);
void auadc_start(void);
void auadc_stop(void);

#endif //_AUADC_H_
