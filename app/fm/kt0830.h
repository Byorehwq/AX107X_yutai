#ifndef _KT0830_H_
#define _KT0830_H_

//#define R32P768K
//#define 	MUTE_WITH_HIZ			       // Turn on only if audio outpus is required to be HiZ in mute state
//#define	LONG_ANTENNA				// Turn on only if the length of antenna exceeds 2m
#define 	SEPERATION_ENHANCE			// Turn on only if stereo seperation needs to be enhanced.
#define 	SEEK_WITH_SNR				// Turn on only if SNR double check is required in SEEK process
// Newly Added
//#define         ULTRA_LOW_Q				// Turn on only if inductor'Q is lower than 20

#define    KT0837_RD 		  0x6f
#define    KT0837_WR 		  0x6e

//Newly Added
#define IND_MATCH  85				// Unit is nH
#define CEXT_MATCH 11500				// Unit if fF
#define INDUCTOR_Q 1				// 3 for highest Q;0 for lowest Q
#define INIT_FAIL_TH 20
#define SEEK_UP_LIMIT 10800
#define SEEK_DOWN_LIMIT 8700
#define SEEKUP 1
#define SEEKDOWN 0
#define CYCMODE 0					// Cycling seek mode
#define EDGEMODE 1					// Stop at the band edge mode
#define LONG_ANT 1
#define SHORT_ANT 0
#define LONG_ANT_CAP 27000			// The cap value used in matching long antenna or earphone antenna
#define CHANNEL_STEP 1
#define AFCTH_PREV 10				// Range from 0 to 127
#define AFCTH 15					// Range from 0 to 127
#define AFCTH_NEXT 10				// Range from 0 to 127
#define TST_TH 0x2800
#define SNR_TH 0x30

#if 0
#define INIT_FAIL_TH 20
#define SEEK_UP_LIMIT 10800
#define SEEK_DOWN_LIMIT 8750
#define SEEKUP 1
#define SEEKDOWN 0
#define CHANNEL_STEP 1
#define AFCTH_PREV 0				//Range from 0 to 127
#define AFCTH 15				//Range from 0 to 127
#define AFCTH_NEXT 0				//Range from 0 to 127
#define TST_TH 0x2800
#define SNR_TH 0x40
#endif

void kt0830_unmute(void);
void kt0830_mute(void);
void KT0830E_UnSoftMute(void);
void KT0830E_SoftMute(void);
void KT0830E_Tune(u16 frequency);
void KT_FMTune(u16 Frequency);
u8 KT_FMValidStation(u16 Frequency);
char KT_FMGetAFC(void);
u8 KT_FMGetSNR(void);

extern void kt0830_init(void);
extern void kt0830_set_freq(u16 freq);
extern void kt0830_set_vol(u8 vol);
extern void kt0830_off(void);
extern u8 kt0830_seek(u16 freq);
extern u8 kt0830_online(void);
#endif
