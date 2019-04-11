/*****************************************************************************
* Module    : User
* File      : user_key.c
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : �û���������
*****************************************************************************/
#include "include.h"
#include "touch_key.h"
#include "ir.h"
#include "kedir.h"


extern IAR_CONST type_adc_key tbl_adkey[];
extern IAR_CONST type_adc_key tbl_adkey2[];
extern IAR_CONST type_adc_key tbl_ad_song[];
extern IAR_CONST type_adc_key tbl_ad_sel[];
extern bool vol_less_34V_blink;
extern bool alarm_flag;
extern u8 power_key_cnt;
extern bool Timer1_flag;
IAR_XDATA_A type_key_ctl key_ctl;
IAR_XDATA_A type_scan_ctl scan_ctl;

u8 vol_less_power_warning_flag =0;//�͵�ѹ��ʾ��ػ���־
u8 vol_less_33V_flag =0;         //��ѹ3.3v��־
u8 vol_less_34V_flag =0;         //��ѹ3.4v��־

#if (USE_MIXIO_AD || USE_ADKEY2)
//ʹ������һIO����ͨIO����AD����(USE_KEDIRΪ0ʱ��Ч��FPGA�Ͽ���)
#pragma location="KEY_INIT"
void mixio_adinit(void)
{
	//PIE05:P10; PIE03-P03; PIE02-P02; PIE00-P27;
#if (KEDIR_PORT == KEDIR_P10)
	P1DIR |= BIT(0);            //P10
	PIE0 &= ~(BIT(5));          //P10
#elif (KEDIR_PORT == KEDIR_P03)
	//P0DIR |= BIT(3);            //P03
	//PIE0 &= ~(BIT(3));          //P03
#elif (KEDIR_PORT == KEDIR_P02)
	P0DIR |= BIT(2);            //P02
	PIE0 &= ~(BIT(2));          //P02
#elif (KEDIR_PORT == KEDIR_P27)
	P2DIR |= BIT(7);            //P27
	PIE0 &= ~(BIT(0));          //P27
#else
#error "��ѡ��AD������IO"
#endif

	KEDIR_DISABLE_TOP();        //33K pull down resistance disable
#ifdef AX207X_TAG
	if(DCN == 0x00)
	{
		KEDIR_DISABLE_BIG_1();  //22k,2.2k pull up resistance disable
		KEDIR_DISABLE_TINY_1();
	}
	else
#endif
	{
		KEDIR_DISABLE_BIG();
		KEDIR_DISABLE_TINY();
	}
	LEDCON2 = BIT(2) | (KEDIR_PORT << 3);//select mix IO, AMUX enable.00:P10; 01:P03; 10:P02; 11:P27

	ADCCON = BIT(7) | BIT(3) | ADKEDIR_CHAN;
}
#endif

#if USE_ADKEY2
//�ڶ���AD������ʼ������
#pragma location="KEY_INIT"
void adkey2_init(void)
{
	mixio_adinit();  //����һ��Ϊ�ձ�ADKEY IO
}
#endif

#pragma location="KEY_INIT"
void iokey_init(void)
{
	S1_DIR |= BIT(S1_BIT);					// P03  IO KEY  ��ʼ��
	S2_DIR |= BIT(S2_BIT);					// P06
	S1_PU|= BIT(S1_BIT)|BIT(S2_BIT);		// ������
}

