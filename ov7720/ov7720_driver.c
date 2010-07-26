/*
 * =====================================================================================
 *
 *       Filename:  ov7720_driver.c
 *
 *    Description:  ov7720驱动函数的实现
 *
 *        Version:  1.0
 *        Created:  2010年07月22日 21时20分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/arch/gio.h>
#include <linux/errno.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/wait.h>
#include <asm/uaccess.h>
#include <asm/arch/hardware.h>

#include "ov7720_def.h"
#include "gpio_i2c.h"
#include "ov7720.h"

#define OV7720_NAME "ov7720"
#define OV7720_MAJOR 144
#define OV7720_MINOR 0

static struct ov7720_pin k6_pin = 
{
	.SCL_PIN = K6_SCL_PIN,
	.SDA_PIN = K6_SDA_PIN,
	.RESET_PIN = K6_RESET_PIN,
}


