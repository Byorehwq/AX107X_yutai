/*****************************************************************************
 * Module    : Config
 * File      : config_default.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : Ĭ���û��������ã����Զ�ѡ���û�û�����õ�ѡ��
 *****************************************************************************/
#ifndef CONFIG_DEFAULT_H
#define CONFIG_DEFAULT_H


/*****************************************************************************
 *  Module    : SRAM�����ѡ��
 *****************************************************************************/
/*��ע�⡿��
  1.ʹ��SRAM�壨NO.13-844������ǰ����Ҫ�ֶ���дpll trimֵ������һtrimֵ��
    ÿһ��SRAM���trimֵ�ǲ�ͬ�ģ������ڰ��ϵ���ֽ�ҵ���Щ��ֵ;
  2.SRAM����ʹ��uart����ר��IO,������ʹ��P35,ֻҪ�ڴ�����'UART_JUMP'��������;
*/
//#ifndef SRAM_DEBUG
//#define SRAM_DEBUG                      0   //(def)�Ƿ���SRAM����Թ���
//#endif

//pll trimֵ
#ifndef PLL_INTL
#define PLL_INTL                        0x05
#endif

#ifndef PLL_FRACL
#define PLL_FRACL                       0x00
#endif

#ifndef PLL_FRACH
#define PLL_FRACH                       0x80
#endif

//����һtrimֵ
#ifndef LED_TRIM
#define LED_TRIM                        0x20
#endif


/*****************************************************************************
 * Module    : ϵͳ����ѡ��
 *****************************************************************************/
#ifndef PRINTF_EN
#define PRINTF_EN                       1   //(def)�Ƿ���UART��ӡ����
#endif


/*****************************************************************************
 * Module    : ϵͳ����ѡ��
 *****************************************************************************/
#ifndef SYS_CRY
#define SYS_CRY                         CRY_32K
#endif

#ifndef CRY_PIN_NUM
#define CRY_PIN_NUM                     CRY_2PIN
#endif

/*****************************************************************************
 * Task      : ϵͳ���񼰹����������
 *****************************************************************************/
#ifndef TASK_AUX_EN
#define TASK_AUX_EN                     1   //(def)�Ƿ��LINEIN����
#endif

#ifndef TASK_FM_EN
#define TASK_FM_EN                      1   //(def)�Ƿ��FM����
#endif

#ifndef TASK_SPEAKER_EN
#define TASK_SPEAKER_EN                 1   //(def)�Ƿ������������
#endif

#ifndef TASK_RTC_EN
#define TASK_RTC_EN                     1   //(def)�Ƿ��RTC����
#endif

#ifndef TASK_USBDEV_EN
#define TASK_USBDEV_EN                  1   //(def)�Ƿ��USBDEV����
#endif

#ifndef TASK_POWEROFF_EN
#define TASK_POWEROFF_EN                1   //(def)�Ƿ�򿪹ػ�����
#endif

#ifndef TASK_LOWPOWER_EN
#define TASK_LOWPOWER_EN                1   //(def)�Ƿ�򿪵͹���ģʽ����
#endif

#ifndef TASK_IDLE_EN
#define TASK_IDLE_EN                    1   //(def)�Ƿ�򿪿��й���
#endif

#ifndef TASK_FMAM_DETECT_EN
#define TASK_FMAM_DETECT_EN             0   //(def)�Ƿ��FM/AMƵ�ʼ�⹦��(����AX207X��֧��)
#endif


/*****************************************************************************
 * Task      : ���ֲ�������Ĺ��ܶ���
 *****************************************************************************/
#ifndef WAV_SUPPORT
#define WAV_SUPPORT                     0   //(def)�Ƿ�֧��WAV����
#endif

#ifndef MPX_SUPPORT 						
#define MPX_SUPPORT 					0	//(def)�Ƿ�֧��MPX
#endif
#ifndef MUSIC_SWITCH_DEV
#define MUSIC_SWITCH_DEV                1   //(def)�Ƿ�֧��ģʽ���в����豸
#endif

