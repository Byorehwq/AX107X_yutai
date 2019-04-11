#include "include.h"
#include "user_i2c.h"
#include "rtc6218.h"

#if FM_RTC6218_EN
/*===================================================================================

If using stand alone x'tal the macro "INTERNAL_XTAL_6218" should be defined.
In shared x'tal case, we should add 100pF capacitor on series path in front of RCLK pin.
BTW, if shared x'tal voltage swing is too low, the macro "INTERNAL_XTAL_6218" should be defined.
In full swing case, the macro "INTERNAL_XTAL_6218" should "NOT" be defined.

===================================================================================*/

#if (FM_CLOCK == SHARE_CRYSTAL_32KHz)
#define INTERNAL_XTAL_6218  //��fmʱ����fm��ʱ�ӵķ�Է壨p2p)����900mv�ҿ����󲻴���3.3vʱ�迪�����򿪺��ӦֵΪ0XB4,������������ɳ�����0xf0
#endif

#if FM_SEEK_50K_STEP
#define __RTCFM_STEP_50K__                      //�򿪺�ʹ��50k�������ر�ʹ��100k����
#define __RTCFM_STEP_50K_CHOOSE_BEST_FROM_TW0__ //ʹ��50K����ʱΪ���ظ����٣�̨����ɿ���
#endif

//#define __RTCFM_ST_STATUS_JUDGE__   //��������ȵ��¼�̨���迪��,�������������ж�������ų�����̨�l����ݵ����������


/*u8 RTC6218_turn_on_off[] = {  0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
                                0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
                                0x00,0xAA
                           };*/

#pragma constseg = RTC6218_RX_TABLE
const u8  RTC6218_CKHopping[]= {0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x70,0x00, 0x20,0x00,
                                0x05,0x0F, 0x0E,0x85, 0x5A,0xA6, 0xDC,0x53, 0x80,0x00, 0x00,0xA3, 0x00,0xA3,
                                0xD0,0x08, 0x7F,0x80, 0x3C,0x08, 0xA6,0xCF, 0x81,0x00, 0x00,0x00, 0x01,0x40,
                                0x47,0x00, 0x00,0x00
                               };

const u8  RTC6218_swbk5[] = {   0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x70,0x00, 0x60,0x00,
                                0x35,0x90, 0x63,0x11, 0x30,0x08, 0x00,0x17, 0x0D,0x79, 0x7D,0x2F, 0x80,0x00,
                                0x02,0x01, 0x77,0x1F, 0x32,0x41, 0x26,0x28, 0xA5,0x16, 0x86,0x80, 0x00,0x00,
                                0x00,0x00, 0x00,0x00
                            };

#if ((FM_CLOCK == USE_IO_12MHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
#ifndef __RTCFM_STEP_50K__
const  u8 RTC6218_power_up[] = {0xC0,0x00, 0x14,0x00, 0x00,0x00, 0x00,0x0C, 0x80,0x01, 0x70,0x00, 0x00,0x00, 0x40,0x50};  // last byte is spike threshold, the larger the more, recommand under 0x50, default 0x10
#else
const  u8 RTC6218_power_up[] = {0xC0,0x00, 0x18,0x00, 0x00,0x00, 0x00,0x0C, 0x80,0x01, 0x70,0x00, 0x00,0x00, 0x40,0x20};  // last byte is spike threshold, the larger the more, recommand under 0x50, default 0x10
#endif

#ifndef __RTCFM_STEP_50K__
const u8 RTC6218_Initial_value[] =         {0xC0,0x00,0x14,0x00,0x00,0x00,0x00,0x0c,0x80,0x01,0x70,0x00,0xe0,0x00};  // RSSI_TH = 0x0C
const u8 RTC6218_Initial_gotobk3value[] =  {0xC0,0x00,0x14,0x00,0x00,0x00,0x00,0x0c,0x80,0x01,0x70,0x00,0xc0,0x00};  // RSSI_TH = 0x0C
#else
const u8 RTC6218_Initial_value[] =         {0xC0,0x00,0x18,0x00,0x00,0x00,0x00,0x0c,0x80,0x01,0x70,0x00,0xe0,0x00};  // RSSI_TH = 0x0C
const u8 RTC6218_Initial_gotobk3value[] =  {0xC0,0x00,0x18,0x00,0x00,0x00,0x00,0x0c,0x80,0x01,0x70,0x00,0xc0,0x00};  // RSSI_TH = 0x0C
#endif

