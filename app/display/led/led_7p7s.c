/* Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LED7脚显示屏的驱动
 *****************************************************************************/
#include "include.h"

#if (DISP_TYPE == DISP_LED_7P7S)

IAR_DATA_A u8 dispbuf[5];
IAR_DATA_A u8 p_dispbuff[7];
IAR_XDATA_A u8 COM_T;

#pragma constseg="LED_DISP_TBL"
IAR_CONST u8 tbl_led_num[10] =
{
    LED_7P7S_0, LED_7P7S_1, LED_7P7S_2, LED_7P7S_3, LED_7P7S_4,
    LED_7P7S_5, LED_7P7S_6, LED_7P7S_7, LED_7P7S_8, LED_7P7S_9,
};

IAR_CONST u8 tbl_led_alpha[26] = {
    LED_7P7S_A, LED_7P7S_b, LED_7P7S_C, LED_7P7S_d, LED_7P7S_E,
    LED_7P7S_F, LED_7P7S_g, LED_7P7S_H, LED_7P7S_I, LED_7P7S_J,
    LED_7P7S_K, LED_7P7S_L, LED_7P7S_m, LED_7P7S_n, LED_7P7S_O,
    LED_7P7S_P, LED_7P7S_q, LED_7P7S_r, LED_7P7S_S, LED_7P7S_t,
    LED_7P7S_U, LED_7P7S_V, LED_7P7S_w, LED_7P7S_X, LED_7P7S_y,
    LED_7P7S_Z
};

IAR_CONST u8 tbl_led_energy[3] = {
    (BIT_E | BIT_F), LED_7P7S_C, LED_7P7S_0,
};

#pragma constseg=default

//显示 -, 0~9, a~z的字符串(常量)
#pragma location="LED_DISP_SEG"
void led_7p7s_disp_str(const char __code *str)
{
    u8 i;
    u8 ch;

    for (i=0; i!=4; i++) {
        ch = str[i];
        if (ch >= '0' && ch <= '9') {
            dispbuf[i] = tbl_led_num[ch - '0'];
        } else if (ch >= 'A' && ch <= 'Z') {
            dispbuf[i] = tbl_led_alpha[ch - 'A'];
        } else if (ch >= 'a' && ch <= 'z') {
            dispbuf[i] = tbl_led_alpha[ch - 'a'];
        } else if (ch == ' ') {
            dispbuf[i] = 0;
        } else {
            dispbuf[i] = LED_7P7S__;
        }
    }
    dispbuf[4] = 0;
}

void led_7p7s_io_init(void) AT(LED_7P7S_INIT);
void led_7p7s_io_init(void)
{
#if ((USER_SEL == USER_AX1070_DEMO) || (USER_SEL == USER_AX1076_DEMO)|| (USER_SEL == USER_AX2070_DEMO))
    LED_7C7S_S0_DIR |= BIT(LED_7C7S_S0_PIN);	// P34
    LED_7C7S_S1_DIR |= BIT(LED_7C7S_S1_PIN);	// P33
    LED_7C7S_S2_DIR |= BIT(LED_7C7S_S2_PIN);	// P31
    LED_7C7S_S3_DIR |= BIT(LED_7C7S_S3_PIN);	// P27
    LED_7C7S_S4_DIR |= BIT(LED_7C7S_S4_PIN);	// P22
    LED_7C7S_S5_DIR |= BIT(LED_7C7S_S5_PIN);	// P21
    LED_7C7S_S6_DIR |= BIT(LED_7C7S_S6_PIN);	// P20

#else
    P2 &= ~0x7F;
    P2DIR |= 0x7F;           //P20-P26
#endif
}

//LED初始化函数，不显示
void led_7p7s_init(void) AT(LED_7P7S_INIT);
void led_7p7s_init(void)
{
    led_7p7s_io_init();
    COM_T = 0;
}

//LED 清除显示缓存
void led_7p7s_clear(void) AT(LED_7P7S_INIT);
void led_7p7s_clear(void)
{
    smemset_i(dispbuf, 0, 5);
}

