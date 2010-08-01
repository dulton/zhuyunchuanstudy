//
//  "$Id: ae.c 513 2010-04-26 01:21:14Z wang_jun $"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "aew.h"

int ae_process( AEW_PAXEL_STAT *stat, AEW_PAXEL_INFO *info, int refrence_level, EXPLORE_PARAM *param)
{
	static int y_max = 255;			// 高亮阈值
	static int exp_level = 128;		// 当前曝光等级
	static int exp_level_max = 255;	// 最大曝光等级
	static int exp_level_min = 0;	// 最小曝光等级
	static int ev_default = 72;		// 默认期望的平均电平
	static int ev_diff_max = 8;
	int hist[256];					// 亮度直方图统计
	
	int ev = 0;
	int mid = 0;
	int i, n = 0;
	float gain_extra = 1.0;
	
	memset(hist, 0, sizeof(hist));
	
	// 计算直方图
	for(i = info->hz_cnt; i < info->vt_cnt * info->hz_cnt; i++)
	{
		hist[stat[i].y]++;
	}
	/*jerry n < (info->vt_cnt - 1) * info->hz_cnt / 2 这个条件是干什么*/
	for(mid = 255, n = 0; mid >=0 && n < (info->vt_cnt - 1) * info->hz_cnt / 2; mid--)
	{
		n += hist[mid]; /*直方图已经经过排序了,所以这里只要统计就OK了*/
	}
	
	ev = 0;
	n = 0;
	
	// 只统计不小于中值并且不是高亮区域的亮度
	for(i = info->hz_cnt; i < info->vt_cnt * info->hz_cnt; i++)
	{
		if(stat[i].y >= mid && stat[i].y <= y_max)
		{
			ev += stat[i].y;
			n++;
		}
	}
	
	if(n)
	{
		ev /= n;
	}
	else
	{
		ev = mid;
	}
	
	int ev_avg = ev_default;
	
	// 将0-50-100映射为0-ev_default-255
	if(refrence_level < 50)
	{
		ev_avg = refrence_level * ev_default / 50;
	}
	else
	{
		ev_avg = (ev_default + (refrence_level - 50) * (255 - ev_default) / 50);
	}
	
	int delta = abs(ev - ev_avg);
	if(delta > ev_diff_max)
	{
		int step = sqrt(delta);
		if(ev > ev_avg)
		{
			exp_level -= step;	
		}
		else
		{
			exp_level += step;
		}
		if(exp_level > exp_level_max)
		{
			exp_level = exp_level_max;
			//gain_extra = (float)ev_avg / (ev ? ev : 1);
		}
		else if(exp_level < exp_level_min)
		{
			exp_level = exp_level_min;
			gain_extra = (float)ev_avg / (ev ? ev : 1);
		}
	}
	
	param->ev = ev;
	param->level = exp_level;
	param->gain_extra = gain_extra;
	
	//printf("ev = %d, level = %d, extra = %f\n", ev, exp_level, gain_extra);
	
	return 0;
}
