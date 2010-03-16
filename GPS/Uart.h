/*
 * Uart.h
 *
 *  Created on: 2010-3-9
 *      Author: Administrator
 */

#ifndef UART_H_
#define UART_H_

typedef enum RET_VALUE
{
	RET_OK=0,
	RET_OPEN_FAIL,
	RET_CLOSE_FAIL,
	RET_READ_FAIL,
	RET_INIT_FAIL,
}RET;

typedef enum _COM_NUM COM_NUM;
struct _GPS_MSG;
typedef struct _GPS_MSG GPS_MSG;

struct UART_ATTRIBUTE
{

};
struct GPS_MSG *uart_read(COM_NUM com);
RET uart_create(COM_NUM com);
RET uart_destory(COM_NUM com);
RET uart_setattr(struct UART_ATTRIBUTE, COM_NUM com);
#endif /* UART_H_ */
