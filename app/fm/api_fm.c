/*--------------------------------------------------------------------------*/
/***@file api_fm.c
    @brief  fm操作api接口
    @details 
    @author
    @date
    @note
*/
/*----------------------------------------------------------------------------*/
#include "include.h"
#include "rda5807p.h"
#include "bk1080.h"
#include "kt0830.h"
#include "qn8035.h"
#include "qn8065.h"
#include "si484x.h"
#include "rtc6218.h"

/*----------------------------------------------------------------------------*/
/***@brief fm_read_id
    @details 读取fm模块的id
    @param 无
    @return 无
    @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="API_FM"
u8 fm_read_id(void)
{
	u8 result;
	result = FM_NO;

#if FM_SI484X_EN
	return FM_SI484X;
#else

#if FM_RDA5807_EN
	if(rda5807p_online())
	{
		result = FM_RDA5807;
	}
#endif

#if FM_AR1010_EN
	if(ar1010_online())
	{
		result = FM_AR1010
	}
#endif

#if FM_AR1019_EN
	         if(ar1019_online())
	{
		result = FM_AR1019;
	}
#endif

#if FM_BK1080_EN
	if(bk1080_online())
	{
		result = FM_BK1080;
	}
#endif

#if FM_CL6017S_EN
	if(cl6017s_online())
	{
		result = FM_CL6017S;
	}
#endif

#if FM_CL6017G_EN
	if(cl6017G_online())
	{
		result = FM_CL6017G;
	}
#endif

#if FM_KT0830_EN
	if(kt0830_online())
	{
		result = FM_KT0830;
	}
#endif

#if FM_QN8035_EN
	if(qn8035_online())
	{
		result = FM_QN8035;
	}
#endif

#if FM_QN8065_EN
	if(qn8065_online())
	{
		result = FM_QN8065;
	}
#endif

#if FM_RTC6218_EN
	if(RTC6218_online())
		result = FM_RTC6218;
#endif
	return result;
#endif /* FM_SI484X_EN */
}

/*----------------------------------------------------------------------------*/
/**@brief fm_rx_init
   @details fm 初始化
   @param 无
   @return 无
   @note 无
*/
/*----------------------------------------------------------------------------*/
#pragma location="API_FM"
void fm_rx_init(void)
{
	switch(fm_ctl.fm_type)
	{
#if FM_SI484X_EN
	case FM_SI484X:
		si484x_init();
		break;
#endif

#if FM_RDA5807_EN
	case FM_RDA5807:
		rda5807p_init();
		break;
#endif

#if FM_AR1010_EN
	case FM_AR1010:
		ar1010_init();
		break;
#endif

#if FM_AR1019_EN
	case FM_AR1019:
		ar1019_init();
		break;
#endif

#if FM_BK1080_EN
	case FM_BK1080:
		bk1080_init();
		break;
#endif

#if FM_CL6017S_EN
	case FM_CL6017S:
		cl6017s_init();
		break;
#endif

#if FM_CL6017G_EN
	case FM_CL6017G:
		cl6017g_init();
		break;
#endif

#if FM_KT0830_EN
	case FM_KT0830:
		kt0830_init();
		break;
#endif

#if FM_QN8035_EN
	case FM_QN8035:
#if (FM_CLOCK == USE_MODULE_CRYSTAL)
		while(!fm_ctl.init_sta)     //等待前半部分初始化完成
		{
			WATCHDOG_CLR();
		}
#endif
		qn8035_init();
		break;
#endif

#if FM_QN8065_EN
	case FM_QN8065:
		qn8065_init();
		break;
#endif

#if FM_RTC6218_EN
	case FM_RTC6218:
		RTC6218_init();
		break;
#endif
	}
}

#pragma location="API_FM"
void fm_init_process(void)
{
#if FM_QN8035_EN
#if (FM_CLOCK == USE_MODULE_CRYSTAL)
	if(qn8035_init_process())
	{
		fm_ctl.init_sta = 1;     //初始化完成
	}
#endif
#endif
}

/*----------------------------------------------------------------------------*/
/***@brief fm 设置频率
   @details fm 设置频率
   @param [in]freq: 输入设置频率
   @return 无
   @note 无
*/
/*----------------------------------------------------------------------------*/
#pragma location="API_FM"
void fm_rx_set_freq(u16 freq)
{
	if(freq>=FM_MIN_FREQ && freq<=FM_MAX_FREQ)
	{
		switch(fm_ctl.fm_type)
		{
#if FM_RDA5807_EN
		case FM_RDA5807:
			rda5807p_set_freq(freq);
			break;
#endif

#if FM_AR1010_EN
		case FM_AR1010:
			ar1010_set_freq(freq);
			break;
#endif

#if FM_AR1019_EN
		case FM_AR1019:
			ar1019_set_freq(freq);
			break;
#endif

#if FM_BK1080_EN
		case FM_BK1080:
			bk1080_set_freq(freq,1);
			break;
#endif

#if FM_CL6017S_EN
		case FM_CL6017S:
			cl6017s_set_freq(freq);
			break;
#endif

#if FM_CL6017G_EN
		case FM_CL6017G:
			cl6017g_set_freq(freq);
			break;
#endif

#if FM_KT0830_EN
		case FM_KT0830:
			kt0830_set_freq(freq);
			break;
#endif

#if FM_QN8035_EN
		case FM_QN8035:
			qnd_tune2ch(freq);
			break;
#endif

#if FM_QN8065_EN
		case FM_QN8065:
			qn8065_tune2ch(freq);
			break;
#endif

#if FM_RTC6218_EN
		case FM_RTC6218:
			RTC6218_set_freq(freq);
			break;
#endif
		}
	}
}


