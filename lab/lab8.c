#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define NAMESIZE 20
#define LINESIZE 256

typedef struct name name;
struct name{
	char last[NAMESIZE];
	char first[NAMESIZE];
};

typedef struct record record;
struct record{
	name name;
	int score;
};

int input_record(struct record *rec){
	char line[LINESIZE];
	char firstname[LINESIZE];
	char lastname[LINESIZE];
	int score;
	int scan;
	if(!fgets(line,LINESIZE,stdin)){
		clearerr(stdin);
		return 0;
	}
	scan = sscanf(line,"%s%s%d",firstname,lastname,&score);
	if(scan <= 2)
		return -1;
	if(strlen(firstname) >= NAMESIZE || strlen(lastname) >= NAMESIZE)
		return -1;
	if(score < 0 || score > 100)
		return -1;
	strcpy(rec->name.first, firstname);
	strcpy(rec->name.last, lastname);
	rec->score = score;
	return 1;
}

void print_records(const record a[], size_t n){
	size_t i;	
	for(i = 0; i <n ;i++){
		printf("%s %s %d \n", a[i].name.first , a[i].name.last, a[i].score);
	}
}

int main(void){
	
	record a[3];
	size_t i;
	for(i = 0; i < 3;i++){
		if(input_record(&a[i]) != 1)			
			printf("invalid input.\n");
	}	
	printf("-------------------------\n");	
	print_records(a,3);		
}
