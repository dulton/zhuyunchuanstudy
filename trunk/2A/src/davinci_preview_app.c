//
//  "$Id: davinci_preview_app.c 513 2010-04-26 01:21:14Z wang_jun $"
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
#include <asm-arm/arch-davinci/imp_previewer.h>
#include <asm-arm/arch/imp_common.h>
#include <asm-arm/arch-davinci/dm365_ipipe.h>

#include "aew.h"

static int preview_fd = -1;
static int fd_share = 0;

static int davinci_preview_open(int share_fd)
{
	if(share_fd < 0) // 外部文件句柄无效，自己创建
	{
		preview_fd = open("/dev/davinci_previewer", O_RDWR);
		fd_share = 0;
	}
	else
	{
		preview_fd = share_fd;
		fd_share = 1;
	}
	
	if(preview_fd < 0)
	{
		perror("davinci_preview open failed");
		return -1;
	}

	// 读IPIPE模式
    struct prev_channel_config prev_chan_config;
    struct prev_single_shot_config  prev_ss_config; // continuous mode

	printf("Getting default configuration in previewer\n");
    prev_chan_config.oper_mode = PREV_MODE_SINGLE_SHOT;
    prev_chan_config.len = sizeof(struct prev_single_shot_config);
    prev_chan_config.config = &prev_ss_config;
    if (ioctl(preview_fd, PREV_G_CONFIG, &prev_chan_config) < 0)
    {
        printf("Error in getting default configuration\n");
    }

	printf("shift = %d, clip = %d, gain = %d\n", 
	prev_ss_config.input.data_shift, 
	prev_ss_config.input.clip, 
	prev_ss_config.input.gain);
	
	return 0;
}

static int davinci_preview_close( void )
{
	int ret;
	
	if(fd_share)
	{
		return 0;
	}

	ret = close(preview_fd);
	if(ret < 0)
	{
		perror("davinci_preview close failed");
		return -1;
	}

	return 0;	
}

static int davinci_preview_set_wb( WB_GAIN_PARAM * param)
{
	int ret = 0;
	struct prev_module_param module_param;
	struct prev_wb wb_param;
	int gain;
	static WB_GAIN_PARAM last_param = {1.0,1.0,1.0,1.0,1.0};
	static float gain_max = 8191;//4095;//

	module_param.module_id = PREV_WB;
	strcpy(module_param.version, "5.1");
	module_param.param = &wb_param;
	module_param.len = sizeof(struct prev_wb);
	

	wb_param.ofst_gr = 0;
	wb_param.ofst_r = 0;
	wb_param.ofst_b = 0;
	wb_param.ofst_gb = 0;
	#if 0
	// decimal is 8 bits, data sheet is shit!
	gain = param->gr * param->d * 256;
	if(gain > gain_max)gain = gain_max;
	wb_param.gain_gr.integer = ((gain & 0xf00) >> 8);
	wb_param.gain_gr.decimal = (gain & 0xFF);
	gain = param->r * param->d * 256;
	if(gain > gain_max)gain = gain_max;
	wb_param.gain_r.integer = ((gain & 0xf00) >> 8);
	wb_param.gain_r.decimal = (gain & 0xFF);
	gain = param->b * param->d * 256;
	if(gain > gain_max)gain = gain_max;
	wb_param.gain_b.integer = ((gain & 0xf00) >> 8);
	wb_param.gain_b.decimal = (gain & 0xFF);
	gain = param->gb * param->d * 256;
	if(gain > gain_max)gain = gain_max;
	wb_param.gain_gb.integer = ((gain & 0xf00) >> 8);
	wb_param.gain_gb.decimal = (gain & 0xFF);
	#else
	// decimal is 8 bits, data sheet is shit!
	gain = param->gr * param->d * 512;
	if(gain > gain_max)gain = gain_max;
	wb_param.gain_gr.integer = ((gain & 0x1e00) >> 9);
	wb_param.gain_gr.decimal = (gain & 0x1FF);
	gain = param->r * param->d * 512;
	if(gain > gain_max)gain = gain_max;
	wb_param.gain_r.integer = ((gain & 0x1e00) >> 9);
	wb_param.gain_r.decimal = (gain & 0x1FF);
	gain = param->b * param->d * 512;
	if(gain > gain_max)gain = gain_max;
	wb_param.gain_b.integer = ((gain & 0x1e00) >> 9);
	wb_param.gain_b.decimal = (gain & 0x1FF);
	gain = param->gb * param->d * 512;
	if(gain > gain_max)gain = gain_max;
	wb_param.gain_gb.integer = ((gain & 0x1e00) >> 9);
	wb_param.gain_gb.decimal = (gain & 0x1FF);
	#endif
	/*printf("wb param %d, %d; %d, %d; %d, %d; %d, %d\n",
	 (int)wb_param.gain_r.integer,
	 (int)wb_param.gain_r.decimal,
	 (int)wb_param.gain_gr.integer,
	 (int)wb_param.gain_gr.decimal,
	 (int)wb_param.gain_gb.integer,
	 (int)wb_param.gain_gb.decimal,
	 (int)wb_param.gain_b.integer,
	 (int)wb_param.gain_b.decimal);*/
	
	if( fabs(last_param.d / param->d - 1) > 0.02
	 || fabs(last_param.r / param->r - 1) > 0.02
	 || fabs(last_param.gr / param->gr - 1) > 0.02
	 || fabs(last_param.gb / param->gb - 1) > 0.02
	 || fabs(last_param.b / param->b - 1) > 0.02)
	 {
	  	ret = ioctl(preview_fd, PREV_S_PARAM, &module_param);
	  	last_param = *param;
	}
	
	if(ret < 0)
	{
		perror("davinci_preview set wb param failed");
		
		return -1;
	}
	
	return 0;
}

