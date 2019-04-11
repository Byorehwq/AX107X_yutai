/*****************************************************************************
 * Module    : Task
 * File      : task_music.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ���ֲ�����������
 *****************************************************************************/
#include "include.h"

#define DBG_MUSIC(...)              printf(__VA_ARGS__)
#define MUSIC_ENCRYPT_EN            0

type_t_msc t_msc;
bool first;
extern bool SD_FLAG;

//ɨ���豸�ļ�
#pragma location="TASK_MUSIC_SEG"
static bool task_music_scan_file(void)
{
	if(task_music_scan_music())             //ɨ�������ļ���
	{
		task_ctl.work_sta = TASK_MUSIC;
		return true;
	}

	task_ctl.work_sta = TASK_EXIT;          //�豸��Ч���������˵�
	return false;
}

//���¹����ļ�ϵͳ
#pragma location="TASK_MUSIC_SEG"
static bool task_music_remount_device(void)
{
	//music_stop();                           //ֹͣ��ǰ����
	//printf("[%02x]\n",fs_cur_dev());
//#ifdef AX207X_TAG
	printf("remount%d,%d\n",fs_cur_dev(),device_is_actived(DEVICE_SDMMC1));
	return false;	// Merlin 2015/06/30
#if !ADD_BY_WUPENGFEI
	if((fs_cur_dev() == DEVICE_SDMMC) && (device_is_actived(DEVICE_SDMMC1)))
	{
		t_fat.dev_new = DEVICE_SDMMC1;
	}
//#endif
	if(!device_mount() || !task_music_scan_file())
	{
		return false;
	}
	return true;
#endif
}

//ѡ����һ���豸����
#pragma location="TASK_MUSIC_SEG"
bool task_music_next_device(void)
{
	if(!device_switch() || !task_music_remount_device())
	{
		return false;
	}
#if USE_BREAK_POINT
	b_msc.break_init = 1;
#endif

	mem_ctl.dev = fs_cur_dev();
	mem_read_music_num();                   //�򿪵��ļ����
	b_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
	return true;
}

#if AUTO_CHANGE_DEVICE
//�Զ���һ������һ�豸����
#pragma location="TASK_MUSIC_SEG"
static bool task_music_auto_device(void)
{
	u_msc.music_num = 1;
	mem_write_file_change();

#if USE_BREAK_POINT
	task_music_break_save(0);
#endif

	if(device_switch())                     //����Ƿ�����һ�豸
	{
		if(!task_music_remount_device())    //��һ�豸�����ļ�ϵͳ
		{
			if(!task_music_next_device())   //���ز��ɹ���ѡ��ԭ�豸
			{
				return false;
			}
		}
	}
	return true;
}
#endif

//��ʼ����music_num�׸�
#pragma location="MUSIC_SEG"
static bool task_music_init(u16 music_num)
{
	if(!f_open(music_num))
	{
		return false;
	}

#if DIR_SWITCH_EN
	u_msc.dir_num = t_fat.dir_count;
#endif

#if MUSIC_ENCRYPT_EN
	music_check_encrypt();//�ж��Ƿ�Ϊ����mp3
	music_head_decrypt();
#endif

#if WAV_SUPPORT
	u_msc.music_type = music_init();
#else
	u_msc.music_type = music_init_mp3_only();
#endif

	//printf("mtype:%d\n", u_msc.music_type);
#if WAV_SUPPORT
	if(u_msc.music_type == TYPE_WAV)
	{
		set_sys_clk(SYS_48M);               //WAVʱʹ��48Mʱ��
	}
	else
	{
		set_sys_clk(SYS_24M);
	}
#endif
	if(TYPE_NONE == u_msc.music_type)
	{
		return false;
	}
	return true;
}

#if DIR_SWITCH_EN
//0:��һ�ļ���/1:��һ�ļ���
#pragma location="TASK_MUSIC_SEG"
void task_music_dir(bool direct)
{
	if(u_msc.dir_total <= 1)
	{
		return;
	}

	u_msc.dir_num = loop_auto(direct, u_msc.dir_num, 1, u_msc.dir_total);

	music_stop();
	f_open_dir(u_msc.dir_num);
	b_msc.file_change = 1;
	u_msc.music_num = t_fat.file_count;
}
#endif