#else

#ifndef __RTCFM_STEP_50K__
#ifndef INTERNAL_XTAL_6218
const  u8 RTC6218_power_up[] = {0xC0,0x00, 0x14,0x00, 0x00,0x00, 0x00,0x0C, 0x80,0x00, 0x70,0x00, 0x00,0x00, 0x40,0x50};  // last byte is spike threshold, the larger the more, recommand under 0x50, default 0x10
#else
const  u8 RTC6218_power_up[] = {0xC0,0x00, 0x14,0x00, 0x00,0x00, 0x00,0x0C, 0x80,0x00, 0xB4,0x00, 0x00,0x00, 0x40,0x50};
#endif
#else
#ifndef INTERNAL_XTAL_6218
const  u8 RTC6218_power_up[] = {0xC0,0x00, 0x18,0x00, 0x00,0x00, 0x00,0x0C, 0x80,0x00, 0x70,0x00, 0x00,0x00, 0x40,0x20};  // last byte is spike threshold, the larger the more, recommand under 0x50, default 0x10
#else
const  u8 RTC6218_power_up[] = {0xC0,0x00, 0x18,0x00, 0x00,0x00, 0x00,0x0C, 0x80,0x00, 0xB4,0x00, 0x00,0x00, 0x40,0x50};
#endif
#endif

#ifndef __RTCFM_STEP_50K__
#ifndef INTERNAL_XTAL_6218
const u8 RTC6218_Initial_value[] =         {0xC0,0x00,0x14,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0x70,0x00,0xe0,0x00};  // RSSI_TH = 0x0C
const u8 RTC6218_Initial_gotobk3value[] =  {0xC0,0x00,0x14,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0x70,0x00,0xc0,0x00};  // RSSI_TH = 0x0C
#else
const u8 RTC6218_Initial_value[] =         {0xC0,0x00,0x14,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0xB4,0x00,0xe0,0x00};  // RSSI_TH = 0x0C
const u8 RTC6218_Initial_gotobk3value[] =  {0xC0,0x00,0x14,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0xB4,0x00,0xc0,0x00};  // RSSI_TH = 0x0C
#endif
#else
#ifndef INTERNAL_XTAL_6218
const u8 RTC6218_Initial_value[] =         {0xC0,0x00,0x18,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0x70,0x00,0xe0,0x00};  // RSSI_TH = 0x0C
const u8 RTC6218_Initial_gotobk3value[] =  {0xC0,0x00,0x18,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0x70,0x00,0xc0,0x00};  // RSSI_TH = 0x0C
#else
const u8 RTC6218_Initial_value[] =         {0xC0,0x00,0x18,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0xB4,0x00,0xe0,0x00};  // RSSI_TH = 0x0C
const u8 RTC6218_Initial_gotobk3value[] =  {0xC0,0x00,0x18,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0xB4,0x00,0xc0,0x00};  // RSSI_TH = 0x0C
#endif
#endif

#endif /*END USE_IO_12MHZ_CLK*/

const u8 RTC6218_Initial_value1[] = {0x00,0x72,0x00,0xFF,0x00,0x1F,0x03,0xFF,0x56,0xD1,0x13,0xB7,0x00,0x00};

__no_init __xdata u8 RTC6218_reg_data[32]@"RTC6218_BUF";

#pragma constseg=default

/*
void delay_nop(u8 n)
{
    while (n--)
        asm("nop");
}*/

extern void delay_nop(u8 n);

