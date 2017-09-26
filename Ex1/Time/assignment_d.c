#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/times.h>
#include <unistd.h>

void busy_wait_delay(int seconds)
{
	int i, dummy;
	int tps = sysconf(_SC_CLK_TCK);
	clock_t start;
	struct tms exec_time;
	times(&exec_time);
	start = exec_time.tms_utime;
	while( (exec_time.tms_utime - start) < (seconds * tps))
	{
		for(i=0; i<1000; i++)
		{
			dummy = i;
		}
		times(&exec_time);
	}
}

void *myThreadFunction1(void *vargp)
{
	printf("I am thread 1 \n");
	busy_wait_delay(5);
	//sleep(5);
	printf("I am thread 1 \n");
}

void *myThreadFunction2(void *vargp)
{
	printf("I am thread 2 \n");
	busy_wait_delay(5);
	//sleep(5);
	printf("I am thread 2 \n");
}

int main()
{
	pthread_t tid;
	printf("Before Threads\n");
	pthread_create(&tid, NULL, myThreadFunction1, NULL);
	pthread_create(&tid, NULL, myThreadFunction2, NULL);
	pthread_exit(NULL);
	printf("After thread\n");
	return 0;
}