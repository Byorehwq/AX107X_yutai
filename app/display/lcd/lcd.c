/*--------------------------------------------------------------------------*/
/**@file lcd.C
   @brief �������ĸ�����ʾ����
   @details ��������ʾ��ʹ�õĺ���
   @author LZZ
   @date   2010-08-17
   @note   none
*/
/*----------------------------------------------------------------------------*/
#include "include.h"
#include "lcd.h"
#include "tff.h"
#include "MP3_API.h"
#include "deal_msg.h"
#include "rtc.h"
#include "irtcc.h"

#define   FILE_NAME_SIZE  100


extern __root unsigned char const  AsciiTable[];
extern __root const  u8 SD_CARD[32];
extern __root const  u8 USB_PHOTO[32];
extern __root const  u8 SPEAKER[32];
extern __root const  u8 SINGLE[44];
extern __root const  u8 ALL[44];
extern __root const  u8 _RANDOM[44];
extern __root const  u8 FOLD[44];
extern __root const  u8 LINE[144];
extern __root const u8 LINE_9632[80];
extern __root const  u8 NODEVICE[96];
extern __root const u8 ANTENNA[20];
extern __root const u8 DIR_ICON[32];
extern __root const u8 __code *const PlayModeIcon[];

//#pragma constseg=LCD_ICON
const  u8 numberTable1[] = "0123456789";

const  u8 EQ_BUFF[6][8] = {{"NORMAL"},{"POP"},{"ROCK"},{"JAZZ"},{"CLASSIC"},{"COUNTRY"}};
const  u8 EQ_POS_BUFF[6] = {48,24,32,32,56,56};  //С������ĳ���
const  u8 PLAY_MODE_BUFF[4][7] = {{"ALL"},{"RANDOM"},{"SINGLE"},{"FOLDER"}};
const  u8 PLAY_MODE_POS[3][4] = {{40,16,16,16},
	{40,16+8,16+8,16+8},
	{40,16,16,16}
};
//#include "icon.c"


//#pragma constseg=default
/*----------------------------------------------------------------------------*/
/**@brief ��ʾ��ʼ��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void LCDInit(void)@"LCD_INIT"
{
#ifdef LED_5C7S_EN
	if(g_LCDType == LED_5com7seg)
	{
		LED_5com7seg_Init();
	}
#endif

#ifdef LCD_SERIAL_EN
	if(g_LCDType == LCD_12832_11PIN)
	{
		LCDInit_0();           //������
		LCD_BACKLIGHT_INIT();
	}
#endif

#ifdef LCD_SEG_EN
	if(g_LCDType == LCD_seg39)
	{
		SEG_PORT_INIT();
		LCD_BACKLIGHT_INIT();
	}
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief ��LCD����
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void LCD_BACKLIGHT_ON(void)@"LCD_BANK_COM"    //������:P15��������
{
#ifdef AX2021_48PIN
#ifdef LCD_SERIAL_EN
	P1DIR |= BIT(5);
	PWKEDGE |= BIT(6);
	PDN1 |= BIT(5);
	Flag_LCD_Backlight = 1;
	LCD_Backlight_count = 0;

	//deg("LCD BLACK ON.\n");
#endif
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief ��LCD����
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void LCD_BACKLIGHT_OFF(void)@"LCD_BANK_COM" //�ر���:P15���1,������
{
#ifdef AX2021_48PIN
#ifdef LCD_SERIAL_EN
	PDN1 |= BIT(5);
	PWKEDGE &= ~BIT(6);
	P1 |= BIT(5);
	P1DIR &= ~BIT(5);
	Flag_LCD_Backlight = 0;

	//deg("LCD BLACK OFF.\n");
#endif
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʼ��
   @param ��
   @return ��
   @note �������г�ʼ��������IO�ڣ����ͳ�ʼ������
*/
/*----------------------------------------------------------------------------*/
void LCDInit_0(void)@"LCD_INIT"
{
	PORT_INIT();

	P1DIR &= ~(1<<LCD_RES);   //P12
	SET_LCD_RES;
	delay_LCD(20);
	CLR_LCD_RES;
	delay_LCD(250);
	delay_LCD(250);
	SET_LCD_RES;
	delay_LCD(20);

	SPI_CONFIG();

	LCDSendCmdForIni(LCD_BIAS_SET | 0x00);    //lcd bias setting
	LCDSendCmdForIni(LCD_ADC_SEL | 0x00);     //ADC	 invert display  //0x00 or 0x01
	LCDSendCmdForIni(LCD_COMMON_MODE & 0xc8); //common output mode //0xc0 or oxc8
	LCDSendCmdForIni(LCD_V0_RATIO | 0x03);    //Resistor Ratio
	LCDSendCmdForIni(LCD_ELE_VOLUME);         //electronic Volume
	LCDSendCmdForIni(0x42);                   //line start
	LCDSendCmdForIni(LCD_POWER_MODE);         //lcd power control

	LCDSendCmdForIni(LCD_ON);
	LCD_clear();   //clear lcd buff
}