//������ʼ������
#pragma location="KEY_INIT"
void key_init(void)
{
	memset(&scan_ctl, 0, sizeof(scan_ctl));
	memset(&key_ctl, 0, sizeof(key_ctl));

	ADCBAUD = 0x24;
#if USE_KEDIR
	kedir_init();
#if (USE_KEDIR_TYPE != KEDIR_IR_LED)
	scan_ctl.kedir_ch = ADC_TINY;
	scan_ctl.adc_ch = ADC_KEDIR;
#else
	//����һʹ��IR & LED
	key_keidr_scan();
	ADCCON = BIT(3) | ADKEDIR_CHAN;
	KEDIR_ADCGO();
	scan_ctl.adc_ch = ADC_NULL;
#endif

#endif
#if 0    //use iokey   20170609  wq
    //P4DIR |= BIT(2) | BIT(3);	// P42, P43 input
	//PUP4 |= BIT(2) | BIT(3);	// P42, P3 enable pull-up

	P2DIR |= BIT(0) | BIT(1) | BIT(2) | BIT(7);	// P20, P21, P22, P27 input
	PUP2 |= BIT(0) | BIT(1) | BIT(2) | BIT(7);	// P20, P21, P22, P27 enable pull-up
#endif

#if USE_ADKEY
#if (USER_SEL == USER_AX1071_DEMO)
	P4DIR |= BIT(2);                        //P42
	AIPCON7 |= BIT(6);                      //P42��Ϊģ��
	ADCCON = BIT(7) | BIT(3) | ADKEDIR_CHAN; //ADC_CONV, ADC_EN;
	ADCMODE |= BIT(5);                      //P42 ADC
#else
	P0DIR |= BIT(1);                        //P01
	PIE0 &= ~(BIT(1));                      //P01
	ADCCON = BIT(7) | BIT(3) | ADKEDIR_CHAN; //ADC_CONV, ADC_EN;
#endif
	scan_ctl.adc_ch = ADC_KEY;
#endif

#ifdef AX207X_TAG
#if 0 //SOFT_POWEROFF_EN
	P0DIR |= BIT(1);                        //P01
	PIE0 &= ~(BIT(1));                      //P01
	ADCCON = BIT(7) | BIT(3) | ADKEY_POWER_CHAN; //ADC_CONV, ADC_EN;
	scan_ctl.adc_ch = ADC_KEYPOWER;
#endif
#endif

#if (BATTERY_CHECK || SOFT_POWEROFF_EN)
	AIPCON0 |= BIT(6);  //LDO EN
	AIPCON6 |= BIT(0);  //BAND GAP EN
	ADCCON = BIT(7) | BIT(3) | ADBAT_CHAN;
	scan_ctl.adc_ch = ADC_BAT;
#endif

#if USE_ADKEY2
	adkey2_init();
	scan_ctl.adc_ch = ADC_KEY2;
#endif

	iokey_init();	// P03 P06 ��ʼ��

	//P4DIR |= BIT(1) | BIT(0);			// P42, P43 input
	//AIPCON7 |= BIT(6) | BIT(7);			//P42 , P43 ��Ϊģ�⣻  BIT(0) : ʹ��GPIO

}
#pragma location="SCAN_COM"
u8 get_pwrkey_adcval(void)
{
     WATCHDOG_CLR();
     ADCCON = BIT(7) | BIT(3) | ADKEDIR_CHAN;
     while (ADCCON & BIT(7));  //�ȴ��ɼ����.
     return ADCDATAH;
}

//��ȡADC��ֵ
//#pragma inline=forced
#pragma location="SCAN_COM"
bool adc_get(void)
{
	if(ADCCON & BIT(7))
	{
		printf("adc_ch: %d\n",scan_ctl.adc_ch);
		ADC_CH_KEY2_NEXT();
		return false;
	}
	scan_ctl.adc_val = ADCDATAH;

#if USE_KEDIR
	LEDCON1 &= ~BIT(3);  //clear ADC sample fininsh pending
#endif

	return true;
}

//����ADKey��ⰴ��ֵ
#pragma location="SCAN_COM"
u8 kedir_get(void)
{
	u8 knum = scan_ctl.kedir_ch * kedir_group_num();
	u8 adc_val = scan_ctl.adc_val;

	while(adc_val > tbl_key1[knum].adc_val)
	{
		knum++;
	}
	return tbl_key1[knum].key_code;
}

#pragma location="SCAN_COM"
u8 adkey_get(void)
{
	u8 knum = 0;
	u8 adc_val = scan_ctl.adc_val;

	while(adc_val > tbl_adkey[knum].adc_val)
	{
		knum++;
	}
	return tbl_adkey[knum].key_code;
}


#pragma location="SCAN_COM"
u8 iokey_get(void)
{
    u8 key;

	if(!(S1_PORT & BIT(S1_BIT)))		//  P03
	{
		key = T_KEY_SET_LIGHT;
	}
	else if(!(S2_PORT & BIT(S2_BIT)))	//  P06
	{
		key = T_KEY_SET_TIMER;
	}
	else
	{
		key = NO_KEY;
	}
	return key;
}

