#ifndef _DAC_H_
#define _DAC_H_

typedef union
{
	struct
	{
		u8 fade_en :            1;  //�����뵭��
		u8 mute_en :            1;  //�Ƿ���
		u8 dac_stop :           1;  //DAC�Ƿ�ֹͣ���
		u8 digvol_en :          1;  //��������ʹ��
		u8 sound_en :           1;  //��̬����ʹ��
		u8 sound_flag :         1;  //��̬�����־
		u8 sound_sta :          1;  //��̬����״̬
		u8 key_voice :          1;  //����������
	};
	u8 all;                             //����λ
} type_dac_flag;

typedef struct
{
	u8 sample;              //DAC��ǰ������
	u8 anl_volcur;          //DAC��ǰֵ
	u8 anl_volset;          //DACӦ��ֵ
	u8 anl_vol;             //DACʵ����������Ҫ���ǵ����������ֵ
	u16 dig_vol;            //����DAC��ʵ������
	u8 fade;                //���뵭�����ơ�1Ϊ������0Ϊ����
} type_dac_ctl;

typedef struct
{
	u8 sta;         //DAC��ʼ��״̬
	u8 delay_count; //��ʱ����
} type_dac_init_ctl;

typedef struct
{
	u8 en;                   //�Ƿ���Ҫdac���뵭��
} type_fade_ctl;

extern IAR_XDATA_A type_dac_ctl dac_ctl;
extern IAR_BDATA_A type_dac_flag b_dac;
extern IAR_DATA_A type_dac_init_ctl dac_init_ctl;
extern IAR_XDATA_A type_fade_ctl fade_ctl;

//DAC��ʼ��
#define DAC_INIT_START      0x00
#define DAC_INIT_END        0x20
#define DAC_INIT_IDLE       0xff


void dac_init(void);
void dac_init_process(void);
void dac_init_wait(void);
void dac_channel_disconnect(void);

void dac_calc_eqgain(u8 *tbl_gain, u8 *tbl_eq);
void key_voice_init(void);


//�������
u8   at_read(u8 adr);
void at_write(u8 adr, u8 dat);
void dac_reset(void);
void dac_set_sample(u8 sample);         //����DAC������


//ֱ��DAC
enum
{
    DAC_SPA_8K,         //8k
    DAC_SPA_16K,        //16k
    DAC_SPA_32K,        //32k
    DAC_SPA_44_1K,      //44.1k
    DAC_SPA_48K,        //48k
    DAC_SPA_S8K,        //sync 8k
    DAC_SPA_S16K,       //sync 16k
    DAC_SPA_S32K,       //sync 32k
    DAC_SPA_S44_1K,     //sync 44.1k
    DAC_SPA_S48K,       //sync 48k
};

void dac_out_init(u8 spr_sf);
void task_dac_test(void);
void dac_clr_buf(void);
void copy_to_dacbuf(void *buf, u8 len);

//trim���
void dac_clr_trim(void);
void dac_get_trim(void);
void dac_set_trim(void);

//���뵭������
void dac_ctl_init(void);        //��ʼ����
void dac_fade_in_auto(void);    //�жϵ���
bool dac_fade_out_auto(void);   //�жϵ���

void dac_fade_auto(void);       //���뵭������timer�жϵ���
void dac_fade_in(bool digital); //����������ע������ģ�⡢����������
void dac_fade_out(void);        //��������
void dac_fade_wait(void);       //�ȴ����뵭�����
bool dac_fade_busy(void);       //�Ƿ����ڵ�������
void dac_set_volume(u8 vol);
void dac_set_volume_direct(u8 vol);
void dac_set_anlvol(u8 vol);
void dac_volume_fade_set(void);
void dac_set_digvol(u16 vol);
#endif
