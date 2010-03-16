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

static void* uart_process(COM_NUM com)
{
	uart_setattr(UART_ATTRIBUTE, com);
	uart_create(com);
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