#ifndef AUTO_CHANGE_DEVICE
#define AUTO_CHANGE_DEVICE              1   //(def)��һ�������һ��ʱ�Զ����豸
#endif

#ifndef AB_REPEAT_EN
#define AB_REPEAT_EN                    0   //(def)�Ƿ�ʹ��AB��������
#endif

#ifndef KARAOKE_EN
#define KARAOKE_EN                      0   //(def)�Ƿ�ʹ�ÿ���OK����(MIC_DET_ENΪ1ʱ��Ч,��֧�ֻ���Ч��)
#endif

#ifndef USE_BREAK_POINT
#define USE_BREAK_POINT                 1   //(def)�Ƿ�ʹ�öϵ����
#endif

#ifndef MUSIC_STOP
#define MUSIC_STOP                      0   //(def)�Ƿ�ʹ��STOP����
#endif

#ifndef MUSIC_PLAYMODE
#define MUSIC_PLAYMODE                  1   //(def)�Ƿ����Ҫ����PLAYMODE
#endif

#ifndef MUSIC_EQ
#define MUSIC_EQ                        1   //(def)�Ƿ����Ҫ����EQ
#endif

#ifndef FAST_FORW_BACKW_EN
#define FAST_FORW_BACKW_EN              1   //(def)�Ƿ�ʹ�ÿ�����˹���
#endif

#ifndef PLAUSE_FAST_FORW_BACKW
#define PLAUSE_FAST_FORW_BACKW          0   //(def)��ͣ���Ƿ���Ҫ������˹���(FAST_FORW_BACKW_ENΪ1ʱ��Ч)
#endif

#ifndef PLAY_STARTMUSIC
#define PLAY_STARTMUSIC                 0   //(def)�Ƿ񲥷ſ�������
#endif

#ifndef DIR_SWITCH_EN
#define DIR_SWITCH_EN                   0   //(def)�Ƿ�֧���ļ����л�
#endif

#ifndef FS_SCAN_SPEED_EN
#define FS_SCAN_SPEED_EN                1   //(def)�Ƿ�ʹ�����ļ����ٹ���
#endif

//����ģʽ����ѡ��
#ifndef PLAYMODE_NUM
#define PLAYMODE_NUM                    PLAYMODE_FOUR
#endif


/*****************************************************************************
 * Task      : AUX����Ĺ��ܶ���
 *****************************************************************************/
#ifndef AUX_EQ_EN
#define AUX_EQ_EN                       0   //(def)�Ƿ�ʹ�ô���EQ���ܵ�LINEIN
#endif


/*****************************************************************************
 * Task      : ����������Ĺ��ܶ���
 *****************************************************************************/
#ifndef MIX_DEEP_EN
#define MIX_DEEP_EN                     0   //(def)�Ƿ�������Ч����0Ϊ������0~10����1Ϊ����0~15��
#endif

#ifndef LIMIT_MIX_NOISE_EN
#define LIMIT_MIX_NOISE_EN              1   //(def)�Ƿ�ʹ�����������빦��
#endif


/*****************************************************************************
 * Task      : FM����Ĺ��ܶ���
 *****************************************************************************/
#ifndef FM_SEMIAUTOMATIC_SEEK
#define FM_SEMIAUTOMATIC_SEEK           0   //(def)FM���Զ���̨����
#endif

//FM�Ƿ����50KHz������̨��Ŀǰֻ��QN8065��RTC6218���Ժ�50KHz������̨����Ҫ��������ʡ���񷽰�ʱ�����̨Ч��
#ifndef FM_SEEK_50K_STEP
#define FM_SEEK_50K_STEP                0	//(def)
#endif

#ifndef FM_CLOCK
#define FM_CLOCK			            USE_MODULE_CRYSTAL  //(def)FMʱ��Դѡ��
#endif

#ifndef FM_CLK_IO
#define FM_CLK_IO                       CLK_P34  //(def)�ṩʱ�ӵ�IOѡ��
#endif