//7p7s闪烁控制
void led_7p7s_blink(void) AT(LED_7P7S_INIT);
void led_7p7s_blink(void)
{
    if (ocx.blink_sta) {
        dispbuf[4] |= ocx.blink_icon;
        if(ocx.blink_num & 0x0c) {
            CHAR_HOURH = ocx.blink_numh;
            CHAR_HOURL = ocx.blink_numl;
        } else if(ocx.blink_num & 0x03) {
            CHAR_MINH = ocx.blink_numh;
            CHAR_MINL = ocx.blink_numl;
        }
    } else {
        dispbuf[4] &= ~ocx.blink_icon;
        if(ocx.blink_num & 0x0c) {
            CHAR_HOURH = 0;
            CHAR_HOURL = 0;
        } else if(ocx.blink_num & 0x03) {
            CHAR_MINH = 0;
            CHAR_MINL = 0;
        }
    }
}

//LED扫描函数
void led_7p7s_scan(void) AT(LED_7P7S_SEG);
void led_7p7s_scan(void)
{
    static u8 count = 0;
    u8 dis_seg = 0,dir_temp = 0,out_temp = 0;

    if (count > 6) {
        count = 0;
        COM_T++;
    }

#if (AUX_DET_EN && USE_7P7S_LED_AUX_MUX)
    if (COM_T > 7) {
        COM_T = 0;
    } else if (COM_T == 7) {
        if (count == 0) {
            AUX_CHECK_DIR |= BIT(AUX_CHECK_PIN);  //先设置为输入0.5ms后进行检测
            count = 1;
            return;
        }
        aux_detect();                             //lienin检测
        count = 7;
        return;
    }
#else
    if (COM_T > 6) {
        COM_T = 0;
    }
#endif

    led_7p7s_blink();

    led_7p7s_value_set();
	
    dis_seg = p_dispbuff[COM_T];
    if( (count == COM_T) )//|| ((COM_T == 1) && (count == 4)) )  {   //跳过不存在组合情况
    {
		count += 1;
    }
    dis_seg = dis_seg & BIT(count);

#if((USER_SEL == USER_AX1070_DEMO) || (USER_SEL == USER_AX1076_DEMO)|| (USER_SEL == USER_AX2070_DEMO))
    dir_temp = ((1 << COM_T) | dis_seg);
    out_temp |= BIT(COM_T);
    led_7p7s_io_init();
    if (dir_temp & BIT(0)) {
        if(out_temp & BIT(0)) {
            LED_7C7S_S0_PORT |= BIT(LED_7C7S_S0_PIN);
        } else{
            LED_7C7S_S0_PORT &= ~BIT(LED_7C7S_S0_PIN);
        }
        LED_7C7S_S0_DIR &= ~BIT(LED_7C7S_S0_PIN);
    }
    if (dir_temp & BIT(1)) {
        if(out_temp & BIT(1)) {
            LED_7C7S_S1_PORT |= BIT(LED_7C7S_S1_PIN);
        } else{
            LED_7C7S_S1_PORT &= ~BIT(LED_7C7S_S1_PIN);
        }
        LED_7C7S_S1_DIR &= ~BIT(LED_7C7S_S1_PIN);
    }
    if (dir_temp & BIT(2)) {
        if(out_temp & BIT(2)) {
            LED_7C7S_S2_PORT |= BIT(LED_7C7S_S2_PIN);
        } else{
            LED_7C7S_S2_PORT &= ~BIT(LED_7C7S_S2_PIN);
        }
        LED_7C7S_S2_DIR &= ~BIT(LED_7C7S_S2_PIN);
    }
    if (dir_temp & BIT(3)) {
        if(out_temp & BIT(3)) {
            LED_7C7S_S3_PORT |= BIT(LED_7C7S_S3_PIN);
        } else{
            LED_7C7S_S3_PORT &= ~BIT(LED_7C7S_S3_PIN);
        }
        LED_7C7S_S3_DIR &= ~BIT(LED_7C7S_S3_PIN);
    }
    if (dir_temp & BIT(4)) {
        if(out_temp & BIT(4)) {
            LED_7C7S_S4_PORT |= BIT(LED_7C7S_S4_PIN);
        } else{
            LED_7C7S_S4_PORT &= ~BIT(LED_7C7S_S4_PIN);
        }
        LED_7C7S_S4_DIR &= ~BIT(LED_7C7S_S4_PIN);
    }
    if (dir_temp & BIT(5)) {
        if(out_temp & BIT(5)) {
            LED_7C7S_S5_PORT |= BIT(LED_7C7S_S5_PIN);
        } else{
            LED_7C7S_S5_PORT &= ~BIT(LED_7C7S_S5_PIN);
        }
        LED_7C7S_S5_DIR &= ~BIT(LED_7C7S_S5_PIN);
    }

    if (dir_temp & BIT(6)) {
        if(out_temp & BIT(6)) {
            LED_7C7S_S6_PORT |= BIT(LED_7C7S_S6_PIN);
        } else{
            LED_7C7S_S6_PORT &= ~BIT(LED_7C7S_S6_PIN);
        }
        LED_7C7S_S6_DIR &= ~BIT(LED_7C7S_S6_PIN);
    }

#else
    out_temp = LED_7P7S_PORT;
    dir_temp = LED_7P7S_DIR;
    dir_temp |= 0x7F;                           //P27保持不变
    dir_temp &= ~((1 << COM_T) | dis_seg);
    out_temp &= 0x80;                           //P27保持不变
    out_temp |= (1 << COM_T);
    LED_7P7S_PORT = out_temp;
    LED_7P7S_DIR = dir_temp;                     //点亮一段
#endif
    count++;
   // if ((COM_T >= 4) && (count == 4)) {     //跳过不存在组合情况
   //     count = 7;
   // }
   //if(((COM_T <= 2) && (count == 4))||((COM_T == 5) && (count == 5))){
	//	count = 6;
   //}
}

