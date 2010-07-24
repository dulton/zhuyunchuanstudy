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

struct OV7720
{
	void* priv;
	FuntionSccbRead sccb_read;
	FuntionSccbWrite sccb_write;
	FuntionSccbReset reset;
};

struct OV7720* ov7720_create();
