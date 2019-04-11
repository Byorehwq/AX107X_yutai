/*--------------------------------------------------------------------------*/
/**@file AR1019.C
   @brief FM模块QN8035驱动
   @details FM模块QN8035的驱动函数
   @author LZZ
   @date 2010-08-17
   @note 通过宏定义来选择驱动方式
*/
/*----------------------------------------------------------------------------*/

#include "include.h"
#include "iic.h"
#include "ar1019.h"
#include "fm_api.h"

#if FM_AR1019_EN

#if (FM_CLOCK == _USE_MODULE_CRYSTAL_)
const U16 AR1018reg[18]=
{
	// the initial setting of AR1018 register ( base on ARDSG-X1-Xtal_v001.rgf)             //模块
	0xFFFB,  // R0
	0x5B17,  // R1.
	0xD0B9,  // R2 					//与AR1010兼容不用改
	0xA00A,  // R3 0xA010,   //0xA0A0 (17-5)                       //最后两位调门限   0XA000最小
	0x0780,  // R4
	0x28AB,  // R5
	0x6400,  // R6
	0x1EE7,  // R7
	0x7141,  // R8
	0x007D,  // R9
	0x82C6,  // R10  //seek_wrap off
	0x4E55,  // R11
	0x970C,  // R12
	0xB845,  // R13
	0x0E2D,//0xFE2D,  // R14   //0xFC2D(17-5)
	0x8097,  // R15
	0x04A1,  // R16
	0xDF6A  // R17
};
#endif

#if (FM_CLOCK == _USE_P33_AS_FM_CLOCK_)
const u16 AR1018reg[18]=
{
	// the initial setting of AR1018 register ( base on ARDSG-X1-EXT_v001.rgf)

//for AR1019              //芯片
	0xFF7B,  // R0
	0x5B17,  // R1
	0xD4B9,  // R2			//AR1010此处调为0xD0B9
	0xA00A,  // R3 0xA010         //最后两位调门限   0XA000最小
	0x0780,  // R4
	0x28AB,  // R5
	0x6400,  // R6
	0x1EE7, // 0x1EE7(32K), 0x1f87(24M),
	0x7141,  // R8
	0x007D, // 0x007D(32K) //0x0080 (24M)
	0x81C6,  // R10  //seek_wrap OFF
	0x4F55,  // R11
	0x970C,  // R12
	0xB845,  // R13
	0x0E2D,  // R14  //0x0E2D
	0x8097,  // R15
	0x04A1,  // R16 0x04A1 (DC couple),  0x04e1 (AC couple)
	0xDF6A  // R17
};
#endif
//- Modified code end
#if (FM_CLOCK == _SHARE_CRYSTAL_24MHz_)
//24M 共晶振
const u16 AR1018reg[18]=
{
//for AR1019              //芯片
	0xFF7B,  // R0
	0x5B17,  // R1
	0xD4B9,  // R2
	0xA00A,  // R3 0xA010         //最后两位调门限   0XA000最小
	0x0780,  // R4
	0x28AB,  // R5
	0x6400,  // R6
	0x1f87,  // R7 0x1EE7
	0x7141,  // R8
	0x0080,  // R9 0x007D
	0x81C6,  // R10  //seek_wrap ON
	0x4F55,  // R11
	0x970C,  // R12
	0xB845,  // R13
	0x0E2D,  // R14 SINGLE SEEK           15:12调整音量，F最大，0最小
	0x8097,  // R15
	0x04e1,  // R16 0x04A1
	0xDF6A  // R17
};
#endif
//- Modified code end
const u8 AR1019vol[17]=  // volume control
{
	0x0F,	// step 0
	0x10,	// step 1
	0x10,	// step 1
	0x20,	// step 2
	0x30,	// 3
	0x40,	// 4
	0x50,	// 5
	0x60,	// 6
	0x70,	// 7
	0x80,	// 8
	0x90,	// 9
	0xA0,	//10
	0xB0,	//11
	0xC0,	//12
	0xD0,	//13
	0xE0,	//14
	0xF0	//15 <------ default setting, MAX volume level.
};

u8  dataM, dataL;
u16 last_freq = 875;

void AR1018_WriteByte(u8 CW, u16 data)@"FM_RX_WR"
{
	iic_start();
	iic_send_byte(AR1018_WRITE);
	iic_recv_ack();
	iic_send_byte(CW);
	iic_recv_ack();
	iic_send_byte((u8)(data>>8));
	iic_recv_ack();
	iic_send_byte((u8)(data&0xFF));
	iic_recv_ack();
	iic_stop();
}


void AR1018_ReadByte(u8 CW)@"FM_RX_WR"
{
	iic_start();
	iic_send_byte(AR1018_WRITE);
	iic_recv_ack();
	iic_send_byte(CW);
	iic_recv_ack();
	iic_start();
	iic_send_byte(AR1018_READ);
	iic_recv_ack();
	dataM = iic_read_byte();
	iic_send_ack();
	dataL = iic_read_byte();
	iic_send_nack();
	iic_stop();
}


