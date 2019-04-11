/*****************************************************************************
 * Module    : Task
 * File      : task_mp3res.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : SPI���ֲ�����������
 *****************************************************************************/
#include "include.h"
#include "User_key.h"
#include "spi.h"
extern bool SD_FLAG;
extern u8 count_1min;
extern bool Timer1_flag;

#if TASK_FLASH_EN
extern bool first;
//�޸���Դ�и�������ĸ�����Ŀ����Ҫ�޸Ĵ˱�
#pragma constseg="TASK_FLASH_CONST"
IAR_CONST u8 flash_music_total_tbl[7] =
{
	VOICE_TOTAL,CHILDREN_TOTAL	,ENGLISH_TOTAL,STORY_TOTAL,HABIT_TOTAL,POEM_TOTAL,JOKE_TOTAL
};

#pragma constseg=default

extern u8 t_pwn_duty1, t_pwn_duty2;   // Duty range from 0x00 to 0xFF

extern u8 decext_buf[0x400];
u16 flash_music_total=0;
u8  flash_long_change=0;
bool alarm_flag;
#pragma location="TASK_FLASH_SEG"
void task_flash_music_display(void)
{
	task_music_display();
}

//��ȡ��ǰ�ļ�����Ŀ¼�ĵ�һ���ļ�
#pragma location="TASK_FLASH_SEG"
u16 get_dirstart(void)
{
	printf("dir_start:%d\n",sys_ctl.flash_music_sta);
	if(!sys_ctl.flash_music_sta)
	{
		return 0;
	}

#if 1
	return flash_music_total;
#else
	u16 i, num=0;

	for(i=0; i<sys_ctl.flash_music_sta; i++)
	{
		num += flash_music_total_tbl[i];
		//printf("xx[%d]\n",num);
	}
	return num;
#endif
}


//��ȡFLASH����Ŀ����
#pragma location="TASK_FLASH_SEG"
u16 get_Flash_TotalFiles(void)
{
	u32 address;

	flash_music_total = 0;

	for(address = 0L; address < 8*256L; address += sizeof(decext_buf))
	{
		register u8 value;
		u16 read_offset;

		spi_read(decext_buf, address, sizeof(decext_buf));

		//printf_array(decext_buf,64);

		for(read_offset = 0, value = 0; read_offset < sizeof(decext_buf); read_offset++)
		{
			// Rule:
			//	Offset 0..3: ��ʼ��ַ
			//	offset 4..7: ���ݳ��� (�� 0x100Ϊ��λ)

			// ���������Ч������, ֻ�������ݳ��Ȳ�Ϊ 0�Ĳ���
			if((read_offset & 0x07) == 0x04)
				value = 0;
			value |= decext_buf[read_offset];
			if((read_offset & 0x07) == 0x07)
			{
				if(value != 0x00)
					flash_music_total++;
				else
				{
					//printf("Find %d", flash_music_total);
					//return flash_music_total;
					break;
				}
				value = 0;
			}
		}
	}
	//printf("Find %d", flash_music_total);
	return flash_music_total;
}


//��ȡ��ǰ�ļ�����Ŀ¼���ļ�����
#pragma location="TASK_FLASH_SEG"
u16 get_dirtotal(void)
{
	//u16 flash_music_total = flash_music_total_tbl[sys_ctl.flash_music_sta];
	//u16 flash_music_total = get_Flash_TotalFiles();

	//printf("dir total:[%d]\n",flash_music_total);

	return flash_music_total;
}


//��ʼ����music_num�׸�
#pragma location="TASK_FLASH_SEG"
bool task_flash_music_init(u16 music_num)
{
	u_spi.spi_music_flag = 1;

	SPI_CLK_DIR &= ~BIT(SPI_CLK_PIN);       //CLK
	SPI_CS_DIR &= ~BIT(SPI_CS_PIN);         //CS

	spi_mp3_index_read(music_num);   //��ȡFlash����������Ϣ

	u_msc.music_type = music_init();
	if(TYPE_MP3 != u_msc.music_type)
	{
		return false;
	}
	return true;
}

