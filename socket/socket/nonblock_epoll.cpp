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
#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

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

int main(int argc, char * argv[])
{
	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_size = sizeof(client_addr);
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
	//设置非阻塞,
	setNonblockingMode(server_sock);

	int epfd = epoll_create(EPOLL_SIZE);//创建epool例程
	struct epoll_event* ep_events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = server_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, server_sock, &event);

	int event_count = 0;
	while (1) {
		event_count = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);//默认是条件触发
		if (event_count == -1) {
			puts("epoll_wait() error");
			break;
		}
		puts("epoll_warit");
		for (int i = 0; i < event_count; i++) {
			if (ep_events[i].data.fd == server_sock) {
				client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
				setNonblockingMode(client_sock);
				event.events = EPOLLIN | EPOLLET;//EPOLLET是边缘触发,当收到数据时仅注册一次
				event.data.fd = client_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &event);
				printf("connected client:%d\n", client_sock);
			}
			else {

				while (1) {
					memset(buf, 0, BUF_SIZE);
					int str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);//条件触发，在读取完缓存的数据前会一直注册事件
					if (str_len == 0) {
						epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
						close(ep_events[i].data.fd);
						printf("str_len == 0, closed client:%d\n", ep_events[i].data.fd);
						break;
					}
					else if (str_len < 0) {
						if (errno == EAGAIN) {
							printf("str_len < 0 errno == EAGAIN, \n");
							break;
						}
						else {
							printf("str_len < 0 errno != EAGAIN, \n");
						}
					}
					else {
						write(ep_events[i].data.fd, buf, str_len);
					}
					printf("str_len = %d read message:%s  \n", str_len, buf);
				}			
			}
		}
	}
	close(server_sock);
	close(epfd);
	return 0;
}


#endif