/*--------------------------------------------------------------------------*/
/**@file RDA5807P.C
   @brief FMģ��RDA5807P����
   @details FMģ��RDA5807P����������
   @author LZZ
   @date 2010-08-17
   @note ������֧��ģ�飬֧��32K������,֧��P05���ʱ��,���ַ�ʽʱ�ӵĳ���һ��
         ͨ��ʶ��ID��������ְ汾
*/
/*----------------------------------------------------------------------------*/

#include "include.h"
#include "rda5807P.h"


#if FM_RDA5807_EN

/*********************************************************************************/
/*          ֧��32K������,֧��P05���ʱ��,���ַ�ʽʱ�ӵĳ���һ��                 */
/*********************************************************************************/
u16 rda5807_chipID;
u8 dat[24];
u8 readdata[10];

void rda5807_fill(const u8 __code *pSrc);
void rda5807_write_c(const u8 __code *writedata, u8 size);
void rda5807_write(u8 size);
void rda5807_read(u8 size);

/*----------------------------------------------------------------------------*/
/**@brief FMģ��RDA5807P��ʼ��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma constseg = RDA5807_RX_TABLE     //������ֻ�ܱ�bank���������ã���˱���Ҫ��RDA5807_RXͬbank��

#if RDA5807_PE_SP
/* �˳�ʼ��֧�� RDA5807PE  RDA5807SP �汾 */
const u8 rad5807_init_tbl[104] =           //    ԭ����5807SP
{
#if (FM_CLOCK == USE_MODULE_CRYSTAL)
	0xC0, 0x01, //02H: internal 32.768M
#elif (FM_CLOCK == SHARE_CRYSTAL_24MHz)
	0x84, 0x51, //02H: external 24M
#elif (FM_CLOCK == SHARE_CRYSTAL_12MHz)
	0xc4, 0x11, //02H: external 12M
#elif (FM_CLOCK == SHARE_CRYSTAL_32KHz)
	0xc0, 0x01, //02H: external 32.768M
#elif ((FM_CLOCK == USE_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_32KHZ_CLK))
	0xc0, 0x01, //02H: external 32.768M
#elif ((FM_CLOCK == USE_IO_12MHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
	0xc4, 0x11, //02H: external 12M
#else
#error "RDA5807: ��ѡ��FMʱ��Դ"
#endif

	0x00, 0x10,
	0x04, 0x00,
	0x88, 0xaf, //05H:0x80~0x8fԽС̨Խ��
	0x80, 0x00,
	0x76, 0xc6, //07H
	0x5e, 0xc6,
	0x00, 0x00,
	0x40, 0x6e, //0AH:
	0x2d, 0x80,
	0x58, 0x03,
	0x58, 0x04,
	0x58, 0x04,
	0x58, 0x04,
	0x00, 0x47, //10H:
	0x90, 0x00,
	0xF5, 0x87,
	0x7F, 0x0B, //13H:
	0x04, 0xF1,
	0x5E, 0xc0, //15H: 0x42, 0xc0
	0x41, 0xe0,
	0x50, 0x6f,
	0x55, 0x92,
	0x00, 0x7d,
	0x10, 0xC0, //1AH:
	0x07, 0x80,
	0x41, 0x1d, //1CH:
	0x40, 0x06,
	0x1f, 0x9B,
	0x4c, 0x2b, //1FH:
	0x81, 0x10, //20H:
	0x45, 0xa0, //21H:
	0x19, 0x3F, //22H:
	0xaf, 0x40,
	0x06, 0x81,
	0x1b, 0x2a, //25H:
	0x0D, 0x04,
	0x80, 0x9F, //0x80, 0x2F,
	0x17, 0x8A,
	0xD3, 0x49,
	0x11, 0x42,
	0xA0, 0xC4, //2BH:
	0x3E, 0xBB,
	0x00, 0x00,
	0x58, 0x04,
	0x58, 0x04, //2FH:
	0x58, 0x04,
	0x00, 0x74,
	0x3D, 0x00,
	0x03, 0x0C,
	0x2F, 0x68,
	0x38, 0x77, //35H:
};
#endif


