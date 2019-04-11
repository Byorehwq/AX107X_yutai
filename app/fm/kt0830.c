/*--------------------------------------------------------------------------*/
/**@file KT0830.C
   @brief FM模块QN8035驱动
   @details FM模块QN8035的驱动函数
   @author LZZ
   @date 2010-08-17
   @note 通过宏定义来选择驱动方式
*/
/*----------------------------------------------------------------------------*/

#include "include.h"
#include "kt0830.h"

#if FM_KT0830_EN

#ifdef ISD_DEBUG
//放在全局变量，可省一点空间
s8 afc[3];				//AFC value for previous, current and next channels
u16 freq[3];			//frequency values for previous, current and next channels
#ifdef SEEK_WITH_SNR
u8 snr[3];				//SNR for previous, current and next channels
#endif
#endif /* ISD_DEBUG */

__no_init u8 mem_vol;//@"FM_BUF";				//Rememberred volume before mute
__no_init char mem_afc[3];//@"FM_BUF";			//Rememberred afc values for previous, current and next stations
__no_init u16 mem_freq[3];//@"FM_BUF";			//Rememberred channel frequencies for previous, current and next stations
#ifdef SEEK_WITH_SNR
__no_init u8 mem_snr[3];//@"FM_BUF";			  //Rememberred SNR values for previous, current and next stations
#endif


#pragma location="KT0830EG"
void KT0830_Write(u8 adrres,u16 dat)
{
	fm_i2c_start();

	fm_i2c_send_byte(KT0837_WR);			//写命令
	fm_i2c_recv_ack();

	fm_i2c_send_byte(adrres);
	fm_i2c_recv_ack();

	fm_i2c_send_byte((u8)(dat>>8));
	fm_i2c_recv_ack();

	fm_i2c_send_byte((u8)(dat));
	fm_i2c_recv_ack();

	fm_i2c_stop();
}

#pragma location="KT0830EG"
u16 KT0830_Read(u8 adrres)
{
	u16 value = 0;

	fm_i2c_start();
	fm_i2c_send_byte(KT0837_WR); 			//写命令
	fm_i2c_recv_ack();

	fm_i2c_send_byte(0x00);
	fm_i2c_recv_ack();

	fm_i2c_start();
	fm_i2c_send_byte(KT0837_RD); 			//写命令
	fm_i2c_recv_ack();
	for(; adrres>0; adrres--)
	{
		fm_i2c_read_byte();
		fm_i2c_send_ack();

		fm_i2c_read_byte();
		fm_i2c_send_ack();
	}
	value = (u16)fm_i2c_read_byte();
	fm_i2c_send_ack();

	value <<= 8;
	value += (u16)fm_i2c_read_byte();
	fm_i2c_send_nack();

	fm_i2c_stop();

	return (value);
}

#ifdef ISD_DEBUG
#pragma location="KT0830EG"
void KT0830_Modify(u8 adrres, u16 and, u16 or)
{
	u16 reg;
	reg = KT0830_Read(adrres) & and;
	KT0830_Write(adrres, reg|or);
}
#endif


