/*****************************************************************************
 * Module    : Module
 * File      : dac.c
 * Author    : jingfa
 * Email     :
 * Function  : DAC相关配置
 *****************************************************************************/
#include "include.h"
#include "dac.h"

#pragma constseg="USER_SETTING_CONST"
#if 0 
#ifdef AX207X_TAG
IAR_CONST u8 tbl_anlvol[VOLUME_MAX+1] =
{
	//取值范围：(60-60) ~ (60+5),共66个值;
	//说明：值(60-60)对应音量-60dB; 60对应音量0dB;
	60-60, 60-43, 60-32, 60-26, 60-22, 60-20, 60-17, 60-14,
	60-12, 60-11, 60-10, 60-8, 60-7,   60-5,  60-3,  60,    60+1
};
#else
IAR_CONST u8 tbl_anlvol[VOLUME_MAX+1] =
{
	//取值范围：(60-60) ~ (60+5),共66个值;
	//说明：值(60-60)对应音量-60dB; 60对应音量0dB;
	60-60, 60-42, 60-31, 60-25, 60-21, 60-19, 60-16, 60-13,
	60-11, 60-10, 60-9, 60-7, 60-6,   60-4,  60-2,  60+1,    60+2
};
#endif
#endif
#if 1		// 50刻度
IAR_CONST u8 tbl_anlvol[VOLUME_MAX+1] =
{
    //取值范围：(60-60) ~ (60+5),共66个值;
    //说明：值(60-60)对应音量-60dB; 60对应音量0dB;
    60-60,60-55,60-50,60-45,60-44,60-42,60-40,60-38,60-37,60-36,
    60-35,60-34,60-33,60-32,60-31,60-30,60-29,60-28,60-27,60-26,
    60-25,60-24,60-23,60-22,60-21,60-20,60-19,60-18,60-17,60-16,
    60-15,60-14,60-13,60-12,60-11,60-10,60-9, 60-8, 60-7, 60-6, 
    60-5, 60-4, 60-3, 60-2, 60-1, 60,60+1, 60+2, 60+3, 60+4, 60+5
};
#else		// 30刻度
IAR_CONST u8 tbl_anlvol[VOLUME_MAX+1] =
{
    //取值范围：(60-60) ~ (60+5),共66个值;
    //说明：值(60-60)对应音量-60dB; 60对应音量0dB;
    60-60,60-50,60-43,60-40, 60-37,60-34, 60-32,60-30,60-27,60-25, 
    60-24,60-23,60-22, 60-19, 60-18, 60-16,60-15,60-14,60-13,60-12, 
    60-11, 60-10,60-9,60-8, 60-7,60-6,60-5,60-4,60-3,60-2,60
};
#endif
#pragma constseg=default


#if DAC_POWER_SPEED

