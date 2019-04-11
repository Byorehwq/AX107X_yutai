#ifndef _QN8065_H
#define _QN8065_H

/**********************************QN8065's clock source selection**************
1.QN8065's default clock source is 32768HZ.
2.setting QN8065's clock source and clock source type(like sine-wave clock or digital clock).
3.user need to modify clock source according to actual hardware platform.
4.clock formula,the details please refer to the QN8065's datasheet
  XTAL_DIV = Round(Clock/32768);
  PLL_DLT = Round((28800000*512*XTAL_DIV)/Clock)-446464
*******************************************************************************/
#if ((FM_CLOCK == USE_IO_32KHZ_CLK) || (FM_CLOCK == SHARE_CRYSTAL_32KHz) || (FM_CLOCK == USE_SD_IO_32KHZ_CLK))
//crystal clock is 32768HZ
#define QN8065_XTAL_DIV0_VAL                   	0x01
#define QN8065_XTAL_DIV1_VAL                   	0x80
#define QN8065_XTAL_DIV2_VAL                   	0x6e

#elif ((FM_CLOCK == USE_IO_12MHZ_CLK) || (FM_CLOCK == SHARE_CRYSTAL_12MHz) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK))
//crystal clock is 12MHZ
#define QN8065_XTAL_DIV0_VAL                   	0x6e
#define QN8065_XTAL_DIV1_VAL                   	0x69
#define QN8065_XTAL_DIV2_VAL                   	0x66
#endif

/*
//crystal clock is 32768HZ
#define QN8065_XTAL_DIV0_VAL                   	0x01
#define QN8065_XTAL_DIV1_VAL                   	0x80
#define QN8065_XTAL_DIV2_VAL                   	0x6e

//crystal clock is 4MHZ
#define QN8065_XTAL_DIV0_VAL                   	0x7a
#define QN8065_XTAL_DIV1_VAL                   	0x68
#define QN8065_XTAL_DIV2_VAL                   	0x66

//crystal clock is 6MHZ
#define QN8065_XTAL_DIV0_VAL                   	0xb7
#define QN8065_XTAL_DIV1_VAL                   	0x68
#define QN8065_XTAL_DIV2_VAL                   	0x66

//crystal clock is 7.3728MHZ
#define QN8065_XTAL_DIV0_VAL                   	0xe1
#define QN8065_XTAL_DIV1_VAL                   	0x80
#define QN8065_XTAL_DIV2_VAL                   	0x6e


//crystal clock is 8MHZ
#define QN8065_XTAL_DIV0_VAL                   	0xf4
#define QN8065_XTAL_DIV1_VAL                   	0x68
#define QN8065_XTAL_DIV2_VAL                   	0x66

//crystal clock is 10MHZ
#define QN8065_XTAL_DIV0_VAL                   	0x31
#define QN8065_XTAL_DIV1_VAL                   	0x69
#define QN8065_XTAL_DIV2_VAL                   	0x66

//crystal clock is 11.0592MHZ
#define QN8065_XTAL_DIV0_VAL                   	0x52
#define QN8065_XTAL_DIV1_VAL                   	0x59
#define QN8065_XTAL_DIV2_VAL                   	0x83

//crystal clock is 12MHZ
#define QN8065_XTAL_DIV0_VAL                   	0x6e
#define QN8065_XTAL_DIV1_VAL                   	0x69
#define QN8065_XTAL_DIV2_VAL                   	0x66

//crystal clock is 16MHZ
#define QN8065_XTAL_DIV0_VAL                   	0xe8
#define QN8065_XTAL_DIV1_VAL                   	0x69
#define QN8065_XTAL_DIV2_VAL                   	0x66

//crystal clock is 20MHZ
#define QN8065_XTAL_DIV0_VAL                   	0x62
#define QN8065_XTAL_DIV1_VAL                   	0x6a
#define QN8065_XTAL_DIV2_VAL                   	0x66

//crystal clock is 24MHZ
#define QN8065_XTAL_DIV0_VAL                   	0xdc
#define QN8065_XTAL_DIV1_VAL                   	0x6a
#define QN8065_XTAL_DIV2_VAL                   	0x66

//crystal clock is 26MHZ
#define QN8065_XTAL_DIV0_VAL                   	0x19
#define QN8065_XTAL_DIV1_VAL                   	0x6b
#define QN8065_XTAL_DIV2_VAL                   	0x66

//crystal clock is 27MHZ
#define QN8065_XTAL_DIV0_VAL                   	0x38
#define QN8065_XTAL_DIV1_VAL                   	0xf3
#define QN8065_XTAL_DIV2_VAL                   	0x6e
*/

//QN8065 chip ID
#define CHIPID_QN8065                   		0x8c

#define QN8065_SINE_WAVE_CLOCK             		0x00    //inject sine-wave clock
#define QN8065_DIGITAL_CLOCK               		0x80    //inject digital clock,default is inject digital clock

/*******************************************************************************
Performance configuration
*******************************************************************************/
#define QN8065_SMSTART_VAL                     	12
#define QN8065_HCCSTART_VAL                    	18
#define QN8065_SNCSTART_VAL                    	51

/*******************************************************************************
limitation configuration
*******************************************************************************/
#define QN8065_CH_SETUP_DELAY_TIME             	300

