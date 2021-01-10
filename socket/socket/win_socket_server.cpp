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

	//С��:���ֽ��ڸߵ�ַ
	//��ˣ����ֽ��ڵ͵�ַ
	printf("host_port  : %#x \n", host_port);
	printf("��� net_port  : %#x \n", net_port);
	printf("host_addr  : %#x \n", host_addr);
	printf("��� net_addr  : %#x \n", net_addr);
}

void DoItServer(char* argv[])
{
	WSADATA wsaData;
	SOCKET hServerSock, hClientSock;
	SOCKADDR_IN serAddr, clientAddr;

	int szClientAddr = sizeof(clientAddr);
	char message[] = "Hello World!";
	//printf("size:%d\n", sizeof(message));

	//��ʼ���׽��ֿ�
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		s_error_handling("WSAStartup error");
	}

	endian_conv();

	//#�����׽���,���ؾ��
	hServerSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServerSock == INVALID_SOCKET) {
		s_error_handling("socket() error");
	}
	//#����Ip�Ͷ˿�
	memset(&serAddr, 0, sizeof(serAddr));
	serAddr.sin_family = AF_INET;
	serAddr.sin_addr.s_addr = htonl(INADDR_ANY);//htonlת����long���͵������ֽ���
	serAddr.sin_port = htons(11000);//htonsת����short���͵������ֽ���

	//#��IP�Ͷ˿�
	if (bind(hServerSock, (SOCKADDR*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		s_error_handling("bind error");
	}
	//#����
	if (listen(hServerSock, 5) == SOCKET_ERROR) {
		s_error_handling("listen error");
	}
	//#���� acceptĬ��������
	hClientSock = accept(hServerSock, (SOCKADDR*)&clientAddr, &szClientAddr);
	if (hClientSock == INVALID_SOCKET) {
		s_error_handling("accept error");
	}
	printf("����\n");

	char cbuf[32] = { 0 };
	
	while (true) {
		int len = 10000;
		len = recv(hClientSock, cbuf, 32, 0);//hClientSockĬ����������,�������յ����ݲŴ�ӡ,���hClientSock�Ͽ��󲻴���hClientSock����һֱ�յ�EOF
		printf("len = %d\n",len);			//�ͻ��������ر��׽���ʱ��0,ǿ���˳�ʱ��-1
		memset(cbuf,0,32);
	}
	printf("�ر�\n");
	//send(hClientSock, message, sizeof(message), 0);
	closesocket(hServerSock);
	closesocket(hClientSock);
	WSACleanup();
}

 int main(int argc, char* argv[]){
	DoItServer(argv);
	return 0;
}