#ifndef _WIN32
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define NUM_THREAD 100
long long num = 0;
pthread_mutex_t mutex;


void* thread_inc(void* arg)
{
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < 50000000; i++) {
		num += 1;
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}
void* thread_des(void* arg)
{
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < 50000000; i++) {
		num -= 1;
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}



int main(int argc, char* argv[])
{
	int nTime = time(0);
	pthread_mutex_init(&mutex,NULL);
	pthread_t thread[NUM_THREAD];
	printf("sizeof long long :%d\n", sizeof(long long));

	for (int i = 0; i < NUM_THREAD; i++) {
		if (i % 2)
			pthread_create(&thread[i], NULL, thread_inc,NULL);
		else
			pthread_create(&thread[i], NULL, thread_des, NULL);
	
	}
	for (int i = 0; i < NUM_THREAD; i++) {
		pthread_join(thread[i], NULL);
	}
	int nTime2 = time(0);
	pthread_mutex_destroy(&mutex);
	printf("result:%lld\n", num);
	printf("expend time:%d\n", nTime2 - nTime);//试验结果:不加锁13秒,加锁11秒,好神奇
	return 0;
}


#endif 