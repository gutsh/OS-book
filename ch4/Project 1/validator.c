#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define COLUMNS 9
#define ROWS 9
#define SUBGRS 9

void *validator(void*);
char nums[10][10];
int col_res[COLUMNS], row_res[ROWS], subgr_res[SUBGRS];
typedef struct {
	int row;
	int col;
} area_t;

int main(void)
{
	int input=0;
	pthread_t col_valds[COLUMNS], row_valds[ROWS], subgr_valds[SUBGRS];
	area_t *cols, *rows, *subgrs, *temp;
	char inp[10];
	cols=(area_t*)calloc(COLUMNS,sizeof(area_t));
	rows=(area_t*)calloc(ROWS,sizeof(area_t));
	subgrs=(area_t*)calloc(SUBGRS,sizeof(area_t));
	//initializing column areas
	temp=cols;
	for(int i=1;i<=COLUMNS;i++){
		temp->row=0;
		temp->col=i;
		temp++;
	}
	
	//initializing row areas
	temp=rows;
	for(int i=1;i<=ROWS;i++){
		temp->row=i;
		temp->col=0;
		temp++;
	}
	
	//initializing subgrid areas
	temp=subgrs;
	for(int i=1;i<=3;i++){
		for(int j=1;j<=3;j++){
			temp->row=i;
			temp->col=j;
			temp++;
		}
	}
	while(input<9){
		printf("Enter line %i.\n",input+1);
		int in=scanf("%s",inp);
		if(in==EOF || strlen(inp)!=9 || strspn(inp,"123456789")<9){
			printf("Invalid input for line %i, try again.\n",input+1);
			continue;
		}
		for(int i=0;i<COLUMNS;i++) nums[input][i]=inp[i];
		nums[input][9]='\0';
		input++;
	}
	for(int i=0;i<=COLUMNS;i++) nums[9][i]='\0';
	//starting columns validation
	for(int i=0;i!=COLUMNS;i++)
		pthread_create(&col_valds[i],NULL,validator,cols+i);
	//starting rows validation
	for(int i=0;i!=ROWS;i++)
		pthread_create(&row_valds[i],NULL,validator,rows+i);
	//starting subgrids validation
	for(int i=0;i!=SUBGRS;i++)
		pthread_create(&subgr_valds[i],NULL,validator,subgrs+i);
	//waiting for the threads to finish validation
	for(int i=0;i!=COLUMNS;i++)
		pthread_join(col_valds[i],NULL);
	for(int i=0;i!=ROWS;i++)
		pthread_join(row_valds[i],NULL);
	for(int i=0;i!=SUBGRS;i++)
		pthread_join(subgr_valds[i],NULL);
	for(int i=0;i!=COLUMNS;i++)
		if(col_res[i]==0) printf("Incorrect column #%i.\n",i+1);
	for(int i=0;i!=ROWS;i++)
		if(row_res[i]==0) printf("Incorrect row #%i.\n",i+1);
	for(int i=0;i!=SUBGRS;i++)
		if(subgr_res[i]==0) printf("Incorrect subgr #(%i,%i).\n",i/3+1,i%3+1);
	free(rows);free(cols);free(subgrs);
	return 0;
}

void *validator(void *area_ptr)
{
	char cor_c[10]={"123456789\0"}, tmp[10], *fc;
	int col_idx=((area_t*)area_ptr)->col, row_idx=((area_t*)area_ptr)->row;
	if(col_idx==0){
		row_idx--;
		for(int i=0;i<=COLUMNS;i++)
			tmp[i]=nums[row_idx][i];
		for(int i=0;i<9;i++){
			fc=strpbrk(cor_c,tmp);
			if(fc==NULL) continue;
			*fc='0';
		}
		if(strspn(cor_c,"0")==9) row_res[row_idx]=1;
		printf("Validation process for the row #%i is finishing.\n",row_idx+1);
	}
	else if (row_idx==0){
		col_idx--;
		for(int i=0;i<=ROWS;i++)
			tmp[i]=nums[i][col_idx];
		for(int i=0;i<9;i++){
			fc=strpbrk(cor_c,tmp);
			if(fc==NULL) continue;
			*fc='0';
		}
		if(strspn(cor_c,"0")==9) col_res[col_idx]=1;
		printf("Validation process for the column #%i is finishing.\n",col_idx+1);
	}
	else{
		int _col_idx=(col_idx-1)*3, _row_idx=(row_idx-1)*3;
		for(int i=0;i<9;i++){
			if(i%3==0 && i!=0){
				_row_idx++;
				_col_idx-=3;
			}
			tmp[i]=nums[_row_idx][_col_idx];
			_col_idx++;
			}	
		tmp[9]='\0';
		for(int i=0;i<9;i++){
			fc=strpbrk(cor_c,tmp);
			if(fc==NULL) continue;
			*fc='0';
		}
		if(strspn(cor_c,"0")==9) subgr_res[(row_idx-1)*3+(col_idx-1)]=1;
		printf("Validation process for the grid #(%i,%i) is finishing.\n",row_idx,col_idx);
		}
}

