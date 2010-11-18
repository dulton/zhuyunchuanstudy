/*
 * =====================================================================================
 *
 *       Filename:  ov7720_api.c
 *
 *    Description:  读写ov7720寄存器
 *
 *        Version:  1.0
 *        Created:  2010年11月18日 19时05分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char* argv[])
{
	int ret = -1;
	int flag = 0;
	FILE* fd;

	fd = fopen("/dev/", WORD);

	while(1)		
	{
		printf("Enter 1 for write, enter 2 for read");
		scanf("%d", flag);

		if(flag != 2 && flag != 1)
		{
			printf("Enter a vaild flag");
		}

		switch flag
		
	}

}
