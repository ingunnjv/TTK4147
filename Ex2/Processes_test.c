#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "Processes.h"


int main(int argc, char **argv)
{
	
	processTestFork();
	sleep(1);
	processTestVFork();
	sleep(1);
	pthredTest();
	sleep(1);
	printf("*******************NO MUTEX***********************\n");
	mutexTestWithoutMutex();
	printf("*******************MUTEX***********************\n");
	mutexTestWithMutex();
	printf("*******************semaphoresTest***********************\n");
	semaphoresTest();
	return 0;
}
