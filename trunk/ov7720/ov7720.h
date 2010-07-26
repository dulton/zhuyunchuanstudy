/*
 * =====================================================================================
 *
 *       Filename:  ov7720.h
 *
 *    Description:  ov7720提供给上层的借口
 *
 *        Version:  1.0
 *        Created:  2010年07月22日 20时27分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <gpio_i2c.h>

typedef int (*FuntionSccbRead)(unsigned char addr, unsigned char data);
typedef int (*FuntionSccbWrite)(unsigned char addr, unsigned char data);
typedef int (*FuntionSccbReset)(void);


struct ov7720_pin
{
	SCL_PIN;	
	SDA_PIN;
	RESET_PIN;
};

typedef struct _OV7720
{
	void* priv;
	struct ov7720_pin pin;

	FuntionSccbRead read;
	FuntionSccbWrite write;
	FuntionSccbReset reset;
}OV7720;

OV7720* ov7720_create(FuntionSccbRead read, FuntionSccbWrite write, FuntionSccbReset rest, struct ov7720_pin pin);

