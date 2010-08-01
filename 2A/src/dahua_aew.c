//
//  "$Id: dahua_aew.c 513 2010-04-26 01:21:14Z wang_jun $"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "aew.h"
#include "dahua_aew.h"
#include "debug.h"

static H3A_DEVICE h3a_dev;
PREVIEW_DEVICE pre_dev;
SENSOR_DEVICE sen_dev;

static WB_MODE wb_mode = WB_AUTO;
static DNC_MODE dnc_mode = DNC_MULTICOLOR;
static DNC_MODE cur_dnc_mode = DNC_MULTICOLOR;
static COLOR_PARAM color_param = {50, 50, 50, 50};
static FE_AD chip_ad = AD_NONE;
static int working = 0;
static pthread_t aew_thread_id = -1;
static AEW_PAXEL_STAT *aew_stat_buf = NULL;
static int aew_paxels = 0;
static int ae_refrence_level = 50;

extern int dbg_print_mode;

static void sleep_ms(int ms)
{
	struct timeval t;

	t.tv_sec = ms / 1000;
	t.tv_usec = ((ms % 1000) * 1000);
	while(select(0, NULL, NULL, NULL, &t) != 0);
}

static void print_time(const char * name)
{
	return ;
	static unsigned long long tt = 0;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	
	unsigned long long t = tv.tv_sec * 1000000 + tv.tv_usec;
	printf("%12lld us : %s\n", t - tt, name);
	tt = t;
}

static void* dahua_aew_thread(void* data)
{
	AEW_PAXEL_INFO aew_paxel_info;
	
	EXPLORE_PARAM explore_param;
	
	WB_GAIN_PARAM wb_param;

	while(working)
	{
		if (dbg_print_mode == PRINT_RESULT)
			printf("\033[34A\033[0m");//光标上移一行
//		printf("\033[40A\033[0m");//光标上移一行
		
		int ret;
		
		print_time("start");
		ret = h3a_dev.read(aew_stat_buf, &aew_paxel_info, aew_paxels);
		if(ret < 0)
		{
			sleep_ms(1000);
			printf("aew read failed, retrying...\n");
		
			goto set_param;
		}
		print_time("read");
	
		check_debug_cmd();	
		
#if 0
		printf("vt_cnt = %d, hz_cnt = %d-------------------------------------------------------------------------------------------\n", aew_paxel_info.vt_cnt, aew_paxel_info.hz_cnt);
		int i, j;
		AEW_PAXEL_STAT *aew_buf = aew_stat_buf;
		for(i = 0; i < aew_paxel_info.vt_cnt; i++)
		{
			for(j = 0; j < aew_paxel_info.hz_cnt; j++)
			{
			printf("%2x", aew_stat_buf[i * aew_paxel_info.hz_cnt + j].y);
		#if 0
				aew_buf = &aew_stat_buf[i * aew_paxel_info.hz_cnt + j];
				float y = 0.299 * aew_buf->r + 0.587 * (aew_buf->gr + aew_buf->gb) / 2 + 0.114 * aew_buf->b;
				printf("%3u",(unsigned int)y);
				#endif
			}
			printf("\n");
		}
		int mid = ( aew_paxel_info.vt_cnt / 2 * aew_paxel_info.hz_cnt + aew_paxel_info.hz_cnt / 2 );
		printf("center color = (%d %d %d %d)\n", aew_stat_buf[mid].r, aew_stat_buf[mid].gr, aew_stat_buf[mid].gb, aew_stat_buf[mid].b);
		}
#endif		
		//printf("ae_process...\n");
		ae_process(aew_stat_buf, &aew_paxel_info, ae_refrence_level, &explore_param);
		print_time("ae_process");
		
		//printf("awb_process...\n");
		if(wb_mode == WB_AUTO)
		{
			awb_process(aew_stat_buf, &aew_paxel_info, &wb_param);
		}
		print_time("awb_process");
		
set_param:
		// set sensor and previewer parameters
		sen_dev.set_exposure(&explore_param); //jerry
		print_time("sen_dev.set_exposure");

		if(wb_mode != WB_AUTO)
		{
			awb_get_preset_gain(wb_mode, &wb_param);
		}
		wb_param.d = explore_param.gain_extra;
		pre_dev.set_wb(&wb_param);
		print_time("pre_dev.set_wb");
		
		// 彩色黑白切换处理
		DNC_MODE new_dnc_mode = cur_dnc_mode;
		switch(dnc_mode)
		{
			case DNC_MULTICOLOR:
				new_dnc_mode = DNC_MULTICOLOR;
				break;
			case DNC_AUTO:
				if(cur_dnc_mode == DNC_MULTICOLOR && explore_param.ev < 12 && explore_param.gain_extra > 1.1f)
				{
					new_dnc_mode = DNC_BLACKWHITE;
				}
				else if(cur_dnc_mode == DNC_BLACKWHITE && explore_param.ev > 14)
				{
					new_dnc_mode = DNC_MULTICOLOR;
				}
				break;
			case DNC_BLACKWHITE:
				new_dnc_mode = DNC_BLACKWHITE;
				break;
			default:
				new_dnc_mode = DNC_MULTICOLOR;
				break;
		}
		if(new_dnc_mode != cur_dnc_mode)
		{
			//printf("new_dnc_mode = %d\n", new_dnc_mode);
			if(pre_dev.set_color(&color_param, new_dnc_mode) == 0)
			{
				cur_dnc_mode = new_dnc_mode;
			}
		}

		//sleep_ms(1000);
	}
	
	return 0;
}