IAR_DATA_A type_dac_init_ctl dac_init_ctl;
//DAC模拟部分初始化函数，中断中分步完成DAC初始化，上电速度提升
#pragma location="DAC_INIT"
void dac_init_process(void)
{
	u8 i;

	switch(dac_init_ctl.sta)
	{
	case DAC_INIT_START:
#if (DAC_INIT_TYPE == DAC_SIMPLE)
#if VDDHP_VDDIO_SHT
		//VDDHP和VDDIO短接时
		DACACON3 = 0x2A;             //en VCMIR(bit3)
		DACACON2 = 0x63;             //dis VDDHP POWER(bit7)
		DACACON1 = 0x4C;
#else
		DACACON1 = 0x4C;
		DACACON2 = 0xe3;
		DACACON3 = 0x22;
#endif
		DACACON5=0xF8;
#if USE_MULTI_BIT_DAC
		DACACON7 |= BIT(0);
		at_write(DACCFG, 0x09);
#else
		at_write(DACCFG, 0x01);
#endif
		at_write(DACVCON, 0x07);    //dig_vol step 8
		at_write(DACVOLH, 0x00);
		at_write(DACVOLL, 0x00);

		DACACON1 |= BIT(4);         //enable DACTCMP
#else
#if VDDHP_VDDIO_SHT
		//VDDHP和VDDIO短接时
		DACACON3 |=BIT(3);          //en VCMIR(bit3)
		DACACON2 = 0x62;            //dis VDDHP(bit7)
		DACACON1 = 0x4C;
#else
		DACACON1 = 0x4C;
		DACACON2 = 0xe2;
#endif
		//DACACON3 &= ~(BIT(5) | BIT(4));            //DTC=00
		DACACON4 |= BIT(3);         //enalble VCM_INT
#if USE_MULTI_BIT_DAC
		DACACON7 |= BIT(0);
#if (DAC_INIT_TYPE == DAC_VCM_NO_CAP)
		DACACON7 |= BIT(1);
		DACACON3 |=BIT(3);          //en VCMIR(bit3)
		DACACON4 |=BIT(2);
		/*DACACON4[2]是用来选择VCM电压的，当DACACON3[3]置1时，
		如果DACACON4[2]=0，则VCM~=1.29；
		如果DACACON4[2]=1，则VCM~=1.365；
		为了提高DAC的输出幅度和性能，可以将这一bit设1，
		为了降低最低可工作电压，可以将这一bit设0. */
#endif
		at_write(DACCFG, 0x09);
#else
		at_write(DACCFG, 0x01);
#endif
		AUCON10 = 0x10;            //32 samples
		AUCON0 = 0x06;
		for(i=0; i<68; i++)
		{
			while(!(AUCON7 & 0x80));
			AUCON5 = 0xff;
			AUCON5 = 0x7f;
		}
#endif
		dac_init_ctl.sta++;
		break;

	case 1:
#if (DAC_INIT_TYPE == DAC_SIMPLE)
		dac_init_ctl.delay_count++;
		if(dac_init_ctl.delay_count == 125)
		{
			at_write(TRIMCON1, 0x05);  //decrease 1 step 1 sample
			at_write(TRIMCON2, 0x01);
			dac_init_ctl.sta++;
		}
#else
		DACACON2 |= BIT(2);         //enalble EN_PD
		at_write(DACVCON, 0x05);    //dig_vol step 2
		at_write(DACVOLH, 0x00);
		at_write(DACVOLL, 0x00);
		dac_init_ctl.sta++;
#endif
		break;

	case 2:
#if (DAC_INIT_TYPE == DAC_SIMPLE)
		if(at_read(TRIMCON2)&BIT(1))
		{
			DACACON1 &= ~BIT(4);         //disable DACTCMP
#ifdef AX207X_TAG
			dac_get_trim();
#endif
			//fade out anl_vol & mute
			b_dac.digvol_en = 0;
			dac_ctl.anl_volcur = 65;
#if DAC_FADE_EN
			dac_fade_out();
#endif
			dac_init_ctl.sta = 10;
			//dac_init_ctl.sta = DAC_INIT_END;
		}
#else
		if(at_read(DACVOLL) & 0x80)
		{
			AUCON10 = 0x10;            //32 samples
			AUCON0 = 0x06;
			for(i=0; i<68; i++)
			{
				while(!(AUCON7 & 0x80));
				AUCON5 = 0x00;
				AUCON5 = 0x80;
			}

			dac_init_ctl.sta++;
		}
#endif
		break;
#if (DAC_INIT_TYPE != DAC_SIMPLE)
	case 3:
		at_write(DACVCON, 0x05);    //dig_vol step 2
		at_write(DACVOLH, 0xFF);
		at_write(DACVOLL, 0x7F);

		dac_init_ctl.sta++;
		break;

	case 4:
		if(at_read(DACVOLL) & 0x80)
		{
			dac_init_ctl.delay_count = 0;

			dac_init_ctl.sta++;
		}
		break;

	case 5:
		dac_init_ctl.delay_count++;
		if(dac_init_ctl.delay_count == 25)      //25*2ms=50ms
		{
			//tie output to ground
			DACACON2 |= BIT(3);         //enable EN_POPS
			DACACON4 &= ~BIT(3);        //disable VCM_INT

#if (DAC_INIT_TYPE == DAC_VCM_WITH_CAP)
			//for VCM with external cap
			DACACON2 &= ~BIT(2);         //disalble EN_PD
			AUCON10 = 0x10;            //32 samples
			AUCON0 = 0x06;
			for(i=0; i<68; i++)
			{
				while(!(AUCON7 & 0x80));
				AUCON5 = 0x00;
				AUCON5 = 0x00;
			}
			DACACON2 &= ~BIT(5); //disable VCM
			DACACON2 &= ~BIT(1); //disable LPF

			dac_init_ctl.sta = 16;
#else
			//for no VCM cap
			DACACON5=0xfd;              //anl_vol
			DACACON2 &= ~BIT(2);        //disable EN_PD
			dac_init_ctl.sta++;
#endif
		}
		break;

#if (DAC_INIT_TYPE == DAC_VCM_WITH_CAP)
	case 16:
		DACACON1 &= ~BIT(6); //disable DAC
		DACACON1 |= BIT(7); //enable HP MUTE
		DACACON2 |= BIT(0); //enable HP

		dac_init_ctl.delay_count = 0;
		dac_init_ctl.sta++;
		break;

	case 17:
		dac_init_ctl.delay_count++;
		if(dac_init_ctl.delay_count == 5)
		{
			DACACON2 &= ~BIT(3);        //disable EN_POPS
			DACACON2 |= BIT(5); //enable VCM

			dac_init_ctl.delay_count = 0;
			dac_init_ctl.sta++;
		}
		break;

	case 18:
		dac_init_ctl.delay_count++;
		if(dac_init_ctl.delay_count == 125)
		{
			DACACON1 |= BIT(6); //enable DAC
			DACACON2 |= BIT(1); //ensable LPF
			DACACON1 |= BIT(4);         //enable DACTCMP

			dac_init_ctl.sta++;
		}
		break;

	case 19:
		at_write(TRIMCON1, 0x05);  //decrease 1 step 1 sample
		at_write(TRIMCON2, 0x01);

		dac_init_ctl.sta = 9;
		break;
#else
	case 6:
		DACACON2 |= BIT(0);         //enalble EN_HP
		DACACON1 |= BIT(4);         //enable DACTCMP

		dac_init_ctl.sta++;
		break;

	case 7:
		DACACON2 &= ~BIT(3);        //disable EN_POPS

		at_write(DACVCON, 0x07);    //dig_vol step 8
		at_write(DACVOLH, 0x00);
		at_write(DACVOLL, 0x00);

		dac_init_ctl.sta++;
		break;

	case 8:
		if(at_read(DACVOLL) & 0x80)
		{
			at_write(TRIMCON1, 0x05);  //decrease 1 step 1 sample
			at_write(TRIMCON2, 0x01);

			dac_init_ctl.sta++;
		}
		break;
#endif

	case 9:
		if(at_read(TRIMCON2)&BIT(1))
		{
			DACACON1 &= ~BIT(4);         //disable DACTCMP
#ifdef AX207X_TAG
			dac_get_trim();
#endif
			//fade out anl_vol & mute
			b_dac.digvol_en = 0;
			dac_ctl.anl_volcur = 65;
#if DAC_FADE_EN
			dac_fade_out();
#endif
			dac_init_ctl.sta++;
		}
		break;
#endif

	case 10:
#if DAC_FADE_EN
		if(!dac_fade_busy())
#endif
		{
			dac_init_ctl.sta = DAC_INIT_END;
		}
		break;
	default:
		break;
	}
#if (USER_SEL == USER_AX2075_DEMO)
	//右声道混合到左声道输出
	at_write(DACLRMIX0, 64);
	at_write(DACLRMIX1, 64);
#else
#if DACR_ONLY
	/*DAC right channel output only,
	meanwhile,left channel can be used as GPIO controlled by P45*/
	if(dac_init_ctl.sta == DAC_INIT_END)
	{
#ifdef AX207X_TAG
		dac_channel_disconnect();
#endif
		DACACON4 |= BIT(1);//DACROUTONLY

		at_write(DACLRMIX2, 64);
		at_write(DACLRMIX3, 64);
	}
#endif
#endif
}

