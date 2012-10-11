#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define LINESIZE 1024

int get_last_first(FILE *fp){
	size_t i;
	size_t commaposition;
	int scan;
	char lastname[20];
	char firstname[20];
	char comma;
	char line[LINESIZE];
	printf("Please inter your name(lastname,firstname).");
	if(!fgets(line,LINESIZE,fp)){
		return 0;
	} else {
		scan = sscanf(line,"%s%c%s",lastname,&comma,firstname);
		if(scan == EOF){
			printf("problem 1:");
			return 1;
		}
		if(scan == 1){
			if(lastname[0] == ','){
				printf("problem 2:");
				return 1;
			}
			for(i = 0; lastname[i] != '\0'; i++){
				if(isalpha (lastname[i])){
					if(lastname[i] == ','){
						commaposition = i;
						lastname[i] = '\0';
					}
				} else {
					printf("problem 3:");
					return 1;
				}
			}
			strncpy(firstname,&lastname[commaposition + 1],19);
			printf("%s,%s",lastname,firstname);
			return 1;
		}
		if(scan == 2){
			printf("%d",comma);
			printf("problem 4:");
			return 1;
		}
		if(scan == 3){
			if(comma != ','){
				printf("%c",comma);
				printf("problem 5:");
				return 1;
			}
			for(i = 0; lastname[i] != '\0'; i++){
				if(!isalpha (lastname[i])){
					printf("problem 6:");
					return 1;
				}
			}
			for(i = 0; firstname[i] != '\0'; i++){
				if(!isalpha (firstname[i])){
					printf("problem 7:");
					return 1;
				}
			}
			printf("%s,%s",lastname,firstname);
			return 1;			
		}		
	}
}

int main(void){
    printf("%d\n",get_last_first(stdin));
}
