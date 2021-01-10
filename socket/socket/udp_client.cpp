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
	int sock;
	char message[BUFF_SIZE];
	int str_len;

	struct sockaddr_in serv_addr, from_addr;
	socklen_t addr_sz = sizeof(from_addr);
	if (argc != 3) {
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		error_handling("socket error");
	}
	//#ÉèÖÃIpºÍ¶Ë¿Ú
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	while (1) {
		fgets(message, BUFF_SIZE, stdin);
		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		memset(message, 0, BUFF_SIZE);
		str_len = recvfrom(sock, message, BUFF_SIZE, 0, (struct sockaddr*)&from_addr, &addr_sz);
		printf("message from server:%s", message);
	}
	close(sock);
	return 0;
}



#endif