#else

#pragma location="DAC_INIT"
void outbuf_trim_dac(u16 dig_vol)
{
	u8 i;
	AUCON10 = 0x10;            //32 samples
	AUCON0 = 0x06;
	for(i=0; i<68; i++)
	{
		while(!(AUCON7 & 0x80));
		AUCON5 = (u8)dig_vol;
		AUCON5 = (u8)(dig_vol >> 8);
	}
	delay_5ms(1);
}

//DAC上电初始化配置,延时等待DAC初始化完成，影响上电时间，但省代码
#pragma location="DAC_INIT"
void dac_power_on(void)
{
#if (DAC_INIT_TYPE == DAC_SIMPLE)
#if VDDHP_VDDIO_SHT
	//VDDHP和VDDIO短接时
	DACACON3 = 0x2A;             //en VCMIR(bit3)
	DACACON2 = 0x63;             //dis VDDHP POWER(bit7)
	DACACON1 = 0x4C;
#else
	DACACON1 = 0x4C;
	DACACON2 = 0xe3;
	DACACON3 = 0x22;
#endif
	DACACON5 = 0xF8;
#if USE_MULTI_BIT_DAC
	DACACON7 |= BIT(0);
	at_write(DACCFG, 0x09);
#else
	at_write(DACCFG, 0x01);
#endif
	at_write(DACVCON, 0x07);    //dig_vol step 2
	at_write(DACVOLH, 0x00);
	at_write(DACVOLL, 0x00);
	DACACON1 |= BIT(4);         //enable DACTCMP
	delay_5ms(50);
	at_write(TRIMCON1, 0x05);   //decrease 1 step 1 sample
	at_write(TRIMCON2, 0x01);
	while(!(at_read(TRIMCON2) & BIT(1)));
	DACACON1 &= ~BIT(4);         //disable DACTCMP
#ifdef AX207X_TAG
	dac_get_trim();
#endif
	//fade out anl_vol & mute
	b_dac.digvol_en = 0;
	dac_ctl.anl_volcur = 65;
#if DAC_FADE_EN
	dac_fade_out();
	dac_fade_wait();
#endif
#else
#if VDDHP_VDDIO_SHT
	//VDDHP和VDDIO短接时
	DACACON3 |=BIT(3);          //en VCMIR(bit3)
	DACACON2 = 0x62;            //dis VDDHP(bit7)
	DACACON1 = 0x4C;
#else
	DACACON1 = 0x4C;
	DACACON2 = 0xe2;
#endif
	//DACACON3 &= ~(BIT(5) | BIT(4));            //DTC=00
	DACACON4 |= BIT(3);         //enalble VCM_INT
#if USE_MULTI_BIT_DAC
	DACACON7 |= BIT(0);
#if (DAC_INIT_TYPE == DAC_VCM_NO_CAP)
	DACACON7 |= BIT(1);
	DACACON3 |=BIT(3);          //en VCMIR(bit3)
	DACACON4 |=BIT(2);
	/*DACACON4[2]是用来选择VCM电压的，当DACACON3[3]置1时，
	如果DACACON4[2]=0，则VCM~=1.29；
	如果DACACON4[2]=1，则VCM~=1.365；
	为了提高DAC的输出幅度和性能，可以将这一bit设1，
	为了降低最低可工作电压，可以将这一bit设0. */
#endif
	at_write(DACCFG, 0x09);
#else
	at_write(DACCFG, 0x01);
#endif
	outbuf_trim_dac(0x7fff);

	DACACON2 |= BIT(2);         //enalble EN_PD

	at_write(DACVCON, 0x05);    //dig_vol step 2
	at_write(DACVOLH, 0x00);
	at_write(DACVOLL, 0x00);
	while(!(at_read(DACVOLL) & 0x80));

	outbuf_trim_dac(0x8000);

	at_write(DACVCON, 0x05);    //dig_vol step 2
	at_write(DACVOLH, 0xFF);
	at_write(DACVOLL, 0x7F);
	while(!(at_read(DACVOLL) & 0x80));

	delay_5ms(11);
	DACACON2 |= BIT(3);         //enable EN_POPS
	DACACON4 &= ~BIT(3);        //disable VCM_INT
#if (DAC_INIT_TYPE == DAC_VCM_WITH_CAP)
	//for VCM with external cap
	DACACON2 &= ~BIT(2);         //disalble EN_PD

	outbuf_trim_dac(0x0000);

	DACACON2 &= ~BIT(5); //disable VCM
	DACACON2 &= ~BIT(1); //disable LPF
	delay_5ms(1);
	DACACON1 &= ~BIT(6); //disable DAC
	DACACON1 |= BIT(7); //enable HP MUTE
	DACACON2 |= BIT(0); //enable HP
	delay_5ms(3);
	DACACON2 &= ~BIT(3);        //disable EN_POPS
	DACACON2 |= BIT(5); //enable VCM
	delay_5ms(52);
	DACACON1 |= BIT(6); //enable DAC
	DACACON2 |= BIT(1); //ensable LPF
	DACACON1 |= BIT(4);         //enable DACTCMP
	delay_5ms(1);
	at_write(TRIMCON1, 0x05);  //decrease 1 step 1 sample
	at_write(TRIMCON2, 0x01);
#else
	//for no VCM cap
	DACACON5=0xfd;              //anl_vol
	DACACON2 &= ~BIT(2);        //disable EN_PD
	delay_5ms(1);
	DACACON2 |= BIT(0);         //enalble EN_HP
	DACACON1 |= BIT(4);         //enable DACTCMP
	delay_5ms(1);
	DACACON2 &= ~BIT(3);        //disable EN_POPS
	at_write(DACVCON, 0x07);    //dig_vol step 2
	at_write(DACVOLH, 0x00);
	at_write(DACVOLL, 0x00);
	while(!(at_read(DACVOLL) & 0x80));
	at_write(TRIMCON1, 0x05);  //decrease 1 step 1 sample
	at_write(TRIMCON2, 0x01);
#endif
	while(!(at_read(TRIMCON2)&BIT(1)));
	DACACON1 &= ~BIT(4);         //disable DACTCMP

#ifdef AX207X_TAG
	dac_get_trim();
#endif
	//fade out anl_vol & mute
	b_dac.digvol_en = 0;
	dac_ctl.anl_volcur = 65;
#if DAC_FADE_EN
	dac_fade_out();
	dac_fade_wait();
#endif
#endif

#if DACR_ONLY
	/*DAC right channel output only,
	meanwhile,left channel can be used as GPIO controlled by P45*/
#ifdef AX207X_TAG
	dac_channel_disconnect();
#endif

	DACACON4 |= BIT(1);//DACROUTONLY

	at_write(DACLRMIX2, 64);
	at_write(DACLRMIX3, 64);
#endif

#ifdef AX207X_TAG
	dac_clr_trim();
#endif
}
#endif /*END DAC_POWER_SPEED宏*/

