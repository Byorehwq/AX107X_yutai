/*****************************************************************************
 * Module    : Config
 * File      : config_ax1070_demo.h
 * Author    :
 * Email     :
 * Function  : AX1070 DEMO�幦������
 *****************************************************************************/
#ifndef CONFIG_AX1070_DEMO_H
#define CONFIG_AX1070_DEMO_H


#define SRAM_DEBUG                      0   //�Ƿ���SRAM����Թ���, AX1070 no SRAM_DEBUG function

/*****************************************************************************
 * Module    : ϵͳ����ѡ��
 *****************************************************************************/
#define PRINTF_EN                       1   //�Ƿ���UART��ӡ����


/*****************************************************************************
 * Module    : ϵͳ����ѡ��
 *****************************************************************************/
#define SYS_CRY                         CRY_32K //CRY_RC
#define ADD_BY_WUPENGFEI                1

/*****************************************************************************
 * Task      : ϵͳ���񼰹����������
 *****************************************************************************/
#define TASK_AUX_EN                     0   //�Ƿ��LINEIN����
#define TASK_FM_EN                      0   //�Ƿ��FM����
#define TASK_SPEAKER_EN                 0   //�Ƿ������������
#define TASK_RTC_EN                     0   //�Ƿ��RTC����
#define TASK_USBDEV_EN                  0   //�Ƿ��USBDEV����
#define TASK_POWEROFF_EN                1   //�Ƿ�򿪹ػ�����
#define TASK_LOWPOWER_EN                0   //�Ƿ�򿪵͹���ģʽ����
#define TASK_IDLE_EN                    1   //�Ƿ�򿪿��й���
#define TASK_FLASH_EN                   1   //�Ƿ�򿪲���SPI_FLASH����
#define TASK_TIPS_EN                    0   //�Ƿ�֧����ʾ������

/*****************************************************************************
 * Task      : ���ֲ�������Ĺ��ܶ���
 *****************************************************************************/
#define WAV_SUPPORT                     0   //�Ƿ�֧��WAV����
#define MPX_SUPPORT 					1	//�Ƿ�֧��MPX
#define MUSIC_SWITCH_DEV                0   //�Ƿ�֧��ģʽ���в����豸
#define AUTO_CHANGE_DEVICE              0   //��һ�������һ��ʱ�Զ����豸
#define AB_REPEAT_EN                    0   //�Ƿ�ʹ��AB��������
#define KARAOKE_EN                      0   //�Ƿ�ʹ�ÿ���OK����(MIC_DET_ENΪ1ʱ��Ч,��֧�ֻ���Ч��)
#define USE_BREAK_POINT                 1   //�Ƿ�ʹ�öϵ����
#define USE_Flash_BREAK_POINT			1	//Flash�����Ƿ�ʹ�öϵ����

#define MUSIC_STOP                      0   //�Ƿ�ʹ��STOP����
#define MUSIC_PLAYMODE                  0   //�Ƿ����Ҫ����PLAYMODE
#define MUSIC_EQ                        0   //�Ƿ����Ҫ����EQ
#define FAST_FORW_BACKW_EN              0   //�Ƿ�ʹ�ÿ�����˹���
#define PLAUSE_FAST_FORW_BACKW          0   //��ͣ���Ƿ���Ҫ������˹���(FAST_FORW_BACKW_ENΪ1ʱ��Ч)
#define PLAY_STARTMUSIC                 0   //�Ƿ񲥷ſ�������
#define DIR_SWITCH_EN                   0   //�Ƿ�֧���ļ����л�
#define FS_SCAN_SPEED_EN                0   //�Ƿ�ʹ�����ļ����ٹ���
#define REPEAT_AS_ONE                   0   //�Ƿ�ʹ���޶ϵ�ѭ�����Ź���


#define Emcode_Enable					1	// ������ʹ��	20180626	
#define BIT0							0x01
#define BIT1							0x02
#define BIT2							0x04
#define BIT3							0x08
#define BIT4							0x10
#define BIT5							0x20
#define BIT6							0x40
#define BIT7							0x80

//����ģʽ����ѡ��
#define PLAYMODE_NUM                    PLAYMODE_TWO


/*****************************************************************************
 * Task      : AUX����Ĺ��ܶ���
 *****************************************************************************/
#define AUX_EQ_EN                       0   //�Ƿ�ʹ�ô���EQ���ܵ�LINEIN (AX107X��֧��)


/*****************************************************************************
 * Task      : ����������Ĺ��ܶ���
 *****************************************************************************/