/*******************************************************************************
 definition register
*******************************************************************************/
#define QN8065_SYSTEM1         					0x00
#define QN8065_VOL_CTL                         	0x01
#define QN8065_SNR            					0x02
#define QN8065_RSSISIG         					0x03
#define QN8065_STATUS1         					0x04
#define QN8065_CID1            					0x05
#define QN8065_CID2            					0x06
#define	QN8065_CH								0x07
#define	QN8065_CH_START							0x08
#define	QN8065_CH_STOP							0x09
#define	QN8065_CH_STEP							0x0a
#define QN8065_CCA                             	0x0b
#define	QN8065_CCA_SNR_TH   					0x0c
#define	QN8065_CCA_CNT 					    	0x0d
#define	QN8065_XTAL_DIV0						0x0e
#define	QN8065_XTAL_DIV1						0x0f
#define	QN8065_XTAL_DIV2						0x10
#define QN8065_SMP_HLD_THRD						0x11
#define	QN8065_RXAGC_GAIN						0x12
#define	QN8065_SYSTEM_CTL1						0x13
#define QN8065_REG_TEST							0x14
#define QN8065_RXAGC1                          	0x15
#define QN8065_RXAGC2                          	0x16
#define QN8065_RXAGC3                          	0x17
#define QN8065_RXAGC4                          	0x18
#define	QN8065_CCA1								0x19
#define	QN8065_CCA2								0x1a
#define QN8065_CCA_TIME                        	0x1b
#define QN8065_CCS_NOISE                       	0x1c
#define QN8065_IFCNT1                          	0x1d
#define QN8065_IFCNT2                          	0x1e
#define QN8065_CCA_SNR                         	0x1f
#define QN8065_CCOND1                          	0x20
#define QN8065_CCOND2                          	0x21
#define QN8065_NSM                             	0x22
#define QN8065_NHCC                            	0x23
#define QN8065_HYSTERSIS                       	0x24
#define	QN8065_SMSTART							0x25
#define	QN8065_SNCSTART							0x26
#define	QN8065_HCCSTART							0x27
#define QN8065_RC_CAL_TH                       	0x28
#define QN8065_REG_ADC                         	0x29
#define QN8065_REG_REF                         	0x2a
#define QN8065_REG_PD                          	0x2b
#define QN8065_REG_FLL                         	0x31
/***************************************End************************************/

/*******************************************************************************
 definition operation bit of register
*******************************************************************************/
#define QN8065_CCA_CH_DIS      					0x01
#define QN8065_CHSC            					0x02
#define QN8065_RDSEN           					0x08
#define QN8065_CH_CH		    				0x03
#define QN8065_CH_CH_START     					0x0c
#define QN8065_CH_CH_STOP      					0x30
#define QN8065_STNBY_MODE           		    0x20
#define QN8065_RX_MODE         					0x10
#define QN8065_IMR             					0x40
#define QN8065_ST_MO_RX                        	0x01
#define QN8065_STNBY_RX_MASK                   	0x30
#define QN8065_RXCCA_MASK                      	0x03
#define QN8065_RX_CCA                          	0x02
#define QN8065_RXCCA_FAIL                      	0x08
#define QN8065_RX_MONO                         	0x04

#define QN8065_CCA_NSNR_20MS                   	(0x00 << 2)
#define QN8065_CCA_NSNR_40MS                   	(0x01 << 2)
#define QN8065_CCA_NSNR_60MS                   	(0x02 << 2)
#define QN8065_CCA_NSNR_100MS                  	(0x03 << 2)

#define QN8065_CCA_NRSSI_10MS                  	(0x00 << 4)
#define QN8065_CCA_NRSSI_20MS                  	(0x01 << 4)
#define QN8065_CCA_NRSSI_40MS                  	(0x02 << 4)

#define QN8065_CCA_NRSSI_60MS                  	(0x03 << 4)

#define QN8065_CCA_NAGC_10MS                   	(0x00 << 6)
#define QN8065_CCA_NAGC_20MS                   	(0x01 << 6)
#define QN8065_CCA_NAGC_40MS                   	(0x02 << 6)

#define QN8065_CCA_NAGC_60MS                   	(0x03 << 6)

/***************************************End************************************/

#define QN8065_GetMonoMode() \
        QN8065_ReadReg(QN8065_STATUS1) & QN8065_ST_MO_RX
#define QN8065_GetRssi() \
        QN8065_ReadReg(QN8065_RSSISIG)
#define QNM_GetChipNumber()\
        QN8065_ReadReg(QN8065_CID2) & 0xfc
// eg:
// if(QNM_GetChipNumber() == CHIPID_QN8065)
// {
//    // QN8065 processing
// }

extern void qn8065_init(void);
extern u8   qn8065_seek(u16 freq);
extern void qn8065_tune2ch(u16 ch);

extern void qn8065_set_vol(u8 volume);
extern void QN8065_RXConfigAudio(u8 option) ;
extern u8 QN8065_ScanNoiseFloor(u16 start,u16 stop);
extern u8 qn8065_online(void);
extern void QN8065_RXSetTH(void);
extern u8 QN8065_RXValidCH(u16 freq);
extern void qn8065_off(void);

extern u8 QN8065_ReadReg(u8 adr);
extern void QN8065_WriteReg(u8 adr, u8 value);
extern u16 GetIFCount(void);

#endif
