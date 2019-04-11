/*
定义说明
SFR_DEF：常用sfr定义
SFR_DEFC：常用sfr定义，为了避免汇编下的定义冲突
SBIT_DEF：常用sbit定义
SBIT_DEFC：常用sbit定义，为了避免汇编下的定义冲突
XSFR_ADDR：XSFR 类型
*/
#ifndef SFR_H
#define SFR_H

#ifdef __C51__
#define __KEIL_ICC__
#endif

#ifdef __CX51__
#define __KEIL_ICC__
#endif

#ifdef __A51__
#define __KEIL_ASM__
#endif

#ifdef __AX51__
#define __KEIL_ASM__
#endif

#ifdef __KEIL_ICC__
#define SFR_DEF(reg, addr) sfr reg = addr;
#define SFR_DEFC(reg, addr) SFR_DEF(reg, addr)
#define SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
    sfr reg = addr; \
    sbit bit0 = reg^0; \
    sbit bit1 = reg^1; \
    sbit bit2 = reg^2; \
    sbit bit3 = reg^3; \
    sbit bit4 = reg^4; \
    sbit bit5 = reg^5; \
    sbit bit6 = reg^6; \
    sbit bit7 = reg^7;
#define SBIT_DEFC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
    SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
#define SFR16_DEF(reg,reg1,reg2,addr1,addr2) \
    sfr16 reg = addr1; \
    sfr reg1 = addr1; \
    sfr reg2 = addr2;

#define XSFR_ADDR(addr) (*((volatile unsigned char xdata*)addr))
#endif
#ifdef __KEIL_ASM__
#define SFR_DEF(reg, addr) sfr reg = addr;
#define SFR_DEFC(reg, addr)
SBIT_DEFM MACRO reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
sfr reg = addr;
sbit bit0 = reg^0;
sbit bit1 = reg^1;
sbit bit2 = reg^2;
sbit bit3 = reg^3;
sbit bit4 = reg^4;
sbit bit5 = reg^5;
sbit bit6 = reg^6;
sbit bit7 = reg^7;
ENDM
#define SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
    SBIT_DEFM reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
#define SBIT_DEFC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
#define XSFR_ADDR(addr) addr
#endif

#ifdef __IAR_SYSTEMS_ICC__
#define SFR_DEF(reg, addr) __sfr __no_init volatile unsigned char reg @ addr;
#define SFR_DEFC(reg, addr) SFR_DEF(reg, addr)
#define SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
    __sfr __no_init volatile union { \
    unsigned char reg; \
    struct { \
        unsigned char bit0: 1; \
        unsigned char bit1: 1; \
        unsigned char bit2: 1; \
        unsigned char bit3: 1; \
        unsigned char bit4: 1; \
        unsigned char bit5: 1; \
        unsigned char bit6: 1; \
        unsigned char bit7: 1; \
    }; \
} @ addr;

#define SFR16_DEF(reg, reg1, reg2, addr1, addr2) \
    __sfr __no_init volatile union { \
    unsigned short reg; \
    struct { \
        unsigned char reg1; \
        unsigned char reg2; \
    }; \
} @ addr1;

#define SBIT_DEFC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
    SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
#define XSFR_ADDR(addr) (*((volatile unsigned char __xdata*)addr))
#endif
#ifdef __IAR_SYSTEMS_ASM__
#define SFR_DEF(reg, addr) reg DEFINE addr
#define SFR_DEFC(reg, addr)
SBIT_DEFM MACRO reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
reg DEFINE addr
bit0 DEFINE addr.0
bit1 DEFINE addr.1
bit2 DEFINE addr.2
bit3 DEFINE addr.3
bit4 DEFINE addr.4
bit5 DEFINE addr.5
bit6 DEFINE addr.6
bit7 DEFINE addr.7
ENDM
SFR16_DEFM MACRO reg, reg1, reg2, addr1, addr2
reg DEFINE addr1
reg1 DEFINE addr1
reg2 DEFINE addr2
ENDM
#define SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
    SBIT_DEFM reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
#define SBIT_DEFC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
#define XSFR_ADDR(addr) addr
#define SFR16_DEF(reg, reg1, reg2, addr1, addr2) \
    SFR16_DEFM reg, reg1, reg2, addr1, addr2
#endif

