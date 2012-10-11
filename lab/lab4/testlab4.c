#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define LINESIZE 1024

int get_last_first(FILE *fp){
	size_t i;
	int commacounter = 0;
	size_t commaposition = 0;
	int scan;
	char lastname[LINESIZE];
	char firstname[LINESIZE];
	char comma[LINESIZE];
	char line[LINESIZE];
	printf("Please inter your name(lastname,firstname).");
	if(!fgets(line,LINESIZE,fp)){
		return 0;
	} else {
		scan = sscanf(line,"%s%s%s",lastname,comma,firstname);
		if(scan == EOF){
			printf("problem 1:");
			return 0;
		}
		
		if(scan == 1){
			for(i = 0; lastname[i] != '\0'; i++){
				if(isalpha (lastname[i]) || lastname[i] == ',' || lastname[i] == '-'){
					if(lastname[i] == ','){
						commaposition = i;
						commacounter++;
						lastname[i] = '\0';
					}
				} else {
					printf("problem 3:");
					return -1;
				}
			}
			if(commaposition == 0 || commacounter > 1){
				printf("problem 2:");
				return -1;
			}
			strncpy(firstname,&lastname[commaposition + 1],LINESIZE - 1);
			printf("%s,%s\n",lastname,firstname);
			return 1;
		}
		
		if(scan == 2){
			if(comma[0] != ','){
				printf("problem 4:");
				return -1;
			} else {
				for(i = 0; lastname[i] != '\0'; i++){
					if(!isalpha (lastname[i]) && lastname[i] != '-'){
						printf("problem 7:");
						return -1;
					}
				}
				for(i = 1; comma[i] != '\0'; i++){
					if(!isalpha (comma[i]) && comma[i] != '-'){
						printf("problem 8:");
						return -1;
					}
				}
				strncpy(firstname,&comma[1],LINESIZE - 1);
				printf("%s,%s\n",lastname,firstname);
				return 1;
			}

		}	
		if(scan == 3){
			if(comma[0] != ','){
				printf("problem 9:");
				return -1;
			}
			if(strlen(comma) == 1){
				for(i = 0; lastname[i] != '\0'; i++){
					if(!isalpha (lastname[i]) && lastname[i] != '-'){
						printf("problem 10:");
						return -1;
					}
				}
				for(i = 0; firstname[i] != '\0'; i++){
					if(!isalpha (firstname[i]) && firstname[i] != '-'){
						printf("problem 11:");
						return -1;
					}
				}
				printf("%s,%s",lastname,firstname);
				return 1;
			} else {
				for(i = 0; lastname[i] != '\0'; i++){
					if(!isalpha (lastname[i]) && lastname[i] != '-'){
						printf("problem 12:");
						return -1;
					}
				}
				for(i = 1; comma[i] != '\0'; i++){
					if(!isalpha (comma[i] && comma[i] != '-')){
						printf("problem 13:");
						return -1;
					}
				}
				strncpy(firstname,&comma[1],LINESIZE - 1);
				printf("%s,%s\n",lastname,firstname);
				return 1;
			}
							
		}		
	}
}

int main(void){
    printf("%d\n",get_last_first(stdin));
}