#pragma location="SCAN_COM"
void OTHER_ADC_CHECK(void)
{
	u8 adc_val = scan_ctl.adc_val;
	if(adc_val < 0x20)			// no charger  0
	{
		sys_ctl.bat_charger_sta = 0;
		if(sys_ctl.bat_charger_cnt == 0)
			sys_ctl.bat_charger_cnt = 0xFF;

	}else if((adc_val < 0xA0) && (sys_ctl.bat_charger_sta != 1))	// chargering   1.67V  0x80
	{
		sys_ctl.bat_charger_sta = 1;
		sys_ctl.bat_charger_cnt = 0x00;
	}else if((adc_val > 0xB0) && (sys_ctl.bat_charger_sta != 2))	// chargered   2.5V  0xC1
	{
		sys_ctl.bat_charger_sta = 2;
		sys_ctl.bat_charger_cnt = 0x00;
	}
	//printf("%d ",sys_ctl.bat_charger_sta);
}


#pragma location="SCAN_COM"
u8 adkey2_get(void)
{
	u8 knum = 0;
	u8 adc_val = scan_ctl.adc_val;

	while(adc_val > tbl_adkey2[knum].adc_val)
	{
		knum++;
	}
	return tbl_adkey2[knum].key_code;
}

#if SOFT_POWEROFF_EN
//����ADC���
#pragma location="SCAN_COM"
u8 powerkey_get(void)
{
	u8 key;

	/*AX2070��AX1070������ģ�鲻ͬ��
	AX2070����ҪP01��AD��������ȡpower��״̬��
	AX1070��WKORTC����ֱ�ӻ�ȡpower��״̬��*/
#ifdef   AX207X_TAG	// for test only
	//uart_putc('#');
	//if(scan_ctl.adc_val > 80)
	P0DIR |= BIT(7);	// P07 input
	PUP0 |= BIT(7);		// P07 enable pull-up
	//PUP0 &= ~BIT(7);      //pull-down
	if((P0 & BIT(7)) == 0)	// :0 have key, :1 no press key
#else	// for AX1070 used by WKO
	if(irtc_read_cmd(CFG_RD) & BIT(1))
#endif
	{
		//uart_putc('.');

		poweroff_ctl.power_key_sta = POWERKEY_DOWN;
		 //key = T_KEY_SOFT_POWER;
		key = T_KEY_POWER;		// change by Merlin for power even   20170509
		if(poweroff_ctl.wait_powerkey_up == 0)
		{
			poweroff_ctl.wait_powerkey_up = 1;     //����ǰ���ǰ��ż���
		}
		//uart_putc('0'+poweroff_ctl.wait_powerkey_up);
	}
	else
	{
		poweroff_ctl.power_key_sta = POWERKEY_UP;
		poweroff_ctl.wait_powerkey_up = 2;         //�����ſ�
		key = NO_KEY;
	}
	if((poweroff_ctl.wait_powerkey_up == 1)        //��������������Ϣ
#if DCIN_DET_EN
	        || (poweroff_ctl.dc_in_sta == DC_UNSTABLE)  //������ͬ������DCIN������Ϣ
#endif
	  )
	{
		key = NO_KEY;
	}
	return key;
}
#endif

#pragma location="SCAN_COM"
u8 double_click_filter(u8 key_val)	//2MS
{	
	static u8 ms_2_cnt = 0;
	static u8 key_click_cnt = 0;
	
        if (ms_2_cnt > 0)
	{  //˫����ʱ��ʱ.
             ms_2_cnt--;
	        if(1 == ms_2_cnt)
		{
//		printf("kc=%d\n",key_click_cnt);
			if(key_click_cnt == 1)
			{
		          key_val = KU_LIGHT;
			}
			else
			{
				 if(key_click_cnt & 0x01)
				 {
		           // key_val = KU_PREV; // printf(",");
				   	key_val = KU_LED_OFF;  printf(".");
				 }
				 else
				 {
					//key_val = KU_LED_OFF;  printf(".");
				 }
			}
			key_click_cnt = 0;
			return key_val;
	        }
        }

        if(KU_LIGHT == key_val)
	{
		key_click_cnt++;
		ms_2_cnt = 200;  //��ʱ400ms,�ж�˫��
//		printf(".");
		return NO_KEY;
        }
   	return key_val;
}


