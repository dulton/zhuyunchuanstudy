//
//  "$Id: agc.c 307 2009-12-01 06:21:27Z wang_haifeng $"
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

static GAMMA_VALUE default_values[4][512];

int agc_get_preset_gamma_table(GAMMA_MODE mode, GAMMA_TABLE *table)
{
	int c, i;
	int point[3][512][2] = {
		{ // red points
			{0, 0}, {8, 72}, {16, 131}, {32, 213}, {64, 336}, {128, 497}, {256, 719}, {512, 1024}
		//	{0, 0}, {8, 72}, {16, 131}, {32, 218}, {64, 346}, {128, 487}, {256, 649}, {512, 780}
		},
		{ // green points
			{0, 0}, {8, 72}, {16, 131}, {32, 213}, {64, 336}, {128, 497}, {256, 719}, {512, 1024}
		},
		{ // blue points
			{0, 0}, {8, 72}, {16, 131}, {32, 213}, {64, 336}, {128, 497}, {256, 719}, {512, 1024}
		//	{0, 0}, {8, 72}, {16, 131}, {32, 213}, {64, 346}, {128, 497}, {256, 669}, {512, 810}
		}
	};
	
	for(c = 0; c < 3; c++)
	{
		int cursor = 0;
		float slope = 0;

		GAMMA_VALUE *gamma = default_values[c];
		for(i = 0; i < 512; i++)
		{
			if(i >= point[c][cursor][0])
			{
				slope = (float)(point[c][cursor + 1][1] - point[c][cursor][1])/(point[c][cursor + 1][0] - point[c][cursor][0]);
			}
			
			gamma->offset = (unsigned short)(point[c][cursor][1] + (i - point[c][cursor][0]) * slope);
			gamma++;
			if(i >= point[c][cursor][0])
			{
				cursor++;
			}
		}
		
		gamma = default_values[c];
		for(i = 0; i < 512; i++)
		{
			if(i == 511)
			{
				gamma->slope = 0;
			}
			else
			{
				gamma->slope = ((gamma + 1)->offset - gamma->offset);
			}
	
			gamma++;
		}		
	}
    
    table->r = default_values[0];
    table->g = default_values[1];
    table->b = default_values[2];
    
    return 0;
}