//根据7脚LED的真值表进行映射（每段输出高的IO记为COM端 7*7）COM:0~6  SEG:0~6
void led_7p7s_value_set(void) AT(LED_7P7S_SEG);
void led_7p7s_value_set(void)
{
    IE_EA = 0;
    smemset_i(p_dispbuff, 0, 7);

	//p_dispbuff[6] |= BIT(7);
/*
dispbuf 0			1				2			3

    --A65---	  ---56---		---43---	 ---34---
    |		|	  | 	 |		|	   |	 |		|
   F60    B64	  06	46		31	  42	 13    24
    |		|	  | 	 |	25	|	   |	 |		|
    --G54---	  ---45---		---30---	 ---03---
    |		|	  | 	 |	15	|	   |	 |		|
   E61    C63	  16	36		32	  41	 23    14
    |		|	  | 	 |		|	   |	 |		|
    --D62--- xx  ---26--- xx	---40--- xx  ---04--- xx

                 Pin    6 5 4 3 2 1 0

*/

if(((scan_ctl.disp_clock_set_flag == 1) && (scan_ctl.count_KU_TIMER == 0)) || (scan_ctl.disp_clock_set_flag == 0))
{
	 if(disp.display_led_flag == 0)
	 {	
		if(dispbuf[0] & BIT_A)     p_dispbuff[5] |= BIT(6);
	    if(dispbuf[0] & BIT_B)     p_dispbuff[4] |= BIT(6);
	    if(dispbuf[0] & BIT_C)     p_dispbuff[3] |= BIT(6);
	    if(dispbuf[0] & BIT_D)     p_dispbuff[2] |= BIT(6);
	    if(dispbuf[0] & BIT_E)     p_dispbuff[1] |= BIT(6);
	    if(dispbuf[0] & BIT_F)     p_dispbuff[0] |= BIT(6);
	    if(dispbuf[0] & BIT_G)     p_dispbuff[4] |= BIT(5);

	    if(dispbuf[1] & BIT_A)     p_dispbuff[6] |= BIT(5);
	    if(dispbuf[1] & BIT_B)     p_dispbuff[6] |= BIT(4);
	    if(dispbuf[1] & BIT_C)     p_dispbuff[6] |= BIT(3);
	    if(dispbuf[1] & BIT_D)     p_dispbuff[6] |= BIT(2);
	    if(dispbuf[1] & BIT_E)     p_dispbuff[6] |= BIT(1);
	    if(dispbuf[1] & BIT_F)     p_dispbuff[6] |= BIT(0);
	    if(dispbuf[1] & BIT_G)     p_dispbuff[5] |= BIT(4);
	 }
	 	if(dispbuf[2] & BIT_A)     p_dispbuff[3] |= BIT(4);
	    if(dispbuf[2] & BIT_B)     p_dispbuff[2] |= BIT(4);
	    if(dispbuf[2] & BIT_C)     p_dispbuff[1] |= BIT(4);
	    if(dispbuf[2] & BIT_D)     p_dispbuff[0] |= BIT(4);
	    if(dispbuf[2] & BIT_E)     p_dispbuff[2] |= BIT(3);
	    if(dispbuf[2] & BIT_F)     p_dispbuff[1] |= BIT(3);
	    if(dispbuf[2] & BIT_G)     p_dispbuff[0] |= BIT(3);

	    if(dispbuf[3] & BIT_A)     p_dispbuff[4] |= BIT(3);
	    if(dispbuf[3] & BIT_B)     p_dispbuff[4] |= BIT(2);
	    if(dispbuf[3] & BIT_C)     p_dispbuff[4] |= BIT(1);
	    if(dispbuf[3] & BIT_D)     p_dispbuff[4] |= BIT(0);
	    if(dispbuf[3] & BIT_E)     p_dispbuff[3] |= BIT(2);
	    if(dispbuf[3] & BIT_F)     p_dispbuff[3] |= BIT(1);
	    if(dispbuf[3] & BIT_G)     p_dispbuff[3] |= BIT(0);
}
if(((scan_ctl.disp_clock_set_flag == 1) && (scan_ctl.count_KU_TIMER == 1)) || (scan_ctl.disp_clock_set_flag == 0))
{
		if(dispbuf[0] & BIT_A)     p_dispbuff[5] |= BIT(6);
	    if(dispbuf[0] & BIT_B)     p_dispbuff[4] |= BIT(6);
	    if(dispbuf[0] & BIT_C)     p_dispbuff[3] |= BIT(6);
	    if(dispbuf[0] & BIT_D)     p_dispbuff[2] |= BIT(6);
	    if(dispbuf[0] & BIT_E)     p_dispbuff[1] |= BIT(6);
	    if(dispbuf[0] & BIT_F)     p_dispbuff[0] |= BIT(6);
	    if(dispbuf[0] & BIT_G)     p_dispbuff[4] |= BIT(5);

	    if(dispbuf[1] & BIT_A)     p_dispbuff[6] |= BIT(5);
	    if(dispbuf[1] & BIT_B)     p_dispbuff[6] |= BIT(4);
	    if(dispbuf[1] & BIT_C)     p_dispbuff[6] |= BIT(3);
	    if(dispbuf[1] & BIT_D)     p_dispbuff[6] |= BIT(2);
	    if(dispbuf[1] & BIT_E)     p_dispbuff[6] |= BIT(1);
	    if(dispbuf[1] & BIT_F)     p_dispbuff[6] |= BIT(0);
	    if(dispbuf[1] & BIT_G)     p_dispbuff[5] |= BIT(4);
	 if(disp.display_led_flag == 0)
	 {
	    if(dispbuf[2] & BIT_A)     p_dispbuff[3] |= BIT(4);
	    if(dispbuf[2] & BIT_B)     p_dispbuff[2] |= BIT(4);
	    if(dispbuf[2] & BIT_C)     p_dispbuff[1] |= BIT(4);
	    if(dispbuf[2] & BIT_D)     p_dispbuff[0] |= BIT(4);
	    if(dispbuf[2] & BIT_E)     p_dispbuff[2] |= BIT(3);
	    if(dispbuf[2] & BIT_F)     p_dispbuff[1] |= BIT(3);
	    if(dispbuf[2] & BIT_G)     p_dispbuff[0] |= BIT(3);

	    if(dispbuf[3] & BIT_A)     p_dispbuff[4] |= BIT(3);
	    if(dispbuf[3] & BIT_B)     p_dispbuff[4] |= BIT(2);
	    if(dispbuf[3] & BIT_C)     p_dispbuff[4] |= BIT(1);
	    if(dispbuf[3] & BIT_D)     p_dispbuff[4] |= BIT(0);
	    if(dispbuf[3] & BIT_E)     p_dispbuff[3] |= BIT(2);
	    if(dispbuf[3] & BIT_F)     p_dispbuff[3] |= BIT(1);
	    if(dispbuf[3] & BIT_G)     p_dispbuff[3] |= BIT(0);
	 }
}
	if(scan_ctl.disp_cnt > 0)	// 当显示音量、曲目、定时时，隐去冒号显示
	{

	}else{
		if(disp.display_dp_flag == 0)
		{
			p_dispbuff[5] |= BIT(2);
			p_dispbuff[5] |= BIT(1);
		}
	}

    IE_EA = 1;
}