int dahua_aew_start(AEW_WINDOW *window, int prev_share_fd)
{
	int ret;
	
	if(working)
	{
		return -1;
	}
	
	printf("dahua_aew_start window = (%d, %d, %d, %d)\n",
	 window->width, window->height, window->start_x, window->start_y);

	create_dm365_h3a(&h3a_dev);
	create_davinci_previewer(&pre_dev);
	#if 1
	if (chip_ad == AD_9923)
		create_ad9923_ccd(&sen_dev);
	if (chip_ad == AD_9920)
		create_ad9920_ccd(&sen_dev);
	assert(chip_ad != AD_NONE);
	#endif
	aew_paxels = h3a_dev.open(window);
	if(aew_paxels < 0)
	{
		return -1;
	}
	
	ret = pre_dev.open(prev_share_fd);
	assert(ret == 0);
	#if 1
	ret = sen_dev.open();
	assert(ret == 0);
	#endif
	aew_stat_buf = (AEW_PAXEL_STAT *)malloc(aew_paxels * sizeof(AEW_PAXEL_STAT));
	assert(aew_stat_buf);

	#if 0
	// 设置默认GAMMA表
	GAMMA_TABLE gamma_table;
	
	//agc_get_preset_gamma_table(GAMMA_DEFAULT, &gamma_table);
	printf("########setIPIPE_GAMMA_TBL_ROM\n");
	
	pre_dev.set_gamma(IPIPE_GAMMA_TBL_ROM, &gamma_table); // default rom table will be better
	#endif
	working = 1;

	// 创建linux线程	
	ret = pthread_create(&aew_thread_id, NULL, dahua_aew_thread, 0);
	assert(ret == 0);

	return 0;
}
int dahua_aew_set_AD(FE_AD ad)
{
	chip_ad = ad;
	return 0;
}
int dahua_aew_set_color(COLOR_PARAM *param)
{
	color_param = *param;
	
	return pre_dev.set_color(&color_param, cur_dnc_mode); 
}

int dahua_aew_set_wb_mode(WB_MODE mode)
{
	wb_mode = mode;
	
	return 0;
}

int dahua_aew_set_dnc_mode(DNC_MODE mode)
{
	dnc_mode = mode;
	
	return 0;
}
#if 1
int dahua_aew_set_vstd(VSTD vstd)
{
	return sen_dev.set_vstd(vstd);
}

int dahua_aew_get_exposure_speeds(VSTD vstd, float *speeds)
{
	return sen_dev.get_exposure_speeds(vstd, speeds);
}

// 设置曝光等级
int dahua_aew_set_exposure_level(int level, float value1, float value2)
{
	return sen_dev.set_exposure_level(level, value1, value2);
}

int dahua_aew_set_gain(int gain, int auto_gain)
{
	return sen_dev.set_gain(gain, auto_gain);	
}
#endif
int dahua_aew_set_refrence_level(int level)
{
	ae_refrence_level = level;
	
	return 0;
}

int dahua_aew_stop()
{
	int ret;
	
	if(!working)
	{
		return -1;
	}
	
	printf("dahua_aew_stop\n");

	working = 0;
	
	ret = pthread_join(aew_thread_id, NULL);
	assert(ret == 0);

	if(aew_stat_buf)
	{
		free(aew_stat_buf);
		aew_stat_buf = 0;
	}
	
	ret = h3a_dev.close();
	assert(ret == 0);
	
	ret = pre_dev.close();
	assert(ret == 0);
#if 0
	ret = sen_dev.close();
	assert(ret == 0);
#endif
	return 0;
}

