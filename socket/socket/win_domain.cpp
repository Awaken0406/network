#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>

void s_error(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
/*
int main(int argc, char * argv[]) {


	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		s_error("WSAStartup error");
	}

	struct hostent* host;
	host = gethostbyname("www.baidu.com");


	unsigned long ip = inet_addr("14.215.177.39");
	//host = gethostbyaddr((char*)&ip, 4, AF_INET);

	if (!host) {
		s_error("host error");
	}
	//�ٷ�����
	printf("�����б� offcial name:%s\n",host->h_name);
	//�����б�
	for (int i = 0; host->h_aliases[i]; i++) {
		printf("�����б� Aliases %d:%s \n", i + 1, host->h_aliases[i]);
	}
	//IP��ַ����
	printf("IP��ַ���� Address type : %s \n",(host->h_addrtype == AF_INET)?"AF_INET" : "AF_INET6");
	//IP��ַ
	for (int i = 0; host->h_addr_list[i]; i++) {
		printf("IP��ַ IP addr %d:%s \n", i + 1, inet_ntoa(*( in_addr*)host->h_addr_list[i]));
	}

	WSACleanup();
	return 0;
}

*/
