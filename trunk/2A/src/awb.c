//
//  "$Id: awb.c 513 2010-04-26 01:21:14Z wang_jun $"
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
#include <asm-arm/arch-davinci/dm365_aew.h>
#include "aew.h"
#include "debug.h"
/*
RGB->HSI
I=(R+G+B)/3
S=1-(min(R,G,B)/I)


*/

extern int dbg_saturation_limit;
extern int dbg_print_mode ;
extern int dbg_y_min;
extern int dbg_y_max;
extern int dbg_echo;

int awb_process( AEW_PAXEL_STAT *stat, AEW_PAXEL_INFO *info, WB_GAIN_PARAM *param)
{
	int i;
	static int y_min = 4;///28;///48;///
	static int y_max = 200;	
	static int s_max = 141;//151;////136//144///136
	float avg_r = 0, avg_gr = 0, avg_gb = 0, avg_b = 0;
	float avg_y_g = 0,avg_y_r = 0, avg_y_b=0;
	int paxels = 0;
	
	/*支持动态修改参数,方便调试*/
	if ((dbg_saturation_limit > 0) && (s_max != dbg_saturation_limit)){
			s_max = dbg_saturation_limit;
			printf("new saturation limit:%d\n",s_max);
	}
	if ((dbg_y_min > 0) && (y_min != dbg_y_min)){
		y_min = dbg_y_min;
		printf("new y_min=%d\n",y_min);
	}
	if ((dbg_y_max > 0) && (y_max != dbg_y_max)){
		y_max = dbg_y_max;
		printf("new y_max=%d\n",y_max);
	}
	if (dbg_echo > 0){
		printf("y_min=%d,y_max=%d,saturation=%d\n",y_min, y_max, s_max);
		dbg_echo--;
	}

	if ((dbg_print_mode == PRINT_SATURATION) || (dbg_print_mode == PRINT_Y))
		printf("------------aew_process hzcnt=%d,vt_cnt=%d------------------\n",info->hz_cnt, info->vt_cnt);
	for(i = info->hz_cnt; i < info->hz_cnt * info->vt_cnt; i++) // 跳过第一行
	{
	#if 1
	//	int G = (stat[i].gr + stat[i].gb)/2;
	//	int color_min = min(min(stat[i].r, G), min(G, stat[i].b));
	//	int color_max = max(max(stat[i].r, G), max(G, stat[i].b));
		int color_min = min(min(stat[i].r, stat[i].gr), min(stat[i].gb, stat[i].b));
		int color_max = max(max(stat[i].r, stat[i].gr), max(stat[i].gb, stat[i].b));
	
		int color_delta = color_max - color_min;
		int saturation = color_delta ? (color_delta * 255 / color_max) : 0;
	#else
		int G= (stat[i].gr + stat[i].gb)/2;
		int color_min = min(min(stat[i].r, G), min(G, stat[i].b));
		int r_g_b = (stat[i].r + G + stat[i].b);

		int saturation;
		if (r_g_b == 0)
			saturation = 0;
		else
			saturation = (85*r_g_b - color_min*255)*3/r_g_b;
	//	printf("r=%d,gr=%d,gb=%d,b=%d,G=%d,color_min=%d,r_g_b=%d,saturation=%d\n", \
			//stat[i].r,stat[i].gr,stat[i].gb,stat[i].b,G,color_min,r_g_b,saturation\
			);		
	#endif
		if (dbg_print_mode == PRINT_SATURATION)
			printf("%2x",saturation);
		if (dbg_print_mode == PRINT_Y)
			printf("%2x",stat[i].y);
		if ((dbg_print_mode == PRINT_Y) || (dbg_print_mode == PRINT_SATURATION)){
				if(i % info->hz_cnt == info->hz_cnt - 1){
					printf("\n");
				}
		}
		if(saturation > s_max || stat[i].y > y_max || stat[i].y < y_min)
		{
			if (dbg_print_mode == PRINT_RESULT){
				printf(" ");
				if(i % info->hz_cnt == info->hz_cnt - 1){
					printf("\n");
				}
			}
			continue;
		}
		if (dbg_print_mode == PRINT_RESULT){
			printf("X");
			if(i % info->hz_cnt == info->hz_cnt - 1)
			{
				printf("\n");
			}
		}
		paxels ++;
		avg_r += stat[i].r;
		avg_gr += stat[i].gr;
		avg_gb += stat[i].gb;
		avg_y_g = avg_gr + avg_gb;
		avg_b += stat[i].b;
	}
	
	if(paxels)
	{
	#if 0
		avg_y_g = (0.2935*avg_y_g)/paxels;
		avg_y_r = (0.299*avg_r)/paxels;
		avg_y_b = (0.114*avg_b)/paxels;
	#else
		avg_y_g = (0.2935*avg_y_g);
		avg_y_r = (0.299*avg_r);
		avg_y_b = (0.114*avg_b);
	#endif
#if 0
		avg_r /= paxels ;//+ 1
		avg_gr /= paxels ;
		avg_gb /= paxels ;
		avg_b /= paxels ;
#endif
		
//		float y = 0.299 * avg_r + 0.587 * (avg_gr + avg_gb) / 2 + 0.114 * avg_b;

//		float y = 0.299 * avg_r + avg_g / 2 + 0.114 * avg_b;		
#if 0
		float y =  avg_y_r + avg_y_g + avg_y_b;		
		param->r = y / avg_r;
		param->gr = y / avg_gr;
		param->gb = y / avg_gb;
		param->b = y / avg_b;
#else
		float tmp = avg_y_g+avg_y_r+avg_y_b;
		param->r = tmp / avg_r;
		param->gr = tmp / avg_gr;
		param->gb = tmp / avg_gb;
		param->b = tmp / avg_b;
		
#endif
		if (dbg_print_mode == PRINT_RESULT){
			printf("average color = (%d, %d, %d, %d), gain = (%f, %f, %f, %f)\n",
				(int)avg_r,
				(int)avg_gr,
				(int)avg_gb,
				(int)avg_b,
				param->r,
				param->gr,
				param->gb,
				param->b);
		}
	}
	else // 很阴暗的场景、很明亮的场景或者饱和度很深的场景，保持上次的白平衡
	{
		//printf("paxels = %d, not very sure\n", paxels);
		
		return -1;
	}
	if ((dbg_print_mode == PRINT_SATURATION) || (dbg_print_mode == PRINT_Y))
		printf("\n");	
	return 0;
}