#ifndef AMUX_FM_EN
#define AMUX_FM_EN                      1        //(def)FM�����Ƿ��AMUX 1:������ 0����������
#endif

//FMģ��ѡ��,�����ӳ���ռ�򿪶��ٸ�FMоƬ
#ifndef FM_RDA5807_EN
#define FM_RDA5807_EN                   0		//(def)
#endif
#ifndef FM_BK1080_EN
#define FM_BK1080_EN                    0		//(def)
#endif
#ifndef FM_QN8035_EN
#define FM_QN8035_EN                    0		//(def)
#endif
#ifndef FM_QN8065_EN
#define FM_QN8065_EN                    0		//(def)
#endif
#ifndef FM_KT0830_EN
#define FM_KT0830_EN                    0		//(def)
#endif
#ifndef FM_SI484X_EN
#define FM_SI484X_EN                    0   //(def)δ����
#endif
#ifndef FM_AR1010_EN
#define FM_AR1010_EN                    0   //(def)δ����
#endif
#ifndef FM_AR1019_EN
#define FM_AR1019_EN                    0   //(def)δ����
#endif
#ifndef FM_CL6017S_EN
#define FM_CL6017S_EN                   0   //(def)δ����
#endif
#ifndef FM_CL6017G_EN
#define FM_CL6017G_EN                   0   //(def)δ����
#endif

#ifndef FM_RTC6218_EN
#define FM_RTC6218_EN                   0	//(def)
#endif

#if FM_RDA5807_EN
#define RDA5807_PE_SP                   1   //(def)PE/SP�汾������ʱ3�ֶ���Ҫ���ռ䣬��дʱ��3�ֶ���
#define RDA5807_HS_HP                   0   //(def)HS/HP�汾
#define RDA5807_M_MP                    0   //(def)M/MP�汾
#endif

#if FM_SI484X_EN
#define IIC_DATAFILL_EN                 1   //(def)ʹ��SIоƬʱ�����Դ򿪴˺꽵�Ͷ�FM�ĸ���
#define USE_CAPTURE_MODE                0   //(def)ʹ��timer0 Capture���ܻ�ȡSI��IRQ�ź�
#define USE_PORT_INTERRUPT              1   //(def)ʹ���ⲿ�жϻ�ȡSI��IRQ
#endif

/*
�򿪸ú����ʡ��FM AMUX���ݣ�������FM���ֱ�������ص��µ�����������ʱҪ�Ȱ�DAC trim��B����FM��ֵ
��������������trim��A����ʼ��ֵ�����µ��������ٶȹ������ʴ򿪸ú���FMģʽ��ͨ������FM����������������
����DAC trim��Ҫʱ��������������뵭����ʱ�䣬��ʱ����FM���ֱ��ƫ�ú�AMUX����ֱ��ƫ�����������
*/
#ifndef AMUX_FM_WITH_NO_CAP
#define AMUX_FM_WITH_NO_CAP             0   //(def)FM�����������Ƿ�ʡAMUX���ݣ���ʱֻ֧��107X
#endif

/*****************************************************************************
 * Task      : RTC����Ĺ��ܶ���
 *****************************************************************************/
#ifndef ALARM_EN
#define ALARM_EN                        0   //(def)�Ƿ���Ҫ���幦��
#endif

#ifndef ALARM_WAKEUP_EN
#define ALARM_WAKEUP_EN                 0   //(def)�趨���Ӻ���ػ���������ʱ�䵽��WKORTC������ߵ�ƽ����֧��Ӳ������
#endif

/*****************************************************************************
 * Task      : USBDEV����Ĺ��ܶ���
 *****************************************************************************/
#ifndef USB_MASS_SUPPORT
#define USB_MASS_SUPPORT                1   //(def)�Ƿ�ʹ�ö���������
#endif

#ifndef USB_SPK_SUPPORT
#define USB_SPK_SUPPORT                 1   //(def)�Ƿ�ʹ��USBAUDIO����
#endif