//0:��һ��/1:��һ��
#pragma location="TASK_MUSIC_SEG"
static void task_music_direct(bool direct)
{
#if (MUSIC_PLAYMODE && (PLAYMODE_NUM == PLAYMODE_FOUR))
	u16 dir_start, dir_last;
#endif

	b_msc.file_change = 1;
	b_msc.file_prev_next = direct;                       //���÷��򣬹��������ʱʹ��

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
		u_msc.music_num = loop_auto(direct, u_msc.music_num, 1, u_msc.music_total);
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

#if !REPEAT_AS_ONE
//�Զ���һ��
#pragma location="TASK_MUSIC_SEG"
static void task_music_auto_next(void)
{
#if !ADD_BY_WUPENGFEI
	if(music_get_mtime() < 5)
	{
		//���Ų���500ms����Ϊ�������
		if(b_msc.file_prev_next)                //��ʼ��ʧ�ܣ����¶�ȡ��һ���ļ�
		{
			task_music_direct(1);
		}
		else
		{
			task_music_direct(0);
		}
	}
	else
#endif
	{
		b_msc.file_prev_next = 0;                       //�������
		b_msc.file_change = 1;
		if(sys_ctl.mode_play != ITEM_LOOP_SINGLE)
		{
			task_music_direct(0);
		}
	}
}
#endif

//������ʾ��ʱ��
#pragma location="TASK_MUSIC_SEG"
void task_music_cal_time(void)
{
	u32 cur_sec = music_get_mtime() / 10;           //��ǰ����ʱ�䣬��ȷ��0.1s
	u_msc.cur_time.min = cur_sec / 60;
	u_msc.cur_time.sec = cur_sec % 60;
}

//��һ���µ�����
#pragma location="TASK_MUSIC_SEG"
void task_music_new(void)
{
	u16 err_cnt = 0;
	u16 err_total;
	if(sys_ctl.mode_play == ITEM_LOOP_RAMDOM)
	{
		err_total = u_msc.music_total * 2;          //�����������ʱ���������ѭ������
	}
	else
	{
		err_total = u_msc.music_total;
	}
	while(device_is_actived(fs_cur_dev()) && b_msc.file_change)
	{
#if REPEAT_AS_ONE
		if(b_msc.repeat_as_one_flag == 0)
		{
			music_stop();                               //ֹͣ������
		}
		else
		{
			b_msc.repeat_as_one_flag = 0;
		}
#else
		music_stop();//ֹͣ������
#endif
		b_msc.file_change = 0;
		//����ָ������

		sound_ctl_clr();
		if(task_music_init(u_msc.music_num))
		{
			//task_music_clear_a();                   //���A��
			printf("music_play[%d][%d]\n",fs_cur_dev(),u_msc.music_num);
			dac_fade_in(0);
			music_play();

			b_msc.pause = 0;
			//delay_5ms(5);                           //��ʱһС��ʱ�䣬�Խ⼸֡���������Ӧ��Ϣ
		}
		else
		{
			if(!fs_is_mounted() || ++err_cnt >= err_total)       //���е������ļ����޷�����
			{
				//msgbox(STR_FILE_ERROR, NULL);
				task_ctl.work_sta = TASK_EXIT;      //���������ļ����࣬�˳�Ŀ¼����
				break;
			}

			if(b_msc.file_prev_next)                //��ʼ��ʧ�ܣ����¶�ȡ��һ���ļ�
			{
				task_music_direct(1);
			}
			else
			{
				task_music_direct(0);
			}
		}
	}
}

//��ȡ�����ļ��ĸ���
#pragma location="TASK_MUSIC_SEG"
static bool task_music_scan_music(void)
{
	//����ɨ�������Ϊ�����ļ���
#if FS_SCAN_SPEED_EN
	f_scan_set(SCAN_SUB_FOLDER|SCAN_SPEED, &file_filter_music, &dir_filter_music);
#else
	f_scan_set(SCAN_SUB_FOLDER, &file_filter_music, &dir_filter_music);
#endif
	u_msc.music_total = f_gettotal();                   //��ȡ����������
	//printf("total: %d\n", u_msc.music_total);
#if DIR_SWITCH_EN
	u_msc.dir_total = t_fat.dir_count;
#endif
	return (u_msc.music_total != 0);
}

//����������
#pragma location="TASK_MUSIC_SEG"
static void task_music_event(void)
{
	comm_event(0);                          //���ù����¼�
	music_event();                          //������Ҫ���¼�
#if TASK_TIPS_EN
	task_ctl.pre_sta = TASK_MUSIC;
#endif
#if USE_IR_NUM
	if(ocx.event == 1)
	{
		ocx.event = 0;
		if(ocx.number == 0 || ocx.number > u_msc.music_total)
		{
			//u_msc.music_num = 1;
			//msgbox(" err");
		}
		else
		{
			u_msc.music_num = ocx.number;
			b_msc.file_prev_next = 0;                       //�������
			b_msc.file_change = 1;
		}
	}
#endif

	//����豸״̬
	if(!device_is_actived(fs_cur_dev()))
	{
		b_sys.dev_change = 1;               //��ǰ�豸��Ч���豸�����䶯
	}
	if(b_sys.dev_change)
	{
		device_chg |= BIT(device_music);
		music_stop();                       //ֹͣ��ǰ����
		b_msc.file_change = 1;              //��Ҫ��һ�����ļ�
#if USE_BREAK_POINT
		b_msc.break_init =1;
		task_music_break_save(1);
#endif
	}
	if(device_change())
	{
#if (SYS_MEM == MEM_DEVICE)
		mem_ctl.dev_mem_ready = 0;          //�豸�䶯���豸������Ч
#endif
		music_stop();                       //ֹͣ��ǰ����
		if((fs_cur_dev() == DEVICE_SDMMC) || ((fs_cur_dev() == DEVICE_SDMMC1) && (u_spi.spi_online == false)))
		{
			sd_stop(1);
		}
		if(!task_music_remount_device())
		{
			if(!task_music_next_device())
			{			
				task_ctl.work_sta = TASK_EXIT;      //�豸��Ч���˳�
				//showhello();	// Merlin �Ƴ�, ��Ϊ����ÿ�� Taskʱ����ʾ��Ӧ����Ϣ
				if(fs_cur_dev() == DEVICE_SDMMC)
				{
					b_sys.modeswitch_flag = 1;
				}
			}
		}
		b_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
		device_chg |= BIT(device_music);

		if(b_dac.mute_en)                       //���MUTE
		{
			mute_disable();
		}
#if USE_BREAK_POINT
		b_msc.break_init = 1;
#endif
		mem_ctl.dev = fs_cur_dev();
		mem_read_music_num();                   //�򿪵��ļ���
#if MUSIC_SWITCH_DEV
		t_msc.mode = 0;                             //�����л�ģʽ
#endif
		return;
	}

	//�����ļ�
	if(b_msc.file_change)
	{
		b_dac.mute_en = 1; user_set_mute();	// ��ʼ����

#if REPEAT_AS_ONE
		if(b_msc.repeat_as_one_flag == 0)
#endif
		{
			// music_stop();
			dac_fade_in(0);
		}
//#if (DISP_TYPE==DISP_LED)
		ocx.led_sta = LED_STA_ON;
//#endif
#if USE_BREAK_POINT
		if(b_msc.break_init)
		{
			mem_read_music_break();                 //���SD_CMD��IIC���ã����ڶ��ļ�ǰ�����ϵ���Ϣ
		}
#endif
		task_music_new();
		mem_write_file_change();                    //�ļ���ʼ���ɹ����ٱ�����Ŀ
		//shownum(u_msc.music_num);
		if(device_chg)
		{
			ocx_show_device();
			device_chg = 0;
		}
#if AB_REPEAT_EN
		u_msc.repeat = 0;
		task_music_set_a();
#endif
#if USE_BREAK_POINT
		if(b_msc.break_init)
		{
			b_msc.break_init = 0;
			task_music_break_init();
		}
#endif
	}

	//���㵱ǰ��ʾ��ʱ��
	task_music_cal_time();

	//�ж����ֵĲ���״̬
	if(music_get_status() <= STATUS_PLAY_STOPPING)
	{
        if(device_is_actived(fs_cur_dev())) {
            DBG_MUSIC("auto_next %d\n", music_get_status());
#if MUSIC_STOP
            if (!b_msc.stop)
#endif
            {
#if !REPEAT_AS_ONE
                task_music_auto_next();                      //����������ϣ�������һ��
#endif
            }
        } else {
            music_stop();
        }
    }
}


//������Ϣ����
#pragma location="TASK_MUSIC_SEG"
static void task_music_deal_msg(u8 msg)
{
	if(msg != NO_KEY)
		printf("M:%02x\n", msg);
	switch(msg)
	{
	//����ͬʱ���²��ŵ�һ�׸���
	#if 0
	case KL_PREV_NEXT:
		task_music_direct(0);
		u_msc.music_num = 1;
		irtc_write_ram((DEVICE_UDISK * RTCRAM_OFFSET + RTCRAM_MUSIC_NUM_SD), (u8 *)&u_msc.music_num, 2);
		irtc_write_ram((DEVICE_SDMMC * RTCRAM_OFFSET + RTCRAM_MUSIC_NUM_SD), (u8 *)&u_msc.music_num, 2);
		if(u_spi.spi_online)
			u_msc.music_num = 0;	// �� SPI-Flashʱ, ��Ŀ��0 ��ʼ����
		irtc_write_ram((DEVICE_SDMMC1 * RTCRAM_OFFSET + RTCRAM_MUSIC_NUM_SD), (u8 *)&u_msc.music_num, 2);
		u_msc.music_num = 1;
		printf("##hello##");
		break;
	#endif
		
		//��һ������һ��
	//case KL_PREV:	// ��Ϊ�����ӵ��� Merlin 2015/09/08
	case KU_PREV:
	case KU_PREV_VOL_DOWN:
         #if MUSIC_STOP
         		if(b_msc.stop)
         		{
         			break;
         		}
         #endif
         		if(u_msc.music_total > 1)
         		{
         			DBG_MUSIC("Prev\n");
         			task_music_direct(1);
         #if REPEAT_AS_ONE
         			b_msc.repeat_as_one_flag = 0;
         #endif
         			disp.update = 1;
         			ocx.disp_status = OCX_NULL;
         		}
         		break;
	case KU_NEXT:
	case KU_NEXT_VOL_UP:
#if MUSIC_STOP
		if(b_msc.stop)
		{
			break;
		}
#endif
		if(u_msc.music_total > 1)
		{

			DBG_MUSIC("Next\n");
			task_music_direct(0);
#if REPEAT_AS_ONE
			b_msc.repeat_as_one_flag = 0;
#endif
			disp.update = 1;
			ocx.disp_status = OCX_NULL;
		}
		break;

		//���š���ͣ
	case KU_PLAY:
	//case KL_PLAY:
	case KU_PLAY_AND_MODE:

	#if 0
		// test resule is fail on AX2070 SRAM board
		IE_EA = 0;
		u8 v = (irtc_read_cmd(CFG_RD) & BIT(1));
		IE_EA = 1;
		printf("P:%c",v ? '1' : '0');
	#endif

		if(music_start_pause())
		{
			b_msc.pause = 0;            //������ʾ״̬
//#if (DISP_TYPE==DISP_LED)
			ocx.led_sta = LED_STA_ON;
//#endif
			DBG_MUSIC("play\n");
			user_set_mute();
			disp.update = 1;
			ocx.disp_status = OCX_NULL;
		}
		else
		{
			b_msc.pause = 1;
//#if (DISP_TYPE==DISP_LED)
			ocx.led_sta = LED_STA_SLOW_GLITTER;
//#endif
			DBG_MUSIC("Pause\n");
			user_set_mute();
			disp.update = 1;
			ocx.disp_status = OCX_NULL;
		}
#if MUSIC_STOP
		if(b_msc.stop)
		{
			b_msc.stop = 0;
			u_msc.music_num = 1;
			b_msc.file_change = 1;
			shownum(u_msc.music_num);
		}
#endif
		disp.update = 1;
		break;

#if FAST_FORW_BACKW_EN
#if 0
		//���������
	case KL_PREV:
	case KH_PREV:
#if MUSIC_STOP
		if(b_msc.stop)
		{
			break;
		}
#endif
		DBG_MUSIC("P");
#if PLAUSE_FAST_FORW_BACKW
		if(b_msc.pause)
		{
			b_msc.pause = 0;
			msc_ctl.status = STATUS_PLAY_PLAYING;
		}
#endif
		music_fast(MUSIC_FAST_BACKWORD, 3);             //����
		music_fast_end();                               //�������Ͻ������Ϳ��Գ���
		break;
#endif

	case KL_NEXT:
	case KH_NEXT:
#if MUSIC_STOP
		if(b_msc.stop)
		{
			break;
		}
#endif
		DBG_MUSIC("N");
#if PLAUSE_FAST_FORW_BACKW
		if(b_msc.pause)
		{
			b_msc.pause = 0;
			msc_ctl.status = STATUS_PLAY_PLAYING;
		}
#endif
		music_fast(MUSIC_FAST_FORWORD, 3);              //���
		music_fast_end();                               //������Ͻ������Ϳ��Գ���
		break;

	//case KLU_PREV:
	case KLU_NEXT:
		DBG_MUSIC("E");
		music_fast_end();                           //������˽���
#if PLAUSE_FAST_FORW_BACKW
		if(!b_dac.mute_en)
		{
#if DAC_FADE_EN
			dac_fade_in(1);
#endif
		}
#endif
		break;
#endif

#if MUSIC_EQ
		//EQ
	case KU_EQ:
	case KL_PLAYMODE:
		user_set_eq(sys_ctl.eq_num + 1);
		DBG_MUSIC("EQ=%02x\n", sys_ctl.eq_num);
		showeq();
		break;
#endif

#if AB_REPEAT_EN
		//AB����
	case KL_EQ:
#if MUSIC_STOP
		if(b_msc.stop)
		{
			break;
		}
#endif
		if(u_msc.repeat == 0)
		{
			task_music_set_a();    //����A��
			u_msc.repeat = 1;
		}
		else if(u_msc.repeat == 1)
		{
			task_music_repeat_play();
			u_msc.repeat = 2;
		}
		else if(u_msc.repeat == 2)
		{
			music_get_point(&u_msc.point.pb);
			music_jump(&u_msc.point.pb);
			u_msc.repeat = 0;
		}
		disp.update = 1;
		break;
#endif

#if ENERGY_DISP_EN
	case KL_PLAYMODE:
#if MUSIC_STOP
		if(b_msc.stop)
		{
			break;
		}
#endif
		if(!ocx.disp_energy)
		{
			energy_start();
		}
		else
		{
			energy_stop();
		}
		break;
#endif

#if MUSIC_STOP
	case KU_STOP:
		if(!b_msc.stop)
		{
			music_stop();
			b_msc.stop = 1;
		}
		disp.update = 1;
//#if (DISP_TYPE==DISP_LED)
		ocx.led_sta = LED_STA_ON;
//#endif
		break;
#endif

#if USE_IR_NUM
	case T_KEY_NUM_0:
	case T_KEY_NUM_1:
	case T_KEY_NUM_2:
	case T_KEY_NUM_3:
	case T_KEY_NUM_4:
	case T_KEY_NUM_5:
	case T_KEY_NUM_6:
	case T_KEY_NUM_7:
	case T_KEY_NUM_8:
	case T_KEY_NUM_9:
#if MUSIC_STOP
		if(b_msc.stop)
		{
			break;
		}
#endif
		shownumsel(msg - T_KEY_NUM_0);
		break;
#endif
	case KL_PLAY:
		task_ctl.work_sta = TASK_EXIT;
		break;

#if 0 	// 2015-06-29 ͣ���豸�л�
	case KL_PREV:
	
#if USE_BREAK_POINT
		task_music_break_save(1);
#endif
#if MUSIC_SWITCH_DEV
		t_msc.mode++;    //����ģʽ��USB0-USB1-SD0-SD1-����ģʽ
		{
			u8 tmp;
			tmp = device_get_actived_num();
			//printf("tmp:%d\n",tmp);
		}
		device_chg |= BIT(device_music);
		if((t_msc.mode == tmp/*device_get_actived_num()*/) ||(fs_cur_dev()==DEVICE_SDMMC1)|| (!task_music_next_device()))
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
		//���ù�����Ϣ����
	default:
		//K_T=0;
		deal_msg(msg);
		break;
	}
}

/*****************************************************************************
 * Module    : ��������ϵ����
 *****************************************************************************/
#if USE_BREAK_POINT
//����ϵ�
#pragma location="TASK_MUSIC_SEG"
void task_music_break_save(bool flag)
{
	if(flag)
	{
		music_get_point(&u_msc.break_point);
		u_msc.filename_checksum = music_filename_check();
	}
	else
	{
		u_msc.break_point.pos = 0;
		u_msc.break_point.frame = 0;
		u_msc.filename_checksum = 0;
	}
	mem_write_music_break();
	//printf("save:frame=%08lu pos=%08lu\n",u_msc.break_point.frame,u_msc.break_point.pos);
	//printf("num=%02x checksum=%02x\n", u_msc.music_num,u_msc.filename_checksum);
}

//�����������������ϵ㲥��
#pragma location="TASK_MUSIC_SEG"
void task_music_break_init(void)
{
#if (SYS_MEM == MEM_RTCRAM)
	printf("read:frame=%08lu pos=%08lu\n",u_msc.break_point.frame,u_msc.break_point.pos);
	music_jump(&u_msc.break_point);         //RTCRAM�ռ䲻�����������ļ���У���
#else
	if(music_filename_check() == u_msc.filename_checksum)
	{
		music_jump(&u_msc.break_point);
	}
#endif
}
#endif

/*****************************************************************************
 * Module    : ��������AB����
 *****************************************************************************/
#if AB_REPEAT_EN
//����A��
#pragma location="TASK_MUSIC_SEG"
static void task_music_set_a(void)
{
	music_get_point(&u_msc.point.pa);
	music_set_a_point(&u_msc.point.pa);
	//printf("a fra:%08lu\n",u_msc.point.pa.frame);
	//printf("a pos:%08lu\n",u_msc.point.pa.pos);
}

//��ʼ����
#pragma location="TASK_MUSIC_SEG"
static void task_music_repeat_start(u8 repeat_cnt)
{
	music_get_point(&u_msc.point.pb);
	music_jump(&u_msc.point.pa);
	//printf("b fra:%08lu\n",u_msc.point.pb.frame);
	//printf("b pos:%08lu\n",u_msc.point.pb.pos);
	music_repeat(repeat_cnt, &u_msc.point);
}

//����/����
#pragma location="TASK_MUSIC_SEG"
static void task_music_repeat_play(void)
{
	if(b_msc.pause)
	{
		return;
	}
	task_music_repeat_start(REPEAT_LOOP);
}
#endif


/*****************************************************************************
 * Module    : �����������
 *****************************************************************************/
//�����ʼ��
#pragma location="TASK_MUSIC_SEG"
static void task_music_enter(void)
{
#if TASK_FLASH_EN
	u_spi.spi_music_flag = 0;
#endif
	if(!user_music_enter())
	{
		ocx_msgbox("---");
		task_ctl.work_sta = TASK_EXIT;      //�豸��Ч���������˵�
		return;
	}
	if(first==0)
	{
		disp_device();
		delay_5ms(60);
		first=1;
	}
#if DAC_FADE_EN
	b_dac.sound_en = 0;                     //��̬����
	msc_ctl.mscfade_en = 0;
    b_dac.digvol_en = 1;  					//JianRong -2015-06-10
#else
	msc_ctl.mscfade_en = 0;
	b_dac.digvol_en = 1;
	dac_set_volume_direct(sys_ctl.volume);
#endif
	memset(&u_msc, 0, sizeof(u_msc));			//��������ļ���Ϣ
	user_set_eq(0);//����EQ
	//������Ϣ��ʼ��
	b_msc.pause = 0;
#if MUSIC_STOP
	b_msc.stop = 0;
#endif
	b_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
#if USE_BREAK_POINT
	b_msc.break_init = 1;
#endif
#if AB_REPEAT_EN
	u_msc.repeat = 0;                       //����AB����
#endif
#if MUSIC_SWITCH_DEV
	t_msc.mode = 0;                         //�����л�ģʽ
#endif

#if REPEAT_AS_ONE
	b_msc.repeat_as_one_flag = 0;
#endif

//#if (DISP_TYPE==DISP_LED)
	ocx.led_sta = LED_STA_ON;
//#endif
	if(!task_music_scan_file())
	{
		if(!task_music_next_device())
		{
			//showhello();	// Merlin �Ƴ�, ��Ϊ����ÿ�� Taskʱ����ʾ��Ӧ����Ϣ
			task_ctl.work_sta = TASK_EXIT;  //�豸��Ч���������˵�
			return;
		}
	}
	mem_read_music_num();                //�򿪵��ļ����

#if MUSIC_STOP
	if(b_msc.stop)
	{
		b_msc.stop = 0;
		u_msc.music_num = 1;
#if USE_BREAK_POINT
		b_msc.break_init = 0;               //stopģʽ�󣬴ӵ�һ�׸�������뿪ʼ����
#endif
	}
#endif

//#if (DISP_TYPE==DISP_LED)
	ocx.led_sta = LED_STA_ON;
//#endif
	//shownum(u_msc.music_num);               //��ʾ��ǰ��Ŀ
	ocx_show_device();

#if DACR_ONLY
#ifdef AX207X_TAG
	dac_channel_disconnect();
#endif
#endif

#if KARAOKE_EN
	b_sys.karaoke_flag = 0;

	if(device_is_actived(DEVICE_MIC))
	{
		mic_enable();
		b_sys.karaoke_flag = 1;
		b_dac.sound_en = 0;                //��MIC��ʱ������̬����
		msc_ctl.mscfade_en = 0;            //���ֲ���ģʽ���л�����Ҫ���뵭��
	}
#endif

	mute_disable();
	sound_ctl_init(10, 0x20, 100, 0x1a);
#if ENERGY_DISP_EN
	energy_init();
#endif
#if (MUSIC_PLAYMODE && (PLAYMODE_NUM == PLAYMODE_FOUR))
	rand_init(12345 ^ TMR2CNTL);
#endif
}

//�����˳�
#pragma location="TASK_MUSIC_SEG"
static void task_music_exit(void)
{
	SD_FLAG=0;
#if USE_BREAK_POINT
	if(task_ctl.work_sta != TASK_EXIT)
	{
		task_music_break_save(1);
	}
#endif
#if ENERGY_DISP_EN
	energy_stop();
#endif
	msc_ctl.mscfade_en = 1;             //�˳�����ģʽʱ�������������music_stop֮ǰ
	music_stop();
	delay_5ms(20);

#if (SYS_MEM != MEM_DEVICE)
	if(device_is_online(DEVICE_UDISK))
	{
		usb_host_suspend();
	}
#endif

#if (USE_I2C_SD_MUX || FM_CLK_IO_MUX_SDCLK)
	if(device_is_online(DEVICE_SDMMC))
	{
		set_sd_idle_state();           //�˳�MP3�󿨽���IDLEģʽ����ֹSD������Ӱ��IIC
	}
#endif

	b_dac.sound_en = 0;
#if DAC_FADE_EN
	dac_fade_out();
	dac_fade_wait();
#endif
	mute_enable();

#if WAV_SUPPORT
	set_sys_clk(SYS_24M);              //WAV����ʱ�˳�MP3ģʽ�л�24MHz
#endif

#if KARAOKE_EN
	if(device_is_actived(DEVICE_MIC))
	{
		mic_disable();
	}
#endif
}

//����������
#pragma location="TASK_MUSIC_SEG"
void task_music(void)
{
	printf("task_music\n");
	task_music_enter();
	while(task_ctl.work_sta == TASK_MUSIC)
	{
		task_music_event();
		task_music_deal_msg(get_msg());
		task_music_display();
	}
	task_music_exit();
}