/*  BYTE Registers  */
//========= 80H ----> 8FH =========//
SFR_DEF(P0,                0x80)
SFR_DEFC(SP,               0x81)
SFR_DEF(DPL0,              0x82)
SFR_DEF(DPH0,              0x83)
SFR_DEF(DPL1,              0x84)
SFR_DEF(DPH1,              0x85)
SFR_DEF(DPCON,             0x86)
SFR_DEF(P5,                0x87)
SFR_DEF(SDCON0,            0x88)
SFR_DEF(SDCON1,            0x89)
SFR_DEF(SDCON2,            0x8A)
SFR_DEF(MEMCON,            0x8B)
SFR_DEF(TKCON ,            0x8C)
SFR_DEF(TKBAUD,            0x8D)
SFR_DEF(ATADR,             0x8E)
SFR_DEF(ATDAT,             0x8F)

//========= 90H ----> 9FH =========//
SFR_DEF(P1,                0x90)
SFR16_DEF(BFBYTEPTR,BFBYTEPTRL,BFBYTEPTRH,0x91,0x92)
SFR16_DEF(BFDATA,BFDATAL,BFDATAH,0x93,0x94)
SFR_DEF(BFBITPTR,          0x95)
SFR_DEF(BFCON,             0x96)
SFR_DEF(LVDCON,            0x97)
SFR_DEF(PWKEN,             0x98)
SFR_DEF(PWKEDGE,           0x99)
SFR_DEF(TKENC ,            0x9A)
SFR_DEF(SPH,               0x9B)
SFR_DEF(PIE0,              0x9C)
SFR_DEF(PIE1,              0x9D)
SFR_DEF(TKCCC,             0x9E)
SFR_DEF(TKTCC,             0x9F)

//========= A0H ----> AFH =========//
SFR_DEF(P2,                0xA0)
SFR_DEF(RTCON,             0xA1)
SFR_DEF(IRTCON,            0xA2)
SFR_DEF(IRTCON1,           0xA3)
SFR_DEF(IRTCDAT,           0xA4)
SFR_DEF(SECCNT,            0xA5)
SFR_DEF(RANDOM,            0xA6)
SBIT_DEF(IE0,              0xA8, IE_SINT, IE_TM0, IE_TM1, IE_TM2, IE_MP3,IE_HUFF,IE_USBSOF,IE_EA)
SFR_DEF(IE1,               0xA9)
SFR_DEF(IUCON,             0xAA)
SFR_DEF(IUCON1,            0xAB)
SFR_DEF(IUDTA0,            0xAC)
SFR_DEF(SPMODE,            0xAD)

//========= B0H ----> BFH =========//
SFR_DEF(P3,                0xB0)
SFR_DEF(KVCCON,            0xB1)
SFR_DEF(KVCCON2,           0xB2)
SFR_DEF(KVCADR,            0xB3)
SFR_DEF(AUADCCON0,         0xB4)
SFR_DEF(AUADCCON1,         0xB5)
SFR_DEF(AUADCCON2,         0xB6)
SFR_DEF(AUADCCON3,         0xB7)
SFR_DEF(IP0,               0xB8)
SFR_DEF(IP1,               0xB9)
SFR_DEF(P0DIR,             0xBA)
SFR_DEF(P1DIR,             0xBB)
SFR_DEF(P2DIR,             0xBC)
SFR_DEF(P3DIR,             0xBD)
SFR_DEF(P4DIR,             0xBE)
SFR_DEF(PBANK0,            0xBF)
//
//========= C0H ----> CFH =========//
SFR_DEF(P4,                0xC0)
SFR_DEF(TMR2CON0,          0xC1)
SFR_DEF(TMR2CON1,          0xC2)
SFR_DEF(UARTSTA,           0xC3)
SFR_DEF(UARTCON,           0xC4)
SFR_DEF(UARTBAUD,          0xC5)
SFR_DEF(UARTBAUDH,         0xC6)
SFR_DEF(UARTDATA,          0xC7)

