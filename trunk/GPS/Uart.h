/*
 * Uart.h
 *
 *  Created on: 2010-3-9
 *      Author: Administrator
 */

#ifndef UART_H_
#define UART_H_
#define COM1 "/dev/ttyS0"
#define COM2 "/dev/ttyS1"
#define COM3 "/dev/ttyS2"
#define _POSIX_SOURCE 1

typedef enum RET_VALUE
{
	RET_OK=0,
	RET_OPEN_FAIL,
	RET_CLOSE_FAIL,
	RET_READ_FAIL,
	RET_INIT_FAIL,
}RET;

struct _GPS_MSG;
typedef struct _GPS_MSG GPS_MSG;

struct GPS_MSG *uart_read();
RET uart_create();
RET uart_destory();
//RET uart_setattr(struct termio, );
#endif /* UART_H_ */
