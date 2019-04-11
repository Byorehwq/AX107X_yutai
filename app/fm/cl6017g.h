#ifndef _CL6017G_H_
#define _CL6017G_H_


void CL6017G_Write(u8 size);
void CL6017G_Read(u8 size);
__near_func void CL6017G_mute(u8 mute);
extern __near_func void cl6017g_init(void);
extern __near_func void cl6017g_set_freq(u16 freq);
extern __near_func void cl6017g_set_vol(u8 vol);
extern __near_func void cl6017g_off(void);
extern __near_func u8 cl6017g_seek(u16 freq);
extern __near_func u8 cl6017G_online(void);
#endif