#ifndef USB_MIC_SUPPORT
#define USB_MIC_SUPPORT                 1   //(def)�Ƿ�ʹ��USBMIC����
#endif

#ifndef USB_HID_SUPPORT
#define USB_HID_SUPPORT                 1   //(def)�Ƿ�ʹ�ÿ�����������
#endif


/*****************************************************************************
 * Module    : ϵͳ����ϸ�ڶ���
 *****************************************************************************/
#ifndef KEY_VOICE_EN
#define KEY_VOICE_EN                    1   //(def)�Ƿ���Ҫ������
#endif

//#ifndef BATTERY_CHECK
//#define BATTERY_CHECK                   0   //(def)�Ƿ���Ҫ��ص������
//#endif

#ifndef AUX_DET_EN
#define AUX_DET_EN                      1   //(def)�Ƿ���ҪLineIn���
#endif

#ifndef MIC_DET_EN
#define MIC_DET_EN                      1   //(def)�Ƿ���ҪMic���
#endif

#ifndef HP_DET_EN
#define HP_DET_EN                       0   //(def)�Ƿ���ҪHeadphone���
#endif

#ifndef MUTE_TYPE
#define MUTE_TYPE                       MUTE_NO //(def)����MUTE����ѡ��
#endif

#ifndef PLL_TUNE_EN
#define PLL_TUNE_EN                     0   //(def)�Ƿ��PLL TUNE���ܣ��޾��񷽰���Ч��
#endif

#ifndef ENERGY_DISP_EN
#define ENERGY_DISP_EN                  0   //(def)�Ƿ���Ҫ������ʾ
#endif

#ifndef PA_MODE_SEL_EN
#define PA_MODE_SEL_EN                  0   //(def)�Ƿ�ΪD/AB�๦��(��XPT8871)
#endif

//��ʾ��ѡ��
#ifndef DISP_TYPE
#define DISP_TYPE                       DISP_LED_5C7S	//(def)
#endif

//������ѡ��
#ifndef SYS_MEM
#define SYS_MEM                         MEM_RTCRAM		//(def)
#endif


/*****************************************************************************
 * Module    : DAC��AMUX������ö���
 *****************************************************************************/
#ifndef USE_MULTI_BIT_DAC
#define USE_MULTI_BIT_DAC               0   //(def)�Ƿ�ʹ��multi-bit DAC
#endif

#ifndef DAC_FADE_EN
#define DAC_FADE_EN                     0   //(def)�Ƿ���ҪDAC���뵭��
#endif

#ifndef DAC_POWER_SPEED
#define DAC_POWER_SPEED                 0   //(def)�Ƿ���Ҫ����DAC��ʼ��ռ�õĿ���ʱ�䣬�򿪻�����100���ֽ�
#endif

#ifndef DAC_BUF_EN
//��ʡDAC������ݣ����������Ҫʹ��ADC��⣬���ڲ�����10K��
#define DAC_BUF_EN                      0   //(def)�Ƿ�ʹ��VCM BUF��ֻ��AX107X�Ͽ�ѡ)
#endif

#if DAC_BUF_EN
#undef DAC_POWER_SPEED
#define DAC_POWER_SPEED                 0	//(def)
#endif

//DAC��ʼ��ѡ��
#ifndef DAC_INIT_TYPE
#define DAC_INIT_TYPE                   DAC_VCM_WITH_CAP	//(def)
#endif

#ifndef VDDHP_VDDIO_SHT
#define VDDHP_VDDIO_SHT                 0   //(def)VDDHP��VDDIO�Ƿ��һ��
#endif

#ifndef DACR_ONLY
#define DACR_ONLY                       0   //(def)DAC�ϲ���DACR���������
#endif

//LINEINͨ��ѡ��
#ifndef AMUX_LINEIN
#define AMUX_LINEIN                     AMUX_CH_23	//(def)
#endif

//LINEIN ADC����ͨ��ѡ��(AUX EQʱʹ��)
#ifndef AUADC_LINEIN
#define AUADC_LINEIN                    AUADC_CH_AUX_LR	//(def)
#endif

