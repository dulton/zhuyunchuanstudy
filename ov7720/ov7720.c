/*
 * =====================================================================================
 *
 *       Filename:  ov7720.c
 *
 *    Description:  ov7720操作实现
 *
 *        Version:  1.0
 *        Created:  2010年07月22日 21时13分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include <ov7720.h>
#include <ov7720_def.h>
#include <linux/slab>

struct ov7720_pin
{
	SCL_PIN;	
	SDA_PIN;
	RESET_PIN;
};

/*
static OV7720 ov7720 = 
{
	.priv = (struct ov7720)* ov,
	.read = sccb_read,
	.write = sccb_write,
	.reset  = sccb_reset,
};*/

/*
#define K6_SCL_PIN 6 
#define K6_SDA_PIN 47
#define K6_RESET_PIN 7
*/

/*
static struct ov7720_pin k6_pin = 
	.SDA_PIN = K6_SDA_PIN,
	.RESET_PIN = K6_RESET_PIN,
}
*/

OV7720* ov7720_create(FuntionSccbRead read, 
					  FuntionSccbWrite write, 
					  FUnFuntionSccbReset reset,
					  struct ov7720_pin pin)
{
	OV7720* pOV7720 = kmalloc(sizeof(struct OV7720), GFP_KERNEL);

	pOV7720->pin = pin;
	pOV7720->priv = NULL;
	pOV7720->read = read;
	pOV7720->write = write;
	pOV7720->reset = reset;

	return pOV7720;
}
