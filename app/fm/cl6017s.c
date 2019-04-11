/*--------------------------------------------------------------------------*/
/**@file CL6017S.C
   @brief FM模块CL6017S驱动
   @details FM模块CL6017S的驱动函数
   @author LZZ
   @date 2010-08-17
   @note none
*/
/*----------------------------------------------------------------------------*/

#include "include.h"
#include "iic.h"
#include "cl6017s.h"
#include "fm_api.h"


__no_init u8  dat1[24];//@"FM_BUF";
__no_init u8  redata[6];//@"FM_BUF";

//#pragma constseg = CL6017S_RX_TABLE
//const
u8 cl6017s_init_tbl[24] =
{
	0xe4,
	0x81,
	0x3e,
	0xf8,
	0x1f,
	0x40,
	0x38,
	0x56,
	0xe8,
	0xac,
	0xb0,
	0xf1,
	0x8b,
	0xaa,
	0xc6,
	0x04,
	0x6d,
	0x25,
	0xff,
	0xfd,
	0x12,
	0x0f,
	0x45,
	0x1d
};

//#pragma constseg=default
#pragma location="CL6017S"
__near_func void CL6017S_Write(u8 *write_data,u8 size)
{
	u8 i;

	iic_start();
	iic_send_byte(0x20);
	iic_recv_ack();

	for(i=0; i<size; i++)
	{
		iic_send_byte(write_data[i]);
		iic_recv_ack();
	}
	iic_stop();
}

#pragma location="CL6017S"
__near_func void  CL6017S_Read(u8 *read_data,u8 size)//state=0->success  state=1->fail
{
	u8 i;
	iic_start();

	iic_send_byte(0x21);
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
/**@brief FM模块CL6017S初始化
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="CL6017S"
__near_func void cl6017s_init(void)
{
	u8 i;

	for(i=0; i<24; i++)
		dat1[i] = cl6017s_init_tbl[i];

	dat1[0] &= 0x7f; //power up
	dat1[0] |= 0x20;

	CL6017S_Write(dat1,24); // write the initial values for all R/W registers

	u16 temp = 0;
	u8 Vol_FM = 0x0f;

	dat1[2] &= 0xC3;
	temp = Vol_FM << 2;
	temp &= 0x3c; //0x3c=00111100
	dat1[2] |= temp;

	CL6017S_Write(dat1,3);

	DELAY_MS(10);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块CL6017S设置频率
   @param freq 要设置的频率
   @return 无
   @note 频率范围为875~1080
*/
/*----------------------------------------------------------------------------*/
#pragma location="CL6017S"
__near_func void cl6017s_set_freq(u16 freq)
{
	u16 ch;
	ch = 0;
	ch = (freq- 700) *2;
	dat1[2] &= 0xfc;
	dat1[2] |= (ch & 0x0300) >> 8;
	dat1[3] = ch & 0xff;

	// seek operation
	dat1[0] &= 0xDF;
	dat1[0] &= 0xfd; // tune=0
	dat1[0] |= 0x20; // mute
	CL6017S_Write(dat1,4);

	dat1[0] |= 0x02; // TUNE=1, begin tune operation
	dat1[0] &= 0xDF;
	CL6017S_Write(dat1,1);
	DELAY_MS(4); // delay 40ms
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块CL6017S设置音量
   @param vol 音量值
   @return 无
   @note 音量范围0~15
*/
/*----------------------------------------------------------------------------*/
#pragma location="CL6017S"
__near_func void cl6017s_set_vol(u8 vol)
{
	u16 temp;
	if(vol > 0x0f)
		vol = 0x0f;

	dat1[2] &= 0xC3;
	temp = vol << 2;
	temp &= 0x3c; //0x3c=00111100
	dat1[2] |= temp;

	if(vol == 0)
		dat1[0] |= 0x20;
	else
		dat1[0] &= 0xDF;
	CL6017S_Write(dat1,3);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块CL6017S关闭
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="CL6017S"
__near_func void cl6017s_off(void)
{
	dat1[6] |= 0xff;  // mute on
	dat1[0] |= 0x80;  // power down
	CL6017S_Write(dat1,7);
	DELAY_MS(15);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块CL6017S自动搜台
   @param freq 要搜索的频率
   @return 指定的频率下有台返回1，否则返回0
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="CL6017S"
__near_func u8 cl6017s_seek(u16 freq)
{
	cl6017s_set_freq(freq);

	CL6017S_Read(redata,6);
	if((redata[1] >= 140) && ((redata[2]>>2) <= 31))
		return 1;
	else
		return 0;
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块CL6017S检测
   @param 无
   @return 检测到CL6017S模块返回1，否则返回0
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="CL6017S"
__near_func u8 cl6017s_online(void)
{
	CL6017S_Read(redata,6);

	if(redata[4] == 0xA7)
		return 1;
	else
		return 0;
}



