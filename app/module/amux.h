#ifndef _AMUX_H_
#define _AMUX_H_


//AMUX通道
#define CH_L_1               BIT(0)    //单声道，P31-AUXL0
#define CH_L_3               BIT(1)    //单声道，P33-AUXL1
#define CH_L_5               BIT(2)    //单声道，P35-AUXL2
#define CH_L_MIC             BIT(3)    //单声道，MIC-AUXL3
#define CH_R_0               BIT(4)    //单声道，P30-AUXR0
#define CH_R_2               BIT(5)    //单声道，P32-AUXR1
#define CH_R_4               BIT(6)    //单声道，P34-AUXR2
#define CH_R_DACL            BIT(7)    //单声道，DACL-AUXR3

#define CH_MONO              0x100              //单进单出标志
#define CH_MIN_DOUT          0x200              //单进双出标志
//#define CH_MIC_IN            0x08           //MIC输入标志

#define AUADC_BIT_MONO       0x80

//-------------------AMUX_CHANNEL-------------------
//AMUX单声道进双声道出
#define AMUX_CH_00           (CH_R_0 | CH_MIN_DOUT)     //P30-AUXR0
#define AMUX_CH_11           (CH_L_1 | CH_MIN_DOUT)     //P31-AUXL0
#define AMUX_CH_22           (CH_R_2 | CH_MIN_DOUT)     //P32-AUXR1
#define AMUX_CH_33           (CH_L_3 | CH_MIN_DOUT)     //P33-AUXL1
#define AMUX_CH_44           (CH_R_4 | CH_MIN_DOUT)     //P34-AUXR2
#define AMUX_CH_55           (CH_L_5 | CH_MIN_DOUT)     //P35-AUXL2
#define AMUX_CH_MM           (CH_L_MIC | CH_MIN_DOUT)   //MIC-AUXL3   (用于AUX)
//#define AMUX_CH_MIC          (CH_L_MIC | CH_MIC_IN)  //MIC         (用于扩音器)

//AMUX单声道进单声道出
#define AMUX_CH_0            (CH_R_0 | CH_MONO)   //P30-AUXR0
#define AMUX_CH_1            (CH_L_1 | CH_MONO)   //P31-AUXL0
#define AMUX_CH_2            (CH_R_2 | CH_MONO)   //P32-AUXR1
#define AMUX_CH_3            (CH_L_3 | CH_MONO)   //P33-AUXL1
#define AMUX_CH_4            (CH_R_4 | CH_MONO)   //P34-AUXR2
#define AMUX_CH_5            (CH_L_5 | CH_MONO)   //P35-AUXL2
#define AMUX_CH_L            (CH_R_DACL | CH_MONO)//DACL-AUXR3

//AMUX双声道
#define AMUX_CH_01           (CH_R_0 | CH_L_1)   //P30-AUXR0 P31-AUXL0
#define AMUX_CH_03           (CH_R_0 | CH_L_3)   //P30-AUXR0 P33-AUXL1
#define AMUX_CH_05           (CH_R_0 | CH_L_5)   //P30-AUXR0 P35-AUXL2
#define AMUX_CH_0M           (CH_R_0 | CH_L_MIC) //P30-AUXR0 MIC-AUXL3
#define AMUX_CH_21           (CH_R_2 | CH_L_1)   //P32-AUXR1 P31-AUXL0
#define AMUX_CH_23           (CH_R_2 | CH_L_3)   //P32-AUXR1 P33-AUXL1
#define AMUX_CH_25           (CH_R_2 | CH_L_5)   //P32-AUXR1 P35-AUXL2
#define AMUX_CH_2M           (CH_R_2 | CH_L_MIC) //P32-AUXR1 MIC-AUXL3
#define AMUX_CH_41           (CH_R_4 | CH_L_1)   //P34-AUXR2 P31-AUXL0
#define AMUX_CH_43           (CH_R_4 | CH_L_3)   //P34-AUXR2 P33-AUXL1
#define AMUX_CH_45           (CH_R_4 | CH_L_5)   //P34-AUXR2 P35-AUXL2
#define AMUX_CH_4M           (CH_R_4 | CH_L_MIC) //P34-AUXR2 MIC-AUXL3


//----------------------END-------------------------


//MIC gain control
#define GAIN_N3DB            0x00 //-3dB
#define GAIN_0DB             0x01 //0dB
#define GAIN_3DB             0x02 //3dB
#define GAIN_6DB             0x03 //6dB
#define GAIN_9DB             0x04 //9dB
#define GAIN_12DB            0x05 //12dB
#define GAIN_15DB            0x06 //15dB
#define GAIN_18DB            0x07 //18dB
#define GAIN_21DB            0x08 //21dB
#define GAIN_24DB            0x09 //24dB
#define GAIN_27DB            0x0a //27dB
#define GAIN_30DB            0x0b //30dB
#define GAIN_33DB            0x0c //33dB
#define GAIN_36DB            0x0d //36dB

#ifdef AX107X_TAG
#define GAIN_39DB            0x0e //39dB
#define GAIN_42DB            0x0f //42dB
#endif

void amux_init(u16 ch_sel);
void amux_mic_init(u8 mic_gain);
void amux_exit(void);
void amux_volume_set(void);

#endif
