#ifndef _DAC_H_
#define _DAC_H_

typedef union
{
	struct
	{
		u8 fade_en :            1;  //允许淡入淡出
		u8 mute_en :            1;  //是否静音
		u8 dac_stop :           1;  //DAC是否停止输出
		u8 digvol_en :          1;  //数字音量使能
		u8 sound_en :           1;  //动态降噪使能
		u8 sound_flag :         1;  //动态降噪标志
		u8 sound_sta :          1;  //动态降噪状态
		u8 key_voice :          1;  //按键音淡入
	};
	u8 all;                             //所有位
} type_dac_flag;

typedef struct
{
	u8 sample;              //DAC当前采样率
	u8 anl_volcur;          //DAC当前值
	u8 anl_volset;          //DAC应设值
	u8 anl_vol;             //DAC实际音量，主要考虑淡出音量后的值
	u16 dig_vol;            //数字DAC的实际音量
	u8 fade;                //淡入淡出控制。1为有声，0为无声
} type_dac_ctl;

typedef struct
{
	u8 sta;         //DAC初始化状态
	u8 delay_count; //延时计数
} type_dac_init_ctl;

typedef struct
{
	u8 en;                   //是否需要dac濙入淡出
} type_fade_ctl;

extern IAR_XDATA_A type_dac_ctl dac_ctl;
extern IAR_BDATA_A type_dac_flag b_dac;
extern IAR_DATA_A type_dac_init_ctl dac_init_ctl;
extern IAR_XDATA_A type_fade_ctl fade_ctl;

//DAC初始化
#define DAC_INIT_START      0x00
#define DAC_INIT_END        0x20
#define DAC_INIT_IDLE       0xff


void dac_init(void);
void dac_init_process(void);
void dac_init_wait(void);
void dac_channel_disconnect(void);

void dac_calc_eqgain(u8 *tbl_gain, u8 *tbl_eq);
void key_voice_init(void);


//解码相关
u8   at_read(u8 adr);
void at_write(u8 adr, u8 dat);
void dac_reset(void);
void dac_set_sample(u8 sample);         //设置DAC采样率


//直推DAC
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

//trim相关
void dac_clr_trim(void);
void dac_get_trim(void);
void dac_set_trim(void);

//淡入淡出控制
void dac_ctl_init(void);        //初始化用
void dac_fade_in_auto(void);    //中断调用
bool dac_fade_out_auto(void);   //中断调用

void dac_fade_auto(void);       //淡入淡出处理，timer中断调用
void dac_fade_in(bool digital); //淡入音量（注意区分模拟、数字音量）
void dac_fade_out(void);        //淡出音量
void dac_fade_wait(void);       //等待淡入淡出完成
bool dac_fade_busy(void);       //是否正在淡出淡出
void dac_set_volume(u8 vol);
void dac_set_volume_direct(u8 vol);
void dac_set_anlvol(u8 vol);
void dac_volume_fade_set(void);
void dac_set_digvol(u16 vol);
#endif