SFR_DEF(PCON0,             0xC8)
SFR_DEF(PCON1,             0xC9)
SFR_DEF(PCON2,             0xCA)
SFR_DEF(CLKCON,            0xCB)
SFR_DEF(CLKCON1,           0xCC)
SFR_DEF(CLKCON2,           0xCD)
SFR_DEF(CLKCON3,           0xCE)
SFR_DEF(CLKCON4,           0xCF)
//========= D0H ----> DFH =========//
SBIT_DEFC(PSW,             0xD0, P, F1, OV, RS0, RS1, F0, AC, CY)
SFR_DEF(ADCCON,            0xD1)
SFR_DEF(ADCMODE,           0xD2)
SFR_DEF(ADCDATAL,          0xD3)
SFR_DEF(ADCDATAH,          0xD4)
SFR_DEF(SPI0BUF,           0xD5)
SFR_DEF(SPI0CON,           0xD6)
//
SFR_DEF(AUADCCON6,         0xD7)
SFR_DEF(HFMCON,            0xD8)
SFR_DEF(AUCON10,           0xD9)
SFR_DEF(HFMCNT,            0xDA)
SFR_DEF(USBCON0,           0xDB)
SFR_DEF(USBCON1,           0xDC)
SFR_DEF(USBCON2,           0xDD)
SFR_DEF(USBDATA,           0xDE)
SFR_DEF(USBADR,            0xDF)


SFR_DEFC(ACC,              0xE0)
SFR_DEF(TMR1CON0,          0xE1)
SFR_DEF(TMR1CON1,          0xE2)
SFR_DEF(IRCON,             0xE3)
SFR_DEF(IRCFG,             0xE4)
SFR_DEF(AUCON8,            0xE6)

SFR_DEF(AUCON0,            0xE8)
SFR_DEF(AUCON5,            0xED)
SFR_DEF(AUCON7,            0xEF)

//========= F0H ----> FFH =========//
SFR_DEFC(B,                0xF0)
SFR_DEF(TMR3CON,           0xF1)

SFR_DEF(CRCREG,            0xF5)
SFR_DEF(CRCFIFO,           0xF6)
SFR_DEF(WDTCON,            0xF7)

SFR_DEF(TMR0CON,           0xF8)

SFR_DEF(USBDPDM,           0xFC)
SFR_DEF(TKPND ,            0xFD)
SFR_DEF(TKAPC ,            0xFE)

//FOR SIMULATION
SFR_DEF(SIMEND,            0xFF)

#define SOFT_INT                 0
#define TIMER0_INT               1
#define TIMER1_INT               2
#define TIMER2_INT               3
#define AUDIO_INT                4
#define BT_UART_INT              5
#define USB_SOF_INT              6
#define USB_CTL_INT              7
#define SDC_INT                  8
#define PORT_INT                 9
#define SPI0_INT                 10
#define TIMER3_INT               11
#define WDT_INT                  12
/* 注意三个中断共用一个入口，在中断函数里要判断一下标志位 */
#define RTCC_INT                 13
#define UART_INT                 13
#define LVD_INT                  13
#define AUDIO_ADC_INT            14

//#define SPI1_INT                 14
#define VECTOR_OFFSET(x)         (x*0x08 + 0x03)

//defines for ATADR
#define    DACCFG      0
#define    DACSPR      1
#define    DACVOLL     2
#define    DACVOLH     3
#define    DACVCON     4
#define    TRIMCON1    5
#define    TRIMCON2    6
#define    TRREGLL     7
#define    TRREGLH     8
#define    TRREGRL     9
#define    TRREGRH     10
#define    DACLRMIX0   11
#define    DACLRMIX1   12
#define    DACLRMIX2   13
#define    DACLRMIX3   14

//XSFR
#define SDBAUD                XSFR_ADDR(0x3000)
#define SDCPTR                XSFR_ADDR(0x3001)
#define SDDCNT                XSFR_ADDR(0x3002)
#define SDDPTR                XSFR_ADDR(0x3003)
#define BFEPTRL               XSFR_ADDR(0x3004)
#define BFEPTRH               XSFR_ADDR(0x3005)
#define HFMPTR                XSFR_ADDR(0x3006)
#define HFMTLIN               XSFR_ADDR(0x3007)
#define SPIDMAPTRL            XSFR_ADDR(0x3008)
#define SPIDMAPTRH            XSFR_ADDR(0x3009)
#define SPIDMACNT             XSFR_ADDR(0x300a)
#define SPIBAUD               XSFR_ADDR(0x300b)
#define ADCBAUD               XSFR_ADDR(0x300c)
#define AUCON11               XSFR_ADDR(0x300d)
#define AUADCDAT              XSFR_ADDR(0x300e)
#define P4DS	              XSFR_ADDR(0x300f)

