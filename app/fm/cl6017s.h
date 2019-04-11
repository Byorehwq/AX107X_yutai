#ifndef _CL6017S_H_
#define _CL6017S_H_



__near_func void CL6017S_Write(u8 *write_data,u8 size);
__near_func void  CL6017S_Read(u8 *read_data,u8 size);

extern __near_func void cl6017s_init(void);
extern __near_func void cl6017s_set_freq(u16 freq);
extern __near_func void cl6017s_set_vol(u8 vol);
extern __near_func void cl6017s_off(void);
extern __near_func u8 cl6017s_seek(u16 freq);
extern __near_func u8 cl6017s_online(void);
#endif
