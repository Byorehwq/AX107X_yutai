#include "include.h"
#include "iic.h"
#include "cl6017g.h"
#include "fm_api.h"

#define CL6017G_RD_CTRL   0x21
#define CL6017G_WR_CTRL   0x20
#define FM_CL6017G_RSSILEV        173  //165～200//信号强度，越大假台越少
#define FM_CL6017G_ENVELOPELEV    608  //0～672//包络值，越小弱信号台越少
#define FM_CL6017G_FDLEV          45  //0～55频偏，输入参考时钟，最大支持400PPM
__no_init u8 dat2[47];//@"FM_BUF";
__no_init u8 readdata2[10];//@"FM_BUF";
volatile bool fm_rx_stc;
volatile bool fm_rx_rdy;
volatile bool fm_rx_true;

u8 fm_rx_rssi;

//#pragma constseg = CL6017G_RX_TABLE
const u8 cl6017G_online_tbl[16] =
{
	0x64,// index + 22 ---- reg correct index
	0x69,
	0x78,
	0x18,   //0x18 -->32.768K,0X19-->12M,0X1d-->24M,
	0x03,
	0xff,
	0x1f,//R28_bit<5:4>=01 0x2f
	0xb2,
	0x9b,
	0x05,
	0x00,
	0x90,
	0x00,
	0x9a,
	0xb2,
	0x15
};

const u8 cl6017G_init_tbl[47] =
{
	0x64,// index + 22 ---- reg correct index
	0x69,
	0x78,
	0x18,//0x18 -->32.768K,0X19-->12M,0X1d-->24M,
	0x03,
	0xff,
	0x2f,//R28_bit<5:4>=01 0x2f
	0xb2,
	0x9b,
	0x05,
	0x00,
	0x90,
	0x00,
	0x9a,
	0xb2,//
	0x15,
	0x96,
	0x80,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x60,
	0x02,
	0xfe,
	0x02,
	0xa0,
	0x00,
	0x00,
	0x02,
	0x80,
	0x00,
	0x60,
	0x85,
	0x90,
	0x0f,
	0x5f,
	0xbc,
	0xa3,
	0x28,
	0x27,
	0x80,
	0x28,//R66=00010000 0x02
	0x80,
	0xc0//  R68_bit4=1 0xc0
};
//#pragma constseg=default


/******************************************************************************
local function define
******************************************************************************/
//cl_6017G  //cl_6017G //cl_6017G //cl_6017G //cl_6017G //cl_6017G //cl_6017G
//cl_6017G //cl_6017G //cl_6017G //cl_6017G //cl_6017G //cl_6017G //cl_6017G
__near_func void cl6017g_init(void)@"CL6017G_RX"
{
	u8 i;

	for(i=0; i<47; i++)
	{
		dat2[i] = cl6017G_init_tbl[i];
	}

	CL6017G_Write(47);
	dat2[5] &= 0xfe; //power up reg27
	CL6017G_Write(6);
	DELAY_MS(20);
	dat2[4] |= 0x08;// TUNE=1, begin tune operation reg26
	CL6017G_Write(5);
	DELAY_MS(10); // delay 100ms
	dat2[4] |= 0x08;// TUNE=1, begin tune operation reg26
	CL6017G_Write(5);
	DELAY_MS(10); // delay 100ms

	dat2[0] &= 0xdf;   // REG0 BIT5  1:mute om 0:mute off
	CL6017G_Write(1);
}

__near_func void CL6017G_Powerdown(void)@"CL6017G_RX"
{
	dat2[0] |= 0x20;//mute on
	dat2[4] &= 0xf3;//tune=0,seek=0
	CL6017G_Write(5);
	dat2[5] |= 0x01; // set DISABLE=1 to power down
	CL6017G_Write(6);
	DELAY_MS(10);
}


__near_func void cl6017g_set_freq(u16 freq)@"CL6017G_RX"
{
	unsigned long freqQ = (unsigned long) freq * 100;  //modi by alpha 100827
	freqQ >>= 2;//右移两位
	dat2[2] = (unsigned char)(freqQ % 256); //reg24
	dat2[1] = (unsigned char)(freqQ >> 8); //reg23

	dat2[0] &= 0xdf;// mute off reg22//|=0X20;//
	dat2[4] &= 0xf3;//tune=0,seek=0 reg26
	dat2[6] = 0x2f;  //dat2[6] = 0x1f;
	CL6017G_Write(7);

	dat2[4] |= 0x08;//tune=1, begin tune operaton reg26
	CL6017G_Write(5);
	DELAY_MS(5); // wait for 50ms
}

