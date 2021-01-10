#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

void s_error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


void endian_conv()
{
	unsigned short host_port = 0x1234;
	unsigned short net_port = htons(host_port);

	unsigned int host_addr = 0x12345678;
	unsigned int net_addr = htonl(host_addr);

	//小端:高字节在高地址
	//大端：高字节在低地址
	printf("host_port  : %#x \n", host_port);
	printf("大端 net_port  : %#x \n", net_port);
	printf("host_addr  : %#x \n", host_addr);
	printf("大端 net_addr  : %#x \n", net_addr);
}

void DoItServer(char* argv[])
{
	WSADATA wsaData;
	SOCKET hServerSock, hClientSock;
	SOCKADDR_IN serAddr, clientAddr;

	int szClientAddr = sizeof(clientAddr);
	char message[] = "Hello World!";
	//printf("size:%d\n", sizeof(message));

	//初始化套接字库
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		s_error_handling("WSAStartup error");
	}

	endian_conv();

	//#创建套接字,返回句柄
	hServerSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServerSock == INVALID_SOCKET) {
		s_error_handling("socket() error");
	}
	//#设置Ip和端口
	memset(&serAddr, 0, sizeof(serAddr));
	serAddr.sin_family = AF_INET;
	serAddr.sin_addr.s_addr = htonl(INADDR_ANY);//htonl转换成long类型的网络字节序
	serAddr.sin_port = htons(11000);//htons转换成short类型的网络字节序

	//#绑定IP和端口
	if (bind(hServerSock, (SOCKADDR*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		s_error_handling("bind error");
	}
	//#监听
	if (listen(hServerSock, 5) == SOCKET_ERROR) {
		s_error_handling("listen error");
	}
	//#接收 accept默认阻塞的
	hClientSock = accept(hServerSock, (SOCKADDR*)&clientAddr, &szClientAddr);
	if (hClientSock == INVALID_SOCKET) {
		s_error_handling("accept error");
	}
	printf("链接\n");

	char cbuf[32] = { 0 };
	
	while (true) {
		int len = 10000;
		len = recv(hClientSock, cbuf, 32, 0);//hClientSock默认是阻塞的,所以有收到数据才打印,如果hClientSock断开后不处理hClientSock，会一直收到EOF
		printf("len = %d\n",len);			//客户端正常关闭套接字时是0,强制退出时是-1
		memset(cbuf,0,32);
	}
	printf("关闭\n");
	//send(hClientSock, message, sizeof(message), 0);
	closesocket(hServerSock);
	closesocket(hClientSock);
	WSACleanup();
}

 int main(int argc, char* argv[]){
	DoItServer(argv);
	return 0;
}