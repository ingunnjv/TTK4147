#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

int allocate(int value) {
    int *ptr = NULL;
    ptr = malloc(1024*1024*sizeof(char));
    if (ptr == NULL)
    {
    	perror("Error: ");
    	return -1;
    }
    *ptr = value;
    printf("test of allocated memory: %i\n", ptr);
    return 0;
}