//�����¼�
#pragma location="SCAN_COM"
u8 key_deal(u8 key_val)
{
	u8 key_return = NO_KEY;
	u8 key_return_val = NO_KEY;
	if(key_val == NO_KEY || key_val != key_ctl.key_val)
	{
		if(key_ctl.key_up < KEY_UP_TIMES)
		{
			key_ctl.key_up++;
		}
		else
		{
			if(key_ctl.key_cnt >= KEY_LONG_TIMES)                   //����̧��
			{
				key_return = key_ctl.key_val | T_KEY_LONG_UP;
			}
			else if(key_ctl.key_cnt >= KEY_SCAN_TIMES)              //�̰�̧��
			{
				key_return = key_ctl.key_val | T_KEY_SHORT_UP;
			}
			key_return_val = key_ctl.key_val;
			key_ctl.key_val = key_val;
			key_ctl.key_cnt = 0;
		}
	}
	else
	{
		key_ctl.key_cnt++;
		if(key_ctl.key_cnt == KEY_SCAN_TIMES)                       //ȥ��
		{
			key_ctl.key_up = 0;
			key_return = key_ctl.key_val;
		}
		else if(key_ctl.key_cnt == KEY_LONG_TIMES)                  //����
		{
			key_return = key_ctl.key_val | T_KEY_LONG;
		}
		else if(key_ctl.key_cnt == KEY_LONG_HOLD_TIMES)             //����
		{
			key_return = key_ctl.key_val | T_KEY_HOLD;
			key_ctl.key_cnt = KEY_LONG_TIMES;
		}
		key_return_val = key_ctl.key_val;
	}
	if((key_return_val & 0x60) && (key_return & 0x80))                  //32-127��Щ��ֵ��֧�ֳ���
	{
		return NO_KEY;
	}
#if USE_KEDIR
	if(!b_sys.kedir_flag)           //�ȴ�ϵͳ�ȶ�������Ӧ����һ������Ϣ
	{
		return NO_KEY;
	}
#endif
	return key_return;
}