//0:��һ��/1:��һ��
#pragma location="TASK_MUSIC_SEG"
static void task_flash_music_direct(bool direct)
{
#if (MUSIC_PLAYMODE && (PLAYMODE_NUM == PLAYMODE_FOUR))
	u16 dir_start, dir_last;
#endif

	b_msc.file_change = 1;
	b_msc.file_prev_next = direct;                       //���÷��򣬹��������ʱʹ��

	//printf("num=%d,total=%d\n", u_msc.music_num,u_msc.music_total);

	switch(sys_ctl.mode_play)
	{
	case ITEM_LOOP_ALL:
#if MUSIC_PLAYMODE
	case ITEM_LOOP_SINGLE:
#endif
		if(direct)      //��һ��
		{
#if AUTO_CHANGE_DEVICE
			if(u_msc.music_num == 1)
			{
				if(task_music_auto_device())
				{
					u_msc.music_num = u_msc.music_total;//�Զ���һ������һ�豸����
					b_msc.file_change = 1;              //��Ҫ��һ�����ļ�
					return;
				}
			}
#endif
		}
		else            //��һ��
		{
#if AUTO_CHANGE_DEVICE
			if(u_msc.music_num == u_msc.music_total)
			{
				if(task_music_auto_device())
				{
					u_msc.music_num = 1;                //�Զ���һ������һ�豸����
					b_msc.file_change = 1;              //��Ҫ��һ�����ļ�
					return;
				}
			}
#endif
		}
		//ȫ��ѭ��
		u_msc.music_num = loop_auto(direct, u_msc.music_num, 0, u_msc.music_total-1);
		break;
#if (MUSIC_PLAYMODE && (PLAYMODE_NUM == PLAYMODE_FOUR))
	case ITEM_LOOP_FOLDER:
		//Ŀ¼ѭ��
		music_stop();
		dir_start = f_get_dirstart();           //��ǰĿ¼��һ���ļ��ı��
		dir_last = dir_start + f_get_dirtotal() - 1;
		u_msc.music_num = loop_auto(direct, u_msc.music_num, dir_start, dir_last);
		break;
	case ITEM_LOOP_RAMDOM:
#ifdef AX207X_TAG
		u_msc.music_num = rand(u_msc.music_total) + 1;
#else
		u_msc.music_num = rand(u_msc.music_total, u_msc.music_num) + 1;
#endif
		break;
#endif
	}
}


#if 0

//��һ��
#pragma location="TASK_FLASH_SEG"
static void task_flash_music_prev(void)
{
	u16 dir_start, dir_last;
	b_msc.file_change = 1;
	b_msc.file_prev_next = 1;                       //���÷��򣬹��������ʱʹ��

	u_msc.music_num--;
	switch(u_spi.spi_play_mode)
	{
	case SINGLE_DIR:
		dir_start = get_dirstart();                     //��ǰ�����һ���ļ��ı��
		dir_last = dir_start + get_dirtotal() - 1;
		printf("dir_last:%d\n",dir_last);

		if((u_msc.music_num < dir_start) || (u_msc.music_num > dir_last))
		{
			u_msc.music_num = dir_last;
		}
		break;
	case ALL_DIR:
		//if(!u_msc.music_num  || u_msc.music_num > ALL_MP3_TOTAL)
		if(!u_msc.music_num  || u_msc.music_num > flash_music_total)
		{
			u_msc.music_num = 0;
		}
		break;
	}
}

//��һ��
#pragma location="TASK_FLASH_SEG"
static void task_flash_music_next(void)
{
	u16 dir_start, dir_last;
	b_msc.file_change = 1;
	b_msc.file_prev_next = 0;                       //���÷��򣬹��������ʱʹ��

	u_msc.music_num++;
	switch(u_spi.spi_play_mode)
	{
	case SINGLE_DIR:
		dir_start = get_dirstart();                     //��ǰ�����һ���ļ��ı��
		dir_last = dir_start + get_dirtotal() - 1;
		printf("dir_last:%d\n",dir_last);

		if(u_msc.music_num < dir_start || u_msc.music_num > dir_last)
		{
			u_msc.music_num = dir_start;
		}
		break;
	case ALL_DIR:
		//if(u_msc.music_num > ALL_MP3_TOTAL)
		if(u_msc.music_num > flash_music_total)
		{
			u_msc.music_num = 0;
		}
		break;
	}
}
#endif

#pragma location="TASK_FLASH_SEG"
void task_flash_music_sel_mode(void)
{
	u16 dir_start/*, dir_last*/;
	b_msc.file_change = 1;
	b_msc.file_prev_next = 0;

	dir_start = get_dirstart();                     //��ǰ�����һ���ļ��ı��
	//dir_last = dir_start + get_dirtotal()-1;
	//printf("dir_last:%d\n",dir_last);
	u_msc.music_num = dir_start+1;
	printf("cur num:%d\n",u_msc.music_num);
}

