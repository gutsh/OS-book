#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#define MIN_PID 300
#define MAX_PID 310
#define PIDS_RANGE MAX_PID-MIN_PID+1

int allocate_map(void);
int allocate_pid(void);
void release_pid(int);
void print_stat(void);

_Bool *pids, *temp;

int main(void)
{
    printf("Result of memory allocation is %i\n",allocate_map());
    print_stat();
    allocate_pid();
    print_stat();
    allocate_pid();
    print_stat();
    release_pid(300);
    print_stat();
    free(pids);
    return 0;
}

void print_stat()
{
    temp=pids;
    for(int i=MIN_PID; i<=MAX_PID; ++i)
    {
	printf("%i-th number: %i.\n", i, *temp);
	temp++;
    }
}

int allocate_map()
{
    if (MAX_PID > INT_MAX || MIN_PID < 0)
    {
	fprintf(stderr, "Minimum or maximum bound exceeded.\n");
	return -1;
    }
    pids=(_Bool *)calloc((size_t)PIDS_RANGE,sizeof(_Bool));
    return 1;
}

int allocate_pid()
{
    temp=pids;
    for(int i=MIN_PID; i<=MAX_PID; ++i)
    {
	if (*temp==0)
	{
	    *temp=1;
	    return i;
	}
	temp++;
    }
    return -1;
}

void release_pid(int pid)
{
    if (pid > MAX_PID || pid < MIN_PID) {return;}
    temp=pids;
    temp+=pid-MIN_PID;
    *temp=0;
}

