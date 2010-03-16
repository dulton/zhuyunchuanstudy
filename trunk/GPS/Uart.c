/*
 * Uart.c
 *
 *  Created on: 2010-3-9
 *      Author: Administrator
 */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "Uart.h"

enum _COM_NUM
{
	COM0,
	COM1,
	COM2,
};

struct _GPS_MSG
{

};

static RET uart_init(COM_NUM com, struct UART_ATTRIBUTE)
{

}
static RET msg_parse(struct GPS_MSG *gps_msg)
{

}

RET uart_create(COM_NUM com)
{
	int fd_com;
	int ret = RET_OPEN_FAIL;

	if ((fd_com = open()) < 0)
	{
		printf("open COM%d error", com);
		return ret;
	}
	printf("open COM%d ok", com);
	return RET_OK;
}

RET uart_destory(COM_NUM com)
{
	int fd_com;
	int ret = RET_CLOSE_FAIL;

	if ((fd_com = close()) < 0)
	{
		printf("close fd_com error");
		return ret;
	}

	printf("close COM%d ok", com);
	ret = RET_OK;
	return ret;
}

RET uart_setattr(struct UART_ATTRIBUTE, COM_NUM com)
{
	int ret = RET_INIT_FAIL;

	if (RET_INIT_FAIL == (ret = uart_init(com, UART_ATTRIBUTE)))
	{
		printf("com init error");
	}

	return ret;
}
struct GPS_MSG *uart_read(COM_NUM com)
{

};
