#include "io.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>

#define TEST_A 1
#define RESPONSE_A 1
#define TEST_B 2
#define RESPONSE_B 2
#define TEST_C 3
#define RESPONSE_C 3

int set_cpu(int cpu_number)
{
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpu);
}

void test_busywait(void *id_in){
	set_cpu(1);
	int channel = (int)id_in;

	while(1){
		if(!io_read(channel)){
			io_write(channel, 0);
			usleep(5);
			io_write(channel, 1);
		}
	}
}

void timespec_add_us(struct timespec *t, long us)
{
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;
	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000){
		t->tv_nsec = t->tv_nsec - 1000000000;
		t->tv_sec += 1;
	}
}

void test_periodic_thread(void *id_in){
	
	set_cpu(1);

	int channel = (int)id_in;
	struct timespec next;

	clock_gettime(CLOCK_REALTIME, &next);

	while(1){
		// Wait for the next cycle
		timespec_add_us(&next, 10);
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
		
		if(!io_read(channel)){
			io_write(channel, 0);
			usleep(5);
			io_write(channel, 1);
		}
	}
}

void disturbance_thread(){
	set_cpu(1);
	int dummy;
	int i;
	while(1){
		for(i=0; i<1000; i++)
		{
			dummy = i;
		}
	}
}

void assignment_a(){
	pthread_t A, B, C;
	pthread_create(&A, NULL, test_busywait, (void*)TEST_A);
	pthread_create(&B, NULL, test_busywait, (void*)TEST_B);
	pthread_create(&C, NULL, test_busywait, (void*)TEST_C);
	pthread_join(A, NULL);
	pthread_join(B, NULL);
	pthread_join(C, NULL);
}

void assignment_b(){
	pthread_t A, B, C;
	pthread_create(&A, NULL, test_busywait, (void*)TEST_A);
	pthread_create(&B, NULL, test_busywait, (void*)TEST_B);
	pthread_create(&C, NULL, test_busywait, (void*)TEST_C);
	

	pthread_t disturb[10];

	for (int i = 0; i < 10; i++)
	{
		pthread_create(&disturb[i], NULL, disturbance_thread, NULL);
	}

	pthread_join(A, NULL);
	pthread_join(B, NULL);
	pthread_join(C, NULL);
	for (int i = 0; i < 10; i++)
	{
		pthread_join(disturb[i], NULL);
	}
}

void assignment_c_no_disturb(){
	pthread_t A, B, C;
	pthread_create(&A, NULL, test_periodic_thread, (void*)TEST_A);
	pthread_create(&B, NULL, test_periodic_thread, (void*)TEST_B);
	pthread_create(&C, NULL, test_periodic_thread, (void*)TEST_C);
	pthread_join(A, NULL);
	pthread_join(B, NULL);
	pthread_join(C, NULL);
}

void assignment_c_disturb(){
	pthread_t A, B, C;
	pthread_create(&A, NULL, test_periodic_thread, (void*)TEST_A);
	pthread_create(&B, NULL, test_periodic_thread, (void*)TEST_B);
	pthread_create(&C, NULL, test_periodic_thread, (void*)TEST_C);
	

	pthread_t disturb[10];

	for (int i = 0; i < 10; i++)
	{
		pthread_create(&disturb[i], NULL, disturbance_thread, NULL);
	}

	pthread_join(A, NULL);
	pthread_join(B, NULL);
	pthread_join(C, NULL);
	for (int i = 0; i < 10; i++)
	{
		pthread_join(disturb[i], NULL);
	}
}


int main(){
	io_init();

	//assignment_c_no_disturb();
	assignment_c_disturb();
	
	
	return 0;
}