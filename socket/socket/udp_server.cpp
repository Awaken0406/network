#ifndef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/select.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUFF_SIZE  30

void error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int serv_sock;
	char message[BUFF_SIZE];
	int str_len;

	struct sockaddr_in serv_addr, client_addr;
	socklen_t client_addr_sz = sizeof(client_addr);
	if (argc != 2) {
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1) {
		error_handling("socket error");
	}
	//#ÉèÖÃIpºÍ¶Ë¿Ú
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind error");

	while (1) {
		str_len = recvfrom(serv_sock, message, BUFF_SIZE, 0, (struct sockaddr*)&client_addr, &client_addr_sz);
		printf("recv message:%s",message);
		sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&client_addr, client_addr_sz);
	}
	close(serv_sock);
	return 0;
}



#endif