#define PUP0                  XSFR_ADDR(0x3010)
#define PUP1                  XSFR_ADDR(0x3011)
#define PUP2L                 XSFR_ADDR(0x3012)
#define PUP3                  XSFR_ADDR(0x3013)
#define PUP4                  XSFR_ADDR(0x3014)
#define PDN0                  XSFR_ADDR(0x3015)
#define PDN1                  XSFR_ADDR(0x3016)
#define PDN2L                 XSFR_ADDR(0x3017)
#define PDN3                  XSFR_ADDR(0x3018)
#define PDN4                  XSFR_ADDR(0x3019)
#define PUP2                  XSFR_ADDR(0x301a)
#define PDN2                  XSFR_ADDR(0x301b)
#define P0DS                  XSFR_ADDR(0x301c)
#define P1DS                  XSFR_ADDR(0x301d)
#define P2DS                  XSFR_ADDR(0x301e)
#define P3DS                  XSFR_ADDR(0x301f)

#define TMR1CNTL              XSFR_ADDR(0x3020)
#define TMR1CNTH              XSFR_ADDR(0x3021)
#define TMR1PRL               XSFR_ADDR(0x3022)
#define TMR1PRH               XSFR_ADDR(0x3023)
#define TMR1PWML              XSFR_ADDR(0x3024)
#define TMR1PWMH              XSFR_ADDR(0x3025)
#define TMR2CNTL              XSFR_ADDR(0x3026)
#define TMR2CNTH              XSFR_ADDR(0x3027)
#define TMR2PRL               XSFR_ADDR(0x3028)
#define TMR2PRH               XSFR_ADDR(0x3029)
#define TMR2PWML              XSFR_ADDR(0x302a)
#define TMR2PWMH              XSFR_ADDR(0x302b)
#define AUADCCON5             XSFR_ADDR(0x302d)

#define USBEP0ADRL            XSFR_ADDR(0x3030)
#define USBEP0ADRH            XSFR_ADDR(0x3031)
#define USBEP1RXADRL          XSFR_ADDR(0x3032)
#define USBEP1RXADRH          XSFR_ADDR(0x3033)
#define USBEP1TXADRL          XSFR_ADDR(0x3034)
#define USBEP1TXADRH          XSFR_ADDR(0x3035)
#define USBEP2RXADRL          XSFR_ADDR(0x3036)
#define USBEP2RXADRH          XSFR_ADDR(0x3037)
#define USBEP2TXADRL          XSFR_ADDR(0x3038)
#define USBEP2TXADRH          XSFR_ADDR(0x3039)
#define USBEP3TXADRL          XSFR_ADDR(0x303a)
#define USBEP3TXADRH          XSFR_ADDR(0x303b)
#define USBCON3               XSFR_ADDR(0x303c)

#define IUDAT1               XSFR_ADDR(0x3040)
#define IUADR                XSFR_ADDR(0x3041)
#define IUDP                 XSFR_ADDR(0x3042)
#define IUGAT                XSFR_ADDR(0x3043)
#define DCN                  XSFR_ADDR(0x3044)
#define LEDHIGH              XSFR_ADDR(0x3048)
#define LEDCYCLE             XSFR_ADDR(0x3049)
#define LEDCON               XSFR_ADDR(0x304a)
#define LEDADC               XSFR_ADDR(0x304b)
#define LEDCON1              XSFR_ADDR(0x304c)
#define LEDTRIM              XSFR_ADDR(0x304d)
#define LED_PAUSE            XSFR_ADDR(0x304e)
#define LEDCON2              XSFR_ADDR(0x304f)

#define AIPCON0              XSFR_ADDR(0x3050)
#define DACACON0             XSFR_ADDR(0x3051)
#define DACACON1             XSFR_ADDR(0x3052)
#define DACACON2             XSFR_ADDR(0x3053)
#define DACACON3             XSFR_ADDR(0x3054)
#define AIPCON5              XSFR_ADDR(0x3055)
#define AIPCON6              XSFR_ADDR(0x3056)
#define AIPCON7              XSFR_ADDR(0x3057)
#define AIPCON8              XSFR_ADDR(0x3058)
#define DACACON4             XSFR_ADDR(0x3059)
#define AIPCON10             XSFR_ADDR(0x305a)
#define AIPCON11             XSFR_ADDR(0x305b)

