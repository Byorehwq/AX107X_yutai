/*--------------------------------------------------------------------------*/
/**@file AR1010.C
   @brief FM模块AR1010驱动
   @details FM模块AR1010的驱动函数
   @author LZZ
   @date   2010-08-17
   @note   none
*/
/*----------------------------------------------------------------------------*/

#include "include.h"
#include "iic.h"
#include "ar1010.h"
#include "fm_api.h"

__no_init __data u8 seek_dir ;//@"FM_BUF";  // 0xFF:none  1:seek up  0:seek down

//#pragma constseg = AR1010_RX_TABLE const

unsigned int AR1000reg[18]=   //using  AR1018 XO function ( base on ARDSG-X1-xtal_v001.rgf )
{
	0xFFFB, // R0
	0x5B15, // R1
	0xD0B9, // R2
	0xA010, // R3
	0x0780, // R4
	0x28AB, // R5
	0x6400, // R6
	0x1EE7, // R7
	0x7141, // R8
	0x007D, // R9
	0x82C6, // R10
	0x4E55, // R11
	0x970C, // R12
	0xB845, // R13
	0xFC2D, // R14
	0x8097, // R15
	0x04A1, // R16
	0xDF6A  // R17
};

unsigned char AR1000vol[17]= // volume control  (increasing)
{
	0x0F,  //0
	//0xCF,//1
	0xDF,  //3
	0xEF,  //4
	//0xFF,//5
	0xEE,  //6
	//0xFE,//7
	0xED,  //8
	//0xFD,//9
	//0xFB,//10
	0xFA,  //11
	0xF9,  //12
	0xF7,  //13
	0xE6,  //14
	0xF6,  //15
	0xE5,  //16
	0xF5,  //17
	0xE3,  //18
	0xF3,  //19
	0xF2,  //20
	0xF1,  //21
	0xF0   //22
};
//#pragma constseg=default

__no_init __idata DATA_TYPE_S Reg_Data[18];


#pragma location="AR1010"
void AR1010_Write(u8 Reg)
{
	iic_start();

	iic_send_byte(AR1000_I2C_WRITE_ADDR);
	iic_recv_ack();

	iic_send_byte(Reg);
	iic_recv_ack();

	iic_send_byte(Reg_Data[Reg].c[1]);
	iic_recv_ack();
	iic_send_byte(Reg_Data[Reg].c[0]);
	iic_recv_ack();

	iic_stop();
}

