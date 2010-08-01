//
//  "$Id: dm365_aew_app.c 385 2010-02-04 12:10:18Z wang_haifeng $"
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
#include <unistd.h>
#include <string.h>
#include <asm-arm/arch-davinci/dm365_aew.h>
#include "aew.h"

static inline int log2i(ulong x)
{
	int r = 31;

	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}

static struct aew_configuration aew_config = 
{
	.window_config.width = 32,					//评价窗口宽度
		.window_config.height = 32,				//评价窗口高度
		.window_config.hz_line_incr = 2,		//
		.window_config.vt_line_incr = 2,		//
		.window_config.hz_cnt = 56,				//窗口水平个数
		.window_config.vt_cnt = 128,			//窗口垂直个数
		.window_config.vt_start = 24,			//左上角第1个窗口垂直起始点
		.window_config.hz_start = 32,			//左上角第1个窗口水平起始点
		.blackwindow_config.height = 8,			//黑窗高度
		.blackwindow_config.vt_start = 0,		//黑窗起始
		.alaw_enable = H3A_AEW_DISABLE,			//不能使用ALAW,否则出来的统计值是非线性的，白平衡不好做
		.saturation_limit = 128,	      		//饱和限制,和颜色范围对应
		.out_format = AEW_OUT_SUM_ONLY,
		.hmf_config.enable = H3A_AEW_DISABLE,
		.hmf_config.threshold = 512,
		.sum_shift = 0
}; 

static int fd = -1;
static unsigned short * aew_stat_buf = 0;
static unsigned int aew_stat_buf_size = 0;
static unsigned int aew_paxel_cnt = 0;
//static char wrong_data[16] = {0,};
//static char wrong_number[16] = {0,};

static int dm365_aew_open( AEW_WINDOW *window )
{
	int bytes;

	fd = open("/dev/dm365_aew", O_RDWR);
	assert(fd > 0);

	aew_config.window_config.hz_cnt = window->width / aew_config.window_config.width;
	aew_config.window_config.vt_cnt = window->height / aew_config.window_config.height;
	aew_config.window_config.hz_start = window->start_x;
	aew_config.window_config.vt_start = window->start_y;
	aew_config.blackwindow_config.height = window->start_y;
	
	// 使颜色值范围为0-255
	if(aew_config.alaw_enable == H3A_AEW_ENABLE)
	{
		aew_config.sum_shift = log2i(aew_config.window_config.width * aew_config.window_config.height / 4);
	}
	else
	{
		aew_config.sum_shift = log2i(aew_config.window_config.width * aew_config.window_config.height / 4) + 2;
	}
	
	bytes = ioctl(fd, AEW_S_PARAM, &aew_config);
	if(bytes < 0)
	{
		perror("dm365_aew set param failed");
		close(fd);
		return -1;
	}
	else
	{
		printf("dm365_aew buffer size = %d\n", bytes);
	}

	
	aew_stat_buf = (unsigned short *)malloc(bytes);
	
	if(aew_stat_buf == 0)
	{
		perror("dm365_aew malloc memory failed");
		close(fd);
		return -1;
	}
	
	aew_stat_buf_size = bytes;
	aew_paxel_cnt = aew_config.window_config.hz_cnt * (aew_config.window_config.vt_cnt + 1);
	
	return aew_paxel_cnt;
}

static int dm365_aew_close( void )
{
	int ret;

	if(aew_stat_buf)
	{
		free(aew_stat_buf);
		aew_stat_buf = 0;
	}
	
	ret = close(fd);
	if(ret < 0)
	{
		perror("dm365_aew close failed");
		return -1;
	}

	return 0;	
}

