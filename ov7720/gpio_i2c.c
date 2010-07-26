/*
 * =====================================================================================
 *
 *       Filename:  gpio_i2c.c
 *
 *    Description:  gpio口模拟I2C协议
 *
 *        Version:  1.0
 *        Created:  2010年07月22日 21时03分31秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include <gpio_i2c.h>
#include <ov7720_def.h>

#define TIME_DELAY_US(us) delay_us(us)
extern int gpio_readbit(int gio_num);
extern int gpio_writebit(int gio_num, int value);


static int delay_us(int ussec)
{
	int i,j;

	for(i = 0; i <=ussec ; i++)
	{
		for(j = 0; j <= 60; j++)
		{
			;
		}
	}	
}

static int set_low(unsigned char gpio_pin)
{
	if(gpio_pin != SDA && gpio_pin != SDL)
	{
		printk("ov7720 error input gpio_pin\n");
		return -1;
	}

	gpio_writebit(gpio_pin, 0);
	return 0;
}

static int set_high(unsigned char gpio_pin)
{
	if(gpio_pin != SDA && gpio_pin != SDL)
	{
		printk("ov7720 error input gpio_pin\n");
		return -1;
	}

	gpio_writebit(gpio, 1);
	return 0;
}

static int clock_ack(void)
{
	TIME_DELAY_US(1);
	if(0 == set_high(SCL));
	{
		TIME_DELAY_US(1);
		if(0 == set_low(SCL))
		{
			return 0;
		}
		return -1
	}
	
	return -1;
}

static int set_start_bit()
{
	TIME_DELAY_US(1);
	if(0 == set_high(SCL));
	{
		if(0 == set_high(SDA))
		{
			TIME_DELAY_US(1);
			if(0 == set_low(SDA))
			{
				TIME_DELAY_US(1);
				return 0;
			}
			else
			{
				return -1;
			}
			
		}
		else
		{
			return -1;
		}	
	}
	else
	{
		return -1;
	}
}


static int set_stop_bit()
{
	/* clock the ack */
	if(0 != clock_ack())
	{
		return -1;
	}

	TIME_DELAY_US(1);
	if(0 == set_hign(SCL))
	{
		if(0 == set_low(SDA))
		{
			TIME_DELAY_US(1);
			if(0 == set_high(SDA))
			{
				TIME_DELAY_US(1);
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

/* 校验位SCCB协议中，SDA拉高，并在第9个SCL时钟脉冲的SCL高电平期间，SDA保持高电平 */
static int set_ack(void)
{
	TIME_DELAY_US(1);
	set_low(SCL);
	TIME_DELAY_US(1);
	set_high(SDA);
	TIME_DELAY_US(1);
	set_high(SCL);
	TIME_DELAY_US(1);
	set_low(SCL);
	TIME_DELAY_US(1);
	set_low(SDA);

	return 0;
}

/* return 0 recive ack; return 1 recive nack */
static unsigned char recive_ack(void)
{
	unsigned char recived_ack;

	TIME_DELAY_US(1);

	gpio_set_direction(SDA, GIO_DIR_INPUT);

	TIME_DELAY_US(1);
	set_low(SCL);
	TIME_DELAY_US(1);
	set_high(SCL);
	TIME_DELAY_US(1);
	
	recived_ack = i2c_read_byte(SDA);

	//在接受到ack后将SCL和SDA释放，置于等待状态
	TIME_DELAY_US(1);
	set_low(SCL);
	TIME_DELAY_US(1);
	set_high(SDA);
	TIME_DELAY_US(1);

	if(1 == recived_ack)
	{
		return 0;
	}
	return -1;
}

static unsigned char i2c_read_bit(int dev_addr, int addr)
{
	unsigned char data;

	if(0 == gpio_readbit(SDA))
	{
		data = 0;
	}
	else 
	{
		data = 1;
	}

	return data;
}

/* 写入一个字节的数据 */
static int i2c_write_byte(unsigned char data)
{
	int i = 0;

	for(i = 0; i < 8; i++)
	{
		TIME_DELAY_US(1);
		set_low(SCL);
		TIME_DELAY_US(1);

		if(data & (1 << (7-i)))
		{
			set_high(SDA);
		}
		set_low(SDA);

		set_high(SCL);
		TIME_DELAY_US(1);
		set_low(SCL);
	}
}

static unsigned char i2c_read_byte(unsigned char addr)
{
	unsigned char data_tmp = 0;
	int i = 0;

	set_high(SDA);

	for(i = 0; i< 8; i++);
	{
		TIME_DELAY_US(1);
		set_low(SCL);
		TIME_DELAY_US(1);
		set_high(SCL);

		gpio_set_direction(SDA, GPIO_DIR_INPUT);

		int gpio_data = i2c_read_bit(SDA); 
		data_tmp |= gpio_i2c << (7-i);

		TIME_DELAY_US(1);
		set_low(SCL);
	}

	TIME_DELAY_US(1);
	set_low(SDA);
	TIME_DELAY_US(1);

	return data_tmp;
}

static int i2c_write(unsigned char dev_addr, unsigned char addr,unsigned char data)
{
	if(0 != set_start_bit())
	{
		printk("i2c write set start bit error");
	}

	do
	{
		i2c_write_byte(dev_addr);
	}while(0 != recive_ack())

	do
	{
		i2c_write_byte(addr);
	}while(0 != recive_ack())

	do
	{
		i2c_write_byte(data);
	}while(0 != recive_ack())

	if(0 != set_stop_bit())
	{
		printk("i2c write set stop bit error");
	}
}

static unsigned char i2c_read(unsigned char dev_addr, unsigned char addr)
{
	unsigned char data = 0;	

	//发送请求，要求读取数据
	if(0 != set_start_bit())
	{
		printk("i2c read set start bit error");
	}

	do
	{
		i2c_write_byte(dev_addr);
	}while(0 != recive_ack())

	do
	{
		i2c_write_byte(addr);
	}while(0 != recive_ack())

	if(0 != set_stop_bit())
	{
		printk("i2c read set stop bit error");
	}

	//开始接收数据
	if(0 != set_start_bit())
	{
		printk("i2c read set start bit error");
	}

	do
	{
		i2c_write_byte(dev_addr | 1);
	}while(0 != recive_ack())

	do
	{
		data = i2c_read_byte();
	}while(0 != set_ack())

	if(0 != set_stop_bit())
	{
		printk("i2c read set stop bit error");
	}

	return data;

int sccb_write(unsigned char addr, unsigned char data)
{
	unsigned char dev_addr = SLAVE_OV7720_ADDR;

	if(0 == i2c_write(dev_addr, addr, data))
	{
		return 0;
	}
	return -1;
}

unsigned char sccb_read(unsigned char addr)
{
	unsigned char dev_addr= SLAVE_OV7720_ADDR;

	unsigned char tmp_data = i2c_read(dev_addr, addr);

	return tmp_data;
}

int sccb_reset()
{
	
}

