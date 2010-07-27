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

static u32 ov7720_major = 144;
static u32 ov7720_minor = 0;
static 

static int ov7720_open(struct inode *inode, struct file *filep);
static int ov7720_release(struct inode *inode, struct file *filep);
static int ov7720_ioctl(struct inode *inode, struct file *filep, int cmd, unsigned long arg);

static OV7720* ov7720 = NULL;

static struct ov7720_pin k6_pin = 
{
	.SCL_PIN = K6_SCL_PIN,
	.SDA_PIN = K6_SDA_PIN,
	.RESET_PIN = K6_RESET_PIN,
}

struct file_operations ov7720_fops = 
{
	.owner = THIS_MODULE,
	.open = ov7720_open,
	.write = ov7720_write,
	.ioctl = ov7720_ioctl,
}

/* 操作寄存器， 对ov7720进行初始化设置*/
static int ov7720_dev_init()
{
}

static int ov7720_init(void)
{
	dev_t dev = 0;
	dev_t dev_num = 0;
	int ret = -1;
	//struct file_operations *ov7720_fops = NULL;

		
	dev = MKDEV(major, minor);
	if((ret = register_chrdev_region(dev, 1, OV7720_NAME)) != 0)
	{
		printk("register ov7720 error");
		return ret;
	}
	
	dev_num = MKDEV(major, minor);
	struct cdev *ov7720_dev = cdev_alloc();
	if(NULL == ov7720_dev)
	{
		printk("alloc ov7720_dev erro");
	}
	ov7720_dev->fops = &ov7720_fops;
	cdev_init(ov7720_dev, &ov7720_fops);
	ov7720_dev->owner = THIS_MODULE;
	kobject_set_name(&(ov7720_dev->kobj), "%s", OV7720_NAME);
	if((ret = cdev_add(ov7720_dev, dev_num, 1)) != 0)
	{
		printk("ov7720 cdev_add error");
	}

	if(NULL == (ov7720 = ov7720_create(sccb_read, sccb_write, sccb_reset, k6_pin)))
	{
		printk("create ov7720 struct error");
	}
}

static int ov7720_exit(void) 
{
	
}

module_init(ov7720_init)
module_exit(ov7720_exit)

MODULE_LICENCE("GPL")
MODULE_AUTHOR("Zhu Yunchuan")
