#include "io.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>
#include <rtdk.h>


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
	//set_cpu(1);

	int channel = (int)id_in;
	//struct timespec next;

	//clock_gettime(CLOCK_REALTIME, &next);
	RTIME now, previous;
	rt_task_set_periodic(NULL, TM_NOW, 10000);

	while(1){
		//timespec_add_us(&next, 10);
		//clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
		if(!io_read(channel)){
			io_write(channel, 0);
			usleep(5);
			io_write(channel, 1);
		}
		rt_task_wait_period(NULL);
	}
}

void test(void *id_in){
	printf("Hei");
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


void assignment_A_NoDist()
{	
	int time_delay = 1000*1000;
	RT_TASK A, B, C;
	rt_task_create(&A, "A", 0, 99, T_CPU(1)|T_JOINABLE);
	rt_task_create(&B, "B", 0, 99, T_CPU(1)|T_JOINABLE);
	rt_task_create(&C, "C", 0, 99, T_CPU(1)|T_JOINABLE);

	//rt_task_set_periodic(&A ,TM_NOW, time_delay );
	//rt_task_set_periodic(&B ,TM_NOW, time_delay );
	//rt_task_set_periodic(&C ,TM_NOW, time_delay );

	rt_task_start(&A, &test_periodic_thread, (void*)TEST_A);
	rt_task_start(&B, &test_periodic_thread, (void*)TEST_B);
	rt_task_start(&C, &test_periodic_thread, (void*)TEST_C);

	rt_task_join(&A);
	rt_task_join(&B);
	rt_task_join(&C);
	

	
}

void assignment_A_Dist()
{
	int time_delay = 1000*1000;	

	RT_TASK A, B, C;
	rt_task_create(&A, "A", 0, 99, T_CPU(1)|T_JOINABLE);
	rt_task_create(&B, "B", 0, 99, T_CPU(1)|T_JOINABLE);
	rt_task_create(&C, "C", 0, 99, T_CPU(1)|T_JOINABLE);


	rt_task_start(&A, &test_periodic_thread, (void*)TEST_A);
	rt_task_start(&B, &test_periodic_thread, (void*)TEST_B);
	rt_task_start(&C, &test_periodic_thread, (void*)TEST_C);

	pthread_t disturb[10];

	int i;

	for (i = 0; i < 10; i++)
	{
		pthread_create(&disturb[i], NULL, disturbance_thread, NULL);
	}
	
	rt_task_join(&A);
	rt_task_join(&B);
	rt_task_join(&C);
	

	for (i = 0; i < 10; i++)
	{
		pthread_join(disturb[i], NULL);
	}
	
}

void assignment_A_Dist_2()
{
	int time_delay = 1000*100;	

	RT_TASK A, B, C;
	rt_task_create(&A, "A", 0, 99, T_CPU(1)|T_JOINABLE);
	rt_task_create(&B, "B", 0, 99, T_CPU(1)|T_JOINABLE);
	rt_task_create(&C, "C", 0, 99, T_CPU(1)|T_JOINABLE);


	rt_task_start(&A, &test_periodic_thread, (void*)TEST_A);
	rt_task_start(&B, &test_periodic_thread, (void*)TEST_B);
	rt_task_start(&C, &test_periodic_thread, (void*)TEST_C);

	pthread_t disturb[10];

	int i;

	for (i = 0; i < 10; i++)
	{
		pthread_create(&disturb[i], NULL, disturbance_thread, NULL);
	}
	
	rt_task_join(&A);
	rt_task_join(&B);
	rt_task_join(&C);
	

	for (i = 0; i < 10; i++)
	{
		pthread_join(disturb[i], NULL);
	}
	
}

void assignment_A_Dist_3()
{
	int time_delay = 1000*10;	

	RT_TASK A, B, C;
	rt_task_create(&A, "A", 0, 99, T_CPU(1)|T_JOINABLE);
	rt_task_create(&B, "B", 0, 99, T_CPU(1)|T_JOINABLE);
	rt_task_create(&C, "C", 0, 99, T_CPU(1)|T_JOINABLE);

	rt_task_start(&A, &test_periodic_thread, (void*)TEST_A);
	rt_task_start(&B, &test_periodic_thread, (void*)TEST_B);
	rt_task_start(&C, &test_periodic_thread, (void*)TEST_C);

	pthread_t disturb[10];

	int i;

	for (i = 0; i < 10; i++)
	{
		pthread_create(&disturb[i], NULL, disturbance_thread, NULL);
	}
	
	rt_task_join(&A);
	rt_task_join(&B);
	rt_task_join(&C);
	

	for (i = 0; i < 10; i++)
	{
		pthread_join(disturb[i], NULL);
	}
	
}


int main(){
	mlockall(MCL_CURRENT|MCL_FUTURE);


	io_init();
	//assignment_A_NoDist();
	assignment_A_Dist();
	//assignment_A_Dist_2();
	//assignment_A_Dist_3();
	
	
	return 0;
}