/*
#pragma location="FmFun"
void kt0830_AntTuneOn(void)
{
	u16 Buf;

	Buf = KT0830_Read(0x23);
	KT0830_Write(0x23, Buf | 0x0004);
	Buf = KT0830_Read(0x1D);
	KT0830_Write(0x1D, (Buf & 0xff7f)|((3-INDUCTOR_Q)<<5));	 		//Write MUTE_B bit to 0
}
//The antenna tuning function will be turn off and certain amount of caps will be connected to RFIN
//The unit of parameter cap is fF. The capactior value is range from 0fF to 49750fF.
u8 KT_FMAntTuneOff(u16 cap)@"FmFun"
{
	u16 regx,capindex;
	if (cap<2000)
	{
	    regx = KT0830_Read(0x23);
		//deg("regx_nxz= %2x\n",regx );
		KT0830_Write(0x23, regx & 0xfffb);								//antenna tunning disable
	    regx = KT0830_Read(0x1D);
		KT0830_Write(0x1D, (regx & 0x00ff) | 0x0080);					//cali loop disable =1 capindex=0
	}
	else
	{
		regx = KT0830_Read(0x23);
		KT0830_Write(0x23, regx | 0x0004);								//antenna tuning enable
		capindex=(cap-2000)/250;
		if (capindex>0xBF)
		capindex=0xBF;
	        regx = KT0830_Read(0x1D);
		KT0830_Write(0x1D, (regx & 0x00ff) | 0x0080 | (capindex<<8));	//cali loop disable =1 capindex
	}
	return(1);
}

#pragma location="FmFun"
void KT_FMTune(u16 Frequency)      //87.5MHz-->Frequency=8750; Mute the chip and Tune to Frequency
{
	u16 channel;
	//u16 tmp_freq,tmp_cap;
	channel=(Frequency*10-6400)/5;

#ifdef ULTRA_LOW_Q
//	if (ant_type_global==SHORT_ANT)
//	{
		tmp_freq=(Frequency*10+50)/100;
		tmp_cap=25330/tmp_freq;
		tmp_cap*=100;tmp_cap=tmp_cap/tmp_freq;
		tmp_cap*=100;tmp_cap=tmp_cap/IND_MATCH;
		tmp_cap*=100;tmp_cap-=CEXT_MATCH;
		KT_FMAntTuneOff(tmp_cap);
//	}else KT_FMAntTuneOff(LONG_ANT_CAP);
#endif

	KT0830_Write(0x03, 0x8800 | channel);	//set tune bit to 1
	DELAY_MS(3);//50);   //++ changed, SO @ 2010-08-27, faster seeking time

}
*/

#pragma location="KT0830EG"
u8 KT_FMValidStation(u16 Frequency) //0-->False Station 1-->Good Station //check AFC_DELTA only
{
	u8 i,j;
	u8 snr2,snr3;
	u16 nextfreq;

#ifndef ISD_DEBUG
	s8 afc[3];				//AFC value for previous, current and next channels
	u16 freq[3];			//frequency values for previous, current and next channels
#ifdef SEEK_WITH_SNR
	u8 snr[3];				//SNR for previous, current and next channels
#endif
#endif /* ISD_DEBUG */

	//afc[0]=0;afc[1]=0;afc[2]=0;		//initialize
	//freq[0]=0;freq[1]=0;freq[2]=0;	//initialize
	my_memset(afc, 0, 3);
	my_memset(freq, 0, 6);
#ifdef SEEK_WITH_SNR
	//snr[0]=0;snr[1]=0;snr[2]=0;		//initialize
	my_memset(snr, 0, 3);
#endif

	nextfreq=Frequency-CHANNEL_STEP;
	//Get AFC values of previous, current and next channels
	for(i=0; i<3; i++)
	{
		//get AFC values for the last station's information
		for(j=0; j<3; j++)
		{
			if(mem_freq[j]==nextfreq)
			{
				afc[i]=mem_afc[j];
				freq[i]=mem_freq[j];
#ifdef SEEK_WITH_SNR
				snr[i]=mem_snr[j];
#endif
			}
		}
		//get new AFC value if the station information is not investigated in the last run
		if(!freq[i])
		{
			kt0830_set_freq(nextfreq);
			afc[i]=KT_FMGetAFC();
#ifdef SEEK_WITH_SNR
			snr[i]=KT_FMGetSNR();
#endif
			freq[i]=nextfreq;
		}
		nextfreq+=CHANNEL_STEP;
	}

	//Record AFC values
	//mem_afc[0]=afc[0];mem_afc[1]=afc[1];mem_afc[2]=afc[2];
	//mem_freq[0]=freq[0];mem_freq[1]=freq[1];mem_freq[2]=freq[2];
	smemcpy(mem_afc, afc, 3);
	smemcpy(mem_freq, freq, 6);
#ifdef SEEK_WITH_SNR
	//mem_snr[0]=snr[0];mem_snr[1]=snr[1];mem_snr[2]=snr[2];
	smemcpy(mem_snr, snr, 3);
#endif

	//Determine the validation of current station
	//changed, 27-2-2011 if ((afc[0]<afc[1]) && (afc[1]<afc[2])&& (afc[0]<-AFCTH_PREV) && (afc[1]>-AFCTH) && (afc[1]<AFCTH) && (afc[2]>AFCTH_NEXT))
	if((afc[0]<afc[1]) && (afc[1]<afc[2]) && (afc[0]<(32-AFCTH_PREV)) && (afc[1]>(32-AFCTH)) && (afc[1]<(AFCTH+32)) && (afc[2]>(AFCTH_NEXT+32)))
	{
#ifdef SEEK_WITH_SNR
		kt0830_set_freq(Frequency);
		snr2=KT_FMGetSNR();
		if((snr[1]>=SNR_TH) && (snr2>=SNR_TH))  //-- changed, SO @ 2010-08-25
//		if ((snr[1]>=*pR1) && (snr2>=*pR1))     //++, nxz
//		if ((snr[1]>=0x30) && (snr2>=0x30))     //++
		{
			//deg("snr1=%d,snr2=%d\n",snr[1],snr2);
			return(1);
		}
		if((snr[1]<SNR_TH) && (snr2<SNR_TH))    //-- changed, SO @ 2010-08-25
//		if ((snr[1]<*pR1) && (snr2<*pR1))       //++, nxz
//		if ((snr[1]<0x30) && (snr2<0x30))       //++
		{
			return(0);
		}
		DELAY_MS(5);
		snr3=KT_FMGetSNR();
		if(snr3>=SNR_TH)    //-- changed, SO @ 2010-08-25
//		if (snr3>=*pR1)     //++, nxz
//		if (snr3>=0x30)     //++
		{
			//deg("snr1=%d,snr2=%d,snr3=%d\n",snr[1],snr2,snr3);
			return(1);
		}
		else
		{
			return(0);
		}
#else
		return(1);
#endif
	}
	else
		return(0);
}


