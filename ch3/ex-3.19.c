#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#define MAX_USEC 999999

int main(int argc, char *argv[])
{
    const int SIZE = 4096;
    const char *name="time";
    pid_t pid;
    int fd;
    struct timeval *ptr, f_t;

    if (argc==1) {
	printf("Argument must be provided.\n");
	return 1;
    }

    fd=shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);
    ptr = (struct timeval *) mmap(0,SIZE,PROT_READ | PROT_WRITE,MAP_SHARED, fd,0);
    pid=fork();
    if (pid<0) {
	printf("Error occured.\n");
	return 1;
    }
    else if (pid==0) {
	argv++;
    	gettimeofday(ptr, NULL);
	execvp(*argv, argv);
    }
    else {
	wait(NULL);
	gettimeofday(&f_t,NULL);
	int s=(int)(f_t.tv_sec - ptr->tv_sec);
	int ms;
	if (ptr->tv_usec > f_t.tv_usec) {
	    s -= 1; 
	    ms = (int) (MAX_USEC + f_t.tv_usec - ptr->tv_usec);
	}
	else {
	    ms = (int)(f_t.tv_usec - ptr->tv_usec);
	}
	printf("Elapsed time: %i.%i.\n", s, ms);
	shm_unlink(name);
	return 0;
    }
}

