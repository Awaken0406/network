
#ifndef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>

void error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

//设置非阻塞
void setNonblockingMode(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}


int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);

	char message[] = "Hello World!";
	if (argc != 2) {
		printf("usage:%s <port>\n", argv[0]);
		exit(1);
	}
	//#创建套接字,返回文件描述符
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		error_handling("socket error");
	}

	setNonblockingMode(serv_sock);
	//#设置Ip和端口
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//#绑定IP和端口
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1 ){
		error_handling("bind error");
	}
	//#监听
	if (listen(serv_sock, 5) == -1) {
		error_handling("listen error");
	}
	//#接收
	printf("wait...\n");
	clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1) {
		error_handling("accept error");
	}


	printf("accept");
	//char strAddr = inet_addr(clnt_addr.sin_addr);
	printf("addr = %#x ,port = %d\n", clnt_addr.sin_addr, clnt_addr.sin_port);

	//sleep(100);

	write(clnt_sock, message, sizeof(message));
	printf("clnt_sock");
	close(serv_sock);
	close(clnt_sock);
}

#endif