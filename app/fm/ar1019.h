#ifndef _AR1019_H_
#define _AR1019_H_


/////////////////////////////////////////////////////
#define UEBAND			0x0000 // US & Europe BAND 87.5MHz to 108MHz
#define JBAND			0x1000 // Japen BAND 76MHz to 90MHz
#define JWBAND			0x1800 // Japen BAND (wide) 76 MHz to 108MHz

#define BAND_USA		0x01		// default: Eur & U.S
#ifdef  BAND_USA						// Eur & U.S
#define MIN_FREQUENCE		875
#define MAX_FREQUENCE		1080
#else								// Japan
#define MIN_FREQUENCE		760
#define MAX_FREQUENCE		910
#endif


#define SPACE100K		1
#define SPACE200K		0
#define SEEKUP			1
#define SEEKDOWN 		0

/* General propose constant */
#define AR1018_WRITE          	0x20
#define AR1018_READ           	0x21
#define ADDR_STATUS		0x13 // the address of status register
#define MASK_STC 		0x0020  // Seek/Tune/PowerOn complete  D5 in  adress 13H
#define MASK_SF	 		0x0010  //  Seek Fail D4 in address 13H
#define MASK_READCHAN 0xFF80 // D7~D15 in address 13H
#define SHIFT_READCHAN 7

#define ADDR_RSSI		0x12
#define MASK_RSSI 		0xFE00
#define SHIFT_RSSI  		9



extern __near_func void ar1019_init(void);
extern __near_func void ar1019_set_freq(u16 curf);
extern __near_func u8 ar1019_seek(u16 freq);
extern __near_func void AR1019_mute(u8 mute);
extern __near_func u16 ar1019_online(void);
extern __near_func void ar1019_off(void);
extern __near_func void ar1019_set_vol(u8 vol);

#endif
