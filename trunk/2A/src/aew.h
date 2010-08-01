//
//  "$Id: aew.h 513 2010-04-26 01:21:14Z wang_jun $"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _AEW_H_
#define _AEW_H_

#include "dahua_aew.h"

typedef struct AEW_PAXEL_INFO
{
	int hz_cnt;
	int vt_cnt;
	int square_hz_cnt;
	int	square_vt_cnt;
} AEW_PAXEL_INFO;

typedef struct AEW_PAXEL_STAT
{
	unsigned short gr;
	unsigned short r;
	unsigned short b;
	unsigned short gb;
	unsigned short sat_gr;
	unsigned short sat_r;
	unsigned short sat_b;
	unsigned short sat_gb;
	unsigned short nosat_cnt;
	unsigned short y;
}AEW_PAXEL_STAT;

typedef struct WB_GAIN_PARAM
{
	float d;
	float gr;
	float r;
	float b;
	float gb;
}WB_GAIN_PARAM;

typedef struct EXPLORE_PARAM
{
	int	ev;				// 加权电平 0-255
	int level;			// 曝光等级 0-255
	float gain_extra;	// 额外增益 >=1.0f
}EXPLORE_PARAM;

typedef enum GAMMA_MODE
{
	GAMMA_DEFAULT,	
}GAMMA_MODE;

typedef struct GAMMA_VALUE
{
	short slope;
   	unsigned short offset;
}GAMMA_VALUE;

typedef struct GAMMA_TABLE
{
	GAMMA_VALUE *r;
	GAMMA_VALUE *g;
	GAMMA_VALUE *b;
}GAMMA_TABLE;

static inline int min(int x, int y)
{
	return x > y ? y : x;	
}

static inline int max(int x, int y)
{
	return x > y ? x : y;	
}

// H3A设备
typedef struct H3A_DEVICE
{
	int (*open)( AEW_WINDOW *window );
	int (*close)( void );
	int (*read)( AEW_PAXEL_STAT *stat, AEW_PAXEL_INFO * info, int paxel_count );
}H3A_DEVICE;

void create_dm365_h3a(H3A_DEVICE* device);

// 预览设备
typedef struct PREVIEW_DEVICE
{
	int (*open)( int share_fd );
	int (*close)( void );
	int (*set_wb)( WB_GAIN_PARAM *param );
	int (*set_gamma)(enum ipipe_gamma_tbl_sel tbl_sel, GAMMA_TABLE *table );
	int (*set_color)( COLOR_PARAM *param, DNC_MODE mode);
}PREVIEW_DEVICE;

void create_davinci_previewer(PREVIEW_DEVICE* device);

// 前端设备
typedef struct SENSOR_DEVICE
{
	int (*open)( void );
	int (*close)( void );
	int (*set_exposure)( EXPLORE_PARAM * param);
	int (*get_exposure_speeds)(VSTD vstd, float *speeds);
	int (*set_vstd)(VSTD vstd);
	int (*set_exposure_level)(int level, float value1, float value2);
	int (*set_gain)(int gain, int auto_gain);
	int (*set_clamp)(int clamp);
}SENSOR_DEVICE;

void create_ad9923_ccd(SENSOR_DEVICE* device);

// 公共算法
int awb_process( AEW_PAXEL_STAT *stat, AEW_PAXEL_INFO *info, WB_GAIN_PARAM *param);
int awb_get_preset_gain(WB_MODE mode, WB_GAIN_PARAM *param);

int ae_process( AEW_PAXEL_STAT *stat, AEW_PAXEL_INFO *info,  int refrence_level, EXPLORE_PARAM *param);

int agc_get_preset_gamma_table(GAMMA_MODE mode, GAMMA_TABLE *table);

#endif
