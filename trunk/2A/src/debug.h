//
//  "$Id: aew.h 453 2010-03-12 05:19:46Z wang_haifeng $"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _AEW_DEBUG_H_
#define _AEW_DEBUG_H_
#include <sys/ipc.h>
#include <sys/shm.h>				
#include <sys/msg.h>	
#include <sys/types.h>	


#define CLEAR(x)				memset (&(x), 0, sizeof (x))
#define ERR(fmt, args ... )    	do {fprintf(stderr, "[ERROR] LibVidEnc : FILE -> %s, LINE -> %d" ": "   fmt, __FILE__, __LINE__, ## args);} while(0)
#define  DEBUG(fmt, args...)   	fprintf(stdout, "libaew  "  ": " fmt, ## args)
//#define  DEBUG(fmt, args...) 
#define PRINT_OFF -1
#define PRINT_RESULT 1
#define PRINT_SATURATION 2
#define PRINT_Y			3
#define PRINT_RAW		4		/*打印从硬件H3A模块统计出来的原始数据*/


#define CMD_VALID	1
#define CMD_ACK		2
#define CMD_EMPTY   3

typedef struct _myMsgStruct	
{
	long flag; /*操作标志*/
	char cmd[128];
}myMsgStruct;	

#define FILE_AEWMSG	"/tmp/dbgaewmsg"
#endif