//显示数字
void led_7p7s_dispnum(u16 num) AT(LED_7P7S_SEG);
void led_7p7s_dispnum(u16 num)
{
    u8 i;
    for (i=2; i!=0xff; i--) {
        dispbuf[i] = tbl_led_num[num % 10];
        num /= 10;
    }
    dispbuf[3] = 0;
}

//显示音量
void led_7p7s_disp_vol(void) AT(LED_7P7S_SEG);
void led_7p7s_disp_vol(void)
{
    disp_numsel(sys_ctl.volume);
    dispbuf[0] = 0;
    //dispbuf[0] = LED_7P7S_V;
}

void led_7p7s_disp_timer(void)
{
	if(scan_ctl.timer_flag == 1){
    	disp_numsel(15);
	}else if(scan_ctl.timer_flag == 2){
    	disp_numsel(30);
	}else if(scan_ctl.timer_flag == 3){
    	disp_numsel(60);
	}else if(scan_ctl.timer_flag == 4){
    	dispbuf[1] = 7;
    	dispbuf[2] = LED_7P7S_H;
	}
    dispbuf[0] = 0;
}

void led_7p7s_disp_clock(void)
{
	
	led_7p7s_disp_time();
	
}

void led_7p7s_disp_song(void)
{
    disp_numsel(u_msc.music_num+1);
    dispbuf[0] = 0;
}

