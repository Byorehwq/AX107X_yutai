#ifndef FM_FADE_H
#define FM_FADE_H

typedef struct
{
	u8 ll;
	u8 lh;
	u8 rl;
	u8 rh;
	u8 trimcon1;
} TYPE_TRIM_VAL;
extern IAR_XDATA_A TYPE_TRIM_VAL trim_val;


void dac_trim_process(void);
void trim_B_to_A(void);
void fm_dac_fade_out(void);
void fm_dac_fade_in(void);

#endif