/*
void KT0830E_UnSoftMute(void)@"FmFun"
{
	u16 Buf;

	Buf = Read_KT0830E(0x04);
	Write_KT0830E(0x04, Buf | 0x8000);	 		//Write MUTE_B bit to 0
	DELAY_MS(10);
}

void KT0830E_SoftMute(void)@"FmFun"
{
	u16 Buf;

	Buf = Read_KT0830E(0x04);
	Write_KT0830E(0x04, Buf & 0x7FFF);	 		//Write MUTE_B bit to 0
	DELAY_MS(10);
}
*/

#pragma location="KT0830EG"
char KT_FMGetAFC(void)							// returned value's range is from -127 to 127, unit is KHz
{
#if 0
	//changed, 27-2-2011
	u8 afc_delta;
	u16 reg14;
	reg14 = KT0830_Read(0x14);					// AFC_DELTAF
	afc_delta = reg14 & 0x003f;
	if(afc_delta & 0x20)
		afc_delta=afc_delta-32;
	else
		afc_delta=afc_delta+32;
	return (afc_delta);
#else
	u8 afc_delta;
	afc_delta = KT0830_Read(0x14);				// AFC_DELTAF
	afc_delta &= 0x3f;
	afc_delta ^= 0x20;							//可节省一点空间
	return (afc_delta);
#endif
}

#pragma location="KT0830EG"
u8 KT_FMGetSNR(void)
{
	return (KT0830_Read(0x1F) & 0x00FF);
}

#pragma location="KT0830EG"
u8 kt0830_online(void)
{
	return (KT0830_Read(0x01) == 0x0440)? true : false;
}

