#include "socket_server.h"
#include <stdio.h>

int sockfd_client;
int sockfd_server;
struct sockaddr_in client_addr;
struct sockaddr_in server_addr;

//建立套接字
int Socket_Create(int domain, int type, int protocol)
{
	unlink("sockfd_server");
	if(-1 == sockfd_server = socket(domain, type, protocol));
	{
		printf("socket create error");

		exit(1);
	}

	printf("socket create succesful");

	return socketfd_server;
}

//close，关闭连接
int Socket_Close(int sock_fd)
{
	if(-1 == close(sock_fd));
	{
		printf("close socket error");

		exit(1);
	}

	printf("close socket succesful");
	return 0;
}

//Bind,绑定接口
int Socket_Bind(int sock_fd, const struct sockaddr *addr, socklen_t len)
{
	if(-1 == bind(sock_fd, addr, len))
	{
		printf("socket bind erroe");

		exit(1);
	}

	printf("bind %s to socket %d", addr, sock_fd);

	return 0;
}

//listen,开始监听
int Listen(int sock_fd, int backlog)
{
	assert(backlog <= BACKLOG);

	if(-1 == listen(sock_fd, backlog))
	{
		printf("listen error");

		exit(1);
	}
	
	return 0;
}

//read,读取发送过来的数据流
ssize_t Read(int filedes, void *buf, size_t nbytes)
{
	ssize_t len = 0;
	if(-1 == len = read(filedes, buf, nbytes))	
	{
		printf("read file error");
		exit(1);
	}

	printf("file len %d", len);

	return 0;
}

//协议解析,解析收到的包，并提取body中内容
/*void* Protocol_Parse(void* fd)
{
	void* fd_tmp = fd;

}
*/

//打印内容
int Display(void* buf)
{
	void* buf_tmp = buf;
	assert(NULL != buf_tmp);

	printf("%s", buf_tmp);
}