void OperationRTC6218_write(const u8 __code *data, u8 numBytes)@"RTC6218_RX"
{
	fm_i2c_start();
	fm_i2c_send_byte(0xc8);
	fm_i2c_recv_ack();
	for(u8 j = 0; j < numBytes; j++)
	{
		fm_i2c_send_byte(data[j]);
		fm_i2c_recv_ack();
	}
	delay_nop(10);
	fm_i2c_stop();
}

void OperationRTC6218_write_a(u8 *data, u8 numBytes)@"RTC6218_RX"
{
	fm_i2c_start();
	fm_i2c_send_byte(0xc8);
	fm_i2c_recv_ack();
	for(u8 j = 0; j < numBytes; j++)
	{
		fm_i2c_send_byte(data[j]);
		fm_i2c_recv_ack();
	}
	delay_nop(10);
	fm_i2c_stop();
}

void OperationRTC6218_power_on_off(u8 power_sta)@"RTC6218_RX"
{
	fm_i2c_start();
	fm_i2c_send_byte(0xc8);
	fm_i2c_recv_ack();
	for(u8 j = 0; j < 28; j++)
	{
		fm_i2c_send_byte(0x00);
		fm_i2c_recv_ack();
	}
	fm_i2c_send_byte(power_sta);
	fm_i2c_recv_ack();
	fm_i2c_send_byte(0xAA);
	fm_i2c_recv_ack();
	delay_nop(10);
	fm_i2c_stop();
}

void OperationRTC6218_read(u8 *data, u8 numBytes)@"RTC6218_RX"
{
	fm_i2c_start();
	fm_i2c_send_byte(0xc9);
	fm_i2c_recv_ack();
	for(u8 j = 0; j < numBytes; j++)
	{
		data[j] = fm_i2c_read_byte();
		if(j==numBytes-1)
			fm_i2c_send_nack();
		else
			fm_i2c_send_ack();
	}
	delay_nop(10);
	fm_i2c_stop();
}

/*************************************************************
*  Radio get ID
*
*************************************************************/
u8 RTC6218_online(void)@"RTC6218_RX"
{
	for(u8 i = 0; i < 3; i++)
	{
		OperationRTC6218_read(RTC6218_reg_data,16);
		if((RTC6218_reg_data[12] == 0x16) && (RTC6218_reg_data[13] == 0x88))
		{
			RTC6218_off();          //��ֹ��λ��������
			return 1;
		}
	}
	return 0;
}

void RTC6218_init(void)@"RTC6218_RX"
{
	//RTC6218_turn_on_off[28] = 0x16;
	//OperationRTC6218_write_a(RTC6218_turn_on_off,30);
	OperationRTC6218_power_on_off(0x16);
	delay_5ms(5);
	//RTC6218_turn_on_off[28] = 0x96;
	//OperationRTC6218_write_a(RTC6218_turn_on_off,30);
	OperationRTC6218_power_on_off(0x96);
	delay_5ms(40);
	OperationRTC6218_write(RTC6218_CKHopping,46);
	delay_5ms(1);
	OperationRTC6218_write(RTC6218_swbk5,46);
	delay_5ms(1);
	OperationRTC6218_write(RTC6218_power_up,16);
	delay_5ms(20);
}



void RTC6218_off(void)@"RTC6218_RX"
{
	//RTC6218_turn_on_off[28] = 0x16;
	//OperationRTC6218_write_a(RTC6218_turn_on_off,30);
	OperationRTC6218_power_on_off(0x16);
	delay_nop(20);
}



/**************************************

RTC6218_Set_Property_FM_Volume()

FM_Volumn: 0~15

***************************************/

void RTC6218_set_vol(u8 FM_Volumn)@"RTC6218_RX"
{
	// Some FM IC has 17 level, RTC serise only level 16
	//u8 RTC6218_set_property[2] = {0xC0,0x0F};
	u8 RTC6218_set_property[2];
	if(FM_Volumn > 15)
	{
		FM_Volumn = 15;
	}
	RTC6218_set_property[0] = 0xC0;
	RTC6218_set_property[1] = FM_Volumn;
	OperationRTC6218_write_a(RTC6218_set_property,2);

}


