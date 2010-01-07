#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H	

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT	8000 //服务器打开的端口号
#define BACKLOG		10  //定义请求队列中最大请求数	    
#define MAX_CONNECTION	10
//#define 

int Init();
int Socket_Creat(int domain, int type, int protocol);
int Socket_Close(int sock_fd);
int Socket_Bind(int sock_fd, const struct *addr, socklen_t len);
int Socket_Listen(int sock_fd, int backlog);
int Read();
void* Protocol_Parse();
void* Socket_Rev();
void Display();

#endif //end of SOCKET_SERVER_H