//ɨ�谴��
#pragma optimize=2
#pragma location="SCAN_COM"
void key_scan(void)
{
	u8 key_val = NO_KEY;
//	static bool key_scan_flag = true;	// Merlin �Ƴ�, ԭ�������ﲻ�� AD����, û�г�ͻ����

//	if(key_scan_flag)
//	{
//		key_val = iokey_get();
//		if(key_val == NO_KEY)
//			key_scan_flag = false;
//	}
//	else
#if 0
	if(key_val == NO_KEY)
	{
		u8 key_val = NO_KEY;
		key_val = iokey_get();
		key_val = key_deal(key_val);
		if(key_val != NO_KEY)
		{
			//printf("%d ",key_val);
			if(key_val == 0x8E)
			{
				if(ocx.timeh < 23){
					ocx.timeh++;
				}else {
					ocx.timeh = 0;
				}
			}else if(key_val == 0x8F)
			{
				if(ocx.timel < 59){
					ocx.timel++;
				}else {
					ocx.timel = 0;
					ocx.timeh += 1;
				}
			}
			return;
		}
		key_val = NO_KEY;
	}
#endif

//	if(key_val == NO_KEY)
//	{
//
//		else
	if(adc_get())
	{
		//uart_putc('0'+scan_ctl.adc_ch);
		//printf("  %d_%d",scan_ctl.adc_ch,scan_ctl.adc_val);
		switch(scan_ctl.adc_ch)
		{
#if (USE_KEDIR && (USE_KEDIR_TYPE != KEDIR_IR_LED))
		case ADC_KEDIR:
			key_val = kedir_get();
			if(key_val == NO_KEY)
			{
#ifdef AX207X_TAG
				if(DCN == 0x00)
				{
					KEDIR_SCAN_BIG_1();
				}
				else
#endif
				{
					KEDIR_SCAN_BIG();
				}
				kedir_resume();
				ADC_CH_KEDIR_NEXT();
			}
			else
			{
				ADC_CH_KEDIR_KICK();
			}
			break;
#endif

#if USE_ADKEY
		case ADC_KEY:
			//if(ir_key_sta)
			//{
			//	break;
			//}
			printf("%d ",scan_ctl.adc_val);
			if(key_val == NO_KEY)
			{
				ADC_CH_KEY_NEXT();
			}
			else
			{
				ADC_CH_KEY_KICK();
			}
			break;
#endif

#if USE_ADKEY2
		case ADC_KEY2:
			OTHER_ADC_CHECK();
			//ADC_CH_KEY2_NEXT();
			scan_ctl.adc_ch = ADC_BAT;
            ADCCON = BIT(7) | BIT(3) | ADBAT_CHAN;
			break;
#endif

#if (BATTERY_CHECK || SOFT_POWEROFF_EN)
		case ADC_BAT:
			scan_ctl.bat_val = scan_ctl.adc_val;
			//ADC_CH_BAT_NEXT();
			scan_ctl.adc_ch = ADC_BAND_GAP;
            ADCCON = BIT(7) | BIT(3) | ADBAND_GAP_CHAN;
			break;
		case ADC_BAND_GAP:
			scan_ctl.band_gap_val = scan_ctl.adc_val;
			//ADC_CH_BAND_NEXT();
			scan_ctl.adc_ch = ADC_KEYPOWER;
            ADCCON = BIT(7) | BIT(3) | ADKEY_POWER_CHAN;
			task_battery_check();
			break;
#endif

#if USE_SDCLK_SDDET_PHONE_MUX
		case ADC_SD_DECT:
			scan_ctl.sddect_val = scan_ctl.adc_val;
			ADC_CH_SDDECT_NEXT();
			return;
#endif

#if (HP_DET_EN)// && DAC_BUF_EN)         //HPDECT(use vcmbuf)
		case ADC_HP_DECT:
			scan_ctl.hpdect_val = scan_ctl.adc_val;
		//	ADC_CH_HPDECT_NEXT();	// original
			ADC_CH_KEY_NEXT();
			return;
#endif

//#ifdef AX207X_TAG
#if SOFT_POWEROFF_EN
		case ADC_KEYPOWER:
			key_val = powerkey_get();

		//	uart_putc(key_val == NO_KEY ? 'm' : '*');
			if(key_val != NO_KEY)
			{
				//ADC_CH_BAND_NEXT();
				scan_ctl.adc_ch = ADC_KEYPOWER;
                ADCCON = BIT(7) | BIT(3) | ADKEY_POWER_CHAN;
			}
			else
			{
				scan_ctl.adc_ch = ADC_KEY2;
				ADCMODE &= ~BIT(5);
            	ADCCON = BIT(7) | BIT(3) | ADKEDIR_CHAN;
			}
			break;
#endif
//#endif

		}

	}

#if USE_IR
		u8 ir_val;
		if(key_val == NO_KEY || (scan_ctl.adc_val>0xE1 && scan_ctl.adc_val<0xF7))
		{
			ir_val = ir_process();
			if(ir_val != NO_KEY)
			{
				key_val = ir_val;
			}
		}
#endif

#if 1
	if(key_val == NO_KEY)
	{
		//printf(".");
		key_val = iokey_get();
	}
#endif

	key_val = key_deal(key_val);                            //������ֵ
	
	key_val = double_click_filter(key_val);

	if(key_val != NO_KEY)
	{
		printf("%02x_",key_val);
		put_msg(key_val);

		if((sys_ctl.bat_charger_sta != 0) && (key_val == KU_POWER))  // ��Դ������ʱ��ʾ10s����״̬����ָ����״̬
			sys_ctl.bat_charger_cnt = 10;

#if KEY_VOICE_EN
		if((key_val != KL_PLAY) && (key_val != KL_MODE) && (key_val != KL_EQ) && (key_val != KL_PLAYMODE))      //�޹��ܰ�������Ӧ������
		{
			if((key_val & 0x80) && !(key_val & 0x40))       //����ʱֻ��һ�ΰ�����
			{
				user_kick_kv();
			}
		}
#endif
	}
}

#if Emcode_Enable

// ���뿪�صķ�����ʽ
// 1. �����¼�����
// 2. �����¼�����
//
s8 SW1_WHIRL_cnt;	// A1B1 �ļ�����תֵ, ��ת�뷴ת��, �����¼� T_KEY_SW1_WHIRL ����
s8 SW2_WHIRL_cnt;	// A2B2 �ļ�����תֵ, ��ת�뷴ת��, �����¼� T_KEY_SW2_WHIRL ����
s8 SW3_WHIRL_cnt;	// A3B3 �ļ�����תֵ, ��ת�뷴ת��, �����¼� T_KEY_SW3_WHIRL ����

const s8 SW_AB_Whire_DIR[16] =
{//	�ۼ�ֵ,			// ����AB, �ϴ�AB
	Whire_None,		// 00, 00
	Whire_Right,	// 00, 01
Whire_None,//	Whire_Left,		// 00, 10		// 2����1
	Whire_None,		// 00, 11
	Whire_Left,		// 01, 00
	Whire_None,		// 01, 01
	Whire_None,		// 01, 10
Whire_None,//	Whire_Right,	// 01, 11
Whire_None,//	Whire_Right,	// 10, 00
	Whire_None,		// 10, 01
	Whire_None,		// 10, 10
	Whire_Left,		// 10, 11
	Whire_None,		// 11, 00
Whire_None,//	Whire_Left,		// 11, 01
	Whire_Right, 	// 11, 10
	Whire_None,		// 11, 11
//	 t-n		...  t   ... 		t+n
//	A 0001111110000000000001111111000
//	B 0000001111110000001111110000000
//	��0001001001001000000000000000000
//	��0000000000000000001001001000100
};

void KEY_Whire1(void)
{
	static u8 Key3;			// Bit32 B3A3����, Bit10 B3A3�ϴ�,

	u8 ABAB;

	
	Key3 &= 0x03;	// �����������
	if(P1 & BIT(3)) Key3 |= 0x08;	// ���±���B3
	if(P1 & BIT(2)) Key3 |= 0x04;	// ���±���A3

	// ���� B3A3����ת����
	ABAB = Key3 & 0x0f;		// ѡ����Ŀ
	if(SW_AB_Whire_DIR[ABAB] != Whire_None)
	{
		scan_ctl.disp_song_flag = true;
		scan_ctl.disp_cnt = Disp_Timer;
		
		SW3_WHIRL_cnt += SW_AB_Whire_DIR[ABAB];
		put_msg(T_KEY_SW3_WHIRL);
		printf("S3:%d\n",SW3_WHIRL_cnt);
	}

	// �������һ�η���ʹ��
	Key3 >>= 2;
}

void KEY_Whire2(void)
{
	static u8 Key21;		// Bit76 B2A2����, Bit54 B2A2�ϴ�,
							// Bit32 B1A1����, Bit10 B1A1�ϴ�,
	u8 ABAB;

	Key21 &= 0x33;	// �����������
	if(P1 & BIT(1)) Key21 |= 0x80;	// ���±���B2
	if(P1 & BIT(0)) Key21 |= 0x40;	// ���±���A2
	if(P0 & BIT(2)) Key21 |= 0x08;	// ���±���B1
	if(P0 & BIT(1)) Key21 |= 0x04;	// ���±���A1
	
	// ���� B1A1����ת����
	ABAB = Key21 & 0x0f;	// ѡ������
	if(SW_AB_Whire_DIR[ABAB] != Whire_None)
	{
		if(scan_ctl.disp_light_set_flag == 0){		// ����������ģʽʱ����
			scan_ctl.disp_volume_flag = true;
			scan_ctl.disp_cnt = Disp_Timer;
		}
		
		SW1_WHIRL_cnt += SW_AB_Whire_DIR[ABAB];
		put_msg(T_KEY_SW1_WHIRL);
		printf("S1%d\n",SW1_WHIRL_cnt);
	}

	// ���� B2A2����ת����
	ABAB = Key21 >> 4;		// ѡ��ʱ
	if(SW_AB_Whire_DIR[ABAB] != Whire_None)
	{
		if(scan_ctl.disp_clock_set_flag == 0){		// ������ʱ��ģʽʱ����
			scan_ctl.disp_timer_flag = true;
			scan_ctl.disp_cnt = Disp_Timer;
		}
		
		SW2_WHIRL_cnt += SW_AB_Whire_DIR[ABAB];
		put_msg(T_KEY_SW2_WHIRL);
		printf("S2:%d\n",SW2_WHIRL_cnt);
	}

	// �������һ�η���ʹ��
	Key21 >>= 2;
}


#endif

