//
//  "$Id: awb.c 385 2010-02-04 12:10:18Z wang_haifeng $"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <asm-arm/arch-davinci/dm365_aew.h>
#include <asm-arm/arch/imp_common.h>
#include <asm-arm/arch-davinci/dm365_ipipe.h>

#include "aew.h"
#include "debug.h"

int dbg_bc_new = 0; /*brightness and contrast control algorithm*/
int dbg_dc_add = -1;	/*dc 值 加多少, -1表示不加*/
int dbg_dc_sub = 0;	/*dc 值 减多少*/
int dbg_gamma_en = 0;

int dbg_saturation_limit = -1;
int dbg_print_mode = PRINT_OFF;
int dbg_y_min = -1;
int dbg_y_max = -1;
int dbg_echo = 0;

extern SENSOR_DEVICE sen_dev;
extern PREVIEW_DEVICE pre_dev;
int aew_cmd_match(char *buf,char *cmd, int size)
{
	int ret = -1;
	size--;
	if (!strncmp(buf, cmd, size)){
		ret = strtoul(buf+size, NULL, 10);
		printf("cmd: %s,para:%d\n", cmd, ret);
	}
	return ret;
}
//用于调试
int dahua_aew_debug(char *buf)
{
	int ret;
	GAMMA_TABLE gamma_table;
	
	printf("#####dahua_aew_debug cmd = %s \n",buf);

	ret = 0;
	ret = aew_cmd_match(buf,"gamma",sizeof("gamma"));
	if (ret >= 0){
		dbg_gamma_en = ret;
		if (dbg_gamma_en == 1)
			pre_dev.set_gamma(IPIPE_GAMMA_TBL_ROM, &gamma_table);
		else
			pre_dev.set_gamma(IPIPE_GAMMA_TBL_ROM, NULL); //bypass
		
	}
	
	ret = 0;	
	ret = aew_cmd_match(buf,"dcadd",sizeof("dcadd"));
	if (ret >= 0){
		dbg_dc_add = ret;
		if (sen_dev.set_clamp != NULL){
			if (sen_dev.set_clamp(dbg_dc_add) <0){
				printf("debug: set_clamp %d failed\n",dbg_dc_add);
			}
		}
	}
	#if 0	/*jerry. 这一项由MyVtest程序中实现*/
	ret = aew_cmd_match(buf,"dcsub",sizeof("dcsub"));
	if (ret >= 0){
		dbg_dc_sub = ret;
	}
	#endif
	ret = 0;
	ret = aew_cmd_match(buf,"bcnew",sizeof("bcnew"));
	if (ret >= 0){
		dbg_bc_new = ret;
	}

	ret = 0;
	ret = aew_cmd_match(buf,"y_min", sizeof("y_min"));
	if (ret >= 0)
		dbg_y_min = ret;

	ret = 0;
	ret = aew_cmd_match(buf, "y_max", sizeof("y_max"));
	if (ret >= 0)
		dbg_y_max = ret;

	ret = 0;
	ret = aew_cmd_match(buf, "saturation", sizeof("satu"));
	if (ret >= 0)
		dbg_saturation_limit = ret;

	if (!strncmp(buf, "poff", sizeof("poff"))){
		dbg_print_mode = PRINT_OFF;
	}
	if (!strncmp(buf, "py", sizeof("py"))){
		dbg_print_mode = PRINT_Y;
	}
	if (!strncmp(buf, "ps", sizeof("ps"))){
		dbg_print_mode = PRINT_SATURATION;
	}
	if (!strncmp(buf, "praw", sizeof("praw"))){
		dbg_print_mode = PRINT_RAW;
	}
	if (!strncmp(buf, "pxx", sizeof("pxx"))){
		dbg_print_mode = PRINT_RESULT;
	}

	ret = 0;
	ret = aew_cmd_match(buf, "echo", sizeof("echo"));
	if (ret > 0)
		dbg_echo = ret;
	else
		dbg_echo = 4; /*默认打印4组*/
	
	return 0;
}
static myMsgStruct recvmsg;
static myMsgStruct *g_dbg_msgp =  NULL;
static int msqid = -1;

int	initialize_share_mem(void)
{
	key_t 	msgKey = -1;

	msgKey = ftok(FILE_AEWMSG,16);				
	if(msgKey == -1){
		ERR("create msgKey error ,please check the exist of filepath %s \n",FILE_AEWMSG);
		return -1;
	}
	msqid = msgget(msgKey,IPC_CREAT|0666);
  	if(msqid == -1)
  	{
   		ERR(" create shmID error \n");
		return -1;
  	}
		
	memset(&recvmsg,0,sizeof(recvmsg));
	g_dbg_msgp = &recvmsg;

	return 0;	
}	
int check_debug_cmd()
{
	if (g_dbg_msgp == NULL)
		initialize_share_mem();
	if (g_dbg_msgp == NULL)
		return -1;
		
		//自消息队列接收信息___ 非阻塞式读取
	if (msgrcv(msqid,g_dbg_msgp,sizeof(g_dbg_msgp->cmd),0,MSG_NOERROR|IPC_NOWAIT) < 0)
		return -1;  //没有消息
	printf("msgrcv :: %d:%s\n ",g_dbg_msgp->flag,g_dbg_msgp->cmd);
	if ((g_dbg_msgp->cmd[0]!= 'M') ||(g_dbg_msgp->cmd[1]!= 'S')||(g_dbg_msgp->cmd[2]!= 'K') || (g_dbg_msgp->cmd[3]!= CMD_VALID))
		return 0;	/*无命令*/
	g_dbg_msgp->cmd[2]= CMD_ACK;	/*响应下先*/
	dahua_aew_debug(&g_dbg_msgp->cmd[4]);
	memset(g_dbg_msgp->cmd, 0, sizeof(g_dbg_msgp->cmd));
	g_dbg_msgp->cmd[2]= CMD_EMPTY;
	return 0;
}

