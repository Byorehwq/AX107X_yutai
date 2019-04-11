#include "include.h"


#if TASK_RTC_EN


TIME __idata t_time;

#pragma constseg="RTC_TBL"
IAR_CONST u32 month[12] =
{
	0,
	DAY *(31),
	DAY *(31+29),
	DAY *(31+29+31),
	DAY *(31+29+31+30),
	DAY *(31+29+31+30+31),
	DAY *(31+29+31+30+31+30),
	DAY *(31+29+31+30+31+30+31),
	DAY *(31+29+31+30+31+30+31+31),
	DAY *(31+29+31+30+31+30+31+31+30),
	DAY *(31+29+31+30+31+30+31+31+30+31),
	DAY *(31+29+31+30+31+30+31+31+30+31+30)
};
#pragma constseg=default


#if RTC_DATE_EN
//��ÿ���ۼƵ�count ת����������
void sec2date(TIME __idata *t, u32 second) AT(RTC_SEG_API);
void sec2date(TIME __idata *t, u32 second)
{
	sec2time(t, second);

	t->year = t->base_year;                       /* ������ */
	while(1)
	{
		if(((t->year % 4 == 0) && (t->year % 100 != 0)) || (t->year % 400 == 0))  /* ���괦�� */
		{
			if(second >= (YEAR + DAY))
			{
				second -= (YEAR + DAY);
				t->year++;
			}
			else
				break;
		}

		else                                         /* ƽ�괦�� */
			if(second >= YEAR)
			{
				second -= YEAR;
				t->year++;
			}
			else
				break;
	}
	t->month = 12;
	while(1)
	{
		if(((t->year % 4 == 0) && (t->year % 100 != 0)) || (t->year % 400 == 0))
		{
			if(second >= month[t->month - 1])
			{
				second -=  month[t->month - 1];
				break;
			}
			else
				t->month--;
		}
		else
		{
			if(t->month > 2)
			{
				if(second >= (month[t->month - 1] - DAY))
				{
					second -= (month[t->month - 1] - DAY);
					break;
				}
				else
					t->month--;
			}
			else
			{
				if(second >=month[t->month - 1])
				{
					second -= month[t->month - 1];
					break;
				}
				else
					t->month--;
			}
		}
	}
	t->day = (second / DAY + 1);
}

//����������,�������,���������ۼƵ���ֵ
u32 date2sec(TIME __idata *t) AT(RTC_SEG_API);
u32 date2sec(TIME __idata *t)
{
	u32 res;                            /* ������ */
	u8 year,month_tp;
	month_tp = t->month;              /* ȡ�ṹ������� */
	month_tp--;                        /* ��ȥ��ǰ�� */
	year = t->year;
	year -= t->base_year;             /* ��ȥ��ĳ�ʼֵ */
	res = YEAR*year + DAY*((year+3)/4); /* ��������ܷ�����(��������) */
	res += month[month_tp];            /* ���ӵ�����µĴ�ŷ����� */
	if(month_tp>1 && ((year)%4))       /* ϸ���������µ����� */
		res -= DAY;
	res += DAY*(t->day-1);
	res += time2sec(t);
	return res;
}
#endif

//������
#pragma location="RTC_SEG_API"
void deal_minute(bool flag)
{
	u8 tmp = t_time.minute;

	if(flag)
	{
		if(tmp < 59)
		{
			tmp++;
		}
		else
		{
			tmp = 0;
		}
	}
	else
	{
		if(tmp > 0)
		{
			tmp--;
		}
		else
		{
			tmp = 59;
		}
	}

	t_time.minute = tmp;
}


//����ʱ
#pragma location="RTC_SEG_API"
void deal_hour(bool flag)
{
	u8 tmp = t_time.hour;

	if(flag)
	{
		if(tmp < 23)
		{
			tmp++;
		}
		else
		{
			tmp = 0;
		}
	}
	else
	{
		if(tmp > 0)
		{
			tmp--;
		}
		else
		{
			tmp = 23;
		}
	}
	t_time.hour = tmp;
}
#endif

#if (TASK_RTC_EN || (SYS_MEM == MEM_RTCRAM))
//RTC��ʼ��
void irtc_init(void) AT(RTC_INIT);
void irtc_init(void)
{
	SELECT_IROM2();

	u8 rtccon_val = irtc_read_cmd(CFG_RD);
	if(sys_ctl.irtc_poweron)
	{
#if (SYS_CRY == CRY_32K)
		rtccon_val |= BIT(7);   //xosc 32k enable
#endif
#if (CRY_PIN_NUM == CRY_1PIN)
		rtccon_val |= BIT(5);   //xosc
#endif
		rtccon_val |= BIT(3) | BIT(2);
		//bit4-PDFLAG:RTCC timer is working
		//bit3-F1HZEN:1Hz signal output enable
		//bit2-F32KHZEN:32KHz signal output enable
		irtc_write_cmd(CFG_WR, rtccon_val);

#if (SYS_MEM == MEM_RTCRAM)
		mem_ctl.rtcram_poweron = 1;            //�ϵ磬����ָ���Ĭ��ֵ
#endif

#if TASK_RTC_EN
		t_time.second = 1;
		t_time.minute = 0;
		t_time.hour = 0;
#if RTC_DATE_EN
		t_time.day = 1;
		t_time.month = 1;
		t_time.year = 2013;
#endif
		irtc_write_date(date2sec(&t_time));

#if ALARM_EN
		t_time.second = 0;
		irtc_write_alarm(date2sec(&t_time));
#endif
#endif //TASK_RTC_EN
	}
	else
	{
#if (SYS_MEM == MEM_RTCRAM)
		mem_ctl.rtcram_poweron = 0;
#endif
	}

#if TASK_RTC_EN
	IRTCON &= ~BIT(2);              //second interrupt disable
#if ALARM_EN
	IRTCON |= BIT(4);               //alarm interrupt enable
#endif
	IRTCON &= ~(BIT(3)|BIT(5));     //second & alarm pending clear

#if ALARM_WAKEUP_EN
	u8 wkocon_val = irtc_read_cmd(WKO_RD);
	wkocon_val |= 0x98;       //WKO output high
	wkocon_val &= ~(BIT(5) | BIT(0));
	irtc_write_cmd(WKO_WR, wkocon_val);
#endif

#endif
}
#endif
