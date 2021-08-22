#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int avg, max, min;
void *avg_r(void*);
void *min_r(void*);
void *max_r(void*);

int main(int argc, char *argv[])
{
       	pthread_t avg_th, min_th, max_th;
	int *temp,*nums=calloc(argc,sizeof(int));
	temp=nums;
	for(int i=1;i<argc;i++){
		*temp=atoi(argv[i]);
		temp++;
	}
	*temp=0;
	pthread_create(&avg_th,NULL,avg_r,nums);
	pthread_create(&min_th,NULL,min_r,nums);
	pthread_create(&max_th,NULL,max_r,nums);
	pthread_join(avg_th,NULL);
	pthread_join(min_th,NULL);
	pthread_join(max_th,NULL);
	printf("\nAvg: %i, max: %i, min: %i\n",avg,max,min);
	free(nums);
	return 0;
}

void *avg_r(void *p)
{
	int *nums=(int*)p, count=0;
	avg=0;
	while(*nums!=0){
		avg+=*nums;
		count++;
		nums++;
	}
	avg/=count;
}

void *max_r(void *p)
{
	int *nums=(int*)p;
	max=*nums;
	while(*nums!=0){
		if(*nums>max) max=*nums;
		nums++;
	}
}

void *min_r(void *p)
{
	int *nums=(int*)p;
	min=*nums;
	while(*nums!=0){
		if(*nums<min) min=*nums;
		nums++;
	}
}

