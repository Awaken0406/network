#ifndef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30



int main()
{
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;
	FD_ZERO(&reads);
	FD_SET(0, &reads);// 0 is strandard input(console)

	while (1) {
		timeout.tv_sec = 5;//如果不在循环里设置会被替换为超时前剩余的时间
		timeout.tv_usec = 0;

		temps = reads;
		result = select(1, &temps, NULL, NULL, &timeout);
		printf("tv_sec = %d\n", timeout.tv_sec);
		if (result == -1) {
			puts("select() error!");
			break;
		}
		else if (result == 0) {
			puts("time-out!");
		}
		else {
			if (FD_ISSET(0, &temps)) {
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console:%s\n", buf);
			}
		}
	}


	return 0;
}


#endif