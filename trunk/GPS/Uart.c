/*
 * Uart.c
 *
 *  Created on: 2010-3-9
 *      Author: Administrator
 */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Uart.h"
#define BAUDRATE B115200
#define _POSIX_SOURCE 1

struct _GPS_MSG
{

};

static RET uart_init(int fd_com)
{
    int ret = RET_INIT_FAIL;
    struct termios oldtio;
    struct termios newtio;

    tcgetattr(fd_com, &oldtiio);
    bzero(&newtio, sizeof(termios));

    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL |CREAD | HUPCL;
    newtio.c_iflag = IGNBRK;
    newtio.c_oflag = 0;
    newtio.c_lflag = ICANON;

    if ((ret = tcflush(fd_com, TCIFLUSH)) != 0);
    {
       ret = RET_INIT_FAIL;
       printf(" tcflush  error");
       return ret;
    }

    if ((ret = tcsetattr(fd_com, TCSANOW, &newtio)) != 0)
    {
       ret = RET_INIT_FAIL;
       printf("tcsetattr error");
       return ret;
    }

    ret = RET_OK;
}
static RET msg_parse(struct GPS_MSG *gps_msg)
{

}

RET uart_create()
{
	int fd_com;
	int ret = RET_OPEN_FAIL;

	if ((fd_com = open(COM1, O_RDWR | O_NOCTTY)) < 0)
	{
		printf("open COM error");
		return ret;
	}

	if ((ret = uart_init(fd_com)) != RET_OK)
	{
	    printf("init error");
	    return ret;
    }
	printf("open COM1 ok");
	return RET_OK;
}

//fd_com怎么传还没搞清楚
RET uart_destory(int fd_com)
{
	int ret = RET_CLOSE_FAIL;

	if ((fd_com = close(fd_com)) < 0)
	{
		printf("close fd_com error");
		return ret;
	}

	printf("close COM1 ok");

	ret = RET_OK;
	return ret;
}

/*
RET uart_setattr(struct termio, int fd_com)
{
	int ret = RET_INIT_FAIL;

	if (RET_INIT_FAIL == (ret = uart_init(fd_com, termio)))
	{
		printf("com init error");
	}

	return ret;
}
*/

struct GPS_MSG *uart_read()
{

};