/**************************************
RTC6218_FM_Tune_Freq()

channel_freq: 8750~10800
channel_space: 50,100,200
***************************************/
__no_init __xdata u8 RTC6218_channel_start_tune[4] @ "RTC6218_BUF";
__no_init __xdata u8 RTC6218_channel_stop_tune[4] @ "RTC6218_BUF";
void RTC6218_set_freq(u16 channel_freq)@"RTC6218_RX"
{

	u16 freq_reg_data;
	u8 loop_counter = 0;

#ifndef __RTCFM_STEP_50K__
	//u8 RTC6218_channel_start_tune[4] = {0xC0,0x00,0x94,0xCA};    //107.7MHz
	//u8 RTC6218_channel_stop_tune[4] = {0xC0,0x00,0x14,0xCA};
	RTC6218_channel_start_tune[0] = RTC6218_channel_stop_tune[0] = 0xC0;
	RTC6218_channel_start_tune[2] = 0x94;
	RTC6218_channel_stop_tune[2] = 0x14;
	channel_freq=channel_freq*10;
#else
	//u8 RTC6218_channel_start_tune[4] = {0xC0,0x00,0x98,0xCA};    //107.7MHz
	//u8 RTC6218_channel_stop_tune[4] = {0xC0,0x00,0x18,0xCA};
	RTC6218_channel_start_tune[0] = RTC6218_channel_stop_tune[0] = 0xC0;
	RTC6218_channel_start_tune[2] = 0x98;
	RTC6218_channel_stop_tune[2] = 0x18;
#endif
	//u8 volume_temp = 0;
	// store volume setting for hardware seek complete.
	OperationRTC6218_read(RTC6218_reg_data, 18);
	//volume_temp = RTC6218_reg_data[17] & 0x0f;

	//RTC6218_channel_start_tune[1] = (RTC6218_channel_start_tune[1] & 0xf0) | volume_temp;
	//RTC6218_channel_stop_tune[1] = (RTC6218_channel_stop_tune[1] & 0xf0) | volume_temp;
	RTC6218_channel_start_tune[1] = RTC6218_reg_data[17] & 0x0f;
	RTC6218_channel_stop_tune[1] = RTC6218_channel_start_tune[1];

	//set tune bit
#ifndef __RTCFM_STEP_50K__
	freq_reg_data = (channel_freq - 7600)/10;
#else
	freq_reg_data = (channel_freq - 7600)/5;
#endif

	RTC6218_channel_start_tune[3] = freq_reg_data & 0xff;
	RTC6218_channel_start_tune[2] = (RTC6218_channel_start_tune[2] & 0xfc) | (freq_reg_data >> 8);

	RTC6218_channel_stop_tune[3] = RTC6218_channel_start_tune[3] ;

	RTC6218_channel_start_tune[2] &= 0x7f;
	OperationRTC6218_write_a(RTC6218_channel_start_tune, 4);
	delay_nop(10);
	RTC6218_channel_start_tune[2] |= 0x80;
	OperationRTC6218_write_a(RTC6218_channel_start_tune, 4);
	delay_nop(10);
	do
	{
		delay_5ms(10);
		OperationRTC6218_read(RTC6218_reg_data, 2);
		loop_counter++;
		WATCHDOG_CLR();
	}
	while(((RTC6218_reg_data[0]&0x40) == 0) && (loop_counter < 80));        //for loop_counter, when tune, the loop time must > 60ms
	loop_counter = 0;

	//clear tune bit
	OperationRTC6218_write_a(RTC6218_channel_stop_tune, 4);
	//wait STC=0
	do
	{
		OperationRTC6218_read(RTC6218_reg_data, 2);
		loop_counter++;
	}
	while(((RTC6218_reg_data[0]&0x40) != 0) && (loop_counter < 0XFF));

}
#ifdef __RTCFM_ST_STATUS_JUDGE__
bool g_need_st_judge = 0;
u8 killed_by_mono_count=0, real_count=0;

