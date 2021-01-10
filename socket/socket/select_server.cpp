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
#define BUF_SIZE 100

void error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char * argv[])
{
	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	struct timeval timeout;
	fd_set reads, cpy_reads;
	
	socklen_t addr_size = sizeof(client_addr);
	int fd_max, str_len, fd_num = 0;
	char buf[BUF_SIZE];

	if (argc != 2) {
		exit(1);
	}
	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));


	//#绑定IP和端口
	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		error_handling("bind error");
	}
	//#监听
	if (listen(server_sock, 5) == -1) {
		error_handling("listen error");
	}

	FD_ZERO(&reads);
	FD_SET(server_sock, &reads);
	fd_max = server_sock;

	while (1) {
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;
		fd_num = select(fd_max + 1, &cpy_reads, NULL, NULL, &timeout);//第一个参数是文件描述符的数量,linux文件描述符从0开始
		if (fd_num == -1) {											//每次都传递整个监视对象信息cpy_reads,这是对性能影响最大的原因
			printf("fd_num == -1\n");
			break;
			//异常
		}
		else if (fd_num == 0){
			printf("time out\n");
			continue;
			//超时
		}
		else {
			for (int i = 0; i < fd_max + 1; i++) {//遍历文件描述符,
				if (FD_ISSET(i, &cpy_reads)) {//查找状态发送变化的
					if (i == server_sock) {//处理连接
						client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
						FD_SET(client_sock, &reads);
						if (fd_max < client_sock) {
							fd_max = client_sock;
						}
						printf("connected client:%d\n", client_sock);
					}
					else {//读取数据
						str_len = read(i, buf, BUF_SIZE);//在linux下强制关闭client会收到-1,然后再收到0;
						if (str_len == 0) {			
							FD_CLR(i, &reads);
							close(i);
							printf("closed client:%d\n", i);
						}
						else {
							write(i, buf, str_len);
						}
						printf("read message str_len = %d \n", str_len);
					}

				}
			}
		}
// 		close(server_sock);
// 		return 0;
	}

	return 0;
}


#endif