__near_func u16 ar1019_online(void)@"AR1019_RX"
{
	uint16 dataRead;
	AR1018_ReadByte(0x1C);
	dataRead = (uint16)(dataM<<8|dataL);

	if(dataRead!= 0x1010)
		return 0;

	return 1;
}

__near_func void AR1019_mute(u8 mute)@"AR1019_RX"
{
	u16 dataRead;

	AR1018_ReadByte(1);
	dataRead = (u16)(dataM<<8|dataL);
	if(mute == 1)
		AR1018_WriteByte(1, (dataRead&0xFFFD)|0x02);
	else
		AR1018_WriteByte(1, (dataRead&0xFFFD));
}

/*
__near_func u16 AR1018_GetCurRSSI(void)@"AR1019_RX"
{
   u16 dataRead;

   AR1018_ReadByte(ADDR_RSSI);
   dataRead = (u16)(dataM<<8|dataL);
   return ((dataRead & MASK_RSSI)>>9);
}



__near_func u16 AR1018_GetCurIF(void)@"AR1019_RX"
{
	u16 dataRead;

   AR1018_ReadByte(ADDR_RSSI);
   dataRead = (u16)(dataM<<8|dataL);
   return (dataRead & 0x1FF);
}
*/

__near_func void AR1018_TUNE_ON_OFF(u8 ON_OFF)@"AR1019_RX"
{
	u16 dataRead;

	AR1018_ReadByte(2);
	dataRead = (u16)(dataM<<8|dataL);
	if(ON_OFF == 1)
		AR1018_WriteByte(2, (dataRead&0xFDFF)|0x200);
	else
		AR1018_WriteByte(2, (dataRead&0xFDFF));
}


__near_func void AR1018_SEEK_ON_OFF(u8 ON_OFF)@"AR1019_RX"
{
	u16 dataRead;

	AR1018_ReadByte(3);
	dataRead = (u16)(dataM<<8|dataL);
	if(ON_OFF == 1)
		AR1018_WriteByte(3, (dataRead&0xBFFF)|0x4000);
	else
		AR1018_WriteByte(3, (dataRead&0xBFFF));
}




__near_func void ar1019_init(void)@"AR1019_RX"
{

	u8 i;
	u16 dataRead;
	//- Modified code start
	AR1018_WriteByte(0, 0xFF7A); //0xFF7E
	//- Modified code end


	for(i=1; i<18; i++)
	{
		AR1018_WriteByte(i, AR1018reg[i]);
	}
	AR1018_WriteByte(0, AR1018reg[0]);                     //开启FM





	do {
		AR1018_ReadByte(ADDR_STATUS);
		dataRead = (u16)(dataM<<8|dataL);

	}
	while((dataRead & MASK_STC)==0);

	DELAY_MS(50);	////

	AR1019_mute(0);
}


__near_func void ar1019_set_freq(u16 curf)@"AR1019_RX"    /* input value: 875 - 1080 ( 87.5 MHz - 108.0 MHz)*/
{
	u16 dataRead, CHAN;

	AR1019_mute(1);

	do {
		AR1018_ReadByte(ADDR_STATUS);
		dataRead = (u16)(dataM<<8|dataL);
	}
	while((dataRead & MASK_STC)==0);

	// Restore setting after seek
	AR1018_WriteByte(17, AR1018reg[17]);

	AR1018_TUNE_ON_OFF(0);
	AR1018_SEEK_ON_OFF(0);

	AR1018_ReadByte(3);
	dataRead = (u16)(dataM<<8|dataL);
	AR1018_WriteByte(3, (dataRead&0xC7FF)|0x2000|UEBAND);//set space(100k/200k)and band(875~1080)

	CHAN = curf - 690;
	CHAN = CHAN|0x200;//turn on TUNE
	AR1018_ReadByte(2);
	dataRead = (u16)(dataM<<8|dataL);
	AR1018_WriteByte(2, (dataRead&0xFC00)|CHAN);


	do {
		AR1018_ReadByte(ADDR_STATUS);
		dataRead = (u16)(dataM<<8|dataL);
	}
	while((dataRead & MASK_STC)==0);

	AR1019_mute(0);
	ar1019_set_vol(15);
}