#if RDA5807_HS_HP
/* �˳�ʼ��֧�� RDA5807HS  RDA5807HP �汾 */
const u8 rad5807H_init_tbl[110] =      //���º��5807���ͺ�5807HP
{
#if (FM_CLOCK == USE_MODULE_CRYSTAL)
	0xC0, 0x01, //02H: internal 32.768M
#elif (FM_CLOCK == SHARE_CRYSTAL_24MHz)
	0x84, 0x51, //02H: external 24M
#elif (FM_CLOCK == SHARE_CRYSTAL_12MHz)
	0xc4, 0x11, //02H: external 12M
#elif (FM_CLOCK == SHARE_CRYSTAL_32KHz)
	0xc0, 0x01, //02H: external 32.768M
#elif ((FM_CLOCK == USE_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_32KHZ_CLK))
	0xc0, 0x01, //02H: external 32.768M
#elif ((FM_CLOCK == USE_IO_12MHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
	0xc4, 0x11, //02H: external 12M
#else
#error "RDA5807H: ��ѡ��FMʱ��Դ"
#endif

	0x00,0x10,
	0x04,0x00,
	0x88,0xBf, //05H://86    ���20H�Ĵ��������ڴ˵���̨����,��SP��һ����0X80��̨���
	0x40,0x00,
	0x7E,0xc6,
	//���¼Ĵ������ò���ʡȥ
	0x00,0x00,
	0x00,0x00,
	0x00,0x00, //0AH:
	0x00,0x00,
	0x00,0x00,
	0x00,0x00,
	0x00,0x00,
	0x00,0x00,
	0x00,0x06, //10H:
	0x00,0x19, //0x00,0x09,//0830
	0x2a,0x11,
	0x00,0x2e,
	0x2a,0x30,
	0xb8,0x3c, //15H:
	0x90,0x00,
	0x2a,0x91,
	0x84,0x12,
	0x00,0xa8,
	0xc4,0x00, //1AH:
	0xe0,0x00,
	0x30,0x1d, //0x24,0x9d
	0x81,0x6a,
	0x46,0x08,
	0x00,0x86, //1FH:
	0x06,0x61, //20H:0x16,0x61  //0X16Ϊ�򿪵�05H�Ĵ���������05H����̨��
	0x00,0x00,
	0x10,0x9e,
	0x25,0x4A, //0x24,0Xc9̨��  //   0x23,0x46 //0x25,0x4a //0x25,0xCB  //0x26,0x4c̨�����,�����̨���࣬���ݿͻ�����ѡ��  //23h  ��̨����ֵ����
	0x04,0x08, //0830
	0x0c,0x16, //25H://0x06,0x08,//0830
	0xe1,0x05,
	0x3b,0x6c,
	0x2b,0xec,
	0x09,0x0f,
	0x34,0x14, //2AH:
	0x14,0x50,
	0x09,0x6d,
	0x2d,0x96,
	0x01,0xda,
	0x2a,0x7b,
	0x08,0x21, //30H:
	0x13,0xd5,
	0x48,0x91,
	0x00,0xbc,
	0x08,0x96, //34H:0830
	0x15,0x3c,
	0x0b,0x80,
	0x25,0xc7,
	0x00,0x00,
};
#endif