/**********************************************************************************
*   set_flag = 0: set bk3_b and bk3_f to defult value
*   set_flag = 1: set bk3_b and bk3_f to low ST judge th value
**********************************************************************************/
void RTC6218_set_bk3_regf(bool set_flag)@"RTC6218_RX"
{
	u8 reg_data_temp[16];
	OperationRTC6218_write(RTC6218_Initial_gotobk3value,14);//change bk3
	delay_nop(10);
	OperationRTC6218_read(RTC6218_reg_data,32);
	delay_nop(10);

	memcpy(&reg_data_temp[0],&RTC6218_reg_data[0],16);

	if(set_flag == 1)
	{
		reg_data_temp[3] = 0xff;      //set bk3_b and bk3_f to low ST judge th  value
		reg_data_temp[10] = 0x0c;
	}
	else {
		reg_data_temp[3] = 0x40;      //set bk3_b and bk3_f to defult value
		reg_data_temp[10] = 0x0d;
	}

	memcpy(&RTC6218_reg_data[0],&RTC6218_reg_data[16],16);
	memcpy(&RTC6218_reg_data[16],&reg_data_temp[0],16);

	OperationRTC6218_write_a(RTC6218_reg_data,32);
	delay_nop(10);
	OperationRTC6218_read(RTC6218_reg_data,32);
	delay_nop(10);

	RTC6218_reg_data[28]=0x00;
	RTC6218_reg_data[29]=0x00;
	OperationRTC6218_write_a(RTC6218_reg_data+16,14); //change to bk0
	delay_nop(10);
}
#endif //endif of  __RTCFM_ST_STATUS_JUDGE__

