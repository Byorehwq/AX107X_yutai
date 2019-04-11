/*--------------------------------------------------------------------------*/
/**@file BK1080.C
   @brief FM模块QN8035驱动
   @details FM模块QN8035的驱动函数
   @author LZZ
   @date 2010-08-17
   @note 通过宏定义来选择驱动方式
*/
/*----------------------------------------------------------------------------*/
#include "include.h"
#include "bk1080.h"

#if FM_BK1080_EN

__no_init __xdata u16 writedata[34] @ "BK1080_BUF";

extern void bk1080_mute(void);
extern void bk1080_unmute(void);

#pragma constseg = BK1080_RX_TABLE
const __code u16 bk1080_init_tbl[34] =
{
	0x0006,	//REG0
	0x1080,	//REG1
	0x5201,	//REG2
	0x0000,	//REG3
	0x00C0,	//REG4
#if FM_SEEK_50K_STEP
	0x0A2f,	//REG5  //0x0A1F:100KHz   0x0A2F:50KHz
#else
	0x0A1f,	//REG5
#endif
	0x002E,	//REG6
	0x02FF,	//REG7
	0x5B11,	//REG8
	0x0000,	//REG9
	0x411E,	//REG10
	0x0000,	//REG11
	0xCE00,	//REG12
	0x0000,	//REG13
	0x0000,	//REG14
	0x1000,	//REG15
	0x0010,	//REG16
	0x0000,	//REG17
	0x13FF,	//REG18
	0x9852,	//REG19
	0x0000,	//REG20
	0x0000,	//REG21
	0x0008,	//REG22
	0x0000,	//REG23
	0x51E0,	//REG24
	0x38BC,//0x28DC,	//REG25
	0x2645,	//REG26
	0x00E4,	//REG27
	0x1CD8,	//REG28
	0x3A50,	//REG29
	0xEAF0,	//REG30
	0x3000,	//REG31
	0x0200,//0x0000,	//REG32
	0x0000	//REG33
};
#pragma constseg=default

#pragma location="BK1080"
void bk1080_Write(u16 *writedata,u8 size)
{
	u8 i;

	fm_i2c_start();
	fm_i2c_send_byte(0x80);
	fm_i2c_recv_ack();

	fm_i2c_send_byte(0x00);
	fm_i2c_recv_ack();

	for(i=0; i<size; i++)
	{
		fm_i2c_send_byte(writedata[i]>>8);
		fm_i2c_recv_ack();
		fm_i2c_send_byte(writedata[i]);
		fm_i2c_recv_ack();
	}

	fm_i2c_stop();
}

#pragma location="BK1080"
void bk1080_Write_reg(u8 reg,u16 dat)
{
	fm_i2c_start();
	fm_i2c_send_byte(0x80);
	fm_i2c_recv_ack();
	fm_i2c_send_byte(reg*2);
	fm_i2c_recv_ack();
	fm_i2c_send_byte(dat>>8);
	fm_i2c_recv_ack();
	fm_i2c_send_byte(dat);
	fm_i2c_recv_ack();
	fm_i2c_stop();
}