#if 0
static int davinci_preview_set_gamma( GAMMA_TABLE *table )
{
	int ret;
	struct prev_module_param module_param;
	struct prev_gamma gamma_param;

	module_param.module_id = PREV_GAMMA;
	strcpy(module_param.version, "5.1");
	module_param.param = &gamma_param;
	module_param.len = sizeof(struct prev_gamma);
	
	// don't try to read, it will bring you to death!!!
	/*ret = ioctl(preview_fd, PREV_G_PARAM, &module_param);
	
	if(ret < 0)
	{
		perror("davinci_preview get gamma param failed");
		
		return -1;
	}
	*/
	
	gamma_param.bypass_r = 0;
	gamma_param.bypass_g = 0;
	gamma_param.bypass_b = 0;
	gamma_param.tbl_sel = IPIPE_GAMMA_TBL_RAM;
	gamma_param.tbl_size = IPIPE_GAMMA_TBL_SZ_512;
	gamma_param.table_r = (struct ipipe_gamma_entry *)table->r;
	gamma_param.table_g = (struct ipipe_gamma_entry *)table->g;
	gamma_param.table_b = (struct ipipe_gamma_entry *)table->b;

		
	/*int i;
	printf("red gamma:\n");
	for(i = 0; i < 512; i++)
	{
		printf("(%d, %d), ", gamma_param.table_r[i].offset, gamma_param.table_r[i].slope);
	}
	printf("\n");
	printf("green gamma:\n");
	for(i = 0; i < 512; i++)
	{
		printf("(%d, %d), ", gamma_param.table_g[i].offset, gamma_param.table_g[i].slope);
	}
	printf("\n");
	printf("blue gamma:\n");
	for(i = 0; i < 512; i++)
	{
		printf("(%d, %d), ", gamma_param.table_b[i].offset, gamma_param.table_b[i].slope);
	}
	printf("\n");*/

	 
	ret = ioctl(preview_fd, PREV_S_PARAM, &module_param);
	
	if(ret < 0)
	{
		perror("davinci_preview set gamma param failed");
		
		return -1;
	}
	
	return 0;	
}
#else
#if 0
/* Struct for configuring Gamma correction module */
struct prev_gamma {
	/* 0 - Enable Gamma correction for Red
	 * 1 - bypass Gamma correction. Data is divided by 16
	 */
	unsigned char bypass_r;
	/* 0 - Enable Gamma correction for Blue
	 * 1 - bypass Gamma correction. Data is divided by 16
	 */
	unsigned char bypass_b;
	/* 0 - Enable Gamma correction for Green
	 * 1 - bypass Gamma correction. Data is divided by 16
	 */
	unsigned char bypass_g;
	/* PREV_GAMMA_TBL_RAM or PREV_GAMMA_TBL_ROM */
	enum ipipe_gamma_tbl_sel tbl_sel;
	/* Table size for RAM gamma table.
	 */
	enum ipipe_gamma_tbl_size tbl_size;
	/* R table */
	struct ipipe_gamma_entry *table_r;
	/* Blue table */
	struct ipipe_gamma_entry *table_b;
	/* Green table */
	struct ipipe_gamma_entry *table_g;
};
enum ipipe_gamma_tbl_sel {
	IPIPE_GAMMA_TBL_RAM,
	IPIPE_GAMMA_TBL_ROM
};
#endif
static int davinci_preview_set_gamma(enum ipipe_gamma_tbl_sel tbl_sel, GAMMA_TABLE *table )
{
	int ret;
	struct prev_module_param module_param;
	struct prev_gamma gamma_param;

	memset(&gamma_param, 0, sizeof(gamma_param));
	module_param.module_id = PREV_GAMMA;
	strcpy(module_param.version, "5.1");
	module_param.param = &gamma_param;
	module_param.len = sizeof(struct prev_gamma);

	
	// don't try to read, it will bring you to death!!!
	/*ret = ioctl(preview_fd, PREV_G_PARAM, &module_param);
	
	if(ret < 0)
	{
		perror("davinci_preview get gamma param failed");
		
		return -1;
	}
	*/
	gamma_param.tbl_sel = tbl_sel;
	switch(gamma_param.tbl_sel){
		case IPIPE_GAMMA_TBL_ROM:			
			if (table == NULL){ /*bypass*/
				gamma_param.bypass_r = 1;
				gamma_param.bypass_g = 1;
				gamma_param.bypass_b = 1;
			}else{
				gamma_param.bypass_r = 0;
				gamma_param.bypass_g = 0;
				gamma_param.bypass_b = 0;
			}
			break;
		case IPIPE_GAMMA_TBL_RAM:
			gamma_param.tbl_size = IPIPE_GAMMA_TBL_SZ_512;
			gamma_param.table_r = (struct ipipe_gamma_entry *)table->r;
			gamma_param.table_g = (struct ipipe_gamma_entry *)table->g;
			gamma_param.table_b = (struct ipipe_gamma_entry *)table->b;
			break;
	}
	 
	ret = ioctl(preview_fd, PREV_S_PARAM, &module_param);
	
	if(ret < 0)
	{
		perror("davinci_preview set gamma param failed");
		
		return -1;
	}
	
	return 0;	
}

