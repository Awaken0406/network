#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

void s_error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void DoItServer(char* argv[])
{
	WSADATA wsaData;
	SOCKET hServerSock, hClientSock;
	SOCKADDR_IN serAddr, clientAddr;

	int szClientAddr = sizeof(clientAddr);
	char message[] = "Hello World!";
	printf("size:%d\n", sizeof(message));

	//初始化套接字库
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		s_error_handling("WSAStartup error");
	}
	//#创建套接字,返回句柄
	hServerSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServerSock == INVALID_SOCKET) {
		s_error_handling("socket() error");
	}
	//#设置Ip和端口
	memset(&serAddr, 0, sizeof(serAddr));
	serAddr.sin_family = AF_INET;
	serAddr.sin_addr.s_addr = htonl(INADDR_ANY);//htonl转换成long类型的网络字节序
	serAddr.sin_port = htons(atoi(argv[1]));//htons转换成short类型的网络字节序

	//#绑定IP和端口
	if (bind(hServerSock, (SOCKADDR*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		s_error_handling("bind error");
	}
	//#监听
	if (listen(hServerSock, 5) == SOCKET_ERROR) {
		s_error_handling("listen error");
	}
	//#接收
	hClientSock = accept(hServerSock, (SOCKADDR*)&clientAddr, &szClientAddr);
	if (hClientSock == INVALID_SOCKET) {
		s_error_handling("accept error");
	}

	send(hClientSock, message, sizeof(message), 0);
	closesocket(hServerSock);
	closesocket(hClientSock);
	WSACleanup();
}

//int main(int argc, char* argv[]){if (argc != 2) {exit(1);}DoItServer(argv);return 0;}