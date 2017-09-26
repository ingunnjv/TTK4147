#include <stdlib.h>
#include <stdio.h>
#include "philosophers.h"
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t forks[5];

int main(){
	
	test1();
	test2();

}


void test1(){
	pthread_t p[5];

	for (int i = 0; i < 5; i++)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			perror("mutex initilization");
		pthread_create(&p[i], NULL, myThreadFunction, (void*)i);
		
	}

	pthread_join(p[0], NULL);
	pthread_join(p[1], NULL);
	pthread_join(p[2], NULL);
	pthread_join(p[3], NULL);
	pthread_join(p[4], NULL);
}

void test2(){
	pthread_t p[5];

	for (int i = 0; i < 5; i++)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			perror("mutex initilization");
		pthread_create(&p[i], NULL, myThreadFunction, (void*)i);
		
	}

	pthread_join(p[0], NULL);
	pthread_join(p[1], NULL);
	pthread_join(p[2], NULL);
	pthread_join(p[3], NULL);
	pthread_join(p[4], NULL);
}

void* myThreadFunction(void *id_in)
{
	int id = (int)id_in;

	int right_fork = id;
	int left_fork = id + 1;

	if ( left_fork == 5)
		left_fork = 0;
	while(1)
	{

		printf("%i\n", id);
		usleep(500);
		pthread_mutex_lock(&forks[left_fork]);
		pthread_mutex_lock(&forks[right_fork]);
		printf("philosopher number %i is eating, num num num\n",id);
		usleep(1000);
		pthread_mutex_unlock(&forks[right_fork]);
		pthread_mutex_unlock(&forks[left_fork]);
	}
}