/*
 * IO driver for Real-time lab IO card communicating with the Butterfly Real-Time Tester
 * Channels are defined from 1 to 3, where 1 is test A on the Butterfly, 2 is B and 3 is C
 * 0V on an IO channel is defined as the logical value 0, while 5V is the logical value 1
 *
 * 2011, Øyvind Netland
 */

#ifndef SEM_H
#define SEM_H

#include <comedilib.h>
#include <native/task.h>

#define IO_DEV 3
#define DI(x) (24-x)
#define DO(x) (16-x)
#define DO_PWR 0

int set_cpu(int cpu_number);
void demo(void *arg);
void synch_task();
void task_init_B();
void task_init_C();

void task1(void *arg);
void task2(void *arg);
void task3(void *arg);
void task4();
void task5();
void print_pri(RT_TASK *task, char *s);
int rt_task_sleep_ms(unsigned long delay);
void busy_wait_ms(unsigned long delay);

#endif /* SEM_h */
