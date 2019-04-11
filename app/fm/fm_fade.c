/*****************************************************************************
 * Module    : Module
 * File      : fm_fade.c
 * Author    :
 * Email     :
 * Function  : FM AMUX���뵭���������
 *****************************************************************************/
#include "include.h"
#include "fm_fade.h"

#ifdef AX107X_TAG//FM_AMUXʡ���ݹ�����ʱֻ֧��107X
#if AMUX_FM_WITH_NO_CAP

bool trim_dir_l,trim_dir_r;
u8 dac_trim_trimll,dac_trim_trimlh;
u8 dac_trim_trimrl,dac_trim_trimrh;


//��ȡtrim�ķ���
#pragma location="FM_FADE"
bool get_trim_dir(u8 dac_ch)
{
	u16 fm_trim_val,trim_init_val;

	if(dac_ch)
	{
		fm_trim_val = dac_trim_trimlh;
		fm_trim_val = (fm_trim_val << 8) + dac_trim_trimll + 0x8000;

		trim_init_val = trim_val.lh;
		trim_init_val = (trim_init_val << 8) + trim_val.ll + 0x8000;
	}
	else
	{
		fm_trim_val = dac_trim_trimrh;
		fm_trim_val = (fm_trim_val << 8) + dac_trim_trimrl + 0x8000;

		trim_init_val = trim_val.rh;
		trim_init_val = (trim_init_val << 8) + trim_val.rl + 0x8000;
	}

	if(fm_trim_val > trim_init_val)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#pragma location="FM_FADE"
void wait_trim_done(bool trim_dir)
{
	if(trim_dir)
	{
		at_write(TRIMCON1, 0x0b);   //decrease 1 step 1 sample
	}
	else
	{
		at_write(TRIMCON1, 0x0f);   //increase 1 step 1 sample
	}
	at_write(TRIMCON2, 0x01);       //kick
	while(!(at_read(TRIMCON2)&BIT(1)))
	{
		WATCHDOG_CLR();
	}
}

//A��ʱ����DAC��B��ʱ������
#pragma location="FM_FADE"
void trim_A_to_B(void)
{
	/*�ȵ��������������������ֲ���*/
	at_write(TRREGLL, dac_trim_trimll);
	at_write(TRREGLH, dac_trim_trimlh);
	at_write(TRREGRL, trim_val.rl);
	at_write(TRREGRH, trim_val.rh);
	wait_trim_done((!trim_dir_l));

	/*�ٵ��������������������ֲ���*/
	at_write(TRREGLL, dac_trim_trimll);
	at_write(TRREGLH, dac_trim_trimlh);
	at_write(TRREGRL, dac_trim_trimrl);
	at_write(TRREGRH, dac_trim_trimrh);
	wait_trim_done((!trim_dir_r));
}

//A��ʱ����DAC��B��ʱ������
#pragma location="FM_FADE"
void trim_B_to_A(void)
{
	/*�ȵ��������������������ֲ���*/
	at_write(TRREGLL, trim_val.ll);
	at_write(TRREGLH, trim_val.lh);
	at_write(TRREGRL, dac_trim_trimrl);
	at_write(TRREGRH, dac_trim_trimrh);
	wait_trim_done(trim_dir_l);

	/*�ٵ��������������������ֲ���*/
	at_write(TRREGLL, trim_val.ll);
	at_write(TRREGLH, trim_val.lh);
	at_write(TRREGRL, trim_val.rl);
	at_write(TRREGRH, trim_val.rh);
	wait_trim_done(trim_dir_r);
}


#pragma location="FM_FADE"
void dac_trim_process(void)
{
	static u8 first_in_flag = 0;
	/***************��ʼ�����trimֵA********************/
	if(first_in_flag == 0)   //��һ�ν���FM����ȡ��ʼtrimֵ
	{
		trim_val.ll = at_read(TRREGLL);
		trim_val.lh = at_read(TRREGLH);
		trim_val.rl = at_read(TRREGRL);
		trim_val.rh = at_read(TRREGRH);
	}

	/***************FM�������trimֵB********************/
	fm_rx_set_vol(0);    //muteסFM
	DACACON4 &= ~BIT(1); //select 2 channel
	DACACON1 = 0xDE;
	DACACON2 = 0xF3;
	DACACON4 |= BIT(5);
	if(first_in_flag == 0)   //��һ�ν���FM,��ģ��trim�����һ�ȡtrim����
	{
		DACACON1 |= BIT(6); //enable DAC
		DACACON4 &= ~BIT(5);
		delay_5ms(1);
		WATCHDOG_CLR();
		at_write(TRIMCON1, 0x05);  //decrease 1 step 1 sample
		at_write(TRIMCON2, 0x01);  //kick start
		while(!(at_read(TRIMCON2)&BIT(1)))
		{
			WATCHDOG_CLR();
		}

		DACACON1 &= ~BIT(4);
		dac_trim_trimll = at_read(TRREGLL);
		dac_trim_trimlh = at_read(TRREGLH);
		dac_trim_trimrl = at_read(TRREGRL);
		dac_trim_trimrh = at_read(TRREGRH);
		trim_dir_l = get_trim_dir(1);           //��ȡ��������trim����
		trim_dir_r = get_trim_dir(0);           //��ȡ��������trim����

	}
	else   //�ڶ��ν���FM����ָ��ֵtrim
	{
		DAC_ANL_EN(); //enable DAC
		DACACON4 &= ~BIT(5);
		delay_5ms(1);
		trim_A_to_B();
	}
	first_in_flag = 1;
}

#pragma location="FM_FADE"
void fm_dac_fade_in(void)
{
	dac_fade_in(0);
	delay_5ms(40);
	trim_B_to_A();
	delay_5ms(1);
	DACACON4 |= BIT(5);
	delay_5ms(1);
	DAC_ANL_DIS(); //disable DAC
}

#pragma location="FM_FADE"
void fm_dac_fade_out(void)
{
	DAC_ANL_EN(); //enable DAC
	delay_5ms(1);
	DACACON4 &= ~BIT(5);
	delay_5ms(1);
	trim_A_to_B();
	delay_5ms(1);
	dac_fade_out();
	dac_fade_wait();
}
#endif  /*END AMUX_FM_WITH_NO_CAP*/
#endif