#pragma location="KT0830EG"
void kt0830_init(void)
{
	u8 i = INIT_FAIL_TH;
	u16 Buf;

	KT0830_Write(0x0f, 0x8a00);

#if (FM_CLOCK == USE_MODULE_CRYSTAL)
	KT0830_Write(0x23, 0x0001);		//internal 32.768k
#elif (FM_CLOCK == SHARE_CRYSTAL_24MHz)
	KT0830_Write(0x23, 0x1701);		//external 24M
#elif (FM_CLOCK == SHARE_CRYSTAL_12MHz) || (FM_CLOCK == USE_IO_12MHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
	KT0830_Write(0x23, 0x1301);		//external 12M
#elif (FM_CLOCK == SHARE_CRYSTAL_32KHz)|| (FM_CLOCK == USE_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_32KHZ_CLK))
	KT0830_Write(0x23, 0x1001);		//external 32.768k
#else
#error "KT0830: 请选择FM时钟源"
#endif

	kt0830_mute();
	KT0830_Write(0x0f, 0x8a00);

	while(i--)
	{
		DELAY_MS(10);
		Buf = KT0830_Read(0x12);		  			//Read power-up indicator
		Buf >>= 8;
		if((Buf&0x88) != 0x88)
			break;
	}

	KT0830_Write(0x0a, 0x0000); 				//Turn On AFC function
	KT0830_Write(0x02, 0x2207); 				//Write Frequency Space to 100KHz

#ifndef ISD_DEBUG
	Buf = KT0830_Read(0x22); 					//
	KT0830_Write(0x22, Buf|0x1000);				//Configure RFAGC parameter of agc_filter_dis=1
	Buf = KT0830_Read(0x21); 					//
	KT0830_Write(0x21, (Buf&0xfff8)|0x0003);	//Configure RFAGC parameter of peaklowth=60mV
	Buf = KT0830_Read(0x09); 					//
	KT0830_Write(0x09, (Buf&0x1fff)|0x2000);	//Configure RFAGC parameter of peakhighth=90mV
	Buf = KT0830_Read(0x0A);
	KT0830_Write(0x0A, Buf | 0x0400);			//loldo_ctrl=highest

	//kt0830_AntTuneOn();
	Buf = KT0830_Read(0x23);
	KT0830_Write(0x23, Buf | 0x0004);
	Buf = KT0830_Read(0x1D);
	KT0830_Write(0x1D, (Buf & 0xff7f)|((3-INDUCTOR_Q)<<5));	 		//Write MUTE_B bit to 0
#else
	KT0830_Modify(0x22, 0xffff, 0x1000);
	KT0830_Modify(0x21, 0xfff8, 0x0003);
	KT0830_Modify(0x09, 0x1fff, 0x2000);
	KT0830_Modify(0x0A, 0xffff, 0x0400);

	KT0830_Modify(0x23, 0xffff, 0x0004);
	KT0830_Modify(0x1D, 0xff7f, ((3-INDUCTOR_Q)<<5));
#endif
}

#pragma location="KT0830EG"
void kt0830_set_freq(u16 freq)
{
#if 0
	KT_FMTune(freq);
#else
	u16 channel;
	channel=(freq*10-6400)/5;
	KT0830_Write(0x03, 0x8800 | channel);	//set tune bit to 1
	DELAY_MS(3);//50);   //++ changed, SO @ 2010-08-27, faster seeking time
#endif
}

#pragma location="KT0830EG"
void kt0830_set_vol(u8 vol)
{
	if(vol > 15)
		vol = 15;

#ifdef ISD_DEBUG
	KT0830_Modify(0x04, 0xfff0, vol | 0x4000);
#else

	u16 Buf;
	Buf = KT0830_Read(0x04);
	KT0830_Write(0x04, (Buf & 0xfff0) | vol | 0x4000);
#endif
}

#pragma location="KT0830EG"
void kt0830_off(void)
{
	kt0830_mute();
	KT0830_Write(0x0F, 0x9A00);      			//Write Standby bit to 1
}

#pragma location="KT0830EG"
u8 kt0830_seek(u16 freq)
{
	return KT_FMValidStation(freq);
}

#pragma location="KT0830EG"
void kt0830_unmute(void)
{
	u16 Buf;

	Buf = KT0830_Read(0x04);
	KT0830_Write(0x04, Buf | 0x4000); 			//Write MUTE_B bit to 1
	DELAY_MS(5);
}

#pragma location="KT0830EG"
void kt0830_mute(void)
{
	u16 Buf;

	Buf = KT0830_Read(0x04);
	KT0830_Write(0x04, Buf & 0xBFFF);	 		//Write MUTE_B bit to 0
	DELAY_MS(5);
}

#endif