//��һ���µ�����
#pragma location="TASK_FLASH_SEG"
void task_flash_music_new(void)
{
	while(b_msc.file_change)
	{
		msc_ctl.mscfade_en = 1;	//ʹ�ܵ���dac��������
		music_stop();				// ֹͣ������
		b_msc.file_change = 0;                      //����ָ������
		//sound_ctl_clr();
		if(task_flash_music_init(u_msc.music_num))
		{
			//printf("music_play\n");
			//printf("flash play:%d\n",u_msc.music_num + 1);
			//printf("flash##%lx, %lx, %lx\n", flash_music.addr, flash_music.len, flash_music.pos);
			//dac_fade_in(0);
			//music_fast_end();

			music_play();
			write_flash_num_rtcram();
			//dac_set_volume_direct(sys_ctl.volume);
			b_msc.pause = 0;
		}
		else
		{
			if(b_msc.file_prev_next)                //��ʼ��ʧ�ܣ����¶�ȡ��һ���ļ�
			{
				task_flash_music_direct(1);
			}
			else
			{
				task_flash_music_direct(0);
			}
		}
	}
}

//�Զ���һ��
#pragma location="TASK_FLASH_SEG"
void task_flash_music_auto_next(void)
{
#if !ADD_BY_WUPENGFEI
	if(music_get_mtime() < 5)                   //���Ų���500ms����Ϊ�������
	{

		if(b_msc.file_prev_next)                //��ʼ��ʧ�ܣ����¶�ȡ��һ���ļ�
		{
			task_flash_music_direct(1);
		}
		else
		{
			task_flash_music_direct(0);
		}
	}
	else
#endif
	{
		b_msc.file_prev_next = 0;                //�������
		b_msc.file_change = 1;
		if(sys_ctl.mode_play != ITEM_LOOP_SINGLE)
		{
			task_flash_music_direct(0);
		}
	}
}


