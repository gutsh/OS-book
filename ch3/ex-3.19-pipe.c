#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#define MAX_USEC 999999
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    pid_t pid;
    int pip[2];
    struct timeval s_t, f_t;

    if (argc==1) {
	fprintf(stderr, "Argument must be provided.\n");
	return 1;
    }

    if(pipe(pip)==-1){
	fprintf(stderr, "Couldnt make a pipe.\n");
	return 1;
    }

    pid=fork();
    if (pid<0) {
	fprintf(stderr, "Error occured.\n");
	return 1;
    }
    else if (pid==0) {
	close(pip[READ_END]);
    	struct timeval t;
	gettimeofday(&t, NULL);
	write(pip[WRITE_END],&t,sizeof(struct timeval));
	close(pip[WRITE_END]);
	argv++;
	execvp(*argv, argv);
    }
    else {
	close(pip[WRITE_END]);
	wait(NULL);
	read(pip[READ_END],&s_t,sizeof(struct timeval));
	close(pip[READ_END]);
	gettimeofday(&f_t,NULL);
	int s=(int)(f_t.tv_sec - s_t.tv_sec);
	int ms;
	if (s_t.tv_usec > f_t.tv_usec) {
	    s -= 1; 
	    ms = (int) (MAX_USEC + f_t.tv_usec - s_t.tv_usec);
	}
	else {
	    ms = (int)(f_t.tv_usec - s_t.tv_usec);
	}
	printf("Elapsed time: %i.%i.\n", s, ms);
	return 0;
    }
}