//使用vcm buf初始化
#pragma location="DAC_INIT"
void dac_init_vcmbuf(void)
{
	DACACON1 |= BIT(7)|BIT(3);  //mute, DACBIAS
#if USE_MULTI_BIT_DAC
	DACACON7 |= BIT(0);
#if (DAC_INIT_TYPE == DAC_VCM_NO_CAP)
	DACACON7 |= BIT(1);
	DACACON3 |=BIT(3);          //en VCMIR(bit3)
	DACACON4 |=BIT(2);
	/*DACACON4[2]是用来选择VCM电压的，当DACACON3[3]置1时，
	如果DACACON4[2]=0，则VCM~=1.29；
	如果DACACON4[2]=1，则VCM~=1.365；
	为了提高DAC的输出幅度和性能，可以将这一bit设1，
	为了降低最低可工作电压，可以将这一bit设0. */
#endif
	at_write(DACCFG, 0x09);
#else
	at_write(DACCFG, 0x01);
#endif
	DACACON3 |= BIT(3);         //SET_VCMIR
	DACACON2 |= BIT(6)|BIT(5);  //EN_VDDDAC, EN_VCM

#if (DAC_INIT_TYPE == DAC_VCM_WITH_CAP)  //有VCM电容，有HP
	DACACON2 |= BIT(7);        //EN_VDDHP
	delay_5ms(60);             //等待VCM电容(105)建立
#else
	delay_5ms(1);
#endif
	DACACON2 |= BIT(4)|BIT(1);  //EN_VCMBUF, EN_LPF
	DACACON1 |= BIT(6);         //EN_DAC
	DACACON2 |= BIT(0)|BIT(4);  //EN_HP, EN_VCMBUF

	//trim dac
	DACACON1 |= BIT(4);         //enable DACTCMP
	delay_5ms(1);
	DACACON3 |= BIT(6);         //low power，放在延时之后可减小开机噪声

	at_write(TRIMCON1, 0x05);   //decrease 1 step 1 sample
	at_write(TRIMCON2, 0x01);
	while(!(at_read(TRIMCON2)&BIT(1)));
	DACACON1 &= ~BIT(4);        //disable DACTCMP

	at_write(DACVCON, 0x07);
	at_write(DACVOLH, 0x00);
	at_write(DACVOLL, 0x00);
	while(!(at_read(DACVOLL) & 0x80));

#ifdef AX207X_TAG
	dac_get_trim();
#endif

}