#pragma location="AR1010"
u16 AR1010_Read(u8 Reg)
{
	DATA_TYPE_S value;

	iic_start();
	iic_send_byte(AR1000_I2C_WRITE_ADDR);
	iic_recv_ack();
	iic_send_byte(Reg);
	iic_recv_ack();
	iic_start();
	iic_send_byte(AR1000_I2C_READ_ADDR);
	iic_recv_ack();
	value.c[1]= iic_read_byte();
	iic_send_ack();
	value.c[0]= iic_read_byte();
	iic_send_nack();
	iic_stop();
	return value.i;
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块AR1010初始化
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="AR1010"
void ar1010_init(void)
{
	u8 Cnt1;           // init R1, R2, ....R17 then R0
	u16 status;

	Reg_Data[0].i=AR1000reg[0]&0xFFFE;
	AR1010_Write(0);

	for(Cnt1=1; Cnt1<18; Cnt1++)
	{
		Reg_Data[Cnt1].i=AR1000reg[Cnt1];
		AR1010_Write(Cnt1);
	}

	Reg_Data[0].i=AR1000reg[0];
	AR1010_Write(0);

	//Power-On Calibration begins
	// then wait for STC flag
	// maybe you need to delay for a while
	// delay ( 100 ms )
	status = AR1010_Read(ADDR_STATUS);
	status &=MASK_STC; // check STC flag
	while(status == 0)
	{
		// maybe you can delay for a while
		// delay ( 100 ms )
		status = AR1010_Read(ADDR_STATUS);
		status &=MASK_STC; // check STC flag
	}
	//Power-On Calibration Ends
	seek_dir = 0xFF;
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块AR1010设置频率
   @param freq 要设置的频率
   @return 无
   @note 频率范围为875~1080
*/
/*----------------------------------------------------------------------------*/
#pragma location="AR1010"
void ar1010_set_freq(u16 freq)
{
	unsigned int status;
	unsigned int flag;
	int rssi;

	AR1000_MUTE_ON;   // Set Muto ON before TUNE
	AR1010_Write(1);

	AR1000_SEEK_OFF;	//clear SEEK
	//Reg_Data[3].BIT.B13=space;  // set SPACE
	//Reg_Data[3].i = (Reg_Data[3].i & 0xE7FF) | band;  // Set BAND
	AR1010_Write(3);

	//Read Low-Side LO Injection
	//R11 --> clear  D15,  clear D0/D2,  D3 is the same as default
	Reg_Data[11].i = Reg_Data[11].i&0x7FFA;
	AR1010_Write(11);

	//TUNE to Freq100KHz with current setting
	SetAR1000_Freq2CHAN(freq); // this function will turn on TUNE

	// TUNE  begins
	// then wait for STC flag

	// maybe you need to delay for a while
	// delay ( 100 ms )
	status = AR1010_Read(ADDR_STATUS);
	flag = status & MASK_STC; // check STC flag
	while(flag == 0)
	{
		// maybe you can delay for a while
		// delay ( 100 ms )
		status = AR1010_Read(ADDR_STATUS);
		flag = status & MASK_STC; // check STC flag
	}
	//Low-side TUNE Ends

	status = AR1010_Read(ADDR_RSSI);
	rssi = (status & MASK_RSSI);

	//Read Hi-Side LO Injection
	// R11-->set D15, set D0/D2,  D3 is the same as default
	Reg_Data[11].i = Reg_Data[11].i|0x8005;
	AR1010_Write(11);
	//TUNE to Freq100KHz with current setting
	SetAR1000_Freq2CHAN(freq); // this function will turn on TUNE

	// TUNE  begins
	// then wait for STC flag

	// maybe you need to delay for a while
	// delay ( 100 ms )
	status = AR1010_Read(ADDR_STATUS);
	flag = status & MASK_STC; // check STC flag
	while(flag == 0)
	{
		// maybe you can delay for a while
		// delay ( 100 ms )
		status = AR1010_Read(ADDR_STATUS);
		flag = status & MASK_STC; // check STC flag
	}
	//High-side TUNE Ends

	status = AR1010_Read(ADDR_RSSI);
	rssi = rssi- (status & MASK_RSSI);
	if(rssi < 0)  //errata in 0.82
	{
		// LO
		// R11--> clear D15, set D0/D2, D3 is the same as default
		Reg_Data[11].i = (Reg_Data[11].i&0x7FFF)|0x0005;
		AR1010_Write(11);
	}
	else
	{
		//HI
		//R11-->  set D15, clear D0/D2, D3 is the same as default
		Reg_Data[11].i = (Reg_Data[11].i|0x8000)&0xFFFA;
		AR1010_Write(11);
	}


	//fine-tune !!
	//TUNE to Freq100KHz with current setting
	SetAR1000_Freq2CHAN(freq); // this function will turn on TUNE

	// TUNE  begins
	// then wait for STC flag

	// maybe you need to delay for a while
	// delay ( 100 ms )
	status = AR1010_Read(ADDR_STATUS);
	flag = status & MASK_STC; // check STC flag
	while(flag == 0)
	{
		// maybe you can delay for a while
		// delay ( 100 ms )
		status = AR1010_Read(ADDR_STATUS);
		flag = status & MASK_STC; // check STC flag
	}
	//TUNE Ends

	//AR1000_MUTE_OFF      <---   we dont mute off now, bcz this function may be called by other functions
	//AR1000_I2C_Write_Data(1);

	AR1000_MUTE_OFF;
	AR1010_Write(1);
}

// calculate AR1000 CHAN id  :  Freq (MHz) = 69 + 0.1*CHAN
#pragma location="AR1010"
void SetAR1000_Freq2CHAN(u16 Freq100KHz)
{
	u16 CHAN =0x0000; //actually it's 9-bit

	AR1000_TUNE_OFF;  //clear TUNE
	AR1010_Write(2);

	CHAN = Freq100KHz-690;
	Reg_Data[2].i &=0xfe00;
	Reg_Data[2].i|=CHAN;
	AR1000_TUNE_ON;
	AR1010_Write(2);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块AR1010设置音量
   @param vol 音量值
   @return 无
   @note 音量范围0~15
*/
/*----------------------------------------------------------------------------*/
#pragma location="AR1010"
void ar1010_set_vol(u8 vol)
{
	Reg_Data[3].i &= 0xF87F;
	Reg_Data[3].i |= (unsigned int)((AR1000vol[vol]&0x0F)<<7);

	Reg_Data[14].i &= 0x0FFF;
	Reg_Data[14].i |= (unsigned int)((AR1000vol[vol]&0xF0)<<8);
	AR1010_Write(3);
	AR1010_Write(14);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块AR1010关闭
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="AR1010"
void ar1010_off(void)
{
	AR1000_TUNE_OFF;          //clear TUNE
	AR1010_Write(2);
	AR1000_SEEK_OFF;	      //clear SEEK
	AR1010_Write(3);

	Reg_Data[0].BIT.B8 = 0;
	AR1010_Write(0);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块AR1010自动搜台
   @param freq 要搜索的频率
   @return 指定的频率下有台返回1，否则返回0
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="AR1010"
u8 ar1010_seek(u16 freq)
{
	unsigned int status;
	unsigned int flag;
	unsigned int Freq100KHz;

	status = AR1010_Read(ADDR_STATUS);
	// seek success, get READCHAN  and fine-tune now !
	Freq100KHz = 690 + ((status & MASK_READCHAN)>> SHIFT_READCHAN);

	if(seek_dir == 0xFF)
	{
		if(Freq100KHz < freq)  seek_dir = 1; //seek up
		else                  seek_dir = 0; //seek down
		if(freq == 1080) seek_dir = 0;
		if(freq == 875)  seek_dir = 1;

		AR1000_MUTE_ON   // Set Muto ON before SEEK
		AR1010_Write(1);

		AR1000_TUNE_OFF  //clear TUNE
		AR1010_Write(2);

		AR1000_SEEK_OFF;	//clear SEEK
		Reg_Data[3].BIT.B5=1;  // set SPACE
		Reg_Data[3].BIT.B7=seek_dir; // Seek up or down
		Reg_Data[3].i = Reg_Data[3].i & 0xE7FF;  // Set BAND
		Reg_Data[3].i = Reg_Data[3].i | 30;  // Set SEEKTH
		AR1010_Write(3);

		// Setting before seek
		Reg_Data[17].i = (Reg_Data[17].i & SEEK_MASK)|SEEK_SETTING;
		AR1010_Write(17);
		//
		AR1000_SEEK_ON
		AR1010_Write(3); // set  and seek

		status = AR1010_Read(ADDR_STATUS);
		flag = status & MASK_STC; // check STC flag
		while(flag == 0)
		{
			// maybe you can delay for a while
			// delay ( 100 ms )
			DELAY_MS(20);
			status = AR1010_Read(ADDR_STATUS);
			flag = status & MASK_STC; // check STC flag
		}
		// Seek Ends
		// check SF if seek fail ?
		flag = status & MASK_SF;
		if(flag)
		{
			//seek fail
			return 0;
		}
		Freq100KHz = 690 + ((status & MASK_READCHAN)>> SHIFT_READCHAN);
		if(Freq100KHz == freq)
		{
			// seek success, get READCHAN  and fine-tune now !
			Freq100KHz = 690 + ((status & MASK_READCHAN)>> SHIFT_READCHAN);

			// Restore setting after seek
			Reg_Data[17].i = AR1000reg[17];
			AR1010_Write(17);
			//

			//fine-tune with auto hilo rejection
			ar1010_set_freq(Freq100KHz);

			AR1000_MUTE_OFF
			AR1010_Write(1);

			seek_dir = 0xFF;
			return 1;
		}
		return 0;
	}
	else
	{
		if(Freq100KHz != freq) return 0;
		else
		{
			// seek success, get READCHAN  and fine-tune now !
			Freq100KHz = 690 + ((status & MASK_READCHAN)>> SHIFT_READCHAN);

			// Restore setting after seek
			Reg_Data[17].i = AR1000reg[17];
			AR1010_Write(17);
			//

			//fine-tune with auto hilo rejection
			ar1010_set_freq(Freq100KHz);

			AR1000_MUTE_OFF
			AR1010_Write(1);

			seek_dir = 0xFF;
			return 1;
		}
	}
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块AR1010检测
   @param 无
   @return 检测到AR1010模块返回1，否则返回0
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="AR1010"
u8 ar1010_online(void)
{
	u16  tep=AR1010_Read(0x1C);
	if(tep == 0x1010)
		return 1;
	else
		return 0;
}



