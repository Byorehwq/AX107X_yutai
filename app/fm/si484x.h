#ifndef _Si484x_H_
#define _Si484x_H_

#define  enable_power_down_command


#define TUNER_MODE_FM   0
#define TUNER_MODE_AM   1
#define TUNER_MODE_SW   2

#define TUNER_MODE_CHANGE    0xA0
#define  RESTSI484XDIROUT()  P1DIR &= ~BIT(4);//P1PU1  |= BIT(4);
#define  SI484X_RST_HIGH()   P1 |= BIT(4)
#define  SI484X_RST_LOW()    P1 &= ~BIT(4)


#define	SM_RADIO_READY 0x80
#define	SM_RADIO_RESET 0x81
#define	SM_RADIO_POWERUP 0x82

#define FM      0
#define AM      1
#define SW      2

#define ATDD_POWER_DOWN 0x11

#define ATDD_POWER_UP   0xE1
#define ATDD_XOSCEN     0x80
#define ATDD_XOWAIT     0x40
//1--> external 32768Hz crystal;0--> RCLK
#define XOSCEN          1
//0--> wait 600ms for crystal oscillator setup, for normal crystal
//1--> wait 850ms for crystal oscillator setup, for worse crystal which the setup time is long
#define XOWAIT_LONG     1

#define ATDD_GET_STATUS 0xE0

#define ATDD_AUDIO_MODE 0xE2
#define ADJPT_STEO      0x10
#define ADJPT_ATTN      0x08
#define FM_MONO         0x04

#define HOSTERR         0x80
#define HOSTRST         0x40
#define HOSTPWRUP       0x20
#define INFORDY         0x10
#define STATION         0x08
#define STEREO          0x04
#define TUNERPRI        0x02
#define HOSTBAND        0x01


#define FM_BAND_TOP     1090
#define FM_BAND_BOTTOM  875
#define FM_BAND_SPACING 10

#define AM_BAND_TOP     1710
#define AM_BAND_BOTTOM  522
#define AM_BAND_SPACING 9

#define SW_BAND_TOP     26500
#define SW_BAND_BOTTOM  2300
#define SW_BAND_SPACING 5



#define SI484X_OUT_VOL          63

void atdd_audio_mode(u8 adj_st,u8 adj_attn,u8 force_mono,u8 audio_mode);
void set_property(u16 property_id,u16 property_value);
void wait_cts(void);
extern u8 band_index;
extern u8 tuner_mode;
extern void si484x_init(void);
extern void Si484x_set_freq(u16 freq);
extern void Si484x_off(void);
extern void Si484x_set_vol(u8 vol);
extern u8 Si484x_seek(u16 freq);
extern void Si484x_mute(void);
extern void Si484x_unmute(void);
extern u8 Si484x_online(void);
extern void parse_atdd_status(void);
void config_tune(void);
extern void atdd_power_up(u8 xoscen,u8 xowait,u8 band_index,u16 band_bottom,u16 band_top,u8 chspc);
#endif
