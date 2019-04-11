/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户的IO设置
 *****************************************************************************/
#ifndef USER_IO_H
#define USER_IO_H

/*************************************
SD_PORT  AIPCON6[7:6]  DAT0  CLK  CMD
   0        2'00      P15   P16  P17
   1        2'01      P12   P13  P14
   2        2'10      P43   P42  P17
   3        2'11      P31   P06  P30
*************************************/

#if (SD_PORT == 0)
#define SD_PORT_SEL()           {AIPCON6 &= ~(BIT(7) | BIT(6));}

#define SD_DAT0_PORT            P1
#define SD_DAT0_DIR             P1DIR
#define SD_DAT0_PU              PUP1
#define SD_DAT0_PIN			    2

#define SD_CLK_PORT             P1
#define SD_CLK_DIR              P1DIR
#define SD_CLK_PU               PUP1
#define SD_CLK_PIN			    3

#define SD_CMD_PORT             P1
#define SD_CMD_DIR              P1DIR
#define SD_CMD_PU               PUP1
#define SD_CMD_PIN			    4

#define SD_CMD_PU500R_EN()      AIPCON5 |= BIT(4)
#define SD_CMD_PU500R_DIS()     AIPCON5 &= ~BIT(4)

#elif (SD_PORT == 1)
#define SD_PORT_SEL()           {AIPCON6 &= ~BIT(7);AIPCON6 |=  BIT(6);}

#define SD_DAT0_PORT            P1
#define SD_DAT0_DIR             P1DIR
#define SD_DAT0_PU              PUP1
#define SD_DAT0_PIN			    2

#define SD_CLK_PORT             P1
#define SD_CLK_DIR              P1DIR
#define SD_CLK_PU               PUP1
#define SD_CLK_PIN			    3

#define SD_CMD_PORT             P1
#define SD_CMD_DIR              P1DIR
#define SD_CMD_PU               PUP1
#define SD_CMD_PIN			    4

#define SD_CMD_PU500R_EN()      //没有500R上拉
#define SD_CMD_PU500R_DIS()

#elif (SD_PORT == 2)
#define SD_PORT_SEL()           {AIPCON7 |= BIT(0);AIPCON6 |= BIT(7);AIPCON6 &= ~BIT(6);}

#define SD_DAT0_PORT            P4
#define SD_DAT0_DIR             P4DIR
#define SD_DAT0_PU              PUP4
#define SD_DAT0_PIN			    3

#define SD_CLK_PORT             P4
#define SD_CLK_DIR              P4DIR
#define SD_CLK_PU               PUP4
#define SD_CLK_PIN			    2

#define SD_CMD_PORT             P1
#define SD_CMD_DIR              P1DIR
#define SD_CMD_PU               PUP1
#define SD_CMD_PIN			    7

#define SD_CMD_PU500R_EN()      AIPCON5 |= BIT(4)
#define SD_CMD_PU500R_DIS()     AIPCON5 &= ~BIT(4)

#elif (SD_PORT == 3)
#define SD_PORT_SEL()           {AIPCON6 |= BIT(7) | BIT(6);}

#define SD_DAT0_PORT            P3
#define SD_DAT0_DIR             P3DIR
#define SD_DAT0_PU              PUP3
#define SD_DAT0_PIN			    1

#define SD_CLK_PORT             P0
#define SD_CLK_DIR              P0DIR
#define SD_CLK_PU               PUP0
#define SD_CLK_PIN			    6

#define SD_CMD_PORT             P3
#define SD_CMD_DIR              P3DIR
#define SD_CMD_PU               PUP3
#define SD_CMD_PIN			    0

#define SD_CMD_PU500R_EN()      //没有500R上拉
#define SD_CMD_PU500R_DIS()

#else

#error "SD0请选择一组SDIO";

#endif  //SD_PORT


#if (SD1_PORT == 0)
#define SD1_PORT_SEL()           {AIPCON6 &= ~(BIT(7) | BIT(6));}

