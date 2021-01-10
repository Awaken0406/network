
#ifndef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


void read_childProc(int sig) {
	int status;
	printf("read_childProc begin \n");
	sleep(10);
	//要杀死僵尸进程就必须获取子进程的返回值,当调用waitpid时，如果有子进程终止就已经获得了返回值
	pid_t id = waitpid(-1,&status,WNOHANG);//-1,等待任意子进程终止,WNOHANG没有子进程终止也不会阻塞
	printf("read_childProc end \n");
	//if (WIFEXITED(status)) {
	//	printf("remove proc id:%d \n", id);
	//	printf("child return:%d\n", WEXITSTATUS(status));
	//}
}

void alarm_func(int sig) {

	printf("this is alarm timeout,pid:%d\n",getpid());
}

int main(int argc, char* argv[])
{

	struct sigaction act;
	act.sa_handler = read_childProc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGCHLD, &act, 0);//信号在主进程注册，所以发送信号时，主进程的sleep()会被唤醒
	pid_t pid = fork();
	if (pid == 0) {
		puts("Hi,I'm child process 1");

		struct sigaction act;//事实证明哪个进程注册的信号，就由哪个进程处理信号
		act.sa_handler = alarm_func;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;

		sigaction(SIGALRM, &act, 0);
		alarm(10);

		sleep(100);
		return 12;
	}
	else {
		printf("Child proc is id:%d\n", pid);
		pid = fork();
		if (pid == 0) {
			puts("Hi,I'm child process 2");
			sleep(1);
			exit(24);
		}
		else {
			printf("Child proc is id:%d\n", pid);
			for (int i = 0; i < 5; i++) {
				puts("wait.....");
				sleep(10);
			}
		}
		return 0;
	}

}

#endif