#define MIX_DEEP_EN                     0   //�Ƿ�������Ч����0Ϊ������0~10����1Ϊ����0~15��
#define LIMIT_MIX_NOISE_EN              0   //�Ƿ�ʹ�����������빦��


/*****************************************************************************
 * Task      : FM����Ĺ��ܶ���
 *****************************************************************************/
#define FM_SEMIAUTOMATIC_SEEK           0   //FM���Զ���̨����

//FM�Ƿ����50KHz������̨��Ŀǰֻ��QN8065��RTC6218���Ժ�50KHz������̨����Ҫ��������ʡ���񷽰�ʱ�����̨Ч��
#define FM_SEEK_50K_STEP                0

//FMʱ��Դѡ��
#define FM_CLOCK			            SHARE_CRYSTAL_32KHz

//FMģ��ѡ��,�����ӳ���ռ�򿪶��ٸ�FMоƬ
#define FM_RTC6218_EN                   0

/*****************************************************************************
 * Task      : RTC����Ĺ��ܶ���
 *****************************************************************************/
#define ALARM_EN                        0   //�Ƿ���Ҫ���幦��
#define ALARM_WAKEUP_EN                 0   //�Ƿ���Ҫ���ӻ���Ӳ������


/*****************************************************************************
 * Task      : USBDEV����Ĺ��ܶ���
 *****************************************************************************/
#define USB_MASS_SUPPORT                0   //�Ƿ�ʹ�ö���������
#define USB_SPK_SUPPORT                 0   //�Ƿ�ʹ��USBAUDIO����
#define USB_MIC_SUPPORT                 0   //�Ƿ�ʹ��USBMIC����
#define USB_HID_SUPPORT                 0   //�Ƿ�ʹ�ÿ�����������


/*****************************************************************************
 * Module    : ϵͳ����ϸ�ڶ���
 *****************************************************************************/
#define KEY_VOICE_EN                    0   //�Ƿ���Ҫ������
#define BATTERY_CHECK                   1   //�Ƿ���Ҫ��ص������
#define AUX_DET_EN                      0   //�Ƿ���ҪLineIn���
#define MIC_DET_EN                      0   //�Ƿ���ҪMic���
#define HP_DET_EN                       0   //�Ƿ���ҪHeadphone���
#define PLL_TUNE_EN                     0   //�Ƿ��PLL TUNE���ܣ��޾��񷽰���Ч��
#define ENERGY_DISP_EN                  0   //�Ƿ���Ҫ������ʾ(AX107X��֧��)

//����MUTE����ѡ��
#define MUTE_TYPE                       MUTE_HIGH

//��ʾ��ѡ��
#define DISP_TYPE                       DISP_LED_7P7S //DISP_LED_5C7S	 //  DISP_LED_7P7S //
#define DISP_Show_Device				0	// 1: ��ʾ�豸����, 0:����ʾ�豸���� "-1-" or "-2-"

//������ѡ��
#define SYS_MEM                         MEM_RTCRAM  //(AX107X��֧��MEM_INTERNAL)


/*****************************************************************************
 * Module    : DAC��AMUX������ö���
 *****************************************************************************/
#define DAC_FADE_EN                     1   //�Ƿ���ҪDAC���뵭��//����task_flash.c��task_flash_music_enter�����йص����뵭��

#define DAC_POWER_SPEED                 0   //�Ƿ���Ҫ����DAC��ʼ��ռ�õĿ���ʱ�䣬�򿪻�����100���ֽ�

#define USE_MULTI_BIT_DAC               1    //�Ƿ�ʹ��multi-bit DAC

#define DAC_BUF_EN                      0   //�Ƿ�ʹ��VCM BUF��ֻ��AX107X�Ͽ�ѡ), ������AX107x ���԰�����ѡȡ

//DAC��ʼ��ѡ��
#define DAC_INIT_TYPE                   DAC_VCM_WITH_CAP

//LINEINͨ��ѡ�� 
#define AMUX_LINEIN                     AMUX_CH_23 
 
//LINEIN ADC����ͨ��ѡ��(AUX EQʱʹ��)
#define AUADC_LINEIN                    AUADC_CH_AUX_LR

//FMͨ��ѡ��
#define AMUX_FM                         AMUX_CH_45

#define AMUX_FM_WITH_NO_CAP             0   //FM�����������Ƿ�ʡAMUX���ݣ���ʱֻ֧��107X

/*****************************************************************************
 * Module    : EQ���ܶ���
 *****************************************************************************/