/**************************************
RTC6218_FM_Tune_Freq()

channel_freq: 7600~10800
channel_space: 50,100,200
return value:0--����̨,1--���ϲ㴫���l���ҵ�̨��û��__RTCFM_STEP_50K_CHOOSE_BEST_FROM_TW0__)
�����__RTCFM_STEP_50K_CHOOSE_BEST_FROM_TW0__ return value:0,1,2
��ʱ����ֵ��Ҫ�ϲ������������ظ�̨���ڴ�̨��ȫ��ʱ��������Ҫ����0--����̨ 1--���ϲ㴫���l���ҵ�̨ 2--���ϲ㴫���l���ټ�50k���l���ҵ�̨
print info:
No. Freq    DC  spike   Freq(+5)  DC(+5)  spike(+5)   +5/+10  SST_1->10|Cnt   rssi    stereo/killed
***************************************/
u8 RTC6218_seek(u16 channel_freq)@"RTC6218_RX"
{

#ifdef __RTCFM_ST_STATUS_JUDGE__
	u8 Stereo_flag= 0,readCnt=10,stereoCount=0;
#else
	u8 RSSIValue= 0;
#endif

	//u8 Spike_Count_1=0;

#ifdef __RTCFM_STEP_50K_CHOOSE_BEST_FROM_TW0__
	u16 DC_Count_1 = 0, DC_Count_2 = 0;
	u8 shiftindex=0; //�뿴����ͷ�ķ���ֵ˵����0��ʾ��ǰ�ϲ㴫���ײ���l�㣬1��ʾ�ϲ㴫���ײ���l����+50k�����ѵ�̨ʱ�������ϲ��̨�ͷ���ѡ����ȷ���l�㡣
#endif

#ifdef __RTCFM_ST_STATUS_JUDGE__
	/* if ((channel_freq==9000) ||(channel_freq==9595) || (channel_freq==9600) || (channel_freq == 9615) \
	     || (channel_freq == 9620)|| (channel_freq == 9645)||(channel_freq >= 10790)) //����ʵ�ʼ�̨�l��������������������ж�
	 {
	     g_need_st_judge = 1;
	 }
	 else
	 {
	     g_need_st_judge = 0;
	 }*/

	g_need_st_judge = 1;

	if(FM_MIN_FREQ == channel_freq)
	{
		RTC6218_set_bk3_regf(1);                //RTC6218_set_bk3_regf_b_low_th();
	}
#else
#ifndef __RTCFM_STEP_50K__
	if((channel_freq == 960) || (channel_freq == 1080))
		return 0;
#else
	if((channel_freq==9595) || (channel_freq==9600) || (channel_freq >= 10790))
		return 0;
#endif
#endif //endif of __RTCFM_ST_STATUS_JUDGE__

	RTC6218_set_freq(channel_freq);

	OperationRTC6218_write(RTC6218_Initial_value,14);
	delay_nop(10);
	OperationRTC6218_read(RTC6218_reg_data, 24);   // Readout (BK7_5[15])

	//Spike_Count_1 = RTC6218_reg_data[20];
#ifdef __RTCFM_STEP_50K_CHOOSE_BEST_FROM_TW0__
	if(RTC6218_reg_data[22]&0x2!=0)
	{
		DC_Count_1 = (~(((RTC6218_reg_data[22]& 0x1)<<8) | RTC6218_reg_data[23])+1) &0x1ff; //2's complement
	}
	else
	{
		DC_Count_1 = (((RTC6218_reg_data[22]& 0x1)<<8) | RTC6218_reg_data[23]); //2's complement
	}
#endif

	OperationRTC6218_write(RTC6218_Initial_value1,14);

	delay_nop(10);
	//printf("%5d \t",channel_freq);
	//printf("0x%02x\t0x%02x\t", DC_Count_1,Spike_Count_1);

	if((RTC6218_reg_data[22]&0x80)!= 0)
	{
#ifdef __RTCFM_ST_STATUS_JUDGE__
		if(10785 == channel_freq)
		{
			RTC6218_set_bk3_regf(0);        //RTC6218_set_bk3_regf_b_default();
		}
#endif
		//printf(".");
		return 0;
	}
	else
	{
		//printf("\n");
#ifdef __RTCFM_ST_STATUS_JUDGE__
		//printf("%02d\t",real_count+killed_by_mono_count+1);
#endif
		//printf("%5d \t",channel_freq);
		//printf("0x%02x\t0x%02x\t", DC_Count_1,Spike_Count_1);

#ifdef __RTCFM_STEP_50K_CHOOSE_BEST_FROM_TW0__
		// tune second station
		//printf("RTC6218_seek set channel_freq = %d+5 first\n", channel_freq);
		RTC6218_set_freq(channel_freq + 5);  // if seek down channel_freq - 5
		OperationRTC6218_write(RTC6218_Initial_value,14);  // change to bk7
		delay_nop(10);
		OperationRTC6218_read(RTC6218_reg_data, 24); // Readout (BK7_5[15])
		//Spike_Count_2 = RTC6218_reg_data[20];
		if(RTC6218_reg_data[22]&0x2!=0)
		{
			DC_Count_2 = (~(((RTC6218_reg_data[22]& 0x1)<<8) | RTC6218_reg_data[23])+1) &0x1ff; //2's complement
		}
		else
		{
			DC_Count_2 = (((RTC6218_reg_data[22]& 0x1)<<8) | RTC6218_reg_data[23]); //2's complement
		}
		OperationRTC6218_write(RTC6218_Initial_value1,14); // change to bk0

		if((RTC6218_reg_data[22]&0x80)== 0)
		{

			//printf("%5d \t",channel_freq+5);
			//printf("0x%02x\t0x%02x\t", DC_Count_2,Spike_Count_2);

			// select minimun DC
			//min_DC =  (DC_Count_1<DC_Count_2) ? DC_Count_1 : DC_Count_2;
			//printf("min_DC=%x\n",min_DC);

			if(DC_Count_1 < DC_Count_2)
			{
				//printf("RTC6218_seek set channel_freq = %d second,min_DC=%d\n", channel_freq,min_DC);
				RTC6218_set_freq(channel_freq);
				shiftindex = 0;
				//printf("+10\t");
			}
			else
			{
				//printf("RTC6218_seek set channel_freq = %d+5 second,min_DC=%d\n", channel_freq,min_DC);
				RTC6218_set_freq(channel_freq+5);
				shiftindex = 1;
				//printf("+10\t");
			}
		}
		else
		{
			//printf("first station got\n");
			//printf("\t\t\t");
			//printf("\t\t");
			//printf("+5\t");
			RTC6218_set_freq(channel_freq);
			shiftindex = 0;
		}//end of "if((RTC6218_reg_data[22]&0x80)== 0)"
#endif

#ifndef __RTCFM_ST_STATUS_JUDGE__
		delay_nop(10);     //delay_5ms(1);
		OperationRTC6218_read(RTC6218_reg_data, 4);
		RSSIValue = RTC6218_reg_data[3];
		//printf("%d\n",RSSIValue);
#endif
		if(RSSIValue < 10)//
		{
#ifdef __RTCFM_ST_STATUS_JUDGE__
			if(10785 == channel_freq)
			{
				RTC6218_set_bk3_regf(0);        //RTC6218_set_bk3_regf_b_default();
			}
#endif
			return 0;
		}

#ifdef __RTCFM_ST_STATUS_JUDGE__
		if(g_need_st_judge)
		{
			//delay_5ms(200);
			//delay_5ms(60);
			for(readCnt=10; readCnt>0; readCnt--) //stereo��־λ��Ҫ300ms��ˢ��һ�Σ��Ҳ�̫�ɿ��ʶ�10������3������Ϊ��������̨
			{
				WATCHDOG_CLR();
				delay_5ms(60);//300ms
				OperationRTC6218_read(RTC6218_reg_data, 2);
				//printf("reg0x10= 0x%02x%02x\n",RTC6218_reg_data[0],RTC6218_reg_data[1]);
				Stereo_flag = (RTC6218_reg_data[0]&0x04)>>2;
				//printf("%1d",Stereo_flag);//��ӡÿ�ζ�ȡstereo��־λ

				stereoCount += ((Stereo_flag) ? 1:0);


				if(stereoCount >= 3)
				{
					break;//����ʱ�ɲ�break
				}
			}
			//printf("|%d\t %d \t",stereoCount,RSSIValue);
			if(stereoCount >= 3)
			{
				real_count++;
				//printf("streo \t\n");

				//printf("real and stereo real_count = %d,stereoCount=%d\n",real_count,stereoCount);
				if(10785 == channel_freq)
				{
					RTC6218_set_bk3_regf(0);          //RTC6218_set_bk3_regf_b_default();
				}
				// RTC6218_set_vol(15);
#ifdef __RTCFM_STEP_50K_CHOOSE_BEST_FROM_TW0__
				return 1+shiftindex; //����ֵ��Ҫ�ϲ������������ظ�̨��0--����̨ 1--�ҵ�̨���l������ϲ㴫���l�� 2--�ҵ�̨���ϲ㴫���l���ټ�50k���l��
#else
				return 1;
#endif
			}
			else
			{
				//printf("killed \t \n");

				killed_by_mono_count++;
				//printf("killed couse of mono count++ = %d\n",killed_by_mono_count);
				if(10785 == channel_freq)
				{
					RTC6218_set_bk3_regf(0);        //RTC6218_set_bk3_regf_b_default();
				}
				return 0;
			}
		}
		else // else of "if(g_need_st_judge)"
		{
			if(10785 == channel_freq)
			{
				RTC6218_set_bk3_regf(0);            //RTC6218_set_bk3_regf_b_default();
			}
			real_count++;
			//printf("\n");

#ifdef __RTCFM_STEP_50K_CHOOSE_BEST_FROM_TW0__
			return 1+shiftindex;
#else
			return 1;
#endif
		}
#else
		//printf("\n");
#ifdef __RTCFM_STEP_50K_CHOOSE_BEST_FROM_TW0__
		return 1+shiftindex;
#else
		return 1;
#endif
#endif
	}
}
#endif