//������Ϣ����
#pragma location="TASK_FLASH_SEG"
void task_flash_music_deal_msg(u8 msg)
{
	s8 whire_cnt;
	/*
	if(msg != NO_KEY)
	{
		//printf("F:%02x_", msg);
		scan_ctl.now_msg = msg;
		if(scan_ctl.now_msg == scan_ctl.last_msg)
		{

			return;
		}else{
			scan_ctl.last_msg = scan_ctl.now_msg;
		}
		printf("F:%02x_", msg);
	}
	*/
	/*
	if((sys_ctl.alarm_cnt == 1) && (sys_ctl.bat_sta_flag != 2))
	{
		printf("�͵���ʾ_");
		if(u_msc.music_num < Low_Battery_Num)
		u_msc.music_num_last = u_msc.music_num;
		task_flash_music_direct(1);
		u_msc.music_num = Low_Battery_Num;
		sys_ctl.bat_sta_flag = 2;
		alarm_flag = true;
	}
	*/
	switch(msg)
	{
	case T_KEY_SW1_WHIRL:	// A1B1 ��ת������
		IE_EA = 0;	// �ر��ж�, ���ڶ�ȡת��ֵ, ��ֹ�������ж��б���д, ���¼������
		whire_cnt = SW1_WHIRL_cnt;
		SW1_WHIRL_cnt = 0;
		IE_EA = 1;	// �����ж�
		if(whire_cnt != 0)							
		{	// ������� whire_cnt ����������
			if(scan_ctl.disp_light_set_flag == 0)	// ��������
			{
				if(whire_cnt == Whire_Left)
				{
					user_set_volume(sys_ctl.volume + 1);
					deal_vol_msg();
					//if(scan_ctl.hpdect_val < 200)	//����û�в���
					//if(HP_CHECK_PORT & BIT(HP_CHECK_PIN))	//����û�в���
					{
						if(sys_ctl.volume >= 1)
							UN_MUTE();
					}
					//printf("v+:%d ",sys_ctl.volume);
				}else if(whire_cnt == Whire_Right)
				{
					user_set_volume(sys_ctl.volume - 1);
					deal_vol_msg();
					if(sys_ctl.volume == 0)
					MUTE_EN();
					//printf("v-:%d ",sys_ctl.volume);
				}
			}else{				// ����LED����
				if(whire_cnt == Whire_Left)
				{
					if(t_pwn_duty1 == LED_LIGHT2)
					{
						t_pwn_duty1 = LED_LIGHT3;
						t_pwn_duty2 = LED_LIGHT3;
					}else if(t_pwn_duty1 == LED_LIGHT3){
						t_pwn_duty1 = LED_LIGHT1;
						t_pwn_duty2 = LED_LIGHT1;
					}else if(t_pwn_duty1 == LED_LIGHT1){
						t_pwn_duty1 = LED_LIGHT2;
						t_pwn_duty2 = LED_LIGHT2;
					}
				}else if(whire_cnt == Whire_Right)
				{
					if(t_pwn_duty1 == LED_LIGHT2)
					{
						t_pwn_duty1 = LED_LIGHT1;
						t_pwn_duty2 = LED_LIGHT1;
					}else if(t_pwn_duty1 == LED_LIGHT1){
						t_pwn_duty1 = LED_LIGHT3;
						t_pwn_duty2 = LED_LIGHT3;
					}else if(t_pwn_duty1 == LED_LIGHT3){
						t_pwn_duty1 = LED_LIGHT2;
						t_pwn_duty2 = LED_LIGHT2;
					}
				}
			}
		}
		break;

	case T_KEY_SW2_WHIRL:	// A2B2 ��ת������
		IE_EA = 0;	// �ر��ж�, ���ڶ�ȡת��ֵ, ��ֹ�������ж��б���д, ���¼������
		whire_cnt = SW2_WHIRL_cnt;
		SW2_WHIRL_cnt = 0;
		IE_EA = 1;	// �����ж�
		if(whire_cnt != 0)
		{	// ������� whire_cnt ����������
			//if(scan_ctl.timer_flag == 0)	// ��һ����ת
			//	scan_ctl.timer_flag = 1;
			if(scan_ctl.disp_clock_set_flag == 0)	// ���ö�ʱ
			{
				if(whire_cnt == Whire_Left)
				{
					if(scan_ctl.timer_flag < 4){
						scan_ctl.timer_flag++;
					}else if(scan_ctl.timer_flag == 4){
						scan_ctl.timer_flag = 1;
					}
				}else if(whire_cnt == Whire_Right)
				{
					if(scan_ctl.timer_flag > 1){
						scan_ctl.timer_flag--;
					}else if(scan_ctl.timer_flag == 1){
						scan_ctl.timer_flag = 4;
					}	
				}
			}else {									// ����ʱ��
				if(scan_ctl.count_KU_TIMER == 0)
				{
					if(whire_cnt == Whire_Left)
					{
						if(ocx.timeh < 23){
							ocx.timeh++;
						}else {
							ocx.timeh = 0;
						}
							
					}else if(whire_cnt == Whire_Right)
					{
						if(ocx.timeh > 0){
							ocx.timeh--;
						}else {
							ocx.timeh = 23;
						}
					}
				}else if(scan_ctl.count_KU_TIMER == 1)
				{
					if(whire_cnt == Whire_Left)
					{
						if(ocx.timel < 59){
							ocx.timel++;
						}else {
							ocx.timel = 0;
						}							
					}else if(whire_cnt == Whire_Right)
					{
						if(ocx.timel > 0){
							ocx.timel--;
						}else {
							ocx.timel = 59;
						}	
					}
				}
			}
		}
		break;

	case T_KEY_SW3_WHIRL:	// A3B3 ��ת������
		IE_EA = 0;	// �ر��ж�, ���ڶ�ȡת��ֵ, ��ֹ�������ж��б���д, ���¼������
		whire_cnt = SW3_WHIRL_cnt;
		SW3_WHIRL_cnt = 0;
		IE_EA = 1;	// �����ж�
		if(whire_cnt != 0)
		{	// ������� whire_cnt ����������
			if(whire_cnt == Whire_Left)
			{
				task_flash_music_direct(1);
				if((u_msc.music_num < u_msc.music_total-1))
				u_msc.music_num++; 			// ת����Ӧ�ĸ��� 
			}else if(whire_cnt == Whire_Right)
			{
				task_flash_music_direct(1);
				if(u_msc.music_num >= 1)
				u_msc.music_num--; 	
			}
			//printf("%d ",u_msc.music_num);
		}
		break;

#if 1
	#if 0	//  ͣ��ֱ�Ӳ��ŵ�ģʽ
    case T_KEY_1:
    case T_KEY_2:
    case T_KEY_3:
    case T_KEY_4:
    case T_KEY_5:
    case T_KEY_6:
	case T_KEY_7:
	case T_KEY_8:
	case T_KEY_9:
	case T_KEY_10:
        printf("Key:%d\n",msg);
		task_flash_music_direct(1);
		u_msc.music_num = msg-1;	// ת����Ӧ�ĸ���
		break;
	#endif
#else
    case T_KEY_NUM_0:
        printf("Night\n");
		task_flash_music_direct(1);
		u_msc.music_num = 0;
		break;

	case T_KEY_NUM_1:
		printf("Summer\n");
		task_flash_music_direct(1);
		u_msc.music_num = 1;
		break;

	case T_KEY_NUM_2:
		printf("Rain\n");
		task_flash_music_direct(1);
		u_msc.music_num = 2;
		break;
	#if 1
	case T_KEY_NUM_3:
		printf("Ocean\n");
		task_flash_music_direct(1);
		u_msc.music_num = 3;
		break;

    case T_KEY_NUM_4:
		printf("Fan\n");
		task_flash_music_direct(1);
		u_msc.music_num = 4;
		break;

	case T_KEY_NUM_5:
		printf("Brown\n");
		task_flash_music_direct(1);
		u_msc.music_num = 5;
		break;
	#endif
#endif
#if 0
	case KU_STORY:
#if 1
		u_spi.spi_play_mode = SINGLE_DIR;        //���ļ���ѭ������
		sys_ctl.flash_music_sta = CHILDREN;          //����ʫ���ļ���
		mp3_res_play_wait(CHILDREN);               //�����ļ�����ʾ��
		task_flash_music_sel_mode();
#else
		u_spi.spi_play_mode = SINGLE_DIR;        //���ļ���ѭ������
		sys_ctl.flash_music_sta = ENGLISH;          //����ʫ���ļ���
		mp3_res_play_wait(ENGLISH);               //�����ļ�����ʾ��
		task_flash_music_sel_mode();
#endif
		break;
#endif

#if MUSIC_EQ
	//EQ
	case KU_EQ:
    case KL_PLAYMODE:
		user_set_eq(sys_ctl.eq_num + 1);
		printf("eq_num=%02x\n", sys_ctl.eq_num);
		showeq();
		break;
#endif
#if 0	// 2016/06/29 ͣ���豸�л�����
//���� �л��豸
	case KL_PREV:
#if USE_Flash_BREAK_POINT
//				if(task_ctl.work_sta != TASK_EXIT)
				{
//					music_get_point(&u_msc.break_point);
//					u_msc.break_point.pos = flash_music.pos;
//					irtc_write_ram(RTCRAM_MUSIC_POS_SD1,(u8 *)&u_msc.break_point, 8);
				}
#endif
		if(0==(SD_CHECK_PORT & BIT(SD_CHECK_PIN)))
		{
			device_chg = BIT(device_flash);
			task_ctl.work_sta = TASK_MUSIC;
			printf("ChgDev.");
		}
		break;
#endif
#if 0

#if MUSIC_SWITCH_DEV
		t_msc.mode++;    //����ģʽ��USB0-USB1-SD0-SD1-����ģʽ
		tmp = device_get_actived_num();
		printf("tmp:%d\n",tmp);
		device_chg = true;
		if((t_msc.mode == tmp/*device_get_actived_num()*/) ||(fs_cur_dev()==DEVICE_SDMMC1)|| (!task_flash_music_next_device()))
		{
			sd_stop(1);
			if(device_activate(DEVICE_UDISK))
			{
				t_fat.dev_new = DEVICE_UDISK;    //ģʽ�л����´ν���MP3ģʽʱ�Ȳ���U��
			}
#if UDISK_DOUBLE_EN
			else  if(device_activate(DEVICE_UDISK1))
			{
				t_fat.dev_new = DEVICE_UDISK1;    //ģʽ�л����´ν���MP3ģʽʱ�Ȳ���U��
			}
#endif
			else  if(device_activate(DEVICE_SDMMC))
			{
				t_fat.dev_new = DEVICE_SDMMC;    //ģʽ�л����´ν���MP3ģʽʱ�Ȳ���U��
			}
			task_ctl.work_sta = TASK_EXIT;
		}
		if(b_dac.mute_en)
		{
			//���MUTE
			mute_disable();
		}
#else
		task_ctl.work_sta = TASK_EXIT;
#endif
		break;
#endif


#if DIR_SWITCH_EN
	case KU_PREVDIR:
		task_music_dir(1);
		break;

	case KU_NEXTDIR:
		task_music_dir(0);
		break;
#endif


	case KU_MODE:
	case KL_PLAY_AND_MODE:
		//printf("exit\n");
		task_ctl.work_sta = TASK_EXIT;
		break;

	case QSYSTEM_500MS:
		if(u_msc.music_num >= u_msc.music_total)
		{
			u_msc.music_num = 0;
		}
//		printf("flash..%lx, %lx, %lx\n", flash_music.addr, flash_music.len, flash_music.pos);
		music_get_point(&u_msc.break_point);
		u_msc.break_point.pos = flash_music.pos;
		irtc_write_ram(RTCRAM_MUSIC_POS_SD1,(u8 *)&u_msc.break_point, 8);
		break;

		//���ù�����Ϣ����
	default:
		deal_msg(msg);
		break;
	}
}

