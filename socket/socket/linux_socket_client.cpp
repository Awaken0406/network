

#ifndef _WIN32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	if (argc != 3) {
		printf("usage:%s <IP> <port>\n", argv[0]);
		exit(1);
	}

	printf("%s:%s:%s\n", argv[0], argv[1], argv[2]);
	//#创建套接字,返回文件描述符
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1) {
		error_handling("socket error");
	}
	//#设置要连接的服务器IP和端口
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	//#连接
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("connect error");
	}
	char message[30];
	int str_len = 0;
	int idx = 0;
	int read_in = 0;

	while (read_in = read(sock, &message[idx++], 1)) {
		if (read_in == -1) {
			error_handling("read error");
		}
		str_len += read_in;
	} 
// 	if (str_len == -1) {
// 		error_handling("read error");
// 	}
	printf("message from server : %s\n",message);
	printf("function read call count : %d\n", str_len);
	close(sock);
	return 0;
}

#endif