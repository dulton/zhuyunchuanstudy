/*
 * =====================================================================================
 *
 *       Filename:  gpio_i2c.h
 *
 *    Description:  gpio模拟I2C时序，提供给SCCB的接口
 *
 *        Version:  1.0
 *        Created:  2010年07月22日 20时31分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include <ov7720_def.h>

//int i2c_read_byte(unsigned char tagdevaddr, unsigned char addr);
//int i2c_write_byte(unsigned char tagdevaddr, unsigned char addr,unsinged char data);

int write_sccb(unsigned char addr, unsigned char data);
int read_sccb(unsigned char addr, unsigned char data);


