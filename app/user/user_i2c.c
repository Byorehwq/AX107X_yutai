#include "include.h"

#if USE_I2C_SD_MUX

__no_init bool __bit volatile soft_int;

//����i2cǰҪ����enter����
#pragma location="I2C_CB_SEG"
void i2c_sd_mux_enter(void)
{
	if(sd_chk.group == DEVICE_SDMMC)
	{
		if(device_is_online(DEVICE_SDMMC)
#if FM_CLK_IO_MUX_SDCLK
		        && (fm_ctl.fm_mode_flag == 0)
#endif
		  )
		{
			soft_int = IE_SINT;
			IE_SINT = 0;
			sd_stop(1);
		}
		SDCON0 = 0;
	}
}

//������i2c��Ҫ����exit����
#pragma location="I2C_CB_SEG"
void i2c_sd_mux_exit(void)
{
	if(sd_chk.group == DEVICE_SDMMC)
	{
		if(device_is_online(DEVICE_SDMMC)
#if FM_CLK_IO_MUX_SDCLK
		        && (fm_ctl.fm_mode_flag == 0)
#endif
		  )
		{
			sd_port_init(0);
			SDCON0 = 1;
			IE_SINT = soft_int;
		}
		else
		{
			SDCON0 = 0;
		}
	}
#if (((FM_CLOCK == USE_SD_IO_32KHZ_CLK) || (FM_CLOCK == USE_SD_IO_12MHZ_CLK)) && (TASK_FM_EN))
	if(fm_ctl.fm_mode_flag == 1)
	{
		SDCON0 |= (BIT(0) | BIT(1));        //IICͨ�Ž����ָ�SDCLK
	}
#endif
}
#endif

#pragma location="I2C_CB_SEG"
void i2c_port_init(void)
{
#if USE_I2C_SD_MUX
	i2c_sd_mux_enter();
#endif

	/* ����ͨIO��⸴��ʱ��SCL��ΪDIR_OUT */
	I2C_SDA_PU |= BIT(I2C_SDA_PIN);
	I2C_SCL_PU |= BIT(I2C_SCL_PIN);
	I2C_SCL_DIR &= ~BIT(I2C_SCL_PIN);
}

#pragma location="I2C_CB_SEG"
void i2c_port_finish(void)
{
	/* ����ͨIO��⸴��ʱ��SCL��ΪDIR_IN */
	I2C_SCL_PU |= BIT(I2C_SCL_PIN);
	I2C_SCL_DIR |= BIT(I2C_SCL_PIN);

#if USE_I2C_SD_MUX
	i2c_sd_mux_exit();
#endif
}

#pragma location="I2C_CB_SEG"
void i2c_sda_dirout(void)
{
	I2C_SDA_DIR &= ~BIT(I2C_SDA_PIN);
}

#pragma location="I2C_CB_SEG"
void i2c_sda_dirin(void)
{
	I2C_SDA_DIR |= BIT(I2C_SDA_PIN);
}

#pragma location="I2C_CB_SEG"
void i2c_scl_dirout(void)
{
	I2C_SCL_DIR &= ~BIT(I2C_SCL_PIN);
}

#pragma location="I2C_CB_SEG"
void i2c_sda_high(void)
{
	I2C_SDA_PORT |= BIT(I2C_SDA_PIN);
}

#pragma location="I2C_CB_SEG"
void i2c_sda_low(void)
{
	I2C_SDA_PORT &= ~BIT(I2C_SDA_PIN);
}

#pragma location="I2C_CB_SEG"
void i2c_scl_high(void)
{
	I2C_SCL_PORT |= BIT(I2C_SCL_PIN);
}

#pragma location="I2C_CB_SEG"
void i2c_scl_low(void)
{
	I2C_SCL_PORT &= ~BIT(I2C_SCL_PIN);
}

/* ����ֵ����Ϊbool�� */
#pragma location="I2C_CB_SEG"
bool i2c_sda_read(void)
{
	return (I2C_SDA_PORT & BIT(I2C_SDA_PIN)) ? 1 : 0;
}

#pragma location="I2C_CB_SEG"
void i2c_delay(void)
{
#if FM_QN8035_EN
	delay_nop(60);//��FM��ʱ���������
#elif FM_RDA5807_EN
	delay_nop(30);//��FM��ʱ���������
#else
	delay_nop(10);
#endif
}

#pragma location="I2C_CB_SEG"
void i2c_start_mux(void)
{
	i2c_port_init();
	i2c_start();
}

#pragma location="I2C_CB_SEG"
void i2c_stop_mux(void)
{
	i2c_stop();
	i2c_port_finish();
}

#pragma location="I2C_CB_SEG"
void i2c_delay_2(void)
{
#if FM_QN8035_EN
	delay_nop(60);//��FM��ʱ���������
#elif FM_RDA5807_EN
	delay_nop(30);//��FM��ʱ���������
#else
	delay_nop(10);
#endif
}