#define PLLINTH              XSFR_ADDR(0x3060)
#define PLLINTL              XSFR_ADDR(0x3061)
#define PLLFRACH             XSFR_ADDR(0x3062)
#define PLLFRACL             XSFR_ADDR(0x3063)
#define RC_CON               XSFR_ADDR(0x3064)
//#define RC_TRIM              XSFR_ADDR(0x3065)
#define DRSINTH              XSFR_ADDR(0x3066)
#define DRSINTL              XSFR_ADDR(0x3067)
#define DRSFRACH             XSFR_ADDR(0x3068)
#define DRSFRACL             XSFR_ADDR(0x3069)
#define UART1MINUS           XSFR_ADDR(0x306a)
#define UART1POINT           XSFR_ADDR(0x306b)
#define UART1POINTH          XSFR_ADDR(0x306c)
#define UART1LOOPCNT         XSFR_ADDR(0x306d)
#define UART1CNTH            XSFR_ADDR(0x306e)
#define UART1CNTL            XSFR_ADDR(0x306f)

#define UART1CON             XSFR_ADDR(0x3070)
#define UART1BAUD 	         XSFR_ADDR(0x3071)
#define UART1DATA            XSFR_ADDR(0x3072)
#define UART1STA             XSFR_ADDR(0x3073)
#define UARTDMARXPTR         XSFR_ADDR(0x3074)
#define UARTDMATXPTR         XSFR_ADDR(0x3075)
#define UARTDMARXCNT         XSFR_ADDR(0x3076)
#define UARTDMATXCNT         XSFR_ADDR(0x3077)
#define UARTDIV              XSFR_ADDR(0x3078)

#define FMAMCON0             XSFR_ADDR(0x3079)
#define FMAMCON1             XSFR_ADDR(0x307a)
#define FMAMCNTL             XSFR_ADDR(0x307b)
#define FMAMCNTH             XSFR_ADDR(0x307c)
#define FMAMTPR              XSFR_ADDR(0x307d)
#define FMAMCON2             XSFR_ADDR(0x307e)

#define TMR0PWML             XSFR_ADDR(0x3080)
#define TMR0PWMH             XSFR_ADDR(0x3081)
#define TMR0CNTL             XSFR_ADDR(0x3082)
#define TMR0CNTH             XSFR_ADDR(0x3083)
#define TMR0PRL              XSFR_ADDR(0x3084)
#define TMR0PRH              XSFR_ADDR(0x3085)


#define TKTHC0               XSFR_ADDR(0x3086)
#define TKTHC1               XSFR_ADDR(0x3087)
#define TKTHC2               XSFR_ADDR(0x3088)
#define TKTHC3               XSFR_ADDR(0x3089)
#define TKTHC4               XSFR_ADDR(0x308a)
#define TKAC0                XSFR_ADDR(0x308b)
#define TKAC1                XSFR_ADDR(0x308c)
#define TKAC2                XSFR_ADDR(0x308d)
#define TKAC3                XSFR_ADDR(0x308e)
#define TKAC4                XSFR_ADDR(0x308f)

#define TKTHC5               XSFR_ADDR(0x3090)
#define TKAC5                XSFR_ADDR(0x3091)

#define ENGYDISPDATH         XSFR_ADDR(0x3092)
#define ENGYDISPDATL         XSFR_ADDR(0x3093)

#define IRDAT0               XSFR_ADDR(0x3094)
#define IRDAT1               XSFR_ADDR(0x3095)
#define IRDAT2               XSFR_ADDR(0x3096)
#define IRDAT3               XSFR_ADDR(0x3097)
#define DACACON5             XSFR_ADDR(0x3098)

#define TMR3PRL              XSFR_ADDR(0x3099)
#define TMR3PRH              XSFR_ADDR(0x309a)
#define TMR3CNTL             XSFR_ADDR(0x309b)
#define TMR3CNTH             XSFR_ADDR(0x309c)
#define TMR3PWML             XSFR_ADDR(0x309d)
#define TMR3PWMH             XSFR_ADDR(0x309e)

#define DBGCON               XSFR_ADDR(0x309f)
#define DACACON6             XSFR_ADDR(0x30a0)
#define DACACON7             XSFR_ADDR(0x30a1)

#endif
