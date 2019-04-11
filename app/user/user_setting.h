/*****************************************************************************
 * Module    : User
 * File      : user_setting.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 系统参数设置
 *****************************************************************************/
#ifndef USER_SETTING_H
#define USER_SETTING_H

#define VOLUME_MAX          50          //最大音量(更改这个值时，需要同时更改音量表tbl_anlvol[])
#define EQ_NUM_MAX          5           //EQ最大值

typedef struct
{
	u8 mode_play;               //播放模式
	u8 eq_num;                  //EQ选择
	u8 volume;                  //音量
	u8 prv_volume;              //记忆音量
#if TASK_SPEAKER_EN
	u8 mix_step;                //扩音器级数
#endif
	u16 rc2m;
#if (BATTERY_CHECK || SOFT_POWEROFF_EN)
	u8 bat_sta;                 //电池电量状态
#endif
	u8 dev_save;                //上电选择的设备
	u8 volatile delay_cnt;      //延时控制
#if TASK_FLASH_EN
	u8 flash_music_sta;			//SPI MP3播放的目录位置
#endif
#if USB_HID_SUPPORT
	u8 volatile hid_vol_delay_cnt; //用于HID调音量超时退出
#endif
	u8 irtc_poweron;            //irtc上电标志
	u8 user_power_down_flag;
	u8 bat_sta_flag;			// 电磁电压状态标志位  1: 报低压  0: 不报    20180314
	u8 alarm_cnt;				// 报警计时器      						20180315
	u8 bat_charger_sta;			// 电池充电状态  0: 未充电  1: 正在充(未充满)  2: 已充满  20180316
	u8 bat_charger_cnt;			// 充电状态计时器   0xff  正常    0x00  充电中或未充满
} type_sys_ctl;

typedef union
{
	struct
	{
		u8 dev_change :         1;      //是否有设备变动
		u8 lvd_flag :           1;      //低电标志
#if (SYS_CRY == CRY_AUTO)
		u8 xosc_flag :          1;      //是否有外部晶振
#endif
		u8 modeswitch_flag :    1;      //是否音乐播放中拔出SD卡切换到FM
#if USE_KEDIR
		u8 kedir_flag:          1;      //开始检测三合一按键标志
#endif
#if SOFT_POWEROFF_EN
		u8 press_key_power :    1;      //软关机按键状态 1表示按下
		u8 wait_key_power_up:   1;
#endif
#if KARAOKE_EN
		u8 karaoke_flag :       1;
#endif
	};
	u8 all;                             //所有位
} type_sys_flag;

extern IAR_BDATA_A type_sys_flag b_sys;
extern type_sys_ctl sys_ctl;

u8 user_change_eq(u8 eq_num);           //设置当前EQ，初始化时调用
void user_change_volume(u8 volume);     //设置当前音量，初始化时调用

bool user_set_volume(u8 volume);        //保存并设置音量，改变音量时调用
bool user_set_eq(u8 eq_num);            //保存并设置EQ，改变EQ时调用
void user_set_mute(void);
void user_kick_kv(void);
void user_chk_kv(void);
bool user_set_fm_volume(u8 volume);     //FM不进主控，调节音量函数
void sdclk_output_clk_start(void);
void sdclk_output_clk_stop(void);


#endif