//����������

#pragma location="TASK_FLASH_SEG"
void task_flash_music_event(void)
{
#ifdef REMOVED
#if USE_SDCMD_IIC_SPIDAT
	comm_event(2);                          //IO����ʱ�����л�ģʽ���ٳ�ʼ���豸
#else
	comm_event(1);                          //���ù����¼�
#endif                         //���ù����¼�
#else
	if(u_spi.spi_online)
		comm_event(2);
	else
		comm_event(1);
#endif /* REMOVED */
	music_event();                          //������Ҫ���¼�
	//printf("num=%d,total=%d\n", u_msc.music_num,u_msc.music_total);
#if TASK_TIPS_EN
	task_ctl.pre_sta = TASK_FLASH;
#endif

	if(b_sys.dev_change)
	{
		if(!device_is_online(DEVICE_SDMMC))
			b_sys.dev_change = 0;
	}

		if(b_sys.dev_change || (device_chg & BIT(device_flash)))
		{
			device_chg = 0;
			music_stop();						//ֹͣ��ǰ����
			b_msc.file_change = 1;				//��Ҫ��һ�����ļ�
#if USE_Flash_BREAK_POINT
			b_msc.break_init = 1;
			//task_music_break_save(1);
			music_get_point(&u_msc.break_point);
		//	printf("@frame = %ld pos= %ld,", u_msc.break_point.frame, u_msc.break_point.pos);
			//printf("write flash:%ld,%ld,%ld\n",flash_music.addr,flash_music.len,flash_music.pos);
		// ȡ����ǰ�Ĳ���λ�ý��б���
			u_msc.break_point.pos = flash_music.pos;
			irtc_write_ram(RTCRAM_MUSIC_POS_SD1,(u8 *)&u_msc.break_point, 8);
#endif
			//printf("SD change write\n");
			write_flash_num_rtcram();
			task_ctl.work_sta = TASK_EXIT;
		}

		if(task_ctl.work_sta != TASK_FLASH)
		{
			music_stop();

			u_spi.spi_music_flag = 0;
			return;
		}

		//�����ļ�
#if 1

#if 0	// Melrin Remove 2016/04/15
		if(b_msc.file_change)
		{
			b_dac.mute_en = 1; user_set_mute(); // ��ʼ����
	#if REPEAT_AS_ONE
			if(b_msc.repeat_as_one_flag == 0)
	#endif
			{
			#if	1	//JianRong add 2015-06-10
				msc_ctl.mscfade_en = 1;//ʹ�ܵ���dac��������
			 	music_stop();
			#elif (DAC_FADE_EN==1)
			//	dac_fade_in(0);
			#endif
			}
		}
#endif
#else
		if(b_msc.file_change)
		{
			//if(music_get_status() > STATUS_PLAY_STOPPING)
			//	return;
#if LED_EN
			ocx.led_sta = LED_STA_FAST_GLITTER;
#endif
#if USE_Flash_BREAK_POINT
			if(b_msc.break_init)
			{
				rtcram_read_multi(RTCRAM_MUSIC_POS_SD1,(u8 *)&u_msc.break_point, 8);	//���SD_CMD��IIC���ã����ڶ��ļ�ǰ�����ϵ���Ϣ
			}
#endif

			task_flash_music_new();
			irtc_write_ram((1 * RTCRAM_OFFSET + RTCRAM_MUSIC_POS_SD + 8), (u8 *)&u_msc.music_num, 2);

			shownum(u_msc.music_num + 1);
			printf("1111u_msc.music_num=%d\n",u_msc.music_num+1);
#if USE_Flash_BREAK_POINT
			if(b_msc.break_init)
			{
				b_msc.break_init = 0;
				// need modify Merlin 2015/04/25
				//	printf("@frame = %ld pos= %ld,", u_msc.break_point.frame, u_msc.break_point.pos);
				//music_jump(&u_msc.break_point);
				flash_music.pos = u_msc.break_point.pos;
				//printf("flash:%ld,%ld,%ld ",flash_music.addr,flash_music.len,flash_music.pos);
			}
#endif
		}

#endif

#if USE_Flash_BREAK_POINT
		if(b_msc.break_init)
		{
			read_flash_num_rtcram();
		}
#endif

		task_flash_music_new();
#if USE_Flash_BREAK_POINT
		if(b_msc.break_init)
		{
			b_msc.break_init = 0;
			//task_music_break_init();
			rtcram_read_multi(RTCRAM_MUSIC_POS_SD1,(u8 *)&u_msc.break_point, 8);	// �����ϵ���Ϣ
			flash_music.pos = u_msc.break_point.pos;
		}
#endif

		task_music_cal_time();                      //���㵱ǰ��ʾ��ʱ��

#if 1	// ���Գ�ʽ  �޼���Ƶ  20170614
		{
			static u32 last_pos;
			if(last_pos != flash_music.pos)
			{
				last_pos = flash_music.pos;
			//	printf("f.%lx, %lx, %lx(%d).%d\n", flash_music.addr, flash_music.len, flash_music.pos, music_get_status(),u_msc.music_num);
			//	printf("f.%lx, %lx\n", flash_music.len, flash_music.pos);
				/*if(flash_music.pos == 0x50L)
				{
					flash_music.pos = flash_music.len - 0x50L;
				}*/
			}
		}
#endif

#if 1     //  20170509   wq
		if(flash_music.len <= flash_music.pos)   //if(flash_music.len-4 <= flash_music.pos)
		{
			if(sys_ctl.mode_play != ITEM_LOOP_SINGLE)
			{
				task_flash_music_auto_next();
			}
			else
			{
				if(!b_msc.pause)  //ͨ����ӡ���֣��������ͣʱ����������ᵼ�²��ź�û����������Ҳ�����ͣ���л�������Ҳһֱû����
				{
					printf("SINGLE Loop\n");
					/*
					if(sys_ctl.bat_sta_flag == 2)
					{
						if(task_ctl.last_work == TASK_FLASH)
						{
							task_flash_music_direct(1);
							if(u_msc.music_num_last != 6)
							{
								u_msc.music_num = u_msc.music_num_last;
							}else
							{
								u_msc.music_num = 0;
							}
						}else
						{
							if(u_msc.music_num_last != 6)
							{
								u_msc.music_num = u_msc.music_num_last;
							}else
							{
								u_msc.music_num = 0;
							}
							task_ctl.work_sta = task_ctl.last_work;
						}
						sys_ctl.bat_sta_flag = 3;
						sys_ctl.alarm_cnt = 0x00;
						alarm_flag = 0;
						printf("Alarm\n");
					}
					*/
					flash_music.pos = 0L;
					if(music_get_status() <= STATUS_PLAY_STOPPING)
					{
						b_msc.file_change = 1;
					}
				}
			}
		}
#else



		//�ж����ֵĲ���״̬
		if(music_get_status() <= STATUS_PLAY_STOPPING)
		{
#if REPEAT_AS_ONE
			b_msc.repeat_as_one_flag = 1;
#else
#if !ADD_BY_WUPENGFEI
			if(sys_ctl.mode_play != ITEM_LOOP_SINGLE)
#endif
				task_flash_music_auto_next();
#endif
		}
#endif
}
extern void spi_test_2(void);