//FMͨ��ѡ��
#ifndef AMUX_FM
#define AMUX_FM                         AMUX_CH_45	//(def)
#endif


/*****************************************************************************
 * Module    : EQ���ܶ���
 *****************************************************************************/
#ifndef EQ_FADE_EN
#define EQ_FADE_EN                      1   //(def)��EQ�Ƿ���Ҫ����
#endif

#ifndef EQ_MODIFY_DB_EN
#define EQ_MODIFY_DB_EN                 0   //(def)�Ƿ���Ҫ�޸�EQ��DBֵ����Ӧtbl_eq_db[6][9]
#endif

#ifndef EQ_MODIFY_FILTER_EN
#define EQ_MODIFY_FILTER_EN             0   //(def)�Ƿ���Ҫ�޸�EQ�˲���ϵ������Ӧeq_coeff[54]
#endif


/*****************************************************************************
 * Module    : �����ȶ���
 *****************************************************************************/
#ifndef USE_ADKEY
#define USE_ADKEY                       1   //(def)�Ƿ�ʹ��ADKEY
#endif

#ifndef USE_TOUCH_KEY
#define USE_TOUCH_KEY                   0   //(def)�Ƿ�ʹ�ô������� (FPGA�ϲ���ʹ��)
#endif

#ifndef USE_KEDIR
#define USE_KEDIR                       0   //(def)�Ƿ�ʹ������һ���� (FPGA�ϲ���ʹ��)
#endif

#ifndef USE_MIXIO_AD
#define USE_MIXIO_AD                    0   //(def)�Ƿ�ʹ������һIO����ͨIO��AD (USE_KEDIRΪ0ʱ��Ч��FPGA�Ͽ���)
#endif


/*****************************************************************************
 * Module    : ����ң�صȶ���
 *****************************************************************************/
#ifndef USE_IR
#define USE_IR                          1   //(def)�Ƿ�ʹ�ú���ң��
#endif

#ifndef USE_HW_IR
#define USE_HW_IR                       1   //(def)�Ƿ�ʹ��Ӳ��IR��Ϊ0ʱʹ��timer��׽�������⣨USE_IRΪ1ʱ��Ч��
#endif

#ifndef USE_IR_NUM
#define USE_IR_NUM                      1   //(def)�Ƿ�ʹ��ң�����ּ�
#endif

//IRͨ��ѡ��
#ifndef IR_PORT
#define IR_PORT                         IR_P10	//(def)
#endif

//IR�û���ѡ�񣬿��Դ�һ������
#ifndef IR_FF00_EN
#define IR_FF00_EN                      1   //(def)�Ƿ�֧���û���0xFF00
#endif

#ifndef IR_BF00_EN
#define IR_BF00_EN                      0   //(def)�Ƿ�֧���û���0xBF00
#endif

#ifndef IR_FD02_EN
#define IR_FD02_EN                      0   //(def)�Ƿ�֧���û���0xFD02
#endif

#ifndef IR_FE01_EN
#define IR_FE01_EN                      0   //(def)�Ƿ�֧���û���0xFE01
#endif


/*****************************************************************************
 * Module    : ����һ���ܶ���
 *****************************************************************************/
//����һ��ѡ����
#ifndef USE_KEDIR_TYPE
#define USE_KEDIR_TYPE                  KEDIR_KEY_IR_LED	//(def)
#endif

//����һͨ��ѡ��
#ifndef KEDIR_PORT
#define KEDIR_PORT                      KEDIR_P02	//(def)
#endif


/*****************************************************************************
 * Module    : �����������ܶ���
 *****************************************************************************/
#ifndef TOUCH_KEY_0_EN
#define TOUCH_KEY_0_EN                  1   //(def)�Ƿ�ʹ��TK0
#endif

#ifndef TOUCH_KEY_1_EN
#define TOUCH_KEY_1_EN                  1   //(def)�Ƿ�ʹ��TK1
#endif