#pragma location="BK1080"
u16 bk1080_Read_reg(u16 reg)
{
	u16 temp;
	u8 temp1,temp2;

	fm_i2c_start();
	fm_i2c_send_byte(0x80);
	fm_i2c_recv_ack();
	fm_i2c_send_byte(reg*2+1);
	fm_i2c_recv_ack();
	temp1 = fm_i2c_read_byte();
	fm_i2c_send_ack();
	temp2= fm_i2c_read_byte();
	fm_i2c_send_nack();
	fm_i2c_stop();
	temp=temp1;
	temp=(temp<<8)+(temp2);

	return temp;
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块BK1080初始化
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="BK1080"
void bk1080_init(void)
{
	u8 i;

	for(i=0; i<=33; i++)
		writedata[i] = bk1080_init_tbl[i];

#if (FM_CLOCK == USE_MODULE_CRYSTAL)
	writedata[2] = 0x5201;	//internal 32.768K
#elif ((FM_CLOCK == USE_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_32KHZ_CLK) || (FM_CLOCK == SHARE_CRYSTAL_32KHz))
	writedata[2] = 0x4201;	//external 32.768K
#elif(FM_CLOCK== SHARE_CRYSTAL_24MHz)
	writedata[2] =  0x4201;	//external 24M
	writedata[16] = 0x2DC6;
	writedata[17] = 0x8000;
	writedata[25] = 0xA8DC;
#elif ((FM_CLOCK == SHARE_CRYSTAL_12MHz) || (FM_CLOCK == USE_IO_12MHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
	writedata[2] = 0x4201;	//external 12M
	writedata[16] = 0x16E3;
	writedata[17] = 0x8000;
	writedata[25] = 0x28DC;
#else
#error "BK1080: 请选择FM时钟源"
#endif

	for(i=0; i<34; i++)
		bk1080_Write_reg(i, writedata[i]);

	WATCHDOG_CLR();
	delay_5ms(200); //750ms以上
	WATCHDOG_CLR();
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块BK1080设置频率
   @param freq 要设置的频率
   @para  mute_ctrl:  //0 没有mute_ctrl   //1 有mute_ctrl
   @return 无
   @note 频率范围为875~1080
*/
/*----------------------------------------------------------------------------*/
#pragma location="BK1080"
void bk1080_set_freq(u16 freq, u8 mute_ctrl)
{
	u16 ch = 0;
#if FM_SEEK_50K_STEP
	ch = (freq - 8750) / 5;   //50K
#else
	ch = freq - 875;
#endif
	if(mute_ctrl)
	{
		bk1080_mute();
	}
	writedata[2] &= ~0x0100;        //disable seek
	bk1080_Write_reg(0x02,writedata[2]);
	writedata[3] &= ~0x8000;             //reset tune
	bk1080_Write_reg(0x03,writedata[3]);

	writedata[3] = ch | 0x8000;
	bk1080_Write_reg(0x03, writedata[3]);
	delay_5ms(5);//20);      //++ changed, SO @ 2010-08-26, really need 100ms ?
	writedata[2] &= ~0x4000;
	bk1080_Write_reg(0x02,writedata[2]);
	if(mute_ctrl)
	{
		bk1080_unmute();
	}
}


/*----------------------------------------------------------------------------*/
/**@brief FM模块BK1080设置音量
   @param vol 音量值
   @return 无
   @note 音量范围0~15
*/
/*----------------------------------------------------------------------------*/
#pragma location="BK1080"
void bk1080_set_vol(u8 vol)
{
	if(vol>15)
		vol=15;

	writedata[5] &= 0xfff0;
	writedata[5] |= (vol & 0x0f);
	bk1080_Write_reg(0x05, writedata[5]);

	if(vol > 0)
		writedata[2] &= ~0x4000; //not mute
	else
		writedata[2] |= 0x4000;  //mute
	bk1080_Write_reg(0x02,writedata[2]);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块BK1080关闭
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="BK1080"
void bk1080_off(void)
{
	writedata[2] |= (1<<6);
	writedata[2] |= 1;
	bk1080_Write_reg(0x02,writedata[2]);
	delay_5ms(10);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块BK1080自动搜台
   @param freq 要搜索的频率
   @return 指定的频率下有台返回1，否则返回0
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="BK1080"
u8 bk1080_seek(u16 freq)
{

	static u16 last_fre=0;
	static u16 g_freq_deviation_last=0;
	u16 cur_freq_deviation;
	u16 reg7;
	u16 reg10;

	bk1080_set_freq(freq,0);
	delay_5ms(25);   //delay 125ms     //这里延时比较重要,少了的话收不了台
	bk1080_mute();
	reg7 = bk1080_Read_reg(0x07);
	cur_freq_deviation = reg7>>4;

	reg10 = bk1080_Read_reg(0x0A);
	if(reg10 & 0x1000)    //check AFCRL bit12	//invalide channel
	{
		last_fre=freq;
		g_freq_deviation_last=cur_freq_deviation;
		return 0;//false
	}
	if((reg10 & 0x00FF)<5)    //RSSI:Received Signal Strength Indicator
	{
		last_fre=freq;
		g_freq_deviation_last=cur_freq_deviation;
		return 0;//false
	}
	if((reg7 & 0xf) <2)        //SNR
	{
		last_fre = freq;
		g_freq_deviation_last = cur_freq_deviation;
		return 0; //false
	}
	if((cur_freq_deviation>=280)&&(cur_freq_deviation<=(0xfff-280)))   //280
	{
		last_fre=freq;
		g_freq_deviation_last=cur_freq_deviation;
		return 0;//false
	}

//new added 2009-08-25
	//向上搜
	if((freq>=876)&&((freq-last_fre)==1))
	{
		if(g_freq_deviation_last&0x800)
		{
			last_fre=freq;
			g_freq_deviation_last=cur_freq_deviation;
			return 0;//false
		}
		if(g_freq_deviation_last<50)
		{
			last_fre=freq;
			g_freq_deviation_last=cur_freq_deviation;
			return 0;//false
		}
	}
#if 0  //向下搜
	if((freq>=875)&&((last_fre-freq)==1))
	{
		if((g_freq_deviation_last&0x800)==0)
		{
			last_fre=freq;
			g_freq_deviation_last=cur_freq_deviation;
			return 0;//false
		}

		if(g_freq_deviation_last>(0xfff-80))
		{
			last_fre=freq;
			g_freq_deviation_last=cur_freq_deviation;
			return 0;//false
		}
	}
#endif
	last_fre=freq;
	g_freq_deviation_last = cur_freq_deviation;
	bk1080_unmute();
	return 1;//ok
}

#pragma location="BK1080"
void bk1080_mute(void)
{
	writedata[2] |= 0x4000;
	bk1080_Write_reg(0x02,writedata[2]);
}

void bk1080_unmute(void)@"BK1080"
{
	writedata[2] &= ~0x4000;
	bk1080_Write_reg(0x02,writedata[2]);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块BK1080检测
   @param 无
   @return 检测到BK1080模块返回1，否则返回0
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="BK1080"
u8 bk1080_online(void)
{
	u16 tmp;
	tmp = bk1080_Read_reg(0x01);

	if(tmp == 0x1080)
		return 1;
	else
		return 0;
}

#endif