/*****************************************************************************
 * Module    : SPI���ֲ����������
 *****************************************************************************/
//�����ʼ��
#pragma location="TASK_FLASH_SEG"
void task_flash_music_enter(void)
{
	set_sys_clk(SYS_24M);

	spi_sd_mux_enter();
	spi_port_init();
	u_spi.spi_online=spi_flash_init();      //���flash�Ƿ�����

	if(u_spi.spi_online)
	{
		u_spi.spi_music_flag = 1;
	}
	else
	{

		if(!(device_is_actived(DEVICE_SDMMC1) || device_is_actived(DEVICE_SDMMC1)))
		{
			printf("No Device!");
			showhello();	// û�п��õ�װ��, ��ʾ 888
			//delay_5ms(250);
		}
		task_ctl.work_sta = TASK_EXIT;
		return;
	}
	f_Encrypt = false;
	if(first==0)
	{
		disp_device();
		delay_5ms(50);
		first=1;
	}
	user_set_eq(0);//����EQ

#if DAC_FADE_EN
//	b_dac.sound_en = 0;                      //�رն�̬���� ���ӿ��������
	b_dac.sound_en = 0;                      //��̬����
	msc_ctl.mscfade_en = 0;
    b_dac.digvol_en = 1;  					//JianRong 2015-06-10
#else
	msc_ctl.mscfade_en = 0;
	b_dac.digvol_en = 1;
	dac_set_volume_direct(0);
#endif
	dac_set_volume_direct(0);
	//user_set_mute();
	memset(&u_msc, 0, sizeof(u_msc));			//��������ļ���Ϣ

	b_msc.pause = 0;                        //������Ϣ��ʼ��
	b_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
#if USE_Flash_BREAK_POINT
	b_msc.break_init = 1;					// 2015/06/06 Merlin
#endif

	u_msc.music_total = get_Flash_TotalFiles();	//ALL_MP3_TOTAL;
	printf("Total:%d\n",u_msc.music_total);
	u_spi.spi_play_mode = ALL_DIR;			//Ĭ��ȫ���ļ���ѭ��
	sys_ctl.flash_music_sta = 0;//VOICE;	//�ӵ�һ���ļ��п�ʼ��VOICE�ļ���
	//u_msc.music_num = 0;			//��һ�׸���

	read_flash_num_rtcram();	// ��Ŀ��λ�ö�������, �ϵ�λ�� ���� RTCRAM_MUSIC_POS_SD1

	// ����Ƿ񳬳���Χ
	if(u_msc.music_num > u_msc.music_total)
	{
		b_msc.break_init = 0;
		u_msc.music_num = 0;
	}


	//u_msc.music_num = 0;	// for test only



	//if(music_get_status() > STATUS_PLAY_STOPPING)
	//	return;
//#if LED_EN
	ocx.led_sta = LED_STA_ON;
//#endif
#if USE_Flash_BREAK_POINT
	if(b_msc.break_init || (device_chg  & BIT(device_music)))
	{
		rtcram_read_multi(RTCRAM_MUSIC_POS_SD1,(u8 *)&u_msc.break_point, 8);	// ���SD_CMD��IIC���ã����ڶ��ļ�ǰ�����ϵ���Ϣ
		printf("Break %ld, %ld\n", u_msc.break_point.frame, u_msc.break_point.pos);
	}
#endif
//		task_flash_music_new();
#if 1
//	music_stop();                               //ֹͣ������
	b_msc.file_change = 0;                      //����ָ������

	if(task_flash_music_init(u_msc.music_num))
	{
		music_play();
		b_msc.pause = 0;
		user_set_mute();
	}
#endif

#if REPEAT_AS_ONE
	b_msc.repeat_as_one_flag = 0;
#endif

	// ���浱ǰ��Ŀλ��
	irtc_write_ram(RTCRAM_MUSIC_NUM_SD1, (u8 *)&u_msc.music_num, 2);

#if USE_Flash_BREAK_POINT
	printf("device_chg = %d\n", device_chg);
	if(b_msc.break_init || (device_chg  & BIT(device_music)))
	{
		//��������ظ�������
		b_msc.break_init = 0;
		flash_music.pos = u_msc.break_point.pos;
		memset((void *)0x3cb,0x00,0x3D1-0x3CB);		// clear DAC_XDATA
		//memset((void *)0x7D0,0x00,0x1000-0x7D0);	// clear DECODER_BUF
		//printf("start memset\n");
		//printf("read flash:%ld,%ld,%ld\n",flash_music.addr,flash_music.len,flash_music.pos);
//		device_chg = 0;
	}
#endif

//#if LED_EN
	ocx.led_sta = LED_STA_ON;
//#endif

#if DISP_Show_Device
	ocx_msgbox("-1-");
#endif

	dac_set_volume(0);

#if 1	// same as task_music
	mute_disable();
	sound_ctl_init(10, 0x20, 100, 0x1a);
#else
	mute_disable();
	sound_ctl_init(1, 0x00, 100, 0x1a);     //�رյ��뵭��
#endif

	for(u8 i = 60; i; i--)
	{
		delay_5ms(1);
		music_event();
	}
	//dac_set_volume_direct(sys_ctl.volume);
	dac_set_volume(sys_ctl.volume);
//	task_flash_music_direct(0);
}