static int dm365_aew_read( AEW_PAXEL_STAT *stat, AEW_PAXEL_INFO *info, int paxel_count )
{
	int ret;
	int bytes;
	int i, j, n, max;
	unsigned short check_sum = 0;
	static unsigned short check_sum_old = 0;
	static unsigned int repeat_times = 0;
	
	if(paxel_count != aew_paxel_cnt)
	{
		return -1;
	}

	ret = ioctl(fd, AEW_ENABLE);
	if(ret < 0)
	{
		perror("dm365_aew disable failed");

		return -1;
	}

	bytes = read(fd, aew_stat_buf, aew_stat_buf_size);
	
	ret = ioctl(fd, AEW_DISABLE);
	if(ret < 0)
	{
		perror("dm365_aew enable failed");

		return -1;
	}
	
	if(bytes != aew_stat_buf_size)
	{
		printf("dm365_aew bytes = %d, aew_stat_buf_size = %d\n", bytes, aew_stat_buf_size);
		perror("dm365_aew read failed");
		
		return -1;
	}
	
	
	/*for(i = 0; i < aew_stat_buf_size; i++)
	{
		printf("%02x ", ((char*)aew_stat_buf)[i]);
		if(i % 144 == 143)
		{
			printf("\n");
		}
	}*/
	
	unsigned short *buf = aew_stat_buf;
	n = 0;

	for(i = 0; i < aew_paxel_cnt; i+=8)
	{
		max = (i + 8 <= aew_paxel_cnt) ? 8 : aew_paxel_cnt - i;
		
		for(j = 0; j < max; j++)
		{
			// workaroud, don't know why!!!
			if(buf - aew_stat_buf >= aew_stat_buf_size / 2 - 8)
			{
				goto end;
			}
			/*if(n > aew_config.window_config.hz_cnt && memcmp(wrong_data, buf, 16) == 0)
			{
				printf("n = %d\n", n);
				buf += 8;
			}*/
			if((aew_config.window_config.hz_cnt == 50 && n % 200 == 199)
			||(aew_config.window_config.hz_cnt == 22 && (n % 88 == 43 || n % 88 == 65 || n % 88 == 87))
			|| (aew_config.window_config.hz_cnt == 25 && n % 200 == 199))
			{
				buf += 8;
			}				
		
			n = i + j;

			check_sum += *buf;
			stat[n].gr = *buf++;
			check_sum += *buf;
			stat[n].r = *buf++;
			check_sum += *buf;
			stat[n].b = *buf++;
			check_sum += *buf;
			stat[n].gb = *buf++;
			buf += 4;
			//stat[n].sat_gr = *buf++;
			//stat[n].sat_r = *buf++;
			//stat[n].sat_b = *buf++;
			//stat[n].sat_gb = *buf++;
			stat[n].y = (299 * stat[n].r + 587 * (stat[n].gr + stat[n].gb) / 2 + 114 * stat[n].b) / 1000;
			if(stat[n].y > 255)
			{
				stat[n].y = 255;
			}
		}
		
		// workaroud, don't know why!!!
		if(buf - aew_stat_buf >= aew_stat_buf_size / 2 - 8)
		{
			goto end;
		}
		/*if(n > aew_config.window_config.hz_cnt && memcmp(wrong_data, buf, 16) == 0)
		{
			printf("n = %d\n", n);
			buf += 8;
		}*/
		
		buf += max;
		//for(j = 0; j < max; j++)
		//{
		//	stat[i + j].nosat_cnt = *buf++;
		//}
	}
	
end:
	info->hz_cnt = aew_config.window_config.hz_cnt;
	info->vt_cnt = aew_config.window_config.vt_cnt + 1;
	info->square_hz_cnt = aew_config.window_config.width / aew_config.window_config.hz_line_incr;
	info->square_vt_cnt = aew_config.window_config.height / aew_config.window_config.vt_line_incr;
	
	// 校验和多次不变，意味着aew模块可能出错，返回错误
	//printf("check_sum = %8d\n", check_sum);
	if(check_sum_old == check_sum && check_sum != 0)
	{
		repeat_times++;
		if(repeat_times >= 100)
		{
			repeat_times = 0;
			perror("dm365_aew seems crashed");

			return -1;
		}
	}
	else
	{
		check_sum_old = check_sum;
		repeat_times = 0;
	}

	return n;
}

void create_dm365_h3a(H3A_DEVICE* device)
{
	device->open = dm365_aew_open;
	device->close = dm365_aew_close;
	device->read = dm365_aew_read;
}
