/*
 * =====================================================================================
 *
 *       Filename:  ov7720.c
 *
 *    Description:  ov7720操作实现
 *
 *        Version:  1.0
 *        Created:  2010年07月22日 21时13分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include <ov7720.h>

struct ov7720_pin
{
	SLA_PIN;	
	SDA_PIN;
	RESET_PIN;
}

#define K6_SAL_PIN 
#define K6_SDA_PIN
#define K6_RESET_PIN

struct OV7720* create_ov7720()
{}
