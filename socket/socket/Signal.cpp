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

	sigaction(SIGALRM, &act, 0);//sigaction��unix����ϵͳͨ�õ�,һ����sigaction;

	//signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);
	for (int i = 0; i < 3; i++) {
		puts("wait....");
		sleep(100);//�����ź�ʱ���������ڵ���sleep��������������״̬�Ľ���
	}
	int nTime2 = time(0);
	printf("expend %d second !!!\n", nTime2 - nTime);
	return 0;
}


#endif