#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define LINESIZE 256

int get_int(const char prompt[], int min , int max, int *pnum){
	char line[LINESIZE];
	int scan;
	int temp;
	while(1){
		printf("%s\n",prompt);
		
		if(!fgets(line,LINESIZE,stdin)){
			clearerr(stdin);
			return 0;
		}
			
		scan = sscanf(line,"%d", &temp);
		
		if(scan == 0 || scan == -1){
			continue;
		}
		
		if(!is_valid_int(min,max,temp)){
			continue;
		}
		
		*pnum = temp;
		return 1;		
	}
}

int is_valid_int(const int min,const int max,const int num){
	return (num >= min && num <= max);
}

int menu(const char *choices[], const char *prompt){
	int i = 0;
	int command;
	while(choices[i] != 0){
		printf("%d. %s\n",i + 1,choices[i]);
		i++;
	}
	if(get_int(prompt,1,3,&command)){
		return command;
	}
	return 0;		
}

int main(int argc, char *argv[]){
	int command;
	int a;
	char prompt[] = "Please enter an integer.";
	const char *choices[] = {"Add record","Search records","Quit",0};
	while((command = menu(choices,prompt)) != 3 && command != 0){
		printf("the choise is %d.\n\n",command);
	}
	return 0;
}
