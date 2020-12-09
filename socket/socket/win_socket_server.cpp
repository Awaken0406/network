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

	//��ʼ���׽��ֿ�
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		s_error_handling("WSAStartup error");
	}
	//#�����׽���,���ؾ��
	hServerSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServerSock == INVALID_SOCKET) {
		s_error_handling("socket() error");
	}
	//#����Ip�Ͷ˿�
	memset(&serAddr, 0, sizeof(serAddr));
	serAddr.sin_family = AF_INET;
	serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serAddr.sin_port = htons(atoi(argv[1]));

	//#��IP�Ͷ˿�
	if (bind(hServerSock, (SOCKADDR*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		s_error_handling("bind error");
	}
	//#����
	if (listen(hServerSock, 5) == SOCKET_ERROR) {
		s_error_handling("listen error");
	}
	//#����
	hClientSock = accept(hServerSock, (SOCKADDR*)&clientAddr, &szClientAddr);
	if (hClientSock == INVALID_SOCKET) {
		s_error_handling("accept error");
	}

	send(hClientSock, message, sizeof(message), 0);
	closesocket(hServerSock);
	closesocket(hClientSock);
	WSACleanup();
}

int main(int argc, char* argv[]){if (argc != 2) {exit(1);}DoItServer(argv);return 0;}