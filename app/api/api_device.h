/*****************************************************************************
 * Module    : API
 * File      : api_device.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : �豸�������Ӧ�ӿ�
 *****************************************************************************/
#ifndef API_DEVICE_H
#define API_DEVICE_H

/*****************************************************************************
 * Module    : �豸�������ؽӿ�
 *****************************************************************************/

#define MEDIA_DEVICE_NUM        4   //�洢ý���豸����
#define MEDIA_DEVICE_MASK       ((1<<MEDIA_DEVICE_NUM)-1)

enum
{
    //�д洢ý����豸һ��Ҫ����ǰ�棬
    //�������豸��ֻ�ܷ���DEVICE_MIC���棬ͬʱ���ĺ�DEV_TOTAL�ͺ���device_init()
    DEVICE_SDMMC,			// 0 SD/MMC��	// ���SD��
    DEVICE_SDMMC1,			// 1 SD/MMC1��	// �ڽ�SD��, �� SPI-Flash (������2ѡ1)
    DEVICE_UDISK,			// 2 U��
    DEVICE_UDISK1,			// 3 UDisk1
#ifdef	TASK_TIPS_EN
	DEVICE_OTP,
#endif

    DEVICE_LINEIN,			//4 LineIn
    DEVICE_HP,				//5 headphone

    DEVICE_PC,				//6 ����PC
    DEVICE_MIC,				//7 MIC

    DEVICE_INVALID = 0xff	//��Ч���豸
};

enum
{
    DEVICE_IDLE,                //�豸�ޱ仯
    DEVICE_CHANGE,              //�豸�����仯
    DEVICE_REMOVE,              //�豸�Ƴ�
};

typedef struct
{
	u8 status;          //˲ʱ״̬
	u8 cnt;             //�ȶ�����
	u8 delay;           //������ʱ
	u8 stable;          //�ȶ�״̬
	u8 actived;         //�������
} type_dev;

typedef struct
{
	u8 dev_actived;                 //�豸����״̬
	u8 dev_aborted;                 //�豸��ֹ״̬����Ҫ��ʹ�ù����г�����豸��ֻ��ͨ������豸�����
	type_dev sd;                    //SD/MMC��
	type_dev sd1;                   //SD/MMC1��
	type_dev udisk;                 //����U��
	type_dev udisk1;                //udisk1
	type_dev line;                  //LINE
	type_dev ear;                   //���Ӷ���
	type_dev pc;                    //����PC
	type_dev mic;                   //����MIC
} type_dev_ctl;

extern type_dev_ctl dev_ctl;

void device_init(void);
bool device_insert(u8 dev_num);
bool device_remove(u8 dev_num);
bool device_is_online(u8 dev_num);                      //�ж��豸�Ƿ�����
bool device_need_activate(u8 dev_num);                  //�ж��Ƿ���Ҫ�����ʼ��
bool device_is_actived(u8 dev_num);                     //����豸�Ƿ��Ѿ������ʼ��
bool device_activate(u8 dev_num);                       //�����豸
bool device_set_testready(void);                        //����豸�Ƿ����ߣ���Ҫ��Զ�����
u8 device_get_actived(void);                            //��ȡ�Ѽ�����豸
u8 device_change_check(void);                           //��鲢�����豸
u8 device_get_actived_num(void);                        //��ȡ�Ѽ�����豸����


/*****************************************************************************
 * Module    : USB�����ؽӿ�
 *****************************************************************************/
//usb_connected�����ķ���ֵ
enum
{
    USB_CONNECTED_NONE,     //δ����
    USB_CONNECTED_PC,       //����PC����Device
    USB_CONNECTED_UDISK,    //���U�̣���Host
};

typedef struct
{
	u8 host_phy;
	u8 dev_phy;

	struct
	{
		u8 check_mode;       //�ж�USB������״��
		u8 check_cnt;        //���ģʽ�л�
	} usb_phy[2];

} type_usb_chk;

extern volatile IAR_XDATA_A type_usb_chk usb_chk;

//USB���ƽṹ��
typedef struct
{
	u8 error;            //USBͨ�Ŵ���

	struct
	{
		u16 vid;              //VID
		u16 pid;              //PID
		u16 ver;              //Version
	} info;

	//EP0���յ�����
	struct
	{
		u8 rtype;
		u8 request;
		u16 value;
		u16 index;
		u16 length;
	} cmd;

	u8 reserved[2];
} type_usb_ctrl;



extern type_dev_ctl dev_ctl;

typedef struct
{
	u8 group;
} type_sd_chk;

extern IAR_PDATA_A type_usb_ctrl usb_ctrl;
extern type_sd_chk  sd_chk;

void usb_switch_otg(u8 phy);
void usb_switch_host(u8 phy);
u8 usb_connected(u8 phy);

void usb_switch_otg_std(void);
void usb_switch_host_std(u8 phy);
u8 usb_connected_std(u8 phy);
u8 usb_connected_host_only_std(void);
u8 usb_connected_dev_only_std(void);
void usb_switch_stable_device_var(void);
void usb_switch_stable_device_std(u8 phy);
void usb_switch_stable_host_std(u8 phy);
void usb_switch_stable_host_var(void);
bool device_status_stable(u8 dev_num, u8 new_sta);      //�������ȴ��豸�ȶ�

#endif
