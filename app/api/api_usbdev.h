#ifndef _API_USBDEV_H_
#define _API_USBDEV_H_

#define DEV_MASS_BIT            0
#define DEV_SPK_BIT             1
#define DEV_MIC_BIT             2
#define DEV_HID_BIT             3

#define USBIE_EN()    	        IE1 |= (1<<0)
#define USBIE_DIS()   	        IE1 &= ~(1<<0)


extern IAR_CONST u8 tbl_usb_language_id[4];
extern IAR_CONST u8 tbl_usb_str_manufacturer[16];
extern IAR_CONST u8 tbl_usb_str_product[28];

/* ep0 */
__root void usb_ep0_init(void);
__root void usb_ep0_reset(u8 dev_type);
__root void usb_isr_mask(void);
__root void usb_hid_delay(void);//公共区
__root void usb_mass_delay(void);//公共区
extern void usb_ep0_process(void);

/* mass stroage */
__root void usb_mass_init(void);
__root void usb_mass_exit(void);
__root void usb_mass_reset(void);
__root void usb_mass_process(void);

/* audio */
typedef struct
{
	u8 *adc_root;
	u8 *adc_end;
	u8 *adc_ptr;
	s16 adc_cnt;
	s16 adc_total;
	s8 mic_speed;

	s8 spk_vol_dir;
	u8 spk_vol_cur;
	u8 spk_vol_set;
	u8 spk_vol_max;
	bool spk_buf_idx;
} type_usb_audio_ctl;

__root void usb_audio_init(u8 vol_level);
__root void usb_audio_reset(void);
__root u8 usb_spk_set_volume_do(void);
__root void iso_in_packet(u8 len);
void usb_mic_set_ptr(u8 *buf);

/* hid */
#define HID_CTL_STOP        0x80
#define HID_CTL_FASTFORWARD 0x40
#define HID_CTL_PREFILE     0x20
#define HID_CTL_NEXTFILE    0x10
#define HID_CTL_PLAYPAUSE   0x08
#define HID_CTL_MUTE        0x04
#define HID_CTL_VOLDOWN     0x02
#define HID_CTL_VOLUP       0x01
#define HID_CTL_NONE        0x00

void usb_hid_init(void);
__root void usb_hid_reset(void);
bool usb_hid_send(u8 hid_key);
__root bool usb_hid_key(u8 hid_key);

#endif
