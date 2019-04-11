
#ifndef  _LCD_SEG_H_
#define  _LCD_SEG_H_

#include "include.h"

//==3*9==
// ----------------------------------------------------------------
// | COM0  COM1  COM2  S0   S1   S2   S3   S4   S5   S6   S7   S8  |
// |  |     |     |    |    |    |    |    |    |    |    |    |   |
// | P11   P12   P13   P20  P21  P22  P23  P24  P25  P26  P27  P10 |
// ----------------------------------------------------------------

//==4*8==
// ----------------------------------------------------------------
// | COM0  COM1  COM2  COM3  S0   S1   S2   S3   S4   S5   S6   S7 |
// |  |     |     |     |    |    |    |    |    |    |    |    |  |
// | P12   P13   P14   P10  P20  P21  P22  P23  P24  P25  P26  P27 |
// ----------------------------------------------------------------

//==4*7==
// ----------------------------------------------------------------
// | COM0  COM1  COM2  COM3  S0   S1   S2   S3   S4   S5   S6      |
// |  |     |     |     |    |    |    |    |    |    |    |       |
// | P12   P13   P14   P10  P20  P21  P22  P23  P24  P25  P26      |
// ----------------------------------------------------------------
//4*7∂‘”¶±Ì:
//             S7    S6    S5    S4    S3    S2    S1
//      bit7  bit6  bit5  bit4  bit3  bit2  bit1  bit0
//COM0   -     FM    2D    COL   3D    DOT   4D    MHz
//COM1   -     PM    2E    2C    3E    3C    4E    4C
//COM2   -     AM    2F    2G    3F    3G    4F    4G
//COM3   -    1E.1F  2A    2B    3A    3B    4A    4B

#define     LCD_SEG39   1        //4   3*9
#define     LCD_SEG48   2        //7   4*8
#define     LCD_SEG47   3        //8   4*7

#define LCD_SEG_TYPE        LCD_SEG39



#define LCD_COM0_DIR   P1DIR
#define LCD_COM1_DIR   P1DIR
#define LCD_COM2_DIR   P1DIR
//#define LCD_COM3_DIR   P1DIR

#define LCD_S_DIR      P2DIR

#define LCD_COM0       P1
#define LCD_COM1       P1
#define LCD_COM2       P1
//#define LCD_COM3       P1

#define LCDS           P2
#define LCD_S8         P1

#define LCD_S8_bit     0

#define LCD_COM0_bit   1
#define LCD_COM1_bit   2
#define LCD_COM2_bit   3
//#define LCD_COM3_bit   0

#define DOT            0
#define C              1
#define D              2
#define G              3
#define E              4
#define B              5
#define A              6
#define F              7



#define SEG_PORT_INIT() PUP1 |= BIT(1) | BIT(2) | BIT(3);\
                        PDN1 |= BIT(1) | BIT(2) | BIT(3);\
                        P2DIR = 0x00;\
                        P1DIR &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3));

#define SEG_0   0XF6
#define SEG_1   0X22
#define SEG_2   0X7C
#define SEG_3   0X6E
#define SEG_4   0XAA
#define SEG_5   0XCE
#define SEG_6   0XDE
#define SEG_7   0X62
#define SEG_8   0XFE
#define SEG_9   0XEE
#define SEG_NC  0X00
#define SEG_N   0XF3
#define SEG_E   0XDD
#define SEG__   0X08
#define SEG_P   0XF9
#define SEG_A   0XFB
#define SEG_U   0XB7
#define SEG_L   0X94
#define SEG_R   0X18
#define SEG_F   0XD8
#define SEG_D   0X3E
#define SEG_C   0XD4
#define SEG_R   0X18
#define SEG_F   0XD8
#define SEG_S   0XCE
#define SEG_H   0XBA
#define SEG_I   0X90
#define SEG_T   0XD0
#define SEG_d   0X3E


void LCDInit_SEG(void);
void Menu_VOL_1(void);
void Menu_FRE_1(void);
void Menu_Input_Number_1(void);
void MENU_Play_1(void);
void Menu_Play_Mode_1(void);
void Menu_Next_1(void);
void Menu_FMNext_1(void);
void Menu_PCconnect_1(void);
void Menu_OFF_1(void);

void Menu_NO_Device_1(void);
void Menu_NO_File_1(void);

void Menu_Pause_1(void);
void Menu_EQ_1(void);
void MENU_Waiting_1(void);
void MENU_linein_1(void);
void Menu_NULL_1(void);
void Menu_Power_On_1(void);
void Menu_No_File_1(void);
void Menu_USBDevice_1(void);
void Menu_VolumeUp_1(void);
void Menu_VolumeDown_1(void);
void Menu_Time_1(void);

void display_seg(void);
void LCDseg_CLR(void);
void LCDseg39_value_set(void);
void LCDseg48_value_set(void);
void LCDseg47_value_set(void);

void DispMenu_1(u8 menu_number);

#endif