#ifdef RDA5807_M_MP
/* �˳�ʼ��֧�� RDA5807MP  RDA5807M �汾 */
const u8 rad5807M_init_tbl[70] =    //���º��5807���ͺ�5807MP
{
#if (FM_CLOCK == USE_MODULE_CRYSTAL)
	0xC0, 0x05, //02H: internal 32.768M
#elif (FM_CLOCK == SHARE_CRYSTAL_24MHz)
	0x84, 0x51, //02H: external 24M
#elif (FM_CLOCK == SHARE_CRYSTAL_12MHz)
	0xc4, 0x11, //02H: external 12M
#elif (FM_CLOCK == SHARE_CRYSTAL_32KHz)
	0xc0, 0x05, //02H: external 32.768K
#elif ((FM_CLOCK == USE_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_32KHZ_CLK))
	0xc0, 0x05, //02H: external 32.768M
#elif ((FM_CLOCK == USE_IO_12MHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
	0xc4, 0x11, //02H: external 12M
#else
#error "RDA5807M: ��ѡ��FMʱ��Դ"
#endif

	0x00, 0x10,
	0x04, 0x00,
	0x85, 0xaf, //05H:
	0x60, 0x00,
	0x76, 0xc6,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00, //0AH:
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00, //10H:
	0x00, 0x19,
	0x2a, 0x11,
	0x80, 0x53,
	0x22, 0x11,
	0xf8, 0x30, //15H:
	0xc0, 0x00,
	0x2a, 0x91,
	0x94, 0x00,
	0x00, 0xa8,
	0xc4, 0x00, //1AH:
	0xe0, 0x00,
	0x3a, 0x14, //1CH:
	0x81, 0x6a,
	0x46, 0x08,
	0x00, 0x86,
	0x06, 0x61, //20H:
	0x00, 0x00,
	0x10, 0x9e,
	0x22, 0x44,
	0x04, 0x08, //24H:
};
#endif

#pragma constseg=default

void rda5807_seek_tune(u16 freq, u8 diect)@"RDA5807_RX"
{
	u16 ch = 0;

	ch = (freq - 8700)/10;
	dat[0] |=0x40;
	dat[2] = ch>>2;
	dat[3] = ((ch&0x0003)<<6)|0x10;

	rda5807_write(4);
	DELAY_MS(10);

	rda5807_write(4);	//��������
	DELAY_MS(10);
}

void rda5807_read(u8 size)@"RDA5807_RX"
{
	u16 i;

	fm_i2c_start();
	fm_i2c_send_byte(0x21);
	fm_i2c_recv_ack();

	for(i=0; i<size-1; i++)
	{
		readdata[i] = fm_i2c_read_byte();
		fm_i2c_send_ack();
	}

	readdata[i] = fm_i2c_read_byte();
	fm_i2c_send_nack();

	fm_i2c_stop();
}

void rda5807_write(u8 size)@"RDA5807_RX"
{
	u8 i;

	fm_i2c_start();

	fm_i2c_send_byte(0x20);
	fm_i2c_recv_ack();

	for(i=0; i<size; i++)
	{
		fm_i2c_send_byte(dat[i]);
		fm_i2c_recv_ack();
	}

	fm_i2c_stop();
}

void rda5807_fill(const u8 __code *pSrc)@"RDA5807_RX"
{
	u8 i;
	for(i=0; i<= 11; i++)
	{
		dat[i] = *pSrc++;
	}
}

void rda5807_write_c(const u8 __code *writedata, u8 size)@"RDA5807_RX"
{
	u8 i;

	fm_i2c_start();

	fm_i2c_send_byte(0x20);
	fm_i2c_recv_ack();

	for(i=0; i<size; i++)
	{
		fm_i2c_send_byte(*writedata++);
		fm_i2c_recv_ack();
	}

	fm_i2c_stop();
}

/*----------------------------------------------------------------------------*/
/**@brief FMģ��RDA5807P���
   @param ��
   @return ��⵽RDA5807Pģ�鷵��1�����򷵻�0
   @note
*/
/*----------------------------------------------------------------------------*/
u8 rda5807p_online(void)@"RDA5807_RX"
{
	dat[0] = 0x00;
	dat[1] = 0x02;
	rda5807_write(2);

	//read ID
	rda5807_read(10);
	rda5807_chipID = (readdata[8] << 8) | readdata[9];

	if((rda5807_chipID == 0x5801) || (rda5807_chipID == 0x5804) || (rda5807_chipID == 0x5808))
	{
		return 1;
	}
	else {
		return 0;
	}
}

