/*--------------------------------------------------------------------------*/
/**@file SP3777.C
   @brief FM模块SP3777驱动
   @details FM模块SP3777的驱动函数
   @author LZZ
   @date 2010-08-17
   @note none
*/
/*----------------------------------------------------------------------------*/

#include "include.h"
#include "iic.h"
//#include "SP3777.h"
#include "fm_api.h"

#define READ	1
#define WRITE	0
#define sp3769_WR_CTRL 0x20
#define sp3769_RD_CTRL 0x21

__no_init u8 SP3769ControlData[30]@"FM_BUF";

#pragma constseg = SP3777_RX_TABLE
const u8 sp3777_init_tbl[30] =
{
	0xe4,
	0x81,
	0x7E,
	0x30,
	0x19,
	0xc0,
	0x38,
	0x56,
	0x05,
	0x73,
	0xB4,
	0xf4,
	0xad,
	0xc1,
	0xc2,
	0x04,
	0x01,
	0x25,
	0xff,
	0xfd,
	0x06,
	0x0f,
	0x41,
	0x1d,
	0x21,
	0x0d,
	0x23,
	0x6f,
	0x00,
	0x00
};

#pragma constseg=default

void I2C_WriteSP3769Reg(u8 *write_data,u8 size)@"SP3777_RX"
{
	u8  i;

	iic_start();

	iic_send_byte(sp3769_WR_CTRL);
	iic_recv_ack();

	for(i=0; i<size; i++)
	{
		iic_send_byte(write_data[i]);
		iic_recv_ack();
	}

	iic_stop();
}

