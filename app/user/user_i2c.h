/*--------------------------------------------------------------------------*/
/**@file I2C.h
   @brief I2C接口驱动
   @details I2C接口驱动操作函数
   @author LZZ
   @date   2010-08-17
   @note   none
*/
/*----------------------------------------------------------------------------*/

# ifndef __I2C_OTP_H__
# define __I2C_OTP_H__

extern void i2c_start_mux(void);
extern void i2c_stop_mux(void);
extern void i2c_start(void);
extern void i2c_stop(void);
extern void i2c_send_byte(u8 data);
extern u8 i2c_read_byte(void);
extern u8 i2c_recv_ack(void);
extern void i2c_send_ack(void);
extern void i2c_send_nack(void);
extern u8 i2c_read(u8 addr);
extern void i2c_write(u8 addr, u8 *pdata, u8 len);

#define fm_i2c_start()          i2c_start_mux()
#define fm_i2c_stop()           i2c_stop_mux()
#define fm_i2c_send_byte(a)     i2c_send_byte(a)
#define fm_i2c_read_byte()      i2c_read_byte()
#define fm_i2c_recv_ack()       i2c_recv_ack()
#define fm_i2c_send_ack()       i2c_send_ack()
#define fm_i2c_send_nack()      i2c_send_nack()


#endif /* __I2C_OTP_H__ */
