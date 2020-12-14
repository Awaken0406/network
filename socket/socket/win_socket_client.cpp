#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

void error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void DoItClient(char* argv[])
{
	WSADATA wsaData;
	SOCKET hsocket;
	SOCKADDR_IN serAddr;

	char message[30];


	//��ʼ���׽��ֿ�
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		error_handling("WSAStartup error");
	}
	//#�����׽���,���ؾ��
	hsocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hsocket == INVALID_SOCKET) {
		error_handling("socket() error");
	}

	//#����Ҫ���ӵķ�����IP�Ͷ˿�
	memset(&serAddr, 0, sizeof(serAddr));
	serAddr.sin_family = AF_INET;
	serAddr.sin_addr.s_addr = inet_addr(argv[1]);//inet_addr���ַ�����ַת����unsigned int���� ��inet_aton(char* ip,struct in_addr * addr);������ͬ
	serAddr.sin_port = htons(atoi(argv[2]));

	//#����
	if (connect(hsocket, (SOCKADDR*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		error_handling("bind error");
	}

	int strlen = recv(hsocket, message, sizeof(message) - 1,0);
	if (strlen == -1) {
		error_handling("recv error");
	}
	printf("message from sever : %s", message);
	
	closesocket(hsocket);
	WSACleanup();
}

//int main(int argc, char* argv[]){if (argc != 3) {exit(1);}DoItClient(argv);return 0;}