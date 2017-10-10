#include "semaphore.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>
#include <sys/mman.h>
//#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <rtdk.h>
#include <native/mutex.h>


#define TEST_A 1
#define RESPONSE_A 1
#define TEST_B 2
#define RESPONSE_B 2
#define TEST_C 3
#define RESPONSE_C 3

#define NTASKS 3
#define SUPER_HIGH 53
#define HIGH 52 /* high priority */
#define MID 51 /* medium priority */
#define LOW 50  /* low priority */

RT_SEM mysync, mysem;
RT_TASK A, B, C, SYNC, D, E;
RT_MUTEX mymutex, mutexA, mutexB;

int useMutex = 1;

int set_cpu(int cpu_number)
{
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpu);
}


void demo(void *arg)
{
	int num=(int)arg;
	rt_printf("Task  : %d\n",num);
	rt_sem_p(&mysync,TM_INFINITE);
	rt_printf("End Task  : %d\n",num);
}

void task1(void *arg)
{
	//int num=(int)arg;
	//rt_printf("Task  : %d\n",num);
	rt_sem_p(&mysync,TM_INFINITE);

	//lock resource
	if(useMutex)
		rt_mutex_acquire(&mymutex, TM_INFINITE);
	else
		rt_sem_p(&mysem, TM_INFINITE);
	print_pri(&A, "LOW priority task locked resource\n");
	busy_wait_ms(3);
	// release
	print_pri(&A, "LOW priority task finished busy-wait\n");
	if(useMutex)
		rt_mutex_release(&mymutex);
	else
		rt_sem_v(&mysem);
	print_pri(&A, "LOW priority task released resource\n");
	//rt_printf("End Task  : %d\n",num);
}

void task2(void *arg)
{

	rt_sem_p(&mysync,TM_INFINITE);
	rt_task_sleep_ms(1);
	print_pri(&B, "MID priority task busy-waiting...\n");
	busy_wait_ms(5);
	print_pri(&B, "MID priority task finished\n");
   
}

void task3(void *arg)
{

	rt_sem_p(&mysync,TM_INFINITE);

	rt_task_sleep_ms(2);
	if(useMutex)
		rt_mutex_acquire(&mymutex, TM_INFINITE);
	else
		rt_sem_p(&mysem, TM_INFINITE);
	print_pri(&C, "HIGH priority task locked resource\n");
	busy_wait_ms(2);
	// release
	if(useMutex)
		rt_mutex_release(&mymutex);
	else
		rt_sem_v(&mysem);
	print_pri(&C, "HIGH priority task released resource\n");

}

void task4()
{

	rt_sem_p(&mysync,TM_INFINITE);

	rt_mutex_acquire(&mutexA, TM_INFINITE);
	//rt_task_set_priority(&A, SUPER_HIGH);
	print_pri(&A, "LOW priority task locked resource A\n");
	
	busy_wait_ms(3);

	rt_mutex_acquire(&mutexB, TM_INFINITE);
	print_pri(&A, "LOW priority task locked resource B\n");
	busy_wait_ms(3);

	// release
	print_pri(&A, "LOW priority task releases resource B\n");
	rt_mutex_release(&mutexB);
	print_pri(&A, "LOW priority task releases resource A\n");
	rt_mutex_release(&mutexA);
	//rt_task_set_priority(&A, LOW);
	//rt_task_sleep(1);
	print_pri(&A, "LOW priority task busy-waiting...\n");
	busy_wait_ms(1);
	print_pri(&A, "LOW priority task finished\n");

}

void task5()
{

	rt_sem_p(&mysync,TM_INFINITE);

	rt_task_sleep_ms(1);
	rt_mutex_acquire(&mutexB, TM_INFINITE);
	//rt_task_set_priority(&B, SUPER_HIGH);
	print_pri(&B, "HIGH priority task locked resource B\n");

	busy_wait_ms(1);

	rt_mutex_acquire(&mutexA, TM_INFINITE);
	print_pri(&B, "HIGH priority task locked resource A\n");
	busy_wait_ms(2);
	// release
	print_pri(&B, "HIGH priority task releases resource A\n");
	rt_mutex_release(&mutexA);
	print_pri(&B, "HIGH priority task releases resource B\n");
	rt_mutex_release(&mutexB);
	//rt_task_set_priority(&A, HIGH);
	//rt_task_sleep(1);
	print_pri(&A, "HIGH priority task busy-waiting...\n");
	busy_wait_ms(1);
	print_pri(&B, "HIGH priority task finished\n");

}


void synch_task()
{
	usleep(100000);
	rt_printf("wake up all tasks\n");
 	rt_sem_broadcast(&mysync);
 	usleep(100000);
 	rt_printf("Delete semaphore\n");
 	rt_sem_delete(&mysync);
}

void task_init_B()
{
	if(useMutex)
		rt_mutex_create(&mymutex, "MyMutex");
	else
		rt_sem_create(&mysem,"MySem",1,S_FIFO);

	rt_sem_create(&mysync,"MySemaphore",0,S_FIFO);
	
	
	rt_task_create(&A, "A", 0, LOW, T_CPU(1)|T_JOINABLE);
	rt_task_create(&B, "B", 0, MID, T_CPU(1)|T_JOINABLE);
	rt_task_create(&C, "C", 0, HIGH, T_CPU(1)|T_JOINABLE);
	rt_task_create(&SYNC, "Synchronization", 0, SUPER_HIGH, T_CPU(1)|T_JOINABLE);

	rt_task_start(&A, &task1, (void*)TEST_A);
	rt_task_start(&B, &task2, (void*)TEST_B);
	rt_task_start(&C, &task3, (void*)TEST_C);
	rt_task_start(&SYNC, &synch_task, NULL);

	rt_task_join(&A);
	rt_task_join(&B);
	rt_task_join(&C);

	rt_sem_delete(&mysem);
	rt_mutex_delete(&mymutex);
}

void task_init_C(){
	rt_mutex_create(&mutexA, "MyMutexA");
	rt_mutex_create(&mutexB, "MyMutexB");
	rt_sem_create(&mysync,"MySemaphore",0,S_FIFO);

	rt_task_create(&A, "A", 0, LOW, T_CPU(1)|T_JOINABLE);
	rt_task_create(&B, "B", 0, HIGH, T_CPU(1)|T_JOINABLE);
	rt_task_create(&SYNC, "Synchronization", 0, SUPER_HIGH, T_CPU(1)|T_JOINABLE);

	rt_task_start(&A, &task4, NULL);
	rt_task_start(&B, &task5, NULL);
	rt_task_start(&SYNC, &synch_task, NULL);

	rt_task_join(&A);
	rt_task_join(&B);

	rt_mutex_delete(&mutexA);
	rt_mutex_delete(&mutexB);
}


/*
The base and current priority of the running task is printed together
with a provided message.
*/
void print_pri(RT_TASK *task, char *s)
{
	struct rt_task_info temp;
	rt_task_inquire(task, &temp);
	rt_printf("b:%i c:%i ", temp.bprio, temp.cprio);
	rt_printf(s);
}

int rt_task_sleep_ms(unsigned long delay)
{
	return rt_task_sleep(1000*1000*delay);
}

void busy_wait_ms(unsigned long delay)
{
	unsigned long count = 0;
	while (count <= delay*10){
		rt_timer_spin(1000*100);
		count++;
	}
}


int main(){
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT|MCL_FUTURE);

	task_init_C();
	
	
	return 0;
}