#ifndef TOUCH_KEY_2_EN
#define TOUCH_KEY_2_EN                  1   //(def)�Ƿ�ʹ��TK2
#endif

#ifndef TOUCH_KEY_3_EN
#define TOUCH_KEY_3_EN                  1   //(def)�Ƿ�ʹ��TK3
#endif

#ifndef TOUCH_KEY_4_EN
#define TOUCH_KEY_4_EN                  1   //(def)�Ƿ�ʹ��TK4
#endif

#ifndef TOUCH_KEY_5_EN
#define TOUCH_KEY_5_EN                  0   //(def)�Ƿ�ʹ��TK5
#endif


/*****************************************************************************
 * Module    : ��ػ����ܶ���
 *****************************************************************************/
//#ifndef SOFT_POWEROFF_EN
//#define SOFT_POWEROFF_EN                0   //(def)�Ƿ����ػ�����
//#endif

//#ifndef POWERON_LONG_KEY
//#define POWERON_LONG_KEY                1   //(def)�Ƿ�򿪳���POWER����������
//#endif

#ifndef DCIN_DET_EN
#define DCIN_DET_EN                     0   //(def)�Ƿ���ҪDCIN���
#endif


/*****************************************************************************
 * Module    : �豸��IO�븴�õȶ���
 *****************************************************************************/
#ifndef UDISK_DOUBLE_EN
#define UDISK_DOUBLE_EN                 1	//(def)
#endif

#ifndef SD_DOUBLE_EN
#define SD_DOUBLE_EN                    1	//(def)
#endif

//ѡ��һ��SDIO��0~3
#ifndef SD_PORT
#define SD_PORT                         0	//(def)
#endif

//ѡ��һ��SDIO��0~3
#ifndef SD1_PORT
#define SD1_PORT                        1	//(def)
#endif

//SD CLK������Ƿ��á�Ϊ0ʱ������Ҫ������SD����
#ifndef USE_SD_CLK_DET
#define USE_SD_CLK_DET                  1	//(def)
#endif

//SD CLK��SD��⡢������⸴��,��ADC����ʵ�֡�Ϊ0ʱ����Ҫ�����Ķ�������
#ifndef USE_SDCLK_SDDET_PHONE_MUX
#define USE_SDCLK_SDDET_PHONE_MUX       0	//(def)
#endif

//SD CMD��IIC CLK��SD��⸴�� (FPGA�ϲ���ʹ��)
#ifndef USE_SDCMD_IICCLK_SDDET_MUX
#define USE_SDCMD_IICCLK_SDDET_MUX      0	//(def)
#endif

//SD CMD��IIC CLK��MIC��⸴�� (FPGA�ϲ���ʹ��)
#ifndef USE_SDCMD_IICCLK_MIC_MUX
#define USE_SDCMD_IICCLK_MIC_MUX        0	//(def)
#endif

//SD CMD��IIC CLK��PHONE��⸴�� (FPGA�ϲ���ʹ��)
#ifndef USE_SDCMD_IICCLK_PHONE_MUX
#define USE_SDCMD_IICCLK_PHONE_MUX      0	//(def)
#endif

//����7P_LED����IO����LINEIN ���
#ifndef USE_7P7S_LED_AUX_MUX
#define USE_7P7S_LED_AUX_MUX            0	//(def)
#endif

//I2C CLK��DATA�Ƿ���SD���á�Ϊ0ʱ����Ҫ������I2C CLK��DATA
#ifndef USE_I2C_SD_MUX
#define USE_I2C_SD_MUX                  1	//(def)
#endif

//USB HOST(����ȡU��)��PHY�ӿ�ѡ��0ΪP42 P43��1ΪP40 P41 (FPGA�ϲ���ʹ�õ�0��)
#ifndef USB_HOST_PHY
#define USB_HOST_PHY                    1	//(def)
#endif

//USB DEV(������PC)��PHY�ӿ�ѡ��0ΪP42 P43��1ΪP40 P41 (FPGA�ϲ���ʹ�õ�0��)
#ifndef USB_DEV_PHY
#define USB_DEV_PHY                     1	//(def)
#endif