#define SD1_DAT0_PORT            P1
#define SD1_DAT0_DIR             P1DIR
#define SD1_DAT0_PU              PUP1
#define SD1_DAT0_PIN			    2

#define SD1_CLK_PORT             P1
#define SD1_CLK_DIR              P1DIR
#define SD1_CLK_PU               PUP1
#define SD1_CLK_PIN			        3

#define SD1_CMD_PORT             P1
#define SD1_CMD_DIR              P1DIR
#define SD1_CMD_PU               PUP1
#define SD1_CMD_PIN			        4

#define SD1_CMD_PU500R_EN()      AIPCON5 |= BIT(4)
#define SD1_CMD_PU500R_DIS()     AIPCON5 &= ~BIT(4)

#elif (SD1_PORT == 1)
#define SD1_PORT_SEL()           {AIPCON6 &= ~BIT(7);AIPCON6 |=  BIT(6);}

#define SD1_DAT0_PORT            P1
#define SD1_DAT0_DIR             P1DIR
#define SD1_DAT0_PU              PUP1
#define SD1_DAT0_PIN			    2

#define SD1_CLK_PORT             P1
#define SD1_CLK_DIR              P1DIR
#define SD1_CLK_PU               PUP1
#define SD1_CLK_PIN			        3

#define SD1_CMD_PORT             P1
#define SD1_CMD_DIR              P1DIR
#define SD1_CMD_PU               PUP1
#define SD1_CMD_PIN			        4

#define SD1_CMD_PU500R_EN()      //没有500R上拉
#define SD1_CMD_PU500R_DIS()

#elif (SD1_PORT == 2)
#define SD1_PORT_SEL()           {AIPCON7 |= BIT(0);AIPCON6 |= BIT(7);AIPCON6 &= ~BIT(6);}

#define SD1_DAT0_PORT            P4
#define SD1_DAT0_DIR             P4DIR
#define SD1_DAT0_PU              PUP4
#define SD1_DAT0_PIN			    3

#define SD1_CLK_PORT             P4
#define SD1_CLK_DIR              P4DIR
#define SD1_CLK_PU               PUP4
#define SD1_CLK_PIN			        2

#define SD1_CMD_PORT             P1
#define SD1_CMD_DIR              P1DIR
#define SD1_CMD_PU               PUP1
#define SD1_CMD_PIN			        7

#define SD1_CMD_PU500R_EN()      AIPCON5 |= BIT(4)
#define SD1_CMD_PU500R_DIS()     AIPCON5 &= ~BIT(4)

#elif (SD1_PORT == 3)
#define SD1_PORT_SEL()           {AIPCON6 |= BIT(7) | BIT(6);}

#define SD1_DAT0_PORT            P3
#define SD1_DAT0_DIR             P3DIR
#define SD1_DAT0_PU              PUP3
#define SD1_DAT0_PIN			    1

#define SD1_CLK_PORT             P0
#define SD1_CLK_DIR              P0DIR
#define SD1_CLK_PU               PUP0
#define SD1_CLK_PIN			        6

#define SD1_CMD_PORT             P3
#define SD1_CMD_DIR              P3DIR
#define SD1_CMD_PU               PUP3
#define SD1_CMD_PIN			        0

#define SD1_CMD_PU500R_EN()      //没有500R上拉
#define SD1_CMD_PU500R_DIS()

#else

#error "SD1请选择一组SDIO";

#endif  //SD1_PORT


#if 0 // !USE_SD_CLK_DET          //TFDET from P30 to P10    20170509  wq
#define SD_CHECK_PORT		    P1
#define SD_CHECK_DIR		    P1DIR
#define SD_CHECK_PU			    PUP1
#define SD_CHECK_PIN			0
#else
#define SD_CHECK_PORT		    SD_CLK_PORT
#define SD_CHECK_DIR		    SD_CLK_DIR
#define SD_CHECK_PU			    SD_CLK_PU
#define SD_CHECK_PIN			SD_CLK_PIN
#endif

