#ifndef PROCESSES_H
#define PROCESSES_H

extern int globalInt;
extern int running;
extern int var1;
extern int var2;
void processTestFork();
void processTestVFork();
void pthredTest();
void semaphoresTest();
void mutexTestWithMutex();
void mutexTestWithoutMutex();
void *myThreadFunction1(void *vargp);
void *myThreadFunction2(void* id);
void *myThreadFunction3(void *vargp);
void *myThreadFunction4(void *vargp);
void *myThreadFunction5(void *vargp);
void *myThreadFunction6(void *vargp);
#endif
