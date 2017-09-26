#include <stdio.h>
#include "memory.h"

int main(int argc, char *argv[])
{
    while(1)
    {
    int x = 1;
    if(allocate(x) < 0) break;
    }
    return 0;
}
