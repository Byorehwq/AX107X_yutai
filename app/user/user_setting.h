/*****************************************************************************
 * Module    : User
 * File      : user_setting.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ϵͳ��������
 *****************************************************************************/
#ifndef USER_SETTING_H
#define USER_SETTING_H

#define VOLUME_MAX          50          //�������(�������ֵʱ����Ҫͬʱ����������tbl_anlvol[])
#define EQ_NUM_MAX          5           //EQ���ֵ

typedef struct
{
	u8 mode_play;               //����ģʽ
	u8 eq_num;                  //EQѡ��
	u8 volume;                  //����
	u8 prv_volume;              //��������
#if TASK_SPEAKER_EN
	u8 mix_step;                //����������
#endif
	u16 rc2m;
#if (BATTERY_CHECK || SOFT_POWEROFF_EN)
	u8 bat_sta;                 //��ص���״̬
#endif
	u8 dev_save;                //�ϵ�ѡ����豸
	u8 volatile delay_cnt;      //��ʱ����
#if TASK_FLASH_EN
	u8 flash_music_sta;			//SPI MP3���ŵ�Ŀ¼λ��
#endif
#if USB_HID_SUPPORT
	u8 volatile hid_vol_delay_cnt; //����HID��������ʱ�˳�
#endif
	u8 irtc_poweron;            //irtc�ϵ��־
	u8 user_power_down_flag;
	u8 bat_sta_flag;			// ��ŵ�ѹ״̬��־λ  1: ����ѹ  0: ����    20180314
	u8 alarm_cnt;				// ������ʱ��      						20180315
	u8 bat_charger_sta;			// ��س��״̬  0: δ���  1: ���ڳ�(δ����)  2: �ѳ���  20180316
	u8 bat_charger_cnt;			// ���״̬��ʱ��   0xff  ����    0x00  ����л�δ����
} type_sys_ctl;

typedef union
{
	struct
	{
		u8 dev_change :         1;      //�Ƿ����豸�䶯
		u8 lvd_flag :           1;      //�͵��־
#if (SYS_CRY == CRY_AUTO)
		u8 xosc_flag :          1;      //�Ƿ����ⲿ����
#endif
		u8 modeswitch_flag :    1;      //�Ƿ����ֲ����аγ�SD���л���FM
#if USE_KEDIR
		u8 kedir_flag:          1;      //��ʼ�������һ������־
#endif
#if SOFT_POWEROFF_EN
		u8 press_key_power :    1;      //��ػ�����״̬ 1��ʾ����
		u8 wait_key_power_up:   1;
#endif
#if KARAOKE_EN
		u8 karaoke_flag :       1;
#endif
	};
	u8 all;                             //����λ
} type_sys_flag;

extern IAR_BDATA_A type_sys_flag b_sys;
extern type_sys_ctl sys_ctl;

u8 user_change_eq(u8 eq_num);           //���õ�ǰEQ����ʼ��ʱ����
void user_change_volume(u8 volume);     //���õ�ǰ��������ʼ��ʱ����

bool user_set_volume(u8 volume);        //���沢�����������ı�����ʱ����
bool user_set_eq(u8 eq_num);            //���沢����EQ���ı�EQʱ����
void user_set_mute(void);
void user_kick_kv(void);
void user_chk_kv(void);
bool user_set_fm_volume(u8 volume);     //FM�������أ�������������
void sdclk_output_clk_start(void);
void sdclk_output_clk_stop(void);


#endif
