#ifndef __API_RTC__
#define __API_RTC__


#define RTC_WR  0xf0
#define RTC_RD  0xe0

#define ALM_WR  0x53
#define ALM_RD  0x52
#define CFG_WR  0x55
#define CFG_RD  0x54
#define CFG3_WR 0x59
#define RAM_WR  0x57
#define RAM_RD  0x56
#define OSC_WR  0x58
#define WKO_WR  0x5b
#define WKO_RD  0xa1
#define VCL_WR  0xa2
#define PWR_WR  0x5a
#define PWR_RD  0x51



#define YEAR            (DAY*365)
#define DAY             (HOUR*24)
#define HOUR            (MINUTE*60)
#define MINUTE          60L
#define CAL_YEAR_BASE   2004


#define RTC_DATE_EN         0


typedef struct _TIME
{
	u8 second;
	u8 minute;
	u8 hour;
#if RTC_DATE_EN
	u8 day;
	u8 month;
	u16 year;
	u16 base_year;
#endif
} TIME;

u8 irtc_read_cmd(u8 cmd);
void irtc_write_cmd(u8 cmd, u8 para);
u32 irtc_read_date(void);
void irtc_write_date(u32 cnt);
u32 irtc_read_alarm(void);
void irtc_write_alarm(u32 cnt);
u8 irtc_read_ram(u8 addr);
void irtc_write_ram(u8 addr, u8 *wdata, u8 len);
void rtcram_write_byte(u8 addr, u8 val);
__root void sec2time(TIME __idata *t, u32 second);
__root u32 time2sec(TIME __idata *t);


#if RTC_DATE_EN
void sec2date(TIME __idata *t, u32 second);
u32 date2sec(TIME __idata *t);
#else
#define sec2date        sec2time
#define date2sec        time2sec
#endif



#define RTC_ALARM_EN()			irtc_write_cmd(WKO_WR, (irtc_read_cmd(WKO_RD) | 0x02))
#define RTC_ALARM_DIS()			irtc_write_cmd(WKO_WR, (irtc_read_cmd(WKO_RD) & 0xfc))

void irtc_init(void);
void deal_minute(bool flag);
void deal_hour(bool flag);

extern TIME __idata t_time;

#endif


