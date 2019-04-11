#include "include.h"
#include "kedir.h"
#include "ir.h"



#if USE_KEDIR


#if (!USE_IR || USE_HW_IR)

#pragma optimize=no_inline
bool kedir_pause(void)
{
	LED_PAUSE = 1;
	return LED_PAUSE & 0x01;
}

#pragma optimize=no_inline
// !!! 此函數若過於復雜, 請return 2 !!!
void kedir_resume(void)
{
	LED_PAUSE = 0;
}

#else //!USE_HW_IR
#if USE_KEDIR && ((USE_KEDIR_TYPE == KEDIR_KEY_IR_LED) || (USE_KEDIR_TYPE == KEDIR_KEY_IR))
#pragma optimize=no_inline
bool kedir_pause(void)
{
	bool tmp;
	tmp = ir_state;
	return tmp;
}

#pragma optimize=no_inline
// !!! 此函數若過於復雜, 請return 2 !!!
void kedir_resume(void)
{

}
#else
#pragma optimize=no_inline
bool kedir_pause(void)
{
	bool tmp;
	//KEDIR_LED_ON();
	IE_EA = 0;
	tmp = (ir_state & 0x3F) == 0;
	if(tmp)
		ir_state |= 0x80;
	IE_EA = 1;
	return tmp;
}

#pragma optimize=no_inline
// !!! 此函數若過於復雜, 請return 2 !!!
void kedir_resume(void)
{
	//KEDIR_LED_OFF();
	ir_state &= ~0x80;
}
#endif
#endif


#pragma location="SCAN_COM"
void key_keidr_scan(void)
{
#if 1 //USE_TOPKEY
	if(kedir_pause())
	{
		scan_ctl.kedir_ch = ADC_TOP;
		KEDIR_ENABLE_EOC();
#if ((USE_KEDIR_TYPE == KEDIR_KEY_IR_LED) || (USE_KEDIR_TYPE == KEDIR_KEY_LED)|| (USE_KEDIR_TYPE == KEDIR_IR_LED))
#ifdef AX207X_TAG
		if(DCN == 0x00)
		{
			KEDIR_SCAN_TOPB_1();
		}
		else
#endif
		{
			KEDIR_SCAN_TOPB();
		}
#else
#ifdef AX207X_TAG
		if(DCN == 0x00)
		{
			KEDIR_SCAN_TOPO_1();
		}
		else
#endif
		{
			KEDIR_SCAN_TOPO();
		}
#endif
	}
	else
#endif
	{
		scan_ctl.kedir_ch = ADC_BIG;
		KEDIR_ENABLE_EOC();
	}
}

void kedir_init(void)
{
	KEDIR_PDIR &= ~BIT(KEDIR_BIT);
	KEDIR_P &= ~BIT(KEDIR_BIT);

	LEDCYCLE = 0x31; //20k
	LEDADC   = 5;
	LEDHIGH  = 0x18; //high pulse
	ADCBAUD = 11;                            //KEDIR要求速度很快,但等于2时会影响ADKEY
	ADCCON = BIT(3) | ADKEDIR_CHAN;         //ADC_CONV, ADC_EN;
	LEDCON2 = 0x04 | (KEDIR_PORT << 3);     //mix IO, AMUX enable

	LEDTRIM &= ~(BIT(6) | BIT(5) | BIT(4));
#if SRAM_DEBUG
	LEDTRIM |= (LED_TRIM & 0x70);
#else
	LEDTRIM |= (*(u8 __code *)(0x7ff7) & 0x70);
#endif

	KEDIR_DISABLE_TOP();
#ifdef AX207X_TAG
	if(DCN == 0x00)
	{
		KEDIR_DISABLE_BIG_1();
		KEDIR_DISABLE_TINY_1();
	}
	else
#endif
	{
		KEDIR_DISABLE_BIG();
		KEDIR_DISABLE_TINY();
	}

	KEDIR_ENABLE_EOC();
	asm("setb 0xBA");
	IE_TM1 = 1;                             // EOC與Timer1復用中斷
	KEDIR_ADCGO();

	KEDIR_CLEAR();
	KEDIR_ENABLE_KEY();
#if ((USE_KEDIR_TYPE == KEDIR_KEY_IR_LED) || (USE_KEDIR_TYPE == KEDIR_KEY_LED) || (USE_KEDIR_TYPE == KEDIR_IR_LED))
	KEDIR_ENABLE_LED();
#else
	KEDIR_LED_OFF();
#endif

}

#endif

#if USE_KEDIR || USE_IR
#pragma vector = VECTOR_OFFSET(TIMER1_INT)
#pragma register_bank=2
#pragma location="INTERRUPT"
__interrupt void timer1_kedir_isr(void)
{
	ISR_ENTER();

#if USE_IR
#if USE_HW_IR
	if(IRCON & BIT(2))
	{
		IRCON &= ~BIT(2);
		ir_key_sta = 1;
		if(IRCON & BIT(3))
		{
			if(repeat_cnt<255)   //重复码
			{
				repeat_cnt++;
			}
		}
		else
		{
			repeat_cnt = 0;
		}

		if(repeat_cnt != 1)      //接收到第一个重复码不读取数据
		{
			ir_cnt = 0;
			ir_data = IRDAT2;
			ir_code = IRDAT1;
			u8 temp = IRDAT0;
			ir_code = (ir_code<<8) | temp;
		}
		else if(ir_cnt == IR_TIMEOUT_CNT)   //当抬起一个键马上按下另外一个键时，若全码数据错误，后面的重复码全部不处理
		{
			repeat_cnt = 0;
		}
	}
#else
	if(TMR1CON1 & 0xc0)
	{
		ir_cap_isr();
	}
#endif
#endif


#if USE_KEDIR
	if(LEDCON1 & BIT(3))    //ADC sample finish pending
	{
		LEDCON1 &= ~BIT(3);

#if SRAM_DEBUG
		scan_ctl.kedir_ch = kedir_process_sram(scan_ctl.kedir_ch);
#else
		scan_ctl.kedir_ch = kedir_process(scan_ctl.kedir_ch);
#ifdef AX107X_TAG
#if ((USE_KEDIR_TYPE == KEDIR_KEY)||(USE_KEDIR_TYPE == KEDIR_KEY_IR))
		if(scan_ctl.kedir_ch == ADC_BIG)
		{
			KEDIR_ENABLE_BIG();
		}
#endif
#endif
#endif
	}
#endif

	ISR_EXIT();
}
#endif