#endif
extern int dbg_bc_new;
static int davinci_preview_set_color( COLOR_PARAM *param , DNC_MODE mode)
{
	int ret;
	struct prev_module_param module_param;
	struct prev_lum_adj lum_adj;
	struct prev_rgb2rgb rgb2rgb1;
	struct prev_rgb2rgb rgb2rgb2;
	struct prev_cgs cgs = {
		.en = 1,
		.h_thr = 240,
		.h_slope = 10,
		.h_shft = 0,
		.h_min = 100
	};
	

	// 亮度/对比度
	module_param.module_id = PREV_LUM_ADJ;
	strcpy(module_param.version, "5.1");
	module_param.param = &lum_adj;
	module_param.len = sizeof(struct prev_lum_adj);

	if (dbg_bc_new == 0){
	//lum_adj.brightness是无符号的，不好用，改用rgb2rgb1来控制亮度
	//lum_adj.brightness = (mode == DNC_BLACKWHITE) ? 48 : ((float)param->brightness / 50) * 32;
		lum_adj.brightness = 0;

		float contrast = powf(((float)param->contrast / 50), 3.9999);
		lum_adj.contrast =  (((int)contrast) << 4) + (int)(contrast * 16) % 16;
	}
	else{
		lum_adj.brightness = param->brightness *2;
//	lum_adj.contrast = ((param->contrast*15/100) << 4)+ (param->contrast*15)%100;
		float contrast = powf(((float)param->contrast / 50), 3.9999);
		lum_adj.contrast =  (((int)contrast) << 4) + (int)(contrast * 16) % 16;	
		printf("###########brightness=%d,contrast1=%d.%d\n",lum_adj.brightness,lum_adj.contrast>>4,lum_adj.contrast&0x0f);
	}


	//printf("lum_adj.brightness = %d \n",lum_adj.brightness);
	//printf("lum_adj.contrast = %d \n",lum_adj.contrast);

	ret = ioctl(preview_fd, PREV_S_PARAM, &module_param);
	
	if(ret < 0)
	{
		perror("davinci_preview set brightness and contrast failed");
		
		return -1;
	}
	
	// 饱和度
	module_param.module_id = PREV_RGB2RGB_1;
	strcpy(module_param.version, "5.1");
	module_param.param = &rgb2rgb1;
	module_param.len = sizeof(struct prev_rgb2rgb);
	
	float saturation = (mode == DNC_BLACKWHITE) ? 0 : powf(((float)param->saturation / 50), 2.9999);
	int m;

	m = (0.299 + 0.701*saturation) * 256;
	rgb2rgb1.coef_rr.integer = ((m & 0xF00) >> 8);
	rgb2rgb1.coef_rr.decimal = (m & 0xFF);
	m = (0.587 * (1-saturation)) * 256;
	rgb2rgb1.coef_gr.integer = ((m & 0xF00) >> 8);
	rgb2rgb1.coef_gr.decimal = (m & 0xFF);
	m = (0.114 * (1-saturation)) * 256;
	rgb2rgb1.coef_br.integer = ((m & 0xF00) >> 8);
	rgb2rgb1.coef_br.decimal = (m & 0xFF);
 	m = (0.299 * (1-saturation)) * 256;
	rgb2rgb1.coef_rg.integer = ((m & 0xF00) >> 8);
	rgb2rgb1.coef_rg.decimal = (m & 0xFF);
	m = (0.587 + 0.413*saturation) * 256;
	rgb2rgb1.coef_gg.integer = ((m & 0xF00) >> 8);
	rgb2rgb1.coef_gg.decimal = (m & 0xFF);
	m = (0.114 * (1-saturation)) * 256;
	rgb2rgb1.coef_bg.integer = ((m & 0xF00) >> 8);
	rgb2rgb1.coef_bg.decimal = (m & 0xFF);
 	m = (0.299 * (1-saturation)) * 256;
	rgb2rgb1.coef_rb.integer = ((m & 0xF00) >> 8);
	rgb2rgb1.coef_rb.decimal = (m & 0xFF);
	m = (0.587 * (1-saturation)) * 256;
	rgb2rgb1.coef_gb.integer = ((m & 0xF00) >> 8);
	rgb2rgb1.coef_gb.decimal = (m & 0xFF);
	m = (0.114 + 0.886*saturation) * 256;
	rgb2rgb1.coef_bb.integer = ((m & 0xF00) >> 8);
	rgb2rgb1.coef_bb.decimal = (m & 0xFF);
#if 0
	int brightness = (((param->brightness - 50) * 1023 / 50) & 0x1FFF);
	rgb2rgb1.out_ofst_r = brightness;
	rgb2rgb1.out_ofst_g = brightness;
	rgb2rgb1.out_ofst_b = brightness;
#else
	rgb2rgb1.out_ofst_r = 0;
	rgb2rgb1.out_ofst_g = 0;
	rgb2rgb1.out_ofst_b = 0;
#endif
	/*
	printf("rgb2rgb1.coef_rr = {%d, %d}\n",
		(int)rgb2rgb1.coef_rr.integer,
		(int)rgb2rgb1.coef_rr.decimal);
	printf("rgb2rgb1.coef_gr = {%d, %d}\n",
		(int)rgb2rgb1.coef_gr.integer,
		(int)rgb2rgb1.coef_gr.decimal);
	printf("rgb2rgb1.coef_br = {%d, %d}\n",
		(int)rgb2rgb1.coef_br.integer,
		(int)rgb2rgb1.coef_br.decimal);
	printf("rgb2rgb1.coef_rg = {%d, %d}\n",
		(int)rgb2rgb1.coef_rg.integer,
		(int)rgb2rgb1.coef_rg.decimal);
	printf("rgb2rgb1.coef_gg = {%d, %d}\n",
		(int)rgb2rgb1.coef_gg.integer,
		(int)rgb2rgb1.coef_gg.decimal);
	printf("rgb2rgb1.coef_bg = {%d, %d}\n",
		(int)rgb2rgb1.coef_bg.integer,
		(int)rgb2rgb1.coef_bg.decimal);
	printf("rgb2rgb1.coef_rb = {%d, %d}\n",
		(int)rgb2rgb1.coef_rb.integer,
		(int)rgb2rgb1.coef_rb.decimal);
	printf("rgb2rgb1.coef_gb = {%d, %d}\n",
		(int)rgb2rgb1.coef_gb.integer,
		(int)rgb2rgb1.coef_gb.decimal);
	printf("rgb2rgb1.coef_bb = {%d, %d}\n",
		(int)rgb2rgb1.coef_bb.integer,
		(int)rgb2rgb1.coef_bb.decimal);
	printf("rgb2rgb1.out_ofst_r= %d\n",
		rgb2rgb1.out_ofst_r);
	printf("rgb2rgb1.out_ofst_g= %d\n",
		rgb2rgb1.out_ofst_g);
	printf("rgb2rgb1.out_ofst_b= %d\n",
		rgb2rgb1.out_ofst_b);
	*/
	
	ret = ioctl(preview_fd, PREV_S_PARAM, &module_param);
	
	if(ret < 0)
	{
		perror("davinci_preview set saturation failed");
		
		return -1;
	}
	
	// 色调
	module_param.module_id = PREV_RGB2RGB_2;
	strcpy(module_param.version, "5.1");
	module_param.param = &rgb2rgb2;
	module_param.len = sizeof(struct prev_rgb2rgb);
	
	float angle = 3.14159265 * ((float)(param->hue - 50) / 50);
	float c = cosf(angle);
	float s = sinf(angle);

	m = (0.299 + 0.701 * c - 0.299 * s) * 256;
	rgb2rgb2.coef_rr.integer = ((m & 0x700) >> 8);
	rgb2rgb2.coef_rr.decimal = (m & 0xFF);
	m = (0.587 - 0.587 * c - 0.587 * s) * 256;
	rgb2rgb2.coef_gr.integer = ((m & 0x700) >> 8);
	rgb2rgb2.coef_gr.decimal = (m & 0xFF);
	m = (0.114 - 0.114 * c + 0.886 * s) * 256;
	rgb2rgb2.coef_br.integer = ((m & 0x700) >> 8);
	rgb2rgb2.coef_br.decimal = (m & 0xFF);
	m = (0.299 - 0.299 * c + 0.143 * s) * 256;
	rgb2rgb2.coef_rg.integer = ((m & 0x700) >> 8);
	rgb2rgb2.coef_rg.decimal = (m & 0xFF);
	m = (0.587 + 0.413 * c + 0.140 * s) * 256;
	rgb2rgb2.coef_gg.integer = ((m & 0x700) >> 8);
	rgb2rgb2.coef_gg.decimal = (m & 0xFF);
	m = (0.114 - 0.114 * c - 0.283 * s) * 256;
	rgb2rgb2.coef_bg.integer = ((m & 0x700) >> 8);
	rgb2rgb2.coef_bg.decimal = (m & 0xFF);
	m = (0.299 - 0.299 * c - 0.701 * s) * 256;
	rgb2rgb2.coef_rb.integer = ((m & 0x700) >> 8);
	rgb2rgb2.coef_rb.decimal = (m & 0xFF);
	m = (0.587 - 0.587 * c + 0.587 * s) * 256;
	rgb2rgb2.coef_gb.integer = ((m & 0x700) >> 8);
	rgb2rgb2.coef_gb.decimal = (m & 0xFF);
	m = (0.114 + 0.886 * c + 0.114 * s) * 256;
	rgb2rgb2.coef_bb.integer = ((m & 0x700) >> 8);
	rgb2rgb2.coef_bb.decimal = (m & 0xFF);

	rgb2rgb2.out_ofst_r = 0;
	rgb2rgb2.out_ofst_g = 0;
	rgb2rgb2.out_ofst_b = 0;

	/*	
	printf("rgb2rgb2.coef_rr = {%d, %d}\n",
		(int)rgb2rgb2.coef_rr.integer,
		(int)rgb2rgb2.coef_rr.decimal);
	printf("rgb2rgb2.coef_gr = {%d, %d}\n",
		(int)rgb2rgb2.coef_gr.integer,
		(int)rgb2rgb2.coef_gr.decimal);
	printf("rgb2rgb2.coef_br = {%d, %d}\n",
		(int)rgb2rgb2.coef_br.integer,
		(int)rgb2rgb2.coef_br.decimal);
	printf("rgb2rgb2.coef_rg = {%d, %d}\n",
		(int)rgb2rgb2.coef_rg.integer,
		(int)rgb2rgb2.coef_rg.decimal);
	printf("rgb2rgb2.coef_gg = {%d, %d}\n",
		(int)rgb2rgb2.coef_gg.integer,
		(int)rgb2rgb2.coef_gg.decimal);
	printf("rgb2rgb2.coef_bg = {%d, %d}\n",
		(int)rgb2rgb2.coef_bg.integer,
		(int)rgb2rgb2.coef_bg.decimal);
	printf("rgb2rgb2.coef_rb = {%d, %d}\n",
		(int)rgb2rgb2.coef_rb.integer,
		(int)rgb2rgb2.coef_rb.decimal);
	printf("rgb2rgb2.coef_gb = {%d, %d}\n",
		(int)rgb2rgb2.coef_gb.integer,
		(int)rgb2rgb2.coef_gb.decimal);
	printf("rgb2rgb2.coef_bb = {%d, %d}\n",
		(int)rgb2rgb2.coef_bb.integer,
		(int)rgb2rgb2.coef_bb.decimal);
	printf("rgb2rgb2.out_ofst_r= %d\n",
		rgb2rgb2.out_ofst_r);
	printf("rgb2rgb2.out_ofst_g= %d\n",
		rgb2rgb2.out_ofst_g);
	printf("rgb2rgb2.out_ofst_b= %d\n",
		rgb2rgb2.out_ofst_b);
	*/

	ret = ioctl(preview_fd, PREV_S_PARAM, &module_param);
	
	if(ret < 0)
	{
		perror("davinci_preview set hue  failed");
		
		return -1;
	}
	
	// 色度增益抑制
	module_param.module_id = PREV_CGS;
	strcpy(module_param.version, "5.1");
	module_param.param = &cgs;
	module_param.len = sizeof(struct prev_cgs);
	
	/*
	printf("cgs.en = %d\n",
		cgs.en);
	printf("cgs.h_thr = %d\n",
		cgs.h_thr);
	printf("cgs.h_slope = %d\n",
		cgs.h_slope);
	printf("cgs.h_shft = %d\n",
		cgs.h_shft);
	printf("cgs.h_min = %d\n",
		cgs.h_min);
	*/
		
	ret = ioctl(preview_fd,PREV_S_PARAM, &module_param);
	if(ret < 0)
	{
		perror("davinci_preview set cgs  failed");
		
		return -1;
	}
				
	return 0;	
}

void create_davinci_previewer(PREVIEW_DEVICE* device)
{
	device->open = davinci_preview_open;
	device->close = davinci_preview_close;
	device->set_wb = davinci_preview_set_wb;
	device->set_gamma = davinci_preview_set_gamma;
	device->set_color = davinci_preview_set_color;
}