#pragma location="DAC_INIT"
void dac_init(void)
{
	PCON1 &= ~BIT(7);
#if EQ_MODIFY_FILTER_EN
	music_eq_init();   //eq滤波器系数一定要在打开dac clk之后，enable dac之前配置
#else
	music_eq_init_m();   //eq滤波器系数一定要在打开dac clk之后，enable dac之前配置
#endif

#if DAC_BUF_EN
    //dac_init_vcmbuf();
    /*if(vcmbuf_flag == 0)
    {
	  //uart_init();        //uart_tx:P35, baud rate:115200  UARTCON
	  uart_putc('u');
	  //UARTSTA &= ~BIT(0);
	  printf("UARTCON: %d\n", (UARTCON & BIT(4)));
	  //UARTCON &= ~BIT(4);
	  printf("UARTCON: %d\n", (UARTCON & BIT(4)));
    }*/
	dac_init_vcmbuf();
	uart_putc('v');
	/*else 
	{
	  dac_init_vcmbuf();
	  uart_putc('v');
	}*/
#else
#if DAC_POWER_SPEED
	dac_init_ctl.sta = DAC_INIT_START;
#else
	dac_power_on();
#endif
#endif
}

//等待DAC初始化完成
#pragma location="DAC_INIT"
void dac_init_wait(void)
{
	while(dac_init_ctl.sta < DAC_INIT_END)
	{
		WATCHDOG_CLR();
	}
#ifdef AX207X_TAG
	dac_clr_trim();
#endif
}

