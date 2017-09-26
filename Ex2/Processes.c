#include <stdlib.h>
#include <stdio.h>
#include "Processes.h"
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

int globalInt = 0;
sem_t mySem;
pthread_mutex_t lock;
int running = 1;
int var1 = 0;
int var2 = 0;

void processTestVFork()
{
	globalInt = 0;
	printf("%s\n", "************vfork*************");

	int localInt = 0;
	pid_t pid = vfork();
	if(pid == 0){
		for (int i = 0; i < 10; i++)
		{
			globalInt++;
			localInt++;
		}
		printf("Global: %i Local: %i\n",globalInt, localInt);
		_exit(0);
	}
	else if(pid > 0){
		for (int i = 0; i < 10; i++)
		{
			globalInt++;
			localInt++;
		}
		printf("Global: %i Local: %i\n",globalInt, localInt);
	}
	else{
		// Failed
	}
}

void processTestFork()
{
	globalInt = 0;
	printf("%s\n", "************fork*************");

	int localInt = 0;
	pid_t pid = fork();
	if(pid == 0){
		for (int i = 0; i < 10; i++)
		{
			globalInt++;
			localInt++;
		}
		printf("Global: %i Local: %i\n",globalInt, localInt);
		_exit(0);
	}
	else if(pid > 0){
		for (int i = 0; i < 10; i++)
		{
			globalInt++;
			localInt++;
		}
		printf("Global: %i Local: %i\n",globalInt, localInt);
	}
	else{
		// Failed
	}
}

void pthredTest(){
	globalInt = 0;
	pthread_t tid1;
	pthread_t tid2;
	pthread_create(&tid1, NULL, myThreadFunction1, NULL);
	pthread_join(tid1, NULL);
	pthread_create(&tid2, NULL, myThreadFunction1, NULL);
	pthread_join(tid2, NULL);
}

void semaphoresTest(){
	
    pthread_t threads[5];


	if( sem_init(&mySem,0,3) < 0);
	{
		perror("semaphore initilitzation");
		//exit(0);

	}

	for (int i = 0; i < 5; i++)
	{
		pthread_create(&threads[i], NULL, myThreadFunction2, (void*)i);
	}

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
	pthread_join(threads[3], NULL);
	pthread_join(threads[4], NULL);

}

void mutexTestWithoutMutex(){
	pthread_t tid3;
	pthread_t tid4;
	pthread_create(&tid3, NULL, myThreadFunction3, NULL);
	pthread_create(&tid4, NULL, myThreadFunction4, NULL);
	pthread_join(tid4, NULL);

}

void mutexTestWithMutex(){

    running = 1;
    var1 = 0;
    var2 = 0;
	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		perror("mutex initilization");
		//exit(0);
	}
	pthread_t tid3;
	pthread_t tid4;
	pthread_create(&tid3, NULL, myThreadFunction5, NULL);
	pthread_create(&tid4, NULL, myThreadFunction6, NULL);
	pthread_join(tid3, NULL);
	pthread_join(tid4, NULL);
	pthread_mutex_destroy(&lock);
}


void *myThreadFunction1(void *vargp)
{
	printf("%s\n", "************thread*************");

	int localInt = 0;
	for (int i = 0; i < 1000000; i++)
	{
		globalInt++;
		localInt++;
	}
	printf("Global: %i Local: %i\n",globalInt, localInt);

}

void *myThreadFunction2(void* id){
	
	while(1){
		sem_wait(&mySem);
		printf("%i\n", (int)id);
		sleep(1);
		sem_post(&mySem);
	}
}

void *myThreadFunction3(void *vargp){
	while(running){
		var1++;
		var2 = var1;
	}
}

void *myThreadFunction4(void *vargp){
	for (int i = 0; i < 20; i++){
		printf("Number 1 is %i, number 2 is %i\n",var1,var2);
		usleep(100000);
	}
	running = 0;
}

void *myThreadFunction5(void *vargp){
	while(running){
		pthread_mutex_lock(&lock);
		var1++;
		var2 = var1;
		pthread_mutex_unlock(&lock);
	}
}

void *myThreadFunction6(void *vargp){
	for (int i = 0; i < 20; i++){
		pthread_mutex_lock(&lock);
		printf("Number 1 is %i, number 2 is %i\n",var1,var2);
		pthread_mutex_unlock(&lock);
		usleep(100000);
	}
	running = 0;
}