#if 0  //!USE_SD_CLK_DET
#define SD1_CHECK_PORT		    P3
#define SD1_CHECK_DIR		    P3DIR
#define SD1_CHECK_PU			PUP3
#define SD1_CHECK_PIN			1
#else
#define SD1_CHECK_PORT		    SD1_CLK_PORT
#define SD1_CHECK_DIR		    SD1_CLK_DIR
#define SD1_CHECK_PU			SD1_CLK_PU
#define SD1_CHECK_PIN			SD1_CLK_PIN
#endif


#if !USE_I2C_SD_MUX
#define I2C_SDA_PORT            P1
#define I2C_SDA_DIR             P1DIR
#define I2C_SDA_PU              PUP1
#define I2C_SDA_PIN             4       //D4

#define I2C_SCL_PORT            P1
#define I2C_SCL_DIR             P1DIR
#define I2C_SCL_PU              PUP1
#define I2C_SCL_PIN             3       //D5
#else
#define I2C_SDA_PORT            SD_DAT0_PORT
#define I2C_SDA_DIR             SD_DAT0_DIR
#define I2C_SDA_PU              SD_DAT0_PU
#define I2C_SDA_PIN             SD_DAT0_PIN

#define I2C_SCL_PORT            SD_CMD_PORT
#define I2C_SCL_DIR             SD_CMD_DIR
#define I2C_SCL_PU              SD_CMD_PU
#define I2C_SCL_PIN             SD_CMD_PIN
#endif

#define MUTE_PORT_OUTPUT	MUTE_DIR &= ~BIT(MUTE_PIN)
#define MUTE_PORT_INPUT	    MUTE_DIR |= BIT(MUTE_PIN)
#define MUTE_PORT_LOW 		MUTE_PORT &= ~BIT(MUTE_PIN)
#define MUTE_PORT_HIGH 		MUTE_PORT |= BIT(MUTE_PIN)

#if (MUTE_TYPE == MUTE_HIGH)
#define EN_MUTE()		MUTE_DIR |= BIT(MUTE_PIN) //MUTE_PORT |=  BIT(MUTE_PIN)   // mute p00 set input 20170901
#define DIS_MUTE()		MUTE_DIR &= ~BIT(MUTE_PIN),MUTE_PORT &= ~BIT(MUTE_PIN)
#elif (MUTE_TYPE == MUTE_LOW)
#define EN_MUTE()       MUTE_PORT &= ~BIT(MUTE_PIN)
#define DIS_MUTE()      MUTE_PORT |=  BIT(MUTE_PIN)
#elif (MUTE_TYPE == MUTE_DIGITAL)
#define EN_MUTE()       MUTE_PORT &= ~BIT(MUTE_PIN)
#define DIS_MUTE()      {MUTE_PORT_HIGH; delay_nop(18); MUTE_PORT_LOW; delay_nop(18); MUTE_PORT_HIGH;}
#endif

extern void sd_insert(void);
extern void sd_remove(void);

extern void mute_ctl_auto(void);
void usb_port_init(u8 phy);
void sd_port_init(u8 type);

void MUTE_EN(void);
void UN_MUTE(void);
void port_init(void);
void sd_detect(void);           //SD检测
void usb_detect(void);          //USB检测函数
void aux_detect(void);          //LineIn检测函数
void mic_detect(void);          //Mic检测函数
void headphone_detect(void);    //Headphone检测函数
void dcin_detect(void);
bool sd_port_sta(void);
void mute_init(void);
void mute_enable(void);
void mute_disable(void);
void io_output_clk_start(void);
void io_output_clk_stop(void);
void sd_group_sel(u8 sd_group);
void sd1_detect(void);
//void MUTE(void);
//void UN_MUTE(void)
#endif