//USB���ѡ��
#ifndef USB_DET
#define USB_DET                         USBDET_OTG	//(def)
#endif

//AUX���IO
#ifndef AUX_CHECK_IO_SET
#define AUX_CHECK_PORT                  P1	//(def)
#define AUX_CHECK_DIR                   P1DIR	//(def)
#define AUX_CHECK_PU                    PUP1	//(def)
#define AUX_CHECK_PIN                   4	//(def)
#endif

//MIC���IO
#ifndef MIC_CHECK_IO_SET
#define MIC_CHECK_PORT                  P2	//(def)
#define MIC_CHECK_DIR                   P2DIR	//(def)
#define MIC_CHECK_PU                    PUP2	//(def)
#define MIC_CHECK_PIN                   7	//(def)
#endif

//HEADPHONE���IO                //��������������P10(NET:LINE-IN_DET)�ĵ�P01(PHIN_ADC)  2017/05/09  ��Ⱥ�޸�
#ifndef HP_CHECK_IO_SET
#define HP_CHECK_PORT                   P0//(def)
#define HP_CHECK_DIR                    P0DIR//(def)
#define HP_CHECK_PU                     PUP0//(def)
#define HP_CHECK_PIN                    1//(def)
#endif

//DCIN���IO
#ifndef DCIN_CHECK_IO_SET
#define DCIN_CHECK_PORT                 P1	//(def)
#define DCIN_CHECK_DIR                  P1DIR	//(def)
#define DCIN_CHECK_PU                   PUP1	//(def)
#define DCIN_CHECK_PIN                  0	//(def)
#endif

//MUTE����IO
#ifndef MUTE_IO_SET
#define MUTE_PORT                       P0	//(def)
#define MUTE_DIR                        P0DIR	//(def)
#define MUTE_PIN                        0	//(def)
#endif


/*****************************************************************************
 * Module    : LED���������
 *****************************************************************************/
//0=ʹ���ⲿ�������裬1=ʹ���ڲ���������
#ifndef LED_5C7S_EXT_RES_EN
#define LED_5C7S_EXT_RES_EN             0	//(def)
#endif

#ifndef LED_5C7S_IO_SET
#define LED_5C7S_C0_DIR                 P0DIR
#define LED_5C7S_C1_DIR                 P0DIR
#define LED_5C7S_C2_DIR                 P2DIR
#define LED_5C7S_C3_DIR                 P0DIR
#define LED_5C7S_C4_DIR                 P0DIR

#define LED_5C7S_C0_PORT                P4
#define LED_5C7S_C1_PORT                P0
#define LED_5C7S_C2_PORT                P0
#define LED_5C7S_C3_PORT                P0
#define LED_5C7S_C4_PORT                P0

#define LED_5C7S_C0_PIN                 3
#define LED_5C7S_C1_PIN                 4
#define LED_5C7S_C2_PIN                 5
#define LED_5C7S_C3_PIN                 6
#define LED_5C7S_C4_PIN                 7

#define LED_5C7S_S_DN_SEL               PDN2L
#define LED_5C7S_S_DN_EN                PDN2
#define LED_5C7S_S_DIR                  P2DIR
#define LED_5C7S_S_PORT                 P2  //(P20~P26)
#endif


/*****************************************************************************
** ������ЩΪ�Զ�ѡ��ĺ꣬�벻Ҫ�޸�
 *****************************************************************************/
#if (!TASK_POWEROFF_EN)
#undef  SOFT_POWEROFF_EN
#define SOFT_POWEROFF_EN            0	//(def)
#endif
#if (!SOFT_POWEROFF_EN)
#undef  DCIN_DET_EN
#define DCIN_DET_EN                 0	//(def)
#endif

#if (!FAST_FORW_BACKW_EN)
#undef  PLAUSE_FAST_FORW_BACKW
#define PLAUSE_FAST_FORW_BACKW      0	//(def)
#endif

