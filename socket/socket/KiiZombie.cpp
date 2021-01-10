
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
	//Ҫɱ����ʬ���̾ͱ����ȡ�ӽ��̵ķ���ֵ,������waitpidʱ��������ӽ�����ֹ���Ѿ�����˷���ֵ
	pid_t id = waitpid(-1,&status,WNOHANG);//-1,�ȴ������ӽ�����ֹ,WNOHANGû���ӽ�����ֹҲ��������
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

	sigaction(SIGCHLD, &act, 0);//�ź���������ע�ᣬ���Է����ź�ʱ�������̵�sleep()�ᱻ����
	pid_t pid = fork();
	if (pid == 0) {
		puts("Hi,I'm child process 1");

		struct sigaction act;//��ʵ֤���ĸ�����ע����źţ������ĸ����̴����ź�
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