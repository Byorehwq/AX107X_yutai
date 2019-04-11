#ifndef _AR1010_H_
#define _AR1010_H_

#define ADDR_DEVID	0x1B // the address of DEVID register
#define ADDR_CHIPID 0x1C // the address of CHIPID register

#define DEVID_VerF	   	0x65B1 // VERSION+MFID
#define CHIPNO_AR1010	0x1010 // 16'b0001 0000 0000 0000
#define VER_F			0x6

#define ON  1
#define OFF 0

#define UEBAND	0x0000 // US & Europe BAND 87.5MHz to 108MHz
#define JBAND	0x1000 // Japen BAND 76MHz to 90MHz
#define JWBAND	0x1800 // Japen BAND (wide) 76 MHz to 108MHz

#define SPACE100K	1
#define SPACE200K	0
#define SEEKUP	1
#define SEEKDOWN 0

#define ADDR_STATUS	0x13 // the address of status register
#define MASK_STC 0x0020  // Seek/Tune/PowerOn complete  D5 in  adress 13H
#define MASK_SF	 0x0010  //  Seek Fail D4 in address 13H
#define MASK_ST	 0x0008  //  Stereo  D3 in address 13H
#define MASK_READCHAN 0xFF80 // D7~D15 in address 13H
#define SHIFT_READCHAN 7

#define ADDR_RSSI	0x12
#define MASK_RSSI 	0xFE00
#define SHIFT_RSSI  9


#define ADDR_SEEK_SETTING	0x11
#define SEEK_SETTING	0x2000
#define SEEK_MASK		0xC3FF


#define AR1000_I2C_WRITE_ADDR  0x20		// 0010000  0
#define AR1000_I2C_READ_ADDR   0x21		// 0010000  1

#define AR1000_MUTE_ON { Reg_Data[1].BIT.B9 = ON; }
#define AR1000_MUTE_OFF { Reg_Data[1].BIT.B9 = OFF;}

#define AR1000_TUNE_ON 	{ Reg_Data[2].BIT.B1 = ON;}
#define AR1000_TUNE_OFF { Reg_Data[2].BIT.B1 = OFF;}

#define AR1000_SEEK_ON 	{ Reg_Data[3].BIT.B6 = ON;}
#define AR1000_SEEK_OFF { Reg_Data[3].BIT.B6 = OFF;}

#define AR1000_RDSInt_ON 	{ Reg_Data[1].BIT.B14 = ON;}
#define AR1000_RDSInt_OFF   { Reg_Data[1].BIT.B14 = OFF;}

// data structure
typedef union DATA_TYPE_S
{
	unsigned int i;
	unsigned char c[2];		//c[0] MSByte  c[1] LSByte
	struct
	{
		unsigned char
		B8:1,  //LSBit  C[0]
		   B9:1,
		   B10:1,
		   B11:1,
		   B12:1,
		   B13:1,
		   B14:1,
		   B15:1,  //MSBit
		   B0:1,
		   B1:1,
		   B2:1,
		   B3:1,
		   B4:1,
		   B5:1,
		   B6:1,
		   B7:1;
	} BIT;
} DATA_TYPE_S;

void AR1010_Write(u8 Reg);
u16 AR1010_Read(u8 Reg);
void SetAR1000_Freq2CHAN(u16 Freq100KHz);

extern void ar1010_init(void);
extern void ar1010_set_freq(u16 freq);
extern void ar1010_set_vol(u8 vol);
extern void ar1010_off(void);
extern u8 ar1010_seek(u16 freq);
extern u8 ar1010_online(void);
#endif

