#ifndef _BK1080_H_
#define _BK1080_H_

void bk1080_Write(u16 *writedata,u8 size);
u16 bk1080_Read_reg(u16 reg);


extern void bk1080_init(void);
extern void bk1080_set_freq(u16 freq, u8 mute_ctrl);
extern void bk1080_set_vol(u8 vol);
extern void bk1080_off(void);
extern u8 bk1080_seek(u16 freq);
extern u8 bk1080_online(void);

void bk1080_unmute(void)@"BK1080";
void bk1080_mute(void);



#endif