//�����˳�
#pragma location="TASK_FLASH_SEG"
void task_flash_music_exit(void)
{
	//delay_5ms(20);
	//mute_enable();
	SD_FLAG=1;
#if USE_Flash_BREAK_POINT
	if(task_ctl.work_sta != TASK_EXIT)
	{
		music_get_point(&u_msc.break_point);
		u_msc.break_point.pos = flash_music.pos;
		irtc_write_ram(RTCRAM_MUSIC_POS_SD1,(u8 *)&u_msc.break_point, 8);
	}
#endif

	printf("task flash exit\n");
	msc_ctl.mscfade_en = 1;			//�˳�����ģʽʱ�������������music_stop֮ǰ
	music_stop();
	b_dac.sound_en = 0;
	delay_5ms(20);
#if DAC_FADE_EN
	dac_fade_out();
	dac_fade_wait();
#endif
    user_change_volume(0);         // �˳�task_flash ��MUTEǰ����ǰ������Ϊ0����ʱ0.2s,��MUTE   20170901
	delay_5ms(40);
	mute_enable();
#ifdef REMOVED
#if USE_SDCMD_IIC_SPIDAT
	spi_sd_mux_exit();
	sd_port_init(1);
#endif
#else
	if(u_spi.spi_online)
	{
		spi_sd_mux_exit();
		//sd_port_init(1);
	}
	u_spi.spi_music_flag = 0;
#endif /* REMOVED */

}

//����������
#pragma location="TASK_FLASH_SEG"
void task_flash_music(void)
{
	printf("task_flash\n");
	task_flash_music_enter();
	while((task_ctl.work_sta == TASK_FLASH) && (Timer1_flag == false))
	{
		task_flash_music_event();
		task_flash_music_deal_msg(get_msg());
		//task_music_display();
	}
	task_flash_music_exit();
	if(task_ctl.work_sta != TASK_POWEROFF)
	{
	task_ctl.work_sta = TASK_IDLE;
	}
}
#endif