/*----------------------------------------------------------------------------*/
/**@brief fm 设置fm音量
   @details fm 设置fm音量
   @param [in]vol: 输入设置音量
   @return 无
   @note 无
*/
/*----------------------------------------------------------------------------*/
#pragma location="API_FM"
void fm_rx_set_vol(u8 vol)
{
	switch(fm_ctl.fm_type)
	{
#if FM_RDA5807_EN
	case FM_RDA5807:
		rda5807p_set_vol(vol);
		break;
#endif

#if FM_AR1010_EN
	case FM_AR1010:
		ar1010_set_vol(vol);
		break;
#endif

#if FM_AR1019_EN
	case FM_AR1019:
		ar1019_set_vol(vol);
		break;
#endif

#if FM_BK1080_EN
	case FM_BK1080:
		bk1080_set_vol(vol);
		break;
#endif

#if FM_CL6017S_EN
	case FM_CL6017S:
		cl6017s_set_vol(vol);
		break;
#endif

#if FM_CL6017G_EN
	case FM_CL6017G:
		cl6017g_set_vol(vol);
		break;
#endif

#if FM_KT0830_EN
	case FM_KT0830:
		kt0830_set_vol(vol);
		break;
#endif

#if FM_QN8035_EN
	case FM_QN8035:
		qn8035_set_vol(vol);
		break;
#endif

#if FM_QN8065_EN
	case FM_QN8065:
		qn8065_set_vol(vol);
		break;
#endif

#if FM_RTC6218_EN
	case FM_RTC6218:
		RTC6218_set_vol(vol);
		break;
#endif
	}
}

/*----------------------------------------------------------------------------*/
/**@brief fm 搜台
   @details fm 搜台
   @param freq: 输入搜索频率
   @return  0:成功，其它：失败
   @note 无
*/
/*----------------------------------------------------------------------------*/
#if !FM_SI484X_EN
#pragma location="API_FM"
u8 fm_rx_seek(u16 freq)
{
	u8 res = 0;

	switch(fm_ctl.fm_type)
	{
#if FM_RDA5807_EN
	case FM_RDA5807:
		res = rda5807p_seek(freq);
		break;
#endif

#if FM_AR1010_EN
	case FM_AR1010:
		res = ar1010_seek(freq);
		break;
#endif

#if FM_AR1019_EN
	case FM_AR1019:
		res = ar1019_seek(freq);
		break;
#endif

#if FM_BK1080_EN
	case FM_BK1080:
		res = bk1080_seek(freq);
		break;
#endif

#if FM_CL6017S_EN
	case FM_CL6017S:
		res = cl6017s_seek(freq);
		break;
#endif

#if FM_CL6017G_EN
	case FM_CL6017G:
		res = cl6017g_seek(freq);
		break;
#endif

#if FM_KT0830_EN
	case FM_KT0830:
		res = kt0830_seek(freq);
		break;
#endif

#if FM_QN8035_EN
	case FM_QN8035:
		res = qn8035_seek(freq);
		break;
#endif

#if FM_QN8065_EN
	case FM_QN8065:
		res = qn8065_seek(freq);
		break;
#endif

#if FM_RTC6218_EN
	case FM_RTC6218:
		res = RTC6218_seek(freq);
		break;
#endif
	}

	return res;
}
#endif
/*----------------------------------------------------------------------------*/
/***@brief fm 关闭
    @details fm 关闭
    @param 无
    @return 无
    @note 无
*/
/*----------------------------------------------------------------------------*/
#pragma location="API_FM"
void fm_rx_off(void)
{
	switch(fm_ctl.fm_type)
	{
#if FM_SI484X_EN
	case FM_SI484X:
		Si484x_off();
		break;
#endif

#if FM_RDA5807_EN
	case FM_RDA5807:
		rda5807p_off();
		break;
#endif

#if FM_AR1010_EN
	case FM_AR1010:
		ar1010_off();
		break;
#endif

#if FM_AR1019_EN
	case FM_AR1019:
		ar1019_off();
		break;
#endif

#if FM_BK1080_EN
	case FM_BK1080:
		bk1080_off();
		break;
#endif

#if FM_CL6017S_EN
	case FM_CL6017S:
		cl6017s_off();
		break;
#endif

#if FM_CL6017G_EN
	case FM_CL6017G:
		cl6017g_off();
		break;
#endif

#if FM_KT0830_EN
	case FM_KT0830:
		kt0830_off();
		break;
#endif

#if FM_QN8035_EN
	case FM_QN8035:
		qn8035_off();
		break;
#endif

#if FM_QN8065_EN
	case FM_QN8065:
		qn8065_off();
		break;
#endif

#if FM_RTC6218_EN
	case FM_RTC6218:
		RTC6218_off();
		break;
#endif
	}
}

