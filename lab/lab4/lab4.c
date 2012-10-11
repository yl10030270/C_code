#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define NAMESIZE 20
#define LINESIZE 256

int is_valid_name(const char s[]){
	size_t i;
	int length;
	length = strlen(s);
	if( length < 2 || length >= 20){
		printf("problem 1:");
		return 0;
	}
	
	if(!isalpha(s[0]) || !isalpha(s[length - 1])){
		printf("problem 2:");
		return 0;
	}
	
	for(i = 1; i <= length - 2; i++){
		/*printf("length = %d---%c\n",length ,s[i]);*/
		if(!isalpha(s[i]) && s[i] != '-'){
			printf("problem 3:");
			return 0;
		} 
	}
	
	return 1;
}

int get_valid_name(char firstname[], char lastname[]){
	char line[LINESIZE];
	char first[NAMESIZE];
	char last[NAMESIZE];
	int scan;
	
	while(1){
		printf("Please enter your firstname and lastname with space between them.\n");
		
		if(!fgets(line,LINESIZE,stdin)){
			clearerr(stdin);
			return 0;
		} else {
			/*printf("----%s----\n",line);*/
			scan = sscanf(line,"%s%s",first,last);
			/*printf("--scan = %d--\n", scan);*/
			if(scan == 0 || scan == -1){
				return 0;
			}
			if(scan == 1){
				/*printf("problem 4:");*/
				printf("Please enter a valid name.\n");
			} else {
				if(!is_valid_name(first) || !is_valid_name(last)){
					printf("Please enter a valid name.\n");
				} else {
					strncpy(firstname, first, NAMESIZE - 1);
					strncpy(lastname, last, NAMESIZE - 1);
					return 1;
				}
			}
		}
	}
}

int main(void){
	char firstname[NAMESIZE];
	char lastname[NAMESIZE];
	if (get_valid_name(firstname, lastname)){	
		printf("Welcome %s,%s \n", firstname, lastname);
	} else {
		printf("Exit\n");
	}	
}