__near_func void cl6017g_off(void)@"CL6017G_RX"
{
	CL6017G_mute(1);
}

__near_func void CL6017G_mute(u8 mute)@"CL6017G_RX"
{

	if(mute)
		dat2[0] |= ~0xdf;   // REG0 BIT5  1:mute oN 0:mute off
	else
		dat2[0] &= 0xdf;   // REG0 BIT5  1:mute om 0:mute off
	CL6017G_Write(1);
}

__near_func void cl6017g_set_vol(u8 vol)@"CL6017G_RX"
{
	if(vol > 0xf)
	{
		vol = 0x0f;
	}

	vol =vol<<2;
	dat2[5] &= 0XC1;
	dat2[5] |= vol;
	CL6017G_Write(6);
}


__near_func u8 cl6017g_seek(u16 freq)@"CL6017G_RX"
{
	//u8   bStcFlag = 0;
	bool   bSt = 0;
	//u8 unLoop = 0;
	u8 nRssi;
	unsigned int unEnvelope;
	unsigned int unFd;
	//unsigned char ucPga;
	unsigned long ulFreq = (unsigned long) freq* 100;


	ulFreq >>= 2;//右移两位
	dat2[2] = (unsigned char)(ulFreq % 256); //reg24
	dat2[1] = (unsigned char)(ulFreq >> 8); //reg23
	// tune function
	dat2[0] &= 0xdf;// mute off reg22
	dat2[4] &= 0xf3;//tune=0,seek=0 reg26
	dat2[6] = 0xdf;
	CL6017G_Write(7);

	dat2[4] |= 0x08;//tune=1, begin tune operaton reg26
	CL6017G_Write(5);
	DELAY_MS(7); // wait for 70ms
	CL6017G_Read(10);   //add by alpha100827
	nRssi = readdata2[5]-3;

	if(nRssi <= 0)
	{
		nRssi = 0;
	}
	unEnvelope = (readdata2[6] << 2) + (readdata2[7] >> 6);
	unFd = readdata2[4] + ((readdata2[3] & 0x01) << 8); //reg4/reg3 bit0
	if(unFd > 255)
	{
		unFd = 512 - unFd;
	}
	if((nRssi >= FM_CL6017G_RSSILEV)
	&& (unEnvelope <= FM_CL6017G_ENVELOPELEV)
	&& (unFd <= FM_CL6017G_FDLEV)&&(freq!=960))
	{
		bSt = 1;
	}
	else
	{
		bSt = 0;
	}
	return bSt;
}


void  CL6017G_Read(u8 size)@"FM_RX_WR"
{
	u16 i;
	iic_start();
	iic_send_byte(CL6017G_RD_CTRL);
	iic_recv_ack();

	for(i=0; i<size-1; i++)
	{
		readdata2[i] = iic_read_byte();
		iic_send_ack();
	}
	readdata2[i] = iic_read_byte();
	iic_send_nack();
	iic_stop();
}


void CL6017G_Write(u8 size)@"FM_RX_WR"
{
	u8  i;

	iic_start();                //I2C启动

	iic_send_byte(CL6017G_WR_CTRL);   //写地址
	iic_recv_ack();

	for(i=0; i<size; i++)
	{
		iic_send_byte(dat2[i]);      //写数据
		iic_recv_ack();
	}
	iic_stop();                 //I2C停止时序
}

__near_func u8 cl6017G_online(void)@"CL6017G_RX"
{
	u8 i;

	for(i=0; i<16; i++)
	{
		dat2[i] = cl6017G_online_tbl[i];
	}

	CL6017G_Write(16);
	DELAY_MS(1);
	dat2[5] &=0xfe;
	CL6017G_Write(6);
	DELAY_MS(12);  //120ms左右
	CL6017G_Read(2);
	dat2[5] |= 0x01;
	CL6017G_Write(6);

	if(readdata2[0]==0x0a)
	{
		return 1;
	}
	return 0;
}