#if DACR_ONLY
//单声道输出时，断开DACR与DACL之间的连接
#pragma location="DAC_INIT"
void dac_channel_disconnect(void)
{
	AIPCON10 |= BIT(5) | BIT(4);
	//bit5:MCSW,close the switch to output MCOx
	//bit4:LPMT,mute the mic LPF
	AUADCCON0 = 0x02;    //bit1:mic analog enable
}
#endif


//设置DAC音量
#pragma location="DAC_SEG"
void dac_set_volume(u8 vol)
{
	b_dac.fade_en = 0;
#ifdef AX207X_TAG
	dac_ctl.dig_vol = 0x7200;           //数字音量（范围:0x0000 - 0x7fff）
#else
	dac_ctl.dig_vol = 0x7800;           //数字音量（范围:0x0000 - 0x7fff）
#endif
	dac_ctl.anl_vol = tbl_anlvol[vol];

	dac_volume_fade_set();
	b_dac.fade_en = 1;
}

//设置DAC音量(直接配置，不淡入)
#pragma location="DAC_SEG"
void dac_set_volume_direct(u8 vol)
{
#ifdef AX207X_TAG
	dac_set_trim();
#endif

	if(b_dac.digvol_en)
	{
#ifdef AX207X_TAG
		dac_set_digvol(0x7200);             //数字音量（范围:0x0000 - 0x7fff）
#else
		dac_set_digvol(0x7800);             //数字音量（范围:0x0000 - 0x7fff）
#endif
	}

	dac_ctl.anl_vol = tbl_anlvol[vol];
	dac_set_anlvol(dac_ctl.anl_vol);

#ifdef AX207X_TAG
	dac_clr_trim();
#endif
}

#pragma constseg="DAC_INIT_CONST"
IAR_CONST u8 dac_sprcon_sf[] =
{
	//DAC采样率
	0x0a, 0x06, 0x02, 0x01, 0x00, 0x0b, 0x0f, 0x0e, 0x0d, 0x0c
};