#define EQ_FADE_EN                      0   //��EQ�Ƿ���Ҫ����
#define EQ_MODIFY_DB_EN                 0   //�Ƿ���Ҫ�޸�EQ��DBֵ����Ӧtbl_eq_db[6][9]
#define EQ_MODIFY_FILTER_EN             0   //�Ƿ���Ҫ�޸�EQ�˲���ϵ������Ӧeq_coeff[54]


/*****************************************************************************
 * Module    : �����ȶ���d
 *****************************************************************************/
#define USE_ADKEY                       0   //�Ƿ�ʹ��ADKEY
#define USE_ADKEY2                      1   //�Ƿ�ʹ�õڶ���ADKEY
#define USE_TOUCH_KEY                   0   //�Ƿ�ʹ�ô������� (FPGA�ϲ���ʹ��)
#define USE_KEDIR                       0   //�Ƿ�ʹ������һ���� (FPGA�ϲ���ʹ��)
#define USE_MIXIO_AD                    0   //�Ƿ�ʹ������һIO����ͨIO��AD (USE_KEDIRΪ0ʱ��Ч��FPGA�Ͽ���)


/*****************************************************************************
 * Module    : ����ң�صȶ���
 *****************************************************************************/
#define USE_IR                          0   //�Ƿ�ʹ�ú���ң��
#define USE_HW_IR                       0   //�Ƿ�ʹ��Ӳ��IR��Ϊ0ʱʹ��timer��׽�������⣨USE_IRΪ1ʱ��Ч��
#define USE_IR_NUM                      0   //�Ƿ�ʹ��ң�����ּ�

//IRͨ��ѡ��
#define IR_PORT                         IR_P03

//IR�û���ѡ�񣬿��Դ�һ������
#define IR_FF00_EN                      0   //�Ƿ�֧���û���0xFF00
#define IR_BF00_EN                      0   //�Ƿ�֧���û���0xBF00
#define IR_FD02_EN                      0   //�Ƿ�֧���û���0xFD02
#define IR_FE01_EN                      0   //�Ƿ�֧���û���0xFE01


/*****************************************************************************
 * Module    : ����һ���ܶ���
 *****************************************************************************/
//����һ��ѡ����
#define USE_KEDIR_TYPE                  KEDIR_KEY

//����һͨ��ѡ��
#define KEDIR_PORT                      KEDIR_P03 


/*****************************************************************************
 * Module    : �����������ܶ���
 *****************************************************************************/
#define TOUCH_KEY_0_EN                  0   //�Ƿ�ʹ��TK0   P10
#define TOUCH_KEY_1_EN                  0   //�Ƿ�ʹ��TK1   P11
#define TOUCH_KEY_2_EN                  0   //�Ƿ�ʹ��TK2   P12
#define TOUCH_KEY_3_EN                  0   //�Ƿ�ʹ��TK3   P13
#define TOUCH_KEY_4_EN                  0   //�Ƿ�ʹ��TK4   P14
#define TOUCH_KEY_5_EN                  0   //�Ƿ�ʹ��TK5   P01


/******************************************************************** *********
 * Module    : ��ػ����ܶ���
 *****************************************************************************/
#define SOFT_POWEROFF_EN                1   //�Ƿ����ػ�����(TASK_POWEROFF_ENΪ1ʱ����Ч)
#define POWERON_LONG_KEY                1   //�Ƿ�򿪳���POWER���������ܣ�0Ϊ�̰�POWER������
#define DCIN_DET_EN                     0   //�Ƿ���ҪDCIN���
#define SOFT_POWER_CTL_INIT()           P0DIR &= ~BIT(0),P0 &= ~BIT(0)	
#define SOFT_POWER_CTL_ON()             P0DIR &= ~BIT(0),P0 |= BIT(0)	
#define SOFT_POWER_CTL_OFF()            P0DIR &= ~BIT(0),P0 &= ~BIT(0)	 

/*****************************************************************************
 * Module    : �豸��IO�븴�õȶ���
 *****************************************************************************/
#define UDISK_DOUBLE_EN                 0    //�Ƿ�ʹ��˫UDISK����

#define SD_DOUBLE_EN                    0    //�Ƿ�ʹ��˫SD������

//sd0ѡ��һ��SDIO��0~3
#define SD_PORT                         2

//sd1ѡ��һ��SDIO
#define SD1_PORT                        2

//SD CLK������Ƿ��á�Ϊ0ʱ������Ҫ������SD����
#define USE_SD_CLK_DET                  0

//SD CMD��SPI DAT����
#define USE_SDCMD_IIC_SPIDAT            0

//I2C CLK��DATA�Ƿ���SD���á�Ϊ0ʱ����Ҫ������I2C CLK��DATA
#define USE_I2C_SD_MUX                  1

