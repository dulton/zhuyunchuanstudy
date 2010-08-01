//
//  "$Id: ad9923_app.c 513 2010-04-26 01:21:14Z wang_jun $"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "aew.h"

static int fd = -1;
//geyj add @ 091027 
#define   AD9923_IOC_MAGIC		0x99
#define   IOCTL_SET_EXP			_IOWR(AD9923_IOC_MAGIC,0x01,unsigned int)
#define   IOCTL_SET_CLAMP  _IOWR(AD9923_IOC_MAGIC,0x02,unsigned int)
#define   IOCTL_SET_VGA   _IOWR(AD9923_IOC_MAGIC,0x03,unsigned int)

#define EXPOSURE_LEVEL 9
static VSTD video_standard = VSTD_PAL;
static int exposure_level = 0;
static float exposure_value1 = 0.1f;
static float exposure_value2 = 80.0f;
static int auto_gain_control = 1;
static int gain_control = 50;
/*jerry.表中的值是怎么来的?*/
// 自动曝光等级，采用低速模式，保证日关灯，红绿灯等等决不闪烁，同时保证静态低照度效果
static float auto_exposure_speeds[VSTD_NR][EXPOSURE_LEVEL] = 
	{
		{0, 10, 20, 30, 40, 50, 60, 70, 80},
		{0, 8.333, 16.666, 25, 33.333, 41.666, 50, 58.333, 66.666},
		{0, 10, 20, 30, 40, 50, 60, 70, 80},
	};

// 手动曝光等级，采用高速模式，对应web显示，曝光时间很短时需要补光，同时还需要和电源外同步
static float manual_exposure_speeds[VSTD_NR][EXPOSURE_LEVEL] = 
	{
		{0, 20, 10, 4, 2, 1, 0.5, 0.25, 0.1},
		{0, 16.666, 8.333, 4, 2, 1, 0.5, 0.25, 0.1},
		{0, 20, 10, 4, 2, 1, 0.5, 0.25, 0.1},
	};
static int ad9923_set_clamp(int val)
{
	int ret;
	ret = ioctl(fd, IOCTL_SET_CLAMP, val);
	if (ret < 0)
		perror("ad9923 set clamp failed");
	return ret;
}

static int ad9923_open( void )
{
	fd = open("/dev/ad9923_dev", O_RDWR);
	if(fd < 0)
	{
		perror("ad9923 open failed");
		return -1;
	}

	int ret;

	ret = ioctl(fd, IOCTL_SET_CLAMP, 0xc); /*jerry.0xc*/
	
	if(ret < 0)
	{
		perror("ad9923 set clamp failed");
		
		return -1;
	}
	
	ret = ioctl(fd, IOCTL_SET_EXP, 128);
	
	if(ret < 0)
	{
		perror("ad9923 set exp failed");
		
		return -1;
	}

	ret = ioctl(fd, IOCTL_SET_VGA, 15);
	
	if(ret < 0)
	{
		perror("ad9923 set vga failed");
		
		return -1;
	}		
	
	return 0;
}

static int ad9923_close( void )
{
	int ret;

	ret = close(fd);
	if(ret < 0)
	{
		perror("ad9923 close failed");
		return -1;
	}

	return 0;	
}