//显示时间
void led_7p7s_disp_time(void) AT(LED_7P7S_SEG);
void led_7p7s_disp_time(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;

    dispbuf[0] = tbl_led_num[ocx.timeh / 10];
    dispbuf[1] = tbl_led_num[ocx.timeh % 10];
    dispbuf[2] = tbl_led_num[ocx.timel / 10];
    dispbuf[3] = tbl_led_num[ocx.timel % 10];
    dispbuf[4] = LED_7P7S_DDOT;

    if(ocx.blink_num & 0x0c) {
        ocx.blink_numh = CHAR_HOURH;
        ocx.blink_numl = CHAR_HOURL;
    } else if(ocx.blink_num & 0x03) {
        ocx.blink_numh = CHAR_MINH;
        ocx.blink_numl = CHAR_MINL;
    }

    IE_EA = ie_ea;
}

//显示当前设备
void led_7p7s_disp_device(void) AT(LED_7P7S_SEG);
void led_7p7s_disp_device(void)
{
    if (fs_cur_dev() == DEVICE_SDMMC) {
        dispbuf[4] |= LED_7P7S_SD;
    } else if (fs_cur_dev() == DEVICE_UDISK) {
        dispbuf[4] |= LED_7P7S_USB;
    }
}

//显示播放界面
void led_7p7s_disp_play_pause(void) AT(LED_7P7S_SEG);
void led_7p7s_disp_play_pause(void)
{
    disp_time();
    dispbuf[4] = LED_7P7S_DDOT | LED_7P7S_MP3 | (b_msc.pause ? LED_7P7S_PAUSE : LED_7P7S_PLAY);
    disp_device();
}

//显示播放界面
void led_7p7s_disp_numsel_icon(void) AT(LED_7P7S_SEG);
void led_7p7s_disp_numsel_icon(void)
{
    dispbuf[4] = LED_7P7S_MP3;
    disp_device();
}

//显示FM频道
void led_7p7s_disp_ch(u8 ch) AT(LED_7P7S_SEG);
void led_7p7s_disp_ch(u8 ch)
{
    disp_numsel(ch);
    dispbuf[0] = LED_7P7S_C;
    dispbuf[1] = LED_7P7S_H;
}

//显示FM频点
void led_7p7s_disp_freq(void) AT(LED_7P7S_SEG);
void led_7p7s_disp_freq(void)
{
    disp_numsel(ocx.freq);
    if (dispbuf[0] == LED_7P7S_0) {
        dispbuf[0] = 0;
    }
    dispbuf[4] = LED_7P7S_FM;
}

//能量显示
#if ENERGY_DISP_EN
void led_7p7s_disp_energy(void) AT(LED_7P7S_SEG);
void led_7p7s_disp_energy(void)
{
    u8 num1, num2, i;

    num1 = ocx.energy_cnt / 3;
    num2 = ocx.energy_cnt % 3;
    //printf("num1=%dx num2=%d\n", num1, num2);
    for (i=0; i!=5; i++) {
        if (i < num1) {
            dispbuf[i] = tbl_led_energy[2];
        } else if (i == num1) {
            dispbuf[i] = tbl_led_energy[num2];
        } else {
            dispbuf[i] = 0;
        }
    }
}
#endif
#endif /*(DISP_TYPE == DISP_LED_7P7S)*/