int awb_get_preset_gain(WB_MODE mode, WB_GAIN_PARAM *param)
{
	switch(mode)
	{
	case WB_DISABLE:
		param->r = 1.0;
		param->gr = 1.0;
		param->gb = 1.0;
		param->b = 1.0;
		break;
		
	case WB_CLOUDY: //阴天 7500k
		param->r = 1.953125;
		param->gr = 1.0390625;
		param->gb = 1.0390625;
		param->b = 1.0;
		break;
		
	case WB_DAYLIGHT: //日光 6500k
		param->r = 1.2734375;
		param->gr = 1.0;
		param->gb = 1.0;
		param->b = 1.0625;
		break;
		
	case WB_INCANDESCENCE: //白热光 5000k
		param->r = 1.2890625;
		param->gr = 1.0;
		param->gb = 1.0;
		param->b = 1.0625;
		break;
		
	case WB_FLUORESCENT: //日光灯 4400k
		param->r = 1.1875;
		param->gr = 1.0;
		param->gb = 1.0;
		param->b = 1.3125;
		break;
		
	case WB_TUNGSTEN: //钨丝灯 2800k
		param->r = 1.0;
		param->gr = 1.0078125;
		param->gb = 1.0078125;
		param->b = 1.28125;
		break;
		
	default:
		return -1;
	}
	
	param->d = 1.0;

	return 0;
}