#if (!TASK_AUX_EN)
#undef  AUX_DET_EN
#define AUX_DET_EN                  0	//(def)
#endif

#if (!TASK_SPEAKER_EN && !KARAOKE_EN)
#undef  MIC_DET_EN
#define MIC_DET_EN                  0	//(def)
#endif

#if ( ((FM_CLOCK == USE_IO_32KHZ_CLK) || (FM_CLOCK == USE_IO_12MHZ_CLK))\
    && (((FM_CLK_IO == CLK_P16) && (SD_PORT == 0)) || ((FM_CLK_IO == CLK_P06) && (SD_PORT == 3))) )
//����SD_CLK��IO���ʱ�Ӹ�FM
#define FM_CLK_IO_MUX_SDCLK         1	//(def)
#else
#define FM_CLK_IO_MUX_SDCLK         0	//(def)
#endif

#if (!TASK_FM_EN)
#undef  AMUX_FM_EN
#undef  FM_RDA5807_EN
#undef  FM_BK1080_EN
#undef  FM_QN8035_EN
#undef  FM_QN8065_EN
#undef  FM_KT0830_EN
#undef  FM_SI484X_EN
#undef  FM_AR1010_EN
#undef  FM_AR1019_EN
#undef  FM_CL6017S_EN
#undef  FM_CL6017G_EN
#undef  FM_RTC6218_EN
#define AMUX_FM_EN                  0
#define FM_RDA5807_EN               0
#define FM_BK1080_EN                0
#define FM_QN8035_EN                0
#define FM_QN8065_EN                0
#define FM_KT0830_EN                0
#define FM_SI484X_EN                0
#define FM_AR1010_EN                0
#define FM_AR1019_EN                0
#define FM_CL6017S_EN               0
#define FM_CL6017G_EN               0
#define FM_RTC6218_EN               0

#undef FM_CLK_IO_MUX_SDCLK
#define FM_CLK_IO_MUX_SDCLK         0	//(def)
#endif

#if (!TASK_RTC_EN)
#undef  ALARM_EN
#define ALARM_EN                    0	//(def)
#endif

#if (!ALARM_EN)
#undef  ALARM_WAKEUP_EN
#define ALARM_WAKEUP_EN             0	//(def)
#endif

#if (!TASK_USBDEV_EN)
#undef  USB_MASS_SUPPORT
#undef  USB_SPK_SUPPORT
#undef  USB_MIC_SUPPORT
#define USB_MASS_SUPPORT            0   //(def)����������
#define USB_SPK_SUPPORT             0   //(def)USBAUDIO����
#define USB_MIC_SUPPORT             0   //(def)USBMIC����

#undef USB_DET
#define USB_DET                     USBDET_HOST_ONLY        //(def)û��USB DEVICEģʽ��ֻ����HOST���
#endif

#if (!USB_SPK_SUPPORT)
#undef  USB_HID_SUPPORT
#define USB_HID_SUPPORT             0   //(def)������������
#endif


#if (SYS_MEM == MEM_INTERNAL)
#ifndef MEM_WRITE_COMPARE
#define MEM_WRITE_COMPARE           0	//(def)
#endif
#endif

#if (!USE_IR)
#undef  USE_HW_IR
#undef  USE_IR_NUM
#define USE_HW_IR                   0	//(def)
#define USE_IR_NUM                  0	//(def)
#endif

#if MIC_DET_EN
#undef  TOUCH_KEY_3_EN
#define TOUCH_KEY_3_EN              0	//(def)
#endif

#if HP_DET_EN
#undef  TOUCH_KEY_4_EN
#define TOUCH_KEY_4_EN              0	//(def)
#endif

#if (DISP_TYPE == DISP_LED_7P7S)
#undef  LED_7P7S_SCAN_HALF_MS
#define LED_7P7S_SCAN_HALF_MS       1   //(def0LED7���������0.125msɨ��
#else
#undef  LED_7P7S_SCAN_HALF_MS
#define LED_7P7S_SCAN_HALF_MS       0	//(def)
#endif

#endif
