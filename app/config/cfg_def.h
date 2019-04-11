#ifndef __CFG_DEF_H__
#define __CFG_DEF_H__

//�Ѷ����û��б�
#define USER_TEST                       0           //FPGA���԰�
#define USER_AX2070_DEMO                1           //AX2070 LQFP48 DEMO
#define USER_AX2073_DEMO                2           //AX2073 SOP16 DEMO
#define USER_AX1070_DEMO                5           //AX1070 LQFP48 DEMO
#define USER_AX1071_DEMO                6           //AX1071 SOP28 DEMO
#define USER_AX1073_DEMO                7           //AX1073 SOP16 DEMO
#define USER_AX1076_DEMO                8           //AX1076 SSOP24 DEMO


//SPI Flashģʽѡ��
#define SPI_TWO_WIRES_MODE              0           //SPI 2��ģʽ
#define SPI_THREE_WIRES_MODE            1           //SPI 3��ģʽ



//����Ƶ��ѡ��
#define CRY_32K                         0
#define CRY_4M                          1
#define CRY_12M                         2
#define CRY_24M                         3
#define CRY_RC                          4           //�޾���
#define CRY_AUTO                        5           //�Զ��ж��Ƿ���32K���޾���

//��������ѡ��
#define CRY_1PIN                        0           //1pin mode(����һ�Ž�XOSCO��һ�Ž�GND)
#define CRY_2PIN                        1           //2pin mode(����һ�Ž�XOSCO��һ�Ž�XOSCI)

//������ѡ��
#define MEM_NO                          0
#define MEM_RTCRAM                      1           //RTCRAM����
#define MEM_INTERNAL                    2           //���ü���
#define MEM_E2PROM                      3           //EEPROM����
#define MEM_DEVICE                      4           //�豸����

//DAC��ʼ��ѡ��
#define DAC_VCM_WITH_CAP                0           //��VCM���ݵ�DAC��ʼ��
#define DAC_VCM_NO_CAP                  1           //��VCM���ݵ�DAC��ʼ��
#define DAC_SIMPLE                      2           //���׵�DAC��ʼ��,��������,ʱ���.����������,�ʺ���MUTE��Ӧ��

//FM������ʽ
#define USE_MODULE_CRYSTAL		        1           //ʹ��ģ���������
#define USE_IO_32KHZ_CLK		        2           //�����о���IO���32Kʱ��(����)��FM.IO��:P16/P15/P06/P32/P33/P34/P35
#define USE_IO_12MHZ_CLK		        3           //IO���12M(PLL��Ƶ)ʱ��(����)��FM.IO��:P16/P15/P06/P32/P33/P34/P35�������������޾��񷽰�
#define SHARE_CRYSTAL_32KHz		        4           //��MCU����32K����
#define SHARE_CRYSTAL_12MHz		        5           //��MCU����12M����δ����
#define SHARE_CRYSTAL_24MHz		        6           //��MCU����24M����δ����
#define USE_SD_IO_32KHZ_CLK             7           //SDCLK���32Kʱ�ӣ�SDCLK���ܸ��ü��,��Ҫ���������޾��񷽰�������QN8065ʱIIC������SD����
#define USE_SD_IO_12MHZ_CLK             8           //SDCLK���12Mʱ�ӣ�SDCLK���ܸ��ü��,��Ҫ���������޾��񷽰�������QN8065ʱIIC������SD����

//repeatģʽѡ��
#define PLAYMODE_FOUR                   1           //�ĸ�����ģʽ(ALL��RAMDOM��SINGLE��FOLDER)
#define PLAYMODE_TWO                    2           //��������ģʽ(ALL��SINGLE)

//��ʾ��ѡ��
#define DISP_NO                         0
#define DISP_LED_5C7S                   1           //LED�������ʾ
#define DISP_LCD_SEG                    2           //LCD��������ʾ
#define DISP_LED                        3           //LED��ʾ
#define DISP_LED_7P7S                   4           //LED7���������ʾ

//����һ����ѡ��
#define KEDIR_KEY_IR_LED                0           //����&����&LEDȫ����
#define KEDIR_KEY_IR                    1           //���ð���&����
#define KEDIR_KEY_LED                   2           //���ð���&LED
#define KEDIR_IR_LED                    3           //���ú���&LED
#define KEDIR_KEY                       4           //ֻ��ADKEYʹ��

//����һIO��ѡ��
#define KEDIR_P10                       0           //P10
#define KEDIR_P03                       1           //P03
#define KEDIR_P02                       2           //P02
#define KEDIR_P27                       3           //P27

//����IOѡ��
#define IR_P10                          0           //P10
#define IR_P03                          1           //P03
#define IR_P02                          2           //P02
#define IR_P27                          3           //P27

//���ʱ��IO��ѡ��
#define CLK_P16                         1           //P16
#define CLK_P15                         2           //P15
#define CLK_P06                         3           //P06
#define CLK_P32                         4           //P32
#define CLK_P33                         5           //P33
#define CLK_P34                         6           //P34
#define CLK_P35                         7           //P35

//USB���ѡ��
#define USBDET_OTG                      0           //���Ӽ��
#define USBDET_HOST_ONLY                1           //ֻ��USB HOST���
#define USBDET_DEV_ONLY                 2           //ֻ��USB DEVICE���

#define MUTE_NO                         0			// �� MUTE����
#define MUTE_LOW                        1			// Low MUTE, High UNMUTE
#define MUTE_HIGH                       2			// High MUTE, Low UNMUTE
#define MUTE_DIGITAL					3			// Low MUTE, High _-_---- UNMUTE
#endif
