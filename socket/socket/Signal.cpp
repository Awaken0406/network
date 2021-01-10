#ifndef _WIN32
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void timeout(int sig) {
	if (sig == SIGALRM) {
		puts("time out!");
	}
	alarm(2);
}

void keycontrol(int sig) {
	if (sig == SIGINT)
		puts("CTRL+C pressed");
}

int main() {
	int nTime = time(0);
	int i;


	struct sigaction act;
	act.sa_handler = timeout;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGALRM, &act, 0);//sigaction是unix操作系统通用的,一般用sigaction;

	//signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);
	for (int i = 0; i < 3; i++) {
		puts("wait....");
		sleep(100);//发送信号时将唤醒由于调用sleep函数而进入阻塞状态的进程
	}
	int nTime2 = time(0);
	printf("expend %d second !!!\n", nTime2 - nTime);
	return 0;
}


#endif