void I2C_ReadSP3769Reg(u8 *read_data,u8 size)@"SP3777_RX"
{
	u8 i;

	iic_start();
	iic_send_byte(sp3769_RD_CTRL);
	iic_recv_ack();

	for(i=0; i<size-1; i++)
	{
		read_data[i] = iic_read_byte();
		iic_send_ack();
	}
	read_data[i] = iic_read_byte();
	iic_send_nack();
	iic_stop();
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块SP3777初始化
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void sp3777_init(void)@"SP3777_RX"
{
	u8 i;

	for(i=0; i<=29; i++)
		SP3769ControlData[i] = sp3777_init_tbl[i];

	// power up and initial operation
	// if initially set to 87.5mhz radio, channel = (87.5-70)/0.05=350
	SP3769ControlData[2] &= 0xFC;
	SP3769ControlData[2] |= (350 & 0x0300)>>8;
	SP3769ControlData[3] = 350 & 0xFF;

	SP3769ControlData[0] &= 0x7f; //power up
	I2C_WriteSP3769Reg(SP3769ControlData,30);

	DELAY_MS(160); // wait 800ms for crystal oscillate stable

	SP3769ControlData[0] |= 0x02; // TUNE=1, begin tune operation
	I2C_WriteSP3769Reg(SP3769ControlData,1);
	DELAY_MS(20); // delay 100ms

	SP3769ControlData[0] &= 0xdf;  // mute off
	I2C_WriteSP3769Reg(SP3769ControlData,1);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块SP3777设置频率
   @param TunerFrequency 要设置的频率
   @return 无
   @note 频率范围为875~1080
*/
/*----------------------------------------------------------------------------*/
void sp3777_set_freq(u16 TunerFrequency)@"SP3777_RX"
{
	unsigned int ch = 0;
	// frequency transfer to channel number,  channel=(frequencyMHz-60)/0.05, e.g. 87.5Mhz->550
	ch = (TunerFrequency - 700)*2;
	// set channel number
	SP3769ControlData[2] &= 0xfc;
	SP3769ControlData[2] |= (ch & 0x0300)>>8;
	SP3769ControlData[3] =	ch & 0xff;
	// tune function
	SP3769ControlData[0] &= 0xfe;  // seek=0
	SP3769ControlData[0] &= 0xdf;  // mute off
	SP3769ControlData[0] &= 0xfd;  // tune=0
	I2C_WriteSP3769Reg(SP3769ControlData,4);

	SP3769ControlData[0] |= 0x02;  // tune=1
	I2C_WriteSP3769Reg(SP3769ControlData,1);
	DELAY_MS(100); // wait for 100ms
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块SP3777设置音量
   @param vol 音量值
   @return 无
   @note 音量范围0~15
*/
/*----------------------------------------------------------------------------*/
void sp3777_set_vol(u8 vol)@"SP3777_RX"
{
	unsigned char temp;
	if(vol >15)
		vol = 15;

	temp = (vol << 2) & (~0xc3);

	if(vol == 0)
		SP3769ControlData[0] |= 0x20;  // mute on
	else
		SP3769ControlData[0] &= ~0x20;  // mute off

	SP3769ControlData[2] &= 0xc3;
	SP3769ControlData[2] |= temp;
	I2C_WriteSP3769Reg(SP3769ControlData,3);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块SP3777关闭
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
void sp3777_off(void)@"SP3777_RX"
{
	SP3769ControlData[0] |= 0x20;  // mute on
	I2C_WriteSP3769Reg(SP3769ControlData,1);

	SP3769ControlData[0] |= 0x80; // set DISABLE=1 to power down
	I2C_WriteSP3769Reg(SP3769ControlData,1);
}


/*----------------------------------------------------------------------------*/
/**@brief FM模块SP3777自动搜台
   @param Frequency 要搜索的频率
   @return 指定的频率下有台返回1，否则返回0
   @note
*/
/*----------------------------------------------------------------------------*/
u8 sp3777_seek(u16 Frequency)@"SP3777_RX"
{
	int  prefd=-50, preHLSI=0;

	// local variable
	unsigned int ch = 0;
	unsigned char snr,fd,rssi,fd_abs;
	char  fd_comp;
	unsigned char hlsi;
	//unsigned char st;
	unsigned char fdstate=0;
	unsigned char Temp[10];

	ch = (Frequency - 700)*2;
	// set channel number
	SP3769ControlData[2] &= 0xfc;
	SP3769ControlData[2] |= (ch & 0x0300)>>8;
	SP3769ControlData[3] =	ch & 0xff;


	// tune function
	SP3769ControlData[0] &= 0xfe;  // seek=0
	SP3769ControlData[0] &= 0xfd;  // tune=0
	I2C_WriteSP3769Reg(SP3769ControlData,4);
	DELAY_MS(1) ;

	SP3769ControlData[0] |= 0x02;  // tune=1
	I2C_WriteSP3769Reg(SP3769ControlData,2);
	DELAY_MS(10); // wait for 50ms
	I2C_ReadSP3769Reg(Temp,10);

	// read hlsi, rssi, snr, fd, st
	//st = Temp[0]& 0x01;
	hlsi = Temp[0]&0x08;
	rssi=Temp[1];
	snr=Temp[2]/4;
	fd=Temp[5];
	if(fd>127)
	{
		fd_comp=fd-256;
		fd_abs=256-fd;
	}
	else
	{
		fd_abs=fd;
		fd_comp=fd;
	}

	//if(seekUp)
	//{
	if(preHLSI)
	{
		if(-prefd<=-20)
		{
			// prefd inverse
			fdstate=1;
		}
		else
		{
			fdstate=0;
		}

	}
	else
	{
		if(prefd<=-20)
		{
			fdstate=1;
		}
		else
		{
			fdstate=0;
		}
	}
	//}
	/*else
	    {
	        if(preHLSI)
	        {
	            if(-prefd>=20)
	            { // prefd inverse
	                fdstate=1;
	            }
	            else
	            {
	                fdstate=0;
	            }
	        }
	        else
	        {
	            if(prefd>=20)
	            {
	                fdstate=1;
	            }
	            else
	            {
	                fdstate=0;
	            }

	        }
	}*/
	// save to global variables
	prefd = fd_comp;
	preHLSI = hlsi;

	//seek threshod judgement
	//if(rssi>=172 && snr<=40 && fd_abs<=20 && fdstate)
	if(rssi>=185 && snr<=37 && fd_abs<=19 && fdstate)
		//搜台条件：停台门槛主要调节rssi大小，调小停台门槛降低，调大停台门槛增高
		//snr和fd_abs是信噪比和频偏门槛，越小越严格
	{
		/*if((Frequency==960||Frequency==910)&&st==0)
		        {
			return 0;
		}*/
		return 1;
	}
	else{
		return 0;
	}

}

/*----------------------------------------------------------------------------*/
/**@brief FM模块SP3777检测
   @param 无
   @return 检测到SP3777模块返回1，否则返回0
   @note
*/
/*----------------------------------------------------------------------------*/
u8 sp3777_online(void)@"SP3777_RX"
{
	unsigned char  Temp[6];

	I2C_ReadSP3769Reg(Temp,6);

	if((Temp[4] == 0x07))
		return 1;
	else
		return 0;
}

