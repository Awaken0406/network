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
	//官方域名
	printf("域名列表 offcial name:%s\n",host->h_name);
	//域名列表
	for (int i = 0; host->h_aliases[i]; i++) {
		printf("域名列表 Aliases %d:%s \n", i + 1, host->h_aliases[i]);
	}
	//IP地址类型
	printf("IP地址类型 Address type : %s \n",(host->h_addrtype == AF_INET)?"AF_INET" : "AF_INET6");
	//IP地址
	for (int i = 0; host->h_addr_list[i]; i++) {
		printf("IP地址 IP addr %d:%s \n", i + 1, inet_ntoa(*( in_addr*)host->h_addr_list[i]));
	}

	WSACleanup();
	return 0;
}

*/
