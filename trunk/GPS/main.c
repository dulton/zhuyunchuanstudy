/*
 * main.c
 *
 *  Created on: 2010-3-16
 *      Author: Administrator
 */

#include "Uart.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#define _POSIX_SOURCE 1

static void* uart_process()
{
    RET ret = RET_OPEN_FAIL;

    assert(RET_OK == ret);
	while(1)
	{
	    uart_read();
	}
}
int main(int argc, char **argv)
{
	thread_t uart_thread;
	thread_t kalman_thread;
	int ret = 0;

	ret = pthread_create(&uart_thread, NULL, (void*)uart_process, (void*)NULL);
	/*if (ret != 0)
	{
		printf("uart_thread create error");
		return -1;
	}
	*/
	assert(0 == ret);
}