//USB HOST(����ȡU��)��PHY�ӿ�ѡ��0ΪP42 P43��1ΪP40 P41 ����UDISKʱ�˺�����ã�
#define USB_HOST_PHY                    1

//USB DEV(������PC)��PHY�ӿ�ѡ��0ΪP42 P43��1ΪP40 P41
#define USB_DEV_PHY                     1

//AUX���IO
#define AUX_CHECK_IO_SET
#define AUX_CHECK_PORT                  P3
#define AUX_CHECK_DIR                   P3DIR
#define AUX_CHECK_PU                    PUP3
#define AUX_CHECK_PIN                   1

//MIC���IO
#define MIC_CHECK_IO_SET
#define MIC_CHECK_PORT                  P1
#define MIC_CHECK_DIR                   P1DIR
#define MIC_CHECK_PU                    PUP1
#define MIC_CHECK_PIN                   3

//HEADPHONE���IO     //��������������P10(NET:LINE-IN_DET)�ĵ�P01(PHIN_ADC)  2017/05/09  ��Ⱥ�޸�
#define HP_CHECK_IO_SET
#define HP_CHECK_PORT                   P0
#define HP_CHECK_DIR                    P0DIR
#define HP_CHECK_PU                     PUP0
#define HP_CHECK_PIN                    6

//DCIN���IO
#define DCIN_CHECK_IO_SET
#define DCIN_CHECK_PORT                 P1
#define DCIN_CHECK_DIR                  P1DIR
#define DCIN_CHECK_PU                   PUP1
#define DCIN_CHECK_PIN                  0

//MUTE����IO       //������(NET:SHDN)�Ŀ����� P11 �ĵ� P00    2017/05/09  ��Ⱥ�޸�
#define MUTE_IO_SET
#define MUTE_PORT                       P0
#define MUTE_DIR                        P0DIR
#define MUTE_PIN                        0

/*****************************************************************************
 * Module    : LED���������
 *****************************************************************************/
//0=ʹ���ⲿ�������裬1=ʹ���ڲ���������
#define LED_7C7S_S0_DIR                 P2DIR
#define LED_7C7S_S0_PORT                P2
#define LED_7C7S_S0_PIN                 1 


#define LED_7C7S_S1_DIR                 P2DIR
#define LED_7C7S_S1_PORT                P2
#define LED_7C7S_S1_PIN                 2


#define LED_7C7S_S2_DIR                 P2DIR
#define LED_7C7S_S2_PORT                P2
#define LED_7C7S_S2_PIN                 3


#define LED_7C7S_S3_DIR                 P2DIR
#define LED_7C7S_S3_PORT                P2
#define LED_7C7S_S3_PIN                 4

#define LED_7C7S_S4_DIR                 P2DIR
#define LED_7C7S_S4_PORT                P2
#define LED_7C7S_S4_PIN                 5


#define LED_7C7S_S5_DIR                 P2DIR
#define LED_7C7S_S5_PORT                P2
#define LED_7C7S_S5_PIN                 6

#define LED_7C7S_S6_DIR                 P2DIR
#define LED_7C7S_S6_PORT                P2
#define LED_7C7S_S6_PIN                 7

//LED����IO
#define LED0_IO_SET						// P05
#define LED0_PORT                       P0
#define LED0_DIR                        P0DIR
#define LED0_PIN                        5
#define MOV_Carry_To_LED_O()		asm("MOV	P0.5 , C");		// LED 0 -->  P0.5 = Carry;

#define LED1_IO_SET						// P04
#define LED1_PORT                    	P0
#define LED1_DIR                      	P0DIR
#define LED1_PIN                      	4
#define MOV_Carry_To_LED_T()		asm("MOV	P0.4 , C");		// LED 1 -->  P0.4 = Carry;

#define LED_LIGHT1	   0x2A			// LED����ֵ
#define LED_LIGHT2	   0x7F
#define LED_LIGHT3	   0xD4


/*****************************************************************************
 * IO key����
 *****************************************************************************/
#define S1_PORT						P0			// P03
#define S1_BIT						3
#define S1_DIR						P0DIR
#define S1_PU						PUP0

#define S2_PORT						P0			// P06
#define S2_BIT						6
#define S2_DIR						P0DIR
#define S2_PU						PUP0

#define S3_PORT						P0			// P07	����  ʵ��ΪWKORTC
#define S3_BIT						7
#define S3_DIR						P0DIR
#define S3_PU						PUP0

/*****************************************************************************
 * Module    : ȷ��оƬѡ��
 *****************************************************************************/
#ifndef AX107X_TAG
#error "��ѡ��AX107X_Debug"
#endif

#endif

