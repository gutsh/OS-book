#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t z;

    z=fork();

    if (z<0) {
	printf("Fork failed.");
	return 1;
    }
    else if (z==0) {
	exit(0);
    }
    else {
	printf("Zombie-apocalypse! Process #%i is a zombie.\n", z);
	printf("Want to stop it? (just press <Enter>)\n");
	getchar();
	return 0;
    }
}