/*----------------------------------------------------------------------------*/
/**@brief ���������������
   @param cmd ��������
   @return ��
   @note �ڳ�ʼ������ʱ�����ڷ�������
*/
/*----------------------------------------------------------------------------*/
void LCDSendCmdForIni(u8 cmd)@"LCD_BANK_COM"
{
	CLR_LCD_A0;    //LCD_COMMAND
	CLR_LCD_CS;
	asm("nop");
	SPI0BUF = cmd;
	while(!(SPI0CON & 0x80));  //�ȴ��������
	SET_LCD_CS;
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʱ����
   @param n��n�Լ���0������
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void delay_LCD(u8 n)@"LCD_BANK_COM"
{
	while(n--)
		asm("nop");
}

/*----------------------------------------------------------------------------*/
/**@brief ��������������
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void LCD_clear(void)@"LCD_BANK_COM"
{
	u8 i,j;
	for(i = 0; i < PAGE; i++)
	{
		for(j = 0; j < COLUMN; j++)
		{
			LCD_Buff[i][j] =0x00;
		}
	}
}

/*----------------------------------------------------------------------------*/
/**@brief SPI�жϺ��������ڵ�������ʾ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma vector = VECTOR_OFFSET(SPI0_INT)
#pragma register_bank=1
#pragma location="INTERRUPT"
__interrupt void spi_isr(void)
{
	u8 x = 0;

	ISR_ENTER();

	SET_LCD_CS;

	if(pagenum >= 4)
	{
		DIS_SPI_IE;
		pagenum = 0;

		ISR_EXIT();
		return;
	}

	x += 2;

	CLR_LCD_A0;    //LCD_COMMAND
	CLR_LCD_CS;
	asm("nop");
	SPI0BUF = LCD_COLUMN_HIGH | (x >> 4);
	while(!(SPI0CON & 0x80));  //�ȴ��������
	SPI0BUF = LCD_COLUMN_LOW | (x & 0x0f);
	while(!(SPI0CON & 0x80));  //�ȴ��������
	SPI0BUF = LCD_PAGE_ADDR | pagenum;
	while(!(SPI0CON & 0x80));  //�ȴ��������

	SET_LCD_CS;
	SET_LCD_A0;
	asm("nop");
	CLR_LCD_CS;

	SPIDMAPTRH = (u8)(((u16)&LCD_Buff[pagenum][0]) >> 8);//DMA load start address
	SPIDMAPTRL = (u8)(((u16)&LCD_Buff[pagenum][0]) >> 0);

	SPIDMACNT = (COLUMN/2) - 1;
	pagenum++;
	while(!(SPI0CON & 0x80));  //�ȴ��������

	ISR_EXIT();
}

/*----------------------------------------------------------------------------*/
/**@brief ����������Ŵ���������
   @param char_code ��Ҫ�Ŵ���ַ�
   @param code_H �Ŵ���ַ��ĸ��ֽ�
   @param code_L �Ŵ���ַ��ĵ��ֽ�
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void FontDouble(u8 char_code, u8 *code_H, u8 *code_L)@"LCD_API_1"
{
	unsigned char n;

	for(n = 0; n < 4; n++)
	{
		*code_H <<= 2;
		*code_L <<= 2;

		if(char_code & 0x80)
			*code_H |= 0x03;
		if(char_code & 0x08)
			*code_L |= 0x03;

		char_code <<= 1;
	}
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ�ַ�����
   @param char_code ��Ҫ��ʾ���ַ���
   @param size �ַ���С��0Ϊԭʼ��С������Ϊ������С
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void PutChar(unsigned char char_code,unsigned char size)@"LCD_BANK_COM"
{
	unsigned char i,code_H,code_L;
	u16 n;

	if((char_code > 127)||(char_code < 32))   // �Ƿ��ַ���'-'���
	{
		char_code = '-';
	}

	char_code = char_code - ' ';    //���е�һ���ǿո��

	if(size == 0)        //ԭʼ��С
	{
		for(i = 0; i < 8; i++)
		{
			n = (u16)(16*char_code +i+i);

			LCD_Buff[LCD_Page][LCD_Column + i]   = AsciiTable[n];

			n = (u16)(16*char_code +i+i+1);

			LCD_Buff[LCD_Page+1][LCD_Column + i] = AsciiTable[n];
		}
		LCD_Column += 8;      //����������һ����ĸ
	}
	else                 //2����С
	{
		for(i = 0; i < 16; i+=2)
		{
			n = (u16)(16*char_code +i);

			FontDouble(AsciiTable[n],&code_H,&code_L);
			LCD_Buff[LCD_Page][LCD_Column + i]   = code_L;
			LCD_Buff[LCD_Page][LCD_Column + i+1] = code_L;
			LCD_Buff[LCD_Page + 1][LCD_Column + i]   = code_H;
			LCD_Buff[LCD_Page + 1][LCD_Column + i+1] = code_H;

			n = (u16)(16*char_code +i+1);

			FontDouble(AsciiTable[n],&code_H,&code_L);
			LCD_Buff[LCD_Page + 2][LCD_Column + i]   = code_L;
			LCD_Buff[LCD_Page + 2][LCD_Column + i+1] = code_L;
			LCD_Buff[LCD_Page + 3][LCD_Column + i]   = code_H;
			LCD_Buff[LCD_Page + 3][LCD_Column + i+1] = code_H;
		}
		LCD_Column += 16;   //����������һ����ĸ
	}

}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ�ַ�������
   @param str ��Ҫ��ʾ���ַ���
   @param size���ַ���С��0Ϊԭʼ��С������Ϊ������С
   @return ��
   @note ������ʾ�ַ�����������char __code*���͵��ַ���
*/
/*----------------------------------------------------------------------------*/
void PutS(char __code *str,unsigned char size)@"LCD_API_1"
{
	while(*str>0)
	{
		if(LCD_Column + 8 > 128)
			return;
		PutChar(*str,size);
		str++;
	}
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ�ַ�������
   @param str ��Ҫ��ʾ���ַ���
   @param size �ַ���С��0Ϊԭʼ��С������Ϊ������С
   @return ��
   @note һ����ַ�����ʾ����
*/
/*----------------------------------------------------------------------------*/
void PutS_2(u8 *str,unsigned char size)@"LCD_API_1"
{
	while(*str>0)
	{
		if(LCD_Column + 8 > 128)  //ֻ����С���֣���������û��Լ�����
			return;
		PutChar(*str,size);
		str++;
	}
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ����������Ҫ��ʾ�������ֽڵ�ʮ��������ת�����ĸ��ֽ�ʮ���Ƶ�����LCD��ʾ�ĵ���
   @param input �����ֽڵ�ʮ��������
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
u32 pattern(unsigned short input)@"LCD_API_1"
{
#ifdef  AX2021_DEF
	u8 a,b,c,d;
	u32 value;

	a = input/1000;
	b = input/100 - 10*a;
	c = input/10 - 10*(input/100);
	d = input - 10*(input/10);

	a = numberTable1[a];
	b = numberTable1[b];
	c = numberTable1[c];
	d = numberTable1[d];

	value = a;
	value <<= 8;
	value = value + b;
	value <<= 8;
	value = value + c;
	value <<= 8;
	value = value + d;

	return value;
#endif

#ifdef AX2030_DEF
	extern u32 pattern_mask(unsigned short input);
	u32 value;
	value = pattern_mask(input);
	return value;
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ����������������ʾ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_VOL(void)@"LCD_API_1"
{
	unsigned short value;
	value = pattern(volume);

	LCDBufSetPos(16,0);
	PutS("VOL:",1);
	PutChar(value >> 8,1);
	PutChar(value,1);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ��VOLUP��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_VOLUP(void)@"LCD_API_1"
{
	LCDBufSetPos(24,0);
	PutS("VOLUP",1);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ��VOLDOWN��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_VOLDOWN(void)@"LCD_API_1"
{
	LCDBufSetPos(8,0);
	PutS("VOLDOWN",1);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾRTCʱ��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_RTC_Time()@"rtc_lcd"
{

#ifdef RTCMODE_EN
	unsigned short value,year_h,year_l;
	if(Alarm_Rtc_Flag)
	{
		year_h=Alarmdata.year/100;
		year_l=Alarmdata.year%100;
	}
	else
	{
		year_h=RTCdate.year/100;
		year_l=RTCdate.year%100;
	}

	LCD_Page = 0;
	LCD_Column = 24;
	value = pattern(year_h);
	PutChar(value>>8,0);
	PutChar(value,0);
	value = pattern(year_l);
	PutChar(value>>8,0);
	PutChar(value,0);
	PutChar('-',0);
	if(Alarm_Rtc_Flag)
		value = pattern(Alarmdata.month);
	else
		value = pattern(RTCdate.month);
	PutChar(value>>8,0);
	PutChar(value,0);
	PutChar('-',0);
	if(Alarm_Rtc_Flag)
		value = pattern(Alarmdata.day);
	else
		value = pattern(RTCdate.day);
	PutChar(value>>8,0);
	PutChar(value,0);
	LCD_Page = 2;
	LCD_Column = 28;
	if(Alarm_Rtc_Flag)
		value = pattern(Alarmdata.hour);
	else
		value = pattern(RTCdate.hour);
	PutChar(value>>8,0);
	PutChar(value,0);
	PutChar(':',0);
	if(Alarm_Rtc_Flag)
		value = pattern(Alarmdata.minute);
	else
		value = pattern(RTCdate.minute);
	PutChar(value>>8,0);
	PutChar(value,0);
	PutChar(':',0);
	if(Alarm_Rtc_Flag)
		value = pattern(Alarmdata.second);
	else
		value = pattern(RTCdate.second);
	PutChar(value>>8,0);
	PutChar(value,0);

	SetRTCTime();
#endif
}

#define  FRE_POT  16
/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾƵ��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_FRE(void)@"fm_lcd"
{
	u32 value;

	LCD_clear();

	value = pattern(fmFrequency);

	LCDBufSetPos(FRE_POT + 3*16,0);
	PutChar('.',1);
	LCD_Column = FRE_POT+0;
	if((fmFrequency/1000) > 0)
		PutChar(value >> 24,1);

	LCD_Column = FRE_POT+16;

	PutChar(value >> 16,1);
	PutChar(value >> 8,1);
	LCD_Column = FRE_POT + 3*16 + 8;
	PutChar(value,1);

	LCDBufSetPos(FRE_POT+3*16 + 8 + 16,2);
	PutS("MHz",0);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ���������
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_Input_Number(void)@"LCD_API_1"
{
	u32 value;

	value = pattern(inputNum);

	LCD_Column = (128-64)/2;
	LCD_Page = 0;

	PutChar(value >> 24,1);
	PutChar(value >> 16,1);
	PutChar(value >> 8,1);
	PutChar(value,1);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ������PLAYģʽ�µĲ˵���ʾ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void MENU_Play(void)@"LCD_API_1"
{
	DispSmallDevice();
	DispPlayTime();
	DispSmallPlayMode();
	DispAllTime();
	DispFileName();
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ����ģʽ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_Play_Mode(void)@"LCD_API_1"
{
	char __code *p;

	LCD_Page = 0;
	p = (char __code *) &PLAY_MODE_BUFF[(u8)playMode][0];

	LCD_Page = 0;
	LCD_Column = PLAY_MODE_POS[DotMatrix_LCDType][playMode];
	PutS(p,1);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ�ļ����֣��ļ���ţ��ļ�������Ϣ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_Next(void)@"LCD_API_1"
{
	u32 value;
	u8 i=0;

	LCDBufSetPos(6,0);

	FillRegister(16,DIR_ICON,0);

	LCDBufSetPos(26,0);

	if(fs_apiinfo.dirDeep == 0)
		PutS("ROOT",0);
	else
	{
		for(i=0; i<8; i++)
		{
			if(read_dirname[i] != 0x20)//(space)
				PutChar(read_dirname[i],0);
		}
		if((read_dirname[8]!=0x20) || (read_dirname[9]!=0x20) || (read_dirname[10]!=0x20))
		{
			PutS(".",0);
			PutChar(read_dirname[8],0);
			PutChar(read_dirname[9],0);
			PutChar(read_dirname[10],0);
		}
	}

	LCDBufSetPos(20,2);

	value = pattern(fs_apiinfo.fileNumber);
	PutChar(value >> 24,0);
	PutChar(value >> 16,0);
	PutChar(value >> 8,0);
	PutChar(value,0);

	PutS("/",0);

	value = pattern(fs_apiinfo.fileTotal);
	PutChar(value >> 24,0);
	PutChar(value >> 16,0);
	PutChar(value >> 8,0);
	PutChar(value,0);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ��ǰ��̨���
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_FMNext(void)@"fm_lcd"
{
	unsigned short value;
	value = pattern(Cur_Fre_Num);

	LCDBufSetPos(24,0);
	PutS("CH:",1);
	PutChar(value >> 8,1);
	PutChar(value,1);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ��CONNECT PC��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_PCconnect(void)@"LCD_API_1"
{
	LCDBufSetPos(24,1);
	PutS("CONNECT PC",0);
}

/*----------------------------------------------------------------------------*/
/**@brief ����������
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_OFF(void)@"LCD_API_1"
{
	LCD_clear();
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ��NoDevice��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_NULL(void)@"LCD_API_1"
{
	LCDBufSetPos(0,4);
	PutS("NoDevice",1);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������������ʾ��NoDevice��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_NO_Device(void)@"LCD_API_1"
{
	LCD_clear();

	LCDBufSetPos(0,0);
	PutS("NoDevice",1);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ��No File��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_NO_Music(void)@"LCD_API_1"
{
	LCDBufSetPos(8,0);
	PutS("No File",1);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ��ǰ���ߵ�ý�壺SD����USB
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void DispSmallDevice(void)@"LCD_API_1"
{
	LCD_Column = 0;

	if(CurCheckedDev&BIT(0))
		FillRegister(16,SD_CARD,0);
	else
		FillRegister(16,USB_PHOTO,0);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ����ʱ��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void DispPlayTime(void)@"LCD_API_1"
{
	unsigned short value;

	LCD_Page = 0;

	LCD_Column = 19;
	value = pattern(PlayTime.min);
	PutChar(value >> 8,0);
	PutChar(value,0);

	PutChar(':',0);

	value = pattern(PlayTime.sec);
	PutChar(value >> 8,0);
	PutChar(value,0);

}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ����ģʽ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void DispSmallPlayMode(void)@"LCD_API_1"
{
	LCD_Column = 60;

	FillRegister(22,PlayModeIcon[playMode],0);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ�ļ���ʱ��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void DispAllTime(void)@"LCD_API_1"
{
	u32 value;
	LCD_Page = 0;


	LCD_Column = 83;

	value = pattern(AllMin);
	PutChar(value >> 8,0);
	PutChar(value,0);

	PutChar(':',0);

	value = pattern(AllSec);
	PutChar(value >> 8,0);
	PutChar(value,0);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ��ǰƵ��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void DispSmallFm(void)@"LCD_API_1"
{
	u32 value;

	LCD_Page = 0;
	LCD_Column = 76;
	FillRegister(16,SPEAKER,0);

	value = pattern(fmFrequency);
	LCD_Column = 113;
	PutChar('.',0);
	LCD_Column = 113-7*3;
	if(fmFrequency/1000 > 0)
	{
		PutChar(value >> 24,0);
	}
	else
		PutChar(' ',0);

	PutChar(value >> 16,0);
	PutChar(value >> 8,0);
	LCD_Column = 113+5;
	PutChar(value,0);
}

/*----------------------------------------------------------------------------*/
/**@brief ���ĳһ��LCD buff
   @param n �ַ���
   @param buff Ҫ�����ַ���
   @param fill_page ָ��Ҫ����LCD buff
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void FillRegister(u8 n,unsigned char const __code *buff,u8 fill_page)@"LCD_API_1"
{
	u8 a;
	for(a=0; a<n; a++)
	{
		LCD_Buff[fill_page][LCD_Column + a] = (unsigned char)buff[a+a];
		LCD_Buff[fill_page+1][LCD_Column + a] = (unsigned char)buff[a+a+1];
	}
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ"STOP"����"PAUSE"
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_Pause(void)@"LCD_API_1"
{
	LCD_Page = 0;

	if(bStopFlag == true)
	{
		LCD_Column = (128-16*4)/2;
		PutS("STOP",1);
	}
	else
	{
		LCD_Column = (128-16*5)/2;
		PutS("PAUSE",1);
	}
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ��ǰʹ�õ�EQ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void Menu_EQ(void)@"LCD_API_1"
{
	char __code *p;

	p = (char __code *) &EQ_BUFF[eq][0];

	LCD_Page = 0;
	LCD_Column = (128-2*EQ_POS_BUFF[eq])/2;
	PutS(p,1);  //128*32��ʾ�����壻
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ"LINEIN"
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void MENU_linein(void)@"LCD_API_1"
{
	LCDBufSetPos((128-96)/2,0);

	PutS("LINEIN",1);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ"waiting..."
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void MENU_Waiting(void)@"LCD_API_1"
{
	LCD_Column = (128-80)/2;
	LCD_Page = 1;

	PutS("waiting...",0);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ"Hello"
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void MENU_Power_on(void)@"LCD_API_1"
{
	LCDBufSetPos((128-80)/2,0);

	PutS("Hello",1);
}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ�ļ�����
   @param ��
   @return ��
   @note �ļ���������ʾ��Χ��̬��ʾ
*/
/*----------------------------------------------------------------------------*/
void DispFileName(void)@"LCD_API_1"
{
	u8 i;
	u8 *str;

	if(filename[FILE_NAME_SIZE-1] == 50)
	{
		filename[47] = '.';
		filename[48] = '.';
		filename[49] = '.';
	}

	str = filename;
	for(i=0; i<COLUMN; i++)
	{
		LCD_Buff[2][i] = 0x00;
		LCD_Buff[3][i] = 0x00;
	}

	if(filename[FILE_NAME_SIZE-1] < 128/8+1)  //δ������ʾ��Χ��filename_buff[1]��ŵ����ļ����ĳ���
	{
		LCDBufSetPos((128-(8*filename[FILE_NAME_SIZE-1]))/2,2);
		PutS_2(str,0);
	}
	else    //������ʾ��Χ����̬��ʾ
	{
		LCDBufSetPos(0,2);
		PutS_2(str+dispFileNameCounter,0);

		dispFileNameCounter++;
		if(dispFileNameCounter > filename[FILE_NAME_SIZE-1])  //filename_buff[1] ��ʾ�ַ��ĸ�����
			dispFileNameCounter = 0;
	}

}

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ��������ʾ����ʱ����ļ�����
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
void DispFileNameTime(void)@"LCD_API_1"
{
	DispPlayTime();
	DispFileName();

	Update_Display();
}

#pragma constseg=LCD_CONST
const H menu_lcd[] =
{
	Menu_NULL,
	Menu_EQ,
	Menu_VOL,
	Menu_FRE,
	Menu_Input_Number,
	Menu_NO_Music,
	MENU_Play,
	Menu_Play_Mode,
	Menu_Next,
	Menu_Pause,
	Menu_NO_Device,
	MENU_Power_on,
	MENU_Waiting,
	MENU_linein,
	Menu_FMNext,
	Menu_PCconnect,
	Menu_OFF,
	Menu_VOLUP,
	Menu_VOLDOWN,
	Menu_RTC_Time,
};
#pragma constseg=default

/*----------------------------------------------------------------------------*/
/**@brief ��������ʾ����
   @param menu_number ��Ҫ��ʾ�Ĳ˵�����
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="LCD_API"
void DispMenu_3(u8 menu_number)
{
	void (*p)(void);

	p = menu_lcd[menu_number];

	LCD_clear();

	returnCount = 0;
	(*p)();

	Update_Display();
}