#if (BATTERY_CHECK || SOFT_POWEROFF_EN)
//��ص������
#pragma location="BAT_CHECK"
void task_battery_check(void)
{
	static u8  bat_count1 = 0,bat_count = 0;
	static u16 r = 1000,z = 0;
	static u8 bat_sta = VOL_INIT,bat_sta_bakeup = 0;

	if(scan_ctl.bat_val > z)
	{
		z = scan_ctl.bat_val;
	}
	if(scan_ctl.band_gap_val < r)
	{
		r = scan_ctl.band_gap_val;
	}
	bat_count++;
	if(bat_count >= 50)
	{
#ifdef AX207X_TAG
			if(z * 123 < r * VOL_AD_32V)		  		//С��3.2V	�ػ�     20180410
			{
				bat_sta = VOL_LESS_33V;
				vol_less_33V_flag = 1;
				vol_less_34V_flag = 1;
				ocx.led_sta = LED_STA_FAST_GLITTER;
			}
			else if(z * 123 < r * VOL_AD_34V)		  	//С��3.4V 	��ѹ����
			{
				bat_sta = VOL_LESS_36V;
				sys_ctl.bat_sta_flag = 1;
				if(sys_ctl.alarm_cnt == 0xFF)
				{
					sys_ctl.alarm_cnt = 0x1E;     		// 30s
				}
			}

#endif

#ifdef AX107X_TAG
			if(z * 123 < r * VOL_AD_30V)		  		//С��3.2V   �ػ�	20180410
			{
				bat_sta = VOL_LESS_33V;
				vol_less_33V_flag = 1;
				vol_less_34V_flag = 1;
				ocx.led_sta = LED_STA_FAST_GLITTER;
			}else if(z * 123 < r * VOL_AD_32V)		  	//С��3.4V	 ��ѹ����
			{
				bat_sta = VOL_LESS_36V;
				sys_ctl.bat_sta_flag = 1;
				if(sys_ctl.alarm_cnt == 0xFF)
				{
					sys_ctl.alarm_cnt = 0x1E;     		// 30s
				}
			}
#endif

		//	else if(z * 123 < r * VOL_AD_32V)		  //С��3.2V
		//	{
		//		bat_sta = VOL_LESS_32V;
		//	}
#if SOFT_POWEROFF_EN
			else if(z * 123 > r * VOL_AD_44V)	 //����4.4V
			{
				bat_sta = VOL_GREATER_44V;
			}
#endif
			else //����3.6v
			{
				bat_sta = VOL_OTHER;
				sys_ctl.alarm_cnt = 0xFF;
				sys_ctl.bat_sta_flag = 0;
				if(vol_less_34V_flag ==1)//����1���ǵ�ѹ����3.1v����
				{
					if(b_msc.pause == 1)
					{
						ocx.led_sta = LED_STA_SLOW_GLITTER;
					}
					else
					{
						ocx.led_sta = LED_STA_ON;
					}

				}
				vol_less_34V_flag = 0;
				vol_less_33V_flag = 0;
				vol_less_power_warning_flag=0;
				vol_less_34V_blink =0;

			}

			r = 1000;
			z = 0;
			bat_count = 0;

			if(bat_sta_bakeup != bat_sta)
			{
				bat_sta_bakeup = bat_sta;
				bat_count1 = 0;
			}
			else
			{
				bat_count1++;
			}
#if SOFT_POWEROFF_EN
			if(sys_ctl.bat_sta == VOL_INIT) 	//�ϵ��һ��
			{
				sys_ctl.bat_sta = bat_sta;
				return;
			}
#endif
			if(bat_count1 >= 5)
			{
				bat_count1 = 0;
				sys_ctl.bat_sta = bat_sta;
			}
		//	printf("bat_sta=%d\n",sys_ctl.bat_sta);
#if TASK_POWEROFF_EN
			if(sys_ctl.bat_sta == VOL_LESS_33V)
			{
				if(vol_less_power_warning_flag==1)
				{
					if(task_ctl.work_sta != TASK_POWEROFF)
					{
						task_ctl.work_sta = TASK_POWEROFF;	 //�͵�ػ�
						sys_ctl.user_power_down_flag = 1;
						printf("�͵�ػ�.\n");

					}
				}

			}
#endif

	}
}
#endif
