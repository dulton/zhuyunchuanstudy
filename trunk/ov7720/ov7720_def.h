/*ov7720的全局定义
 *
 *
 *
 *
 *
 */

#ifndef _OV7720_DEF_H
#define _OV7720_DEF_H

#define K6_SCL_PIN 6 
#define K6_SDA_PIN 47
#define K6_RESET_PIN 7

#define SLAVE_OV7720_ADDR 0x42

#define OV7720_PID 0xa 		//product id number MSB
#define OV7720_VER 0xb 		//product id number LSB

typedef enum _DEVTYPE
{
	NVS = 1,
	IPC_F4X35,
};

#endif//define _OV7720_DEF_H