__near_func u8 ar1019_seek(u16 freq)@"AR1019_RX"
{
#if 1
	u16 dataRead, CHAN=0x0000;

	AR1019_mute(1);
	CHAN = freq - 690-1;
	AR1018_ReadByte(2);
	dataRead = (uint16)(dataM<<8|dataL);
	AR1018_WriteByte(2, (dataRead&0xFC00)|CHAN); // set CHAN and TUNE=0

	// Setting before seek
	AR1018_ReadByte(17);
	dataM = (dataM & 0xC3) | 0x20;
	dataL =(dataL &  0xFF) | 0x00;
	dataRead = (uint16)(dataM<<8|dataL);
	AR1018_WriteByte(17, dataRead);

	AR1018_TUNE_ON_OFF(0);
	AR1018_SEEK_ON_OFF(0);

	AR1018_ReadByte(3);
	dataRead = (uint16)(dataM<<8|dataL);
	AR1018_WriteByte(3, ((dataRead&0x07FF)|(1UL<<15)|0x2000|JWBAND));//set space(100k/200k)and band(875~1080) and SEEK=0
	AR1018_SEEK_ON_OFF(1);  //SEEK ON

	// DELAY_MS(5);
	do {
		AR1018_ReadByte(ADDR_STATUS);
		dataRead = (uint16)(dataM<<8|dataL);
	}
	while((dataRead & MASK_STC)==0);

	if((dataRead & MASK_SF)!=0)
		return 0;

	AR1019_mute(0);
	ar1019_set_vol(15);
	return 1;
#endif
#if 0
	u16 targetFreq;
	u16 dataRead, readchan, CHAN=0x0000;
	u8 seekdir,is_step_up = 1;

	AR1019_mute(1);

	if(last_freq < freq)  is_step_up = 1; //seek up
	else                  is_step_up = 0; //seek down

	last_freq = freq;

	AR1018_ReadByte(3);
	dataRead = (uint16)(dataM<<8|dataL);
	seekdir = dataRead >> 15;

	if(is_step_up!=seekdir)
	{
		do
		{
			AR1018_ReadByte(ADDR_STATUS);
			dataRead = (uint16)(dataM<<8|dataL);


		}
		while((dataRead & MASK_STC)==0);
	}

	if(is_step_up)
	{
		if((freq - 1)<=MIN_FREQUENCE)
		{
			targetFreq = MIN_FREQUENCE;
		}
		else
		{
			targetFreq = (uint16)freq - 1;
		}

	}
	else
	{
		if((freq + 1)>=MAX_FREQUENCE)
		{
			targetFreq = MAX_FREQUENCE;
		}
		else
		{
			targetFreq = (uint16)freq + 1;
		}
	}


	AR1018_ReadByte(ADDR_STATUS);
	dataRead = (uint16)(dataM<<8|dataL);


	if((dataRead & MASK_STC)!=0)
	{
		AR1018_TUNE_ON_OFF(0);
		AR1018_SEEK_ON_OFF(0);

		CHAN = targetFreq - 690;
		AR1018_ReadByte(2);
		dataRead = (uint16)(dataM<<8|dataL);
		AR1018_WriteByte(2, (dataRead&0xFC00)|CHAN); // set CHAN and TUNE=0

		// Setting before seek
		AR1018_ReadByte(17);
		dataM = (dataM & 0xC3) | 0x20;
		dataL =(dataL &  0xFF) | 0x00;
		dataRead = (uint16)(dataM<<8|dataL);
		AR1018_WriteByte(17, dataRead);

		AR1018_ReadByte(3);
		dataRead = (uint16)(dataM<<8|dataL);
		AR1018_WriteByte(3, ((dataRead&0x07FF)|(is_step_up<<15)|0x2000|UEBAND));//set space(100k/200k)and band(875~1080) and SEEK=0
		AR1018_SEEK_ON_OFF(1);  //SEEK ON
	}
readagain:
	AR1018_ReadByte(ADDR_STATUS);
	dataRead = (uint16)(dataM<<8|dataL);
	readchan = 690 + ((dataRead & MASK_READCHAN)>> SHIFT_READCHAN);

	if(is_step_up)
	{
		if(readchan < freq)
		{
			goto readagain;
		}
		if(readchan > freq)
		{
			goto notstation;
		}
	}
	else{
		if(readchan > freq)
		{
			goto readagain;
		}
		if(readchan < freq)
		{
			goto notstation;
		}
	}
	if(readchan == freq)
	{
		if((dataRead & MASK_STC)==0) goto readagain;
	}

	// Restore setting after seek
	AR1018_WriteByte(17, AR1018reg[17]);

	if((dataRead & MASK_SF)!=0)
		goto notstation;
	//if((freq==960)||(freq==1040))
	//  goto notstation;
	AR1019_mute(0);
	return 1;

notstation:
	return 0;

#endif

}

__near_func void ar1019_off(void)@"AR1019_RX"
{
	AR1018_TUNE_ON_OFF(0);
	AR1018_SEEK_ON_OFF(0);
	//- Modified code start
	AR1018_WriteByte(0, 0xFF7A); //0xFF7E
	//- Modified code end
}


// Volume Control
// there are two different fields about volume control in AR1000F
//  Volume   :  D7  ~D10 in register R3
//  Volume2 :  D12~D15 in register R14
//  16 combinations of ( volume2 + volume)  are  recommended.
//
// set volume
__near_func void ar1019_set_vol(u8 vol)@"AR1019_RX"
{
	u16 dataRead;

	AR1018_ReadByte(3);
	dataRead = (uint16)(dataM<<8|dataL);
	AR1018_WriteByte(3, (dataRead&0xF87F)|(u16)((AR1019vol[vol]&0x0F)<<7));

	AR1018_ReadByte(14);
	dataRead = (uint16)(dataM<<8|dataL);
	AR1018_WriteByte(14, (dataRead&0x0FFF)|(u16)((AR1019vol[vol]&0xF0)<<8));
}

#endif /* FM_AR1019_EN */