static int ad9923_set_exposure( EXPLORE_PARAM * param)
{
	int ret = 0;
	unsigned int exp;
	static unsigned int last_exp = -1;
	unsigned int gain;
	static unsigned int last_gain = -1;
	float error = 1.0f;
	
	float exposure_time = 0;
	
	// 设置曝光时间，1-1563 for 1600*1200pixels，做一次指数变化，使等级变化均匀
	// 1563的测试方法：光圈开大对着强光，使用setexp将曝光时间依此减少，减到几乎没有变化，就是曝光时间的最小值。
	if(exposure_level == 0) // 自动曝光
	{
		static float g = 1.01;

		exposure_time = auto_exposure_speeds[video_standard][EXPOSURE_LEVEL - 1] * powf(g, param->level) / powf(g, 255);
		
		// 选择一个合适的曝光时间
		int i;
		for(i = 1; i < EXPOSURE_LEVEL; i++)
		{
			if(exposure_time <= auto_exposure_speeds[video_standard][i])
			{
				break;
			}
		}
		
		error = auto_exposure_speeds[video_standard][i] / exposure_time;
		exposure_time = auto_exposure_speeds[video_standard][i];
	}
	else if(exposure_level < EXPOSURE_LEVEL) // 手动曝光
	{
		exposure_time = manual_exposure_speeds[video_standard][exposure_level];
	}
	else if(exposure_level == EXPOSURE_LEVEL) // 带时间上下限的自动曝光
	{
		float g = powf(exposure_value2 / exposure_value1, 1.f / 255);

		exposure_time = exposure_value1 * powf(g, param->level);
	}
	else if(exposure_level == EXPOSURE_LEVEL + 1) // 自定义时间手动曝光
	{
		exposure_time = exposure_value1;
	}
	else
	{
		assert(0);
	}

	exp = 1563 - exposure_time * 18.5; /*jerry.这公式怎么来的*/
	
	if(last_exp != exp)
	{
		last_exp = exp;
		ret = ioctl(fd, IOCTL_SET_EXP, exp);
	}
	
	//printf("ad9923 set exp to %d\n", exp);
	
	if(ret < 0)
	{
		perror("ad9923 set exp failed");
		
		return -1;
	}
	
	 /*jerry.这公式怎么来的*/
	 
	// ad9923增益范围为0-255转成0-1023
	if(auto_gain_control) // 自动增益
	{
		float ratio = 1023.0f * gain_control / 100 / 255;
		
		gain = param->level * ratio;
		
		if(exposure_level == 0) // 自动曝光，需要针对曝光时间跃阶做校正
		{
			gain -= 20 * log(error)/log(10) / 0.0358; 
		}
	}
	else // 固定增益
	{
		gain = 1023.0f * gain_control / 100;
	}

	if(gain < 0)
	{
		gain = 0;
	}
	if(gain > 0x3ff)
	{
		gain = 0x3ff;
	}
	if(last_gain != gain)
	{
		last_gain = gain;
		ret = ioctl(fd, IOCTL_SET_VGA, gain);
	}
	
	//printf("ad9923 set gain to %d\n", gain);
	
	if(ret < 0)
	{
		perror("ad9923 set gain failed");
		
		return -1;
	}
	
	return 0;
}

static int ad9923_set_vstd(VSTD vstd)
{
	video_standard = vstd;
	
	return 0;
}

static int ad9923_set_exposure_level(int level, float value1, float value2)
{
	if(level < 0)
	{
		level = 0;
	}
	else if(level >= EXPOSURE_LEVEL + 2)
	{
		level = EXPOSURE_LEVEL + 1;
	}
	
	if(value1 < 0.1f)
	{
		value1 = 0.1f;
	}
	if(value2 > 80.0f)
	{
		value2 = 80.0f;
	}

	if(level == EXPOSURE_LEVEL && value1 > value2)
	{
		value1 = value2;
	}
	
	exposure_level = level;
	exposure_value1 = value1;
	exposure_value2 = value2;
	
	return 0;
}

static int ad9923_get_exposure_speeds(VSTD vstd, float *speeds)
{
	int i;
	for(i = 0; i < EXPOSURE_LEVEL; i++)
	{
		speeds[i] = manual_exposure_speeds[vstd][i];
	}
	
	return EXPOSURE_LEVEL;
}

static int ad9923_set_gain(int gain, int auto_gain)
{
	if(gain < 0)
	{
		gain = 0;
	}
	else if(gain > 100)
	{
		gain = 100;
	}
	
	gain_control = gain;

	auto_gain_control = auto_gain;
	
	return 0;
}

void create_ad9923_ccd(SENSOR_DEVICE* device)
{
	device->open = ad9923_open;
	device->close = ad9923_close;
	device->set_exposure = ad9923_set_exposure;
	device->get_exposure_speeds = ad9923_get_exposure_speeds;
	device->set_vstd = ad9923_set_vstd;
	device->set_exposure_level = ad9923_set_exposure_level;
	device->set_gain = ad9923_set_gain;
	device->set_clamp = ad9923_set_clamp;
}