/*----------------------------------------------------------------------------*/
/**@brief FMģ��RDA5807P��ʼ��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void rda5807p_init(void)@"RDA5807_RX"
{
#if RDA5807_PE_SP
	if(0x5804 == rda5807_chipID)
	{
		//power on
		rda5807_fill(rad5807_init_tbl);
		rda5807_write(2);
		DELAY_MS(120);
		//init
		rda5807_write_c(rad5807_init_tbl, 104);
		DELAY_MS(1);
	}
#endif

#if RDA5807_HS_HP
	if(0x5801 == rda5807_chipID)
	{
		//power on
		rda5807_fill(rad5807H_init_tbl);
		rda5807_write(2);
		DELAY_MS(120);
		//init
		rda5807_write_c(rad5807H_init_tbl, 110);
		DELAY_MS(1);
	}
#endif

#if RDA5807_M_MP
	if(0x5808 == rda5807_chipID)
	{
		//power on
		rda5807_fill(rad5807M_init_tbl);
		rda5807_write(2);
		DELAY_MS(120);
		//init
		rda5807_write_c(rad5807M_init_tbl, 70);
		DELAY_MS(1);
	}
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief FMģ��RDA5807P����Ƶ��
   @param freq Ҫ���õ�Ƶ��
   @return ��
   @note Ƶ�ʷ�ΧΪ875~1080
*/
/*----------------------------------------------------------------------------*/
void rda5807p_set_freq(u16 freq)@"RDA5807_RX"
{
	rda5807_seek_tune(freq*10,0);	//������������
}

/*----------------------------------------------------------------------------*/
/**@brief FMģ��RDA5807P��������
   @param vol ����ֵ
   @return ��
   @note ������Χ0~15
*/
/*----------------------------------------------------------------------------*/
void rda5807p_set_vol(u8 vol)@"RDA5807_RX"
{
	if(vol > 15)
		vol = 15;

	dat[7] &=~0x0F;
	dat[7] |= vol;

	dat[3] &= ~BIT(4);  //03H.4=0,disable tune
	rda5807_write(8);
}

/*----------------------------------------------------------------------------*/
/**@brief FMģ��RDA5807P�Զ���̨
   @param freq Ҫ������Ƶ��
   @return ָ����Ƶ������̨����1�����򷵻�0
   @note
*/
/*----------------------------------------------------------------------------*/
u8 rda5807p_seek(u16 freq)@"RDA5807_RX"
{
	rda5807p_set_freq(freq);
	DELAY_MS(10);

	do {
		rda5807_read(4);
		DELAY_MS(4);
	}
	while(!((readdata[3]>>7) & 0x01));
	return (readdata[2] & 0x01)? true : false;
}

/*----------------------------------------------------------------------------*/
/**@brief FMģ��RDA5807P����
   @param
   @return
   @note
*/
/*----------------------------------------------------------------------------*/
void rda5807p_mute(void)@"RDA5807_RX"
{
	dat[0] &= ~(1<<6);//MUTE ENABLE

	rda5807_write(2);
	DELAY_MS(5);
}

/*----------------------------------------------------------------------------*/
/**@brief FMģ��RDA5807P�������
   @param
   @return
   @note
*/
/*----------------------------------------------------------------------------*/
void rda5807p_unmute(void)@"RDA5807_RX"
{
	dat[0] |= (1<<6)|(1<<7);
	dat[1] |= (1<<0);

	rda5807_write(2);
	DELAY_MS(5);
}

/*----------------------------------------------------------------------------*/
/**@brief FMģ��RDA5807P�ر�
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void rda5807p_off(void)@"RDA5807_RX"
{
	dat[1] &= ~(1<<0);
	rda5807_write(2);
	DELAY_MS(100);
}

#endif
