/*****************************************************************************
 * Module    : API
 * File      : api_device.h
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : 设备管理的相应接口
 *****************************************************************************/
#ifndef API_DEVICE_H
#define API_DEVICE_H

/*****************************************************************************
 * Module    : 设备管理的相关接口
 *****************************************************************************/

#define MEDIA_DEVICE_NUM        4   //存储媒介设备数量
#define MEDIA_DEVICE_MASK       ((1<<MEDIA_DEVICE_NUM)-1)

enum
{
    //有存储媒介的设备一定要放在前面，
    //新增加设备，只能放在DEVICE_MIC后面，同时更改宏DEV_TOTAL和函数device_init()
    DEVICE_SDMMC,			// 0 SD/MMC卡	// 外接SD卡
    DEVICE_SDMMC1,			// 1 SD/MMC1卡	// 内接SD卡, 或 SPI-Flash (这里是2选1)
    DEVICE_UDISK,			// 2 U盘
    DEVICE_UDISK1,			// 3 UDisk1
#ifdef	TASK_TIPS_EN
	DEVICE_OTP,
#endif

    DEVICE_LINEIN,			//4 LineIn
    DEVICE_HP,				//5 headphone

    DEVICE_PC,				//6 连接PC
    DEVICE_MIC,				//7 MIC

    DEVICE_INVALID = 0xff	//无效的设备
};

enum
{
    DEVICE_IDLE,                //设备无变化
    DEVICE_CHANGE,              //设备插入或变化
    DEVICE_REMOVE,              //设备移除
};

typedef struct
{
	u8 status;          //瞬时状态
	u8 cnt;             //稳定计数
	u8 delay;           //处理延时
	u8 stable;          //稳定状态
	u8 actived;         //软件激活
} type_dev;

typedef struct
{
	u8 dev_actived;                 //设备激活状态
	u8 dev_aborted;                 //设备中止状态（主要是使用过程中出错的设备，只能通过插拔设备清除）
	type_dev sd;                    //SD/MMC卡
	type_dev sd1;                   //SD/MMC1卡
	type_dev udisk;                 //连接U盘
	type_dev udisk1;                //udisk1
	type_dev line;                  //LINE
	type_dev ear;                   //连接耳机
	type_dev pc;                    //连接PC
	type_dev mic;                   //连接MIC
} type_dev_ctl;

extern type_dev_ctl dev_ctl;

void device_init(void);
bool device_insert(u8 dev_num);
bool device_remove(u8 dev_num);
bool device_is_online(u8 dev_num);                      //判断设备是否在线
bool device_need_activate(u8 dev_num);                  //判断是否需要激活初始化
bool device_is_actived(u8 dev_num);                     //检查设备是否已经激活初始化
bool device_activate(u8 dev_num);                       //激活设备
bool device_set_testready(void);                        //检查设备是否在线，主要针对读卡器
u8 device_get_actived(void);                            //获取已激活的设备
u8 device_change_check(void);                           //检查并激活设备
u8 device_get_actived_num(void);                        //获取已激活的设备数量


/*****************************************************************************
 * Module    : USB检测相关接口
 *****************************************************************************/
//usb_connected函数的返回值
enum
{
    USB_CONNECTED_NONE,     //未连接
    USB_CONNECTED_PC,       //连接PC，当Device
    USB_CONNECTED_UDISK,    //外接U盘，当Host
};

typedef struct
{
	u8 host_phy;
	u8 dev_phy;

	struct
	{
		u8 check_mode;       //判断USB的连接状况
		u8 check_cnt;        //检测模式切换
	} usb_phy[2];

} type_usb_chk;

extern volatile IAR_XDATA_A type_usb_chk usb_chk;

//USB控制结构体
typedef struct
{
	u8 error;            //USB通信错误

	struct
	{
		u16 vid;              //VID
		u16 pid;              //PID
		u16 ver;              //Version
	} info;

	//EP0接收的命令
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
bool device_status_stable(u8 dev_num, u8 new_sta);      //消抖，等待设备稳定

#endif
