//
//  "$Id: dahua_aew.h 513 2010-04-26 01:21:14Z wang_jun $"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _DAHUA_AEW_H_
#define _DAHUA_AEW_H_

#include <asm-arm/arch-davinci/dm365_ipipe.h>

typedef struct COLOR_PARAM
{
	int	brightness;	// 0-100
	int contrast;	// 0-100
	int	hue;		// 0-100
	int saturation;	// 0-100
}COLOR_PARAM;

typedef enum WB_MODE
{
	WB_DISABLE,			// 禁止
	WB_AUTO,			// 自动
	WB_DAYLIGHT,		// 日光 6500k
	WB_CLOUDY,			// 阴天 7500k
	WB_INCANDESCENCE,	// 白热光 5000k
	WB_FLUORESCENT,		// 日光灯 4400k
	WB_TUNGSTEN			// 钨丝灯 2800k
}WB_MODE;

typedef struct AEW_WINDOW
{
	int width;			// 窗口宽度
	int height;			// 窗口高度
	int start_x;		// 窗口x起始坐标
	int start_y;		// 窗口y起始坐标
}AEW_WINDOW;

typedef enum DNC_MODE
{
	DNC_MULTICOLOR,		// 彩色
	DNC_AUTO,			// 自动切换
	DNC_BLACKWHITE,		// 强制为黑白模式
}DNC_MODE;

typedef enum VSTD
{
	VSTD_PAL,
	VSTD_NTSC,
	VSTD_SECAM,
	VSTD_NR
}VSTD;
typedef enum FE_AD
{
	AD_NONE,
	AD_9920,
	AD_9923,
	NR_AD
}FE_AD;
// 创建AEW线程，分辨率变化时先要销毁线程，再重新开启
// AEW窗口设置成和视频窗口一致即可
// prev_share_fd：共享preview设备的文件句柄号，如果为-1，由aew模块自己创建文件句柄
int dahua_aew_start(AEW_WINDOW *window, int prev_share_fd);

// 设置颜色参数
int dahua_aew_set_color(COLOR_PARAM *param);

// 设置白平衡参数
int dahua_aew_set_wb_mode(WB_MODE mode);

// 设置日夜模式
int dahua_aew_set_dnc_mode(DNC_MODE mode);

// 设置视频制式
// vstd:0-pal制，1-n制，2-secam制
int dahua_aew_set_vstd(VSTD vstd);

// 设置A/D类型
// vstd:0-pal制，1-n制，2-secam制
int dahua_aew_set_AD(FE_AD vstd);

// 获取支持的曝光等级数
// 返回值<0，获取失败，>=0表示等级数，具体等级保存在数组speeds中。
// speeds数组长度必须足够大，取16差不多了。
int dahua_aew_get_exposure_speeds(VSTD vstd, float *speeds);

// 设置曝光等级
// 0     自动曝光
// 1~n-1 手动曝光等级
// n     带时间上下限的自动曝光
// n+1   自定义时间手动曝光
// (n表示支持的曝光等级数，即dahua_aew_get_exposure_speeds接口返回值）
// value1:自动曝光时间下限或者手动曝光自定义时间，毫秒为单位，取值0.1ms~80ms
// value2:自动曝光时间上限，毫秒为单位取值0.1ms~80ms，且必须不小于exposureValue1取值
int dahua_aew_set_exposure_level(int level, float value1, float value2);

// 设置增益值
// gain:固定增益值，或者自动增益的上限，取值0-100。
// auto_gain:自动增益，0-不开启，1-开启，同时gain成员表示自动增益的上限。
int dahua_aew_set_gain(int gain, int auto_gain);

// 设置参考电平值
// level:参考电平值，取值0-100。
int dahua_aew_set_refrence_level(int level);

// 销毁AEW线程
int dahua_aew_stop();
//用于调试
int dahua_aew_debug(char *cmd);

#endif