IAR_CONST u8 dac_pcm_buf[96] =
{
	//直推DAC测试数据（正弦波）,16khz 0DB
	0x00, 0x00, 0x00, 0x00, 0xFB, 0x30, 0xFC, 0x30, 0x81, 0x5A, 0x81, 0x5A, 0x40, 0x76, 0x41, 0x76,
	0xFF, 0x7F, 0xFF, 0x7F, 0x41, 0x76, 0x41, 0x76, 0x82, 0x5A, 0x81, 0x5A, 0xFB, 0x30, 0xFB, 0x30,
	0x00, 0x00, 0x00, 0x00, 0x05, 0xCF, 0x05, 0xCF, 0x7E, 0xA5, 0x7F, 0xA5, 0xBF, 0x89, 0xBF, 0x89,
	0x01, 0x80, 0x01, 0x80, 0xBF, 0x89, 0xBF, 0x89, 0x7E, 0xA5, 0x7F, 0xA5, 0x05, 0xCF, 0x05, 0xCF,
	0x00, 0x00, 0x00, 0x00, 0xFB, 0x30, 0xFB, 0x30, 0x82, 0x5A, 0x82, 0x5A, 0x40, 0x76, 0x41, 0x76,
	0xFF, 0x7F, 0xFE, 0x7F, 0x41, 0x76, 0x41, 0x76, 0x82, 0x5A, 0x82, 0x5A, 0xFB, 0x30, 0xFB, 0x30
};
#pragma constseg=default

//pcm数据直接推dac的初始化函数
#pragma location="DAC_INIT"
void dac_out_init(u8 spr_sf)
{
#if USE_MULTI_BIT_DAC
	at_write(DACCFG, 0x09);
#else
	at_write(DACCFG, 0x01);                     //dac digital enable
#endif
	at_write(DACSPR, dac_sprcon_sf[spr_sf]);    //dac sample rate
	AUCON0 = 0x06;                              //normal mode, sync with dac
	AUCON10 = 0x02;                             //OUTBUF,768byte, usbaudio只能用768和1280
}

//直推DAC测试
#pragma location="DAC_INIT"
void task_dac_test(void)
{
	printf("dac_out\n");
	dac_out_init(DAC_SPA_16K);
	user_set_volume(10);
#if DAC_FADE_EN
	dac_fade_in(1);
#endif
	WDTCON = 0;
	while(1)
	{
		u8 i;
		for(i=0; i<64;)
		{
			while(!(AUCON7 & BIT(7)));
			AUCON5 = dac_pcm_buf[i++];
			AUCON5 = dac_pcm_buf[i++];
		}
	}
}


//按键音表(必须要256byte对齐)
#pragma data_alignment=256
#pragma constseg="KEY_VOICE_TBL"
const u8 keyvoice_44100[] =
{
#if 1
	//3k
	0x00, 0x35, 0x61, 0x7B, 0x7F, 0x6C, 0x46, 0x13, 0xDD, 0xAD, 0x8C, 0x80, 0x8B, 0xAB, 0xDA, 0x10,
	0x44, 0x6B, 0x7E, 0x7B, 0x62, 0x38, 0x03, 0xCD, 0xA1, 0x86, 0x81, 0x93, 0xB8, 0xEA, 0x20, 0x51,
	0x73, 0x7F, 0x76, 0x57, 0x28, 0xF2, 0xBF, 0x97, 0x82, 0x84, 0x9C, 0xC6
#else
	//1k
	0x00, 0x12, 0x24, 0x35, 0x45, 0x54, 0x61, 0x6C, 0x74, 0x7B, 0x7F, 0x7F, 0x7F, 0x7B, 0x75, 0x6C,
	0x61, 0x54, 0x46, 0x36, 0x25, 0x13, 0x01, 0xEF, 0xDD, 0xCC, 0xBC, 0xAD, 0xA0, 0x95, 0x8C, 0x86,
	0x81, 0x80, 0x81, 0x85, 0x8B, 0x93, 0x9E, 0xAB, 0xB9, 0xC9, 0xDA, 0xEC
#endif
};
#pragma constseg=default

//按键音初始化
#pragma location="KEY_VOICE"
void key_voice_init(void)
{
	u16 keyv_tbl = (u16)keyvoice_44100;
	KVCCON = (keyv_tbl>>6) | 0x02;                   //key voice dma high address & enable MP3 play
	KVCADR = keyv_tbl;                               //key voice dma low start address
	KVCADR = keyv_tbl + sizeof(keyvoice_44100) - 1;  //end address
	KVCCON2 = (0x0a<<3)|0x02;                        //play cycle a & volume 0db/32
}
