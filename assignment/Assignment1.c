#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define NAMESIZE 20
#define LINESIZE 256

int is_valid_name(const char s[]){
	int i;
	
	for(i = 0; s[i] != '\0'; i++){
		if(!isalpha(s[i]) && s[i] != '-'){
			/*printf("problem 3:\n");*/
			return 0;
		}
	}
	
	if( i < 2 || i >= 20){
		/*printf("problem 1:\n");*/
		return 0;
	}
	
	if(!isalpha(s[0]) || !isalpha(s[i - 1])){
		/*printf("problem 2:\n");*/
		return 0;
	}

	return 1;
}

int get_valid_name(char firstname[], char lastname[]){
	char line[LINESIZE];
	char first[NAMESIZE];
	char last[NAMESIZE];
	int scan;
	
	while(1){
		printf("Please enter student's firstname and lastname with space between them:\n");
		
		if(!fgets(line,LINESIZE,stdin)){
			clearerr(stdin);
			return 0;
		}
		
		scan = sscanf(line,"%s%s",first,last);

		if(scan == 0 || scan == -1){
			return 0;
		}
		
		if(scan == 1){
			continue;
		} 
		
		if(!is_valid_name(first) || !is_valid_name(last)){
			continue;
		} 
		
		strncpy(firstname, first, NAMESIZE - 1);
		strncpy(lastname, last, NAMESIZE - 1);
		
		return 1;
	}
}

int is_valid_score(const int score){
	
	if (score >= 0 && score <= 100){
		return 1;
	} else {
		return 0;
	}
}

int get_valid_score(void){

}

void append_records(FILE *fp,char firstname[],char lastname[],const int score){	
	size_t i;
	
	for (i = 0; firstname[i] != '\0'; i++){
		firstname[i] = tolower(firstname[i]);
	}
	
	for (i = 0; lastname[i] != '\0'; i++){
		lastname[i] = tolower(lastname[i]);
	}
    
    if(fseek(fp,0,SEEK_END) != 0){
    	perror("fseek");
    	printf("Append records :failed.\n");
    } else {	
		fprintf(fp,"%-20s%-20s%-4d",firstname,lastname,score);	
		printf("Append records :successful.\n");
	}
}

int get_records(char firstname[], char lastname[], int *score){
	
	if (!get_valid_name(firstname, lastname)){
		printf("abort inter name.\n");
		return 0;
	}
		
	if ((*score = get_valid_score()) == -1){
		printf("abort inter score.\n");
		return 0;
	}
		
	return 1;
}

int display_record_n(FILE *fp, const int n){
	char firstname[NAMESIZE];
	char lastname[NAMESIZE];
    int score;
	
	if(fseek(fp,(n-1)*44,SEEK_SET) != 0){
		fprintf(stderr,"No records to display!\n");
		return 0;
	}
		
	if(fscanf(fp,"%s%s%d",firstname,lastname,&score) != 3){
		fprintf(stderr,"No records to display!\n");
		return 1;
	}
	
	fprintf(stderr,"%s, %s: %d\n",lastname,firstname,score);
	return 1;
}

int display_records_from_n(FILE *fp, const int n){
	char firstname[NAMESIZE];
	char lastname[NAMESIZE];
	int score;
	
	if(fseek(fp,(n-1)*44,SEEK_SET) != 0){
		fprintf(stderr,"No records to display!\n");
		return 0;
	}
	
	if(fscanf(fp,"%s%s%d",firstname,lastname,&score) != 3){
		fprintf(stderr,"No records to display!\n");
		return 1;
	}
	
	while(1){					
		fprintf(stderr,"%s, %s: %d\n",lastname,firstname,score);
		if(fscanf(fp,"%s%s%d",firstname,lastname,&score) != 3){
			break;
		}
	}
	return 1;
} 

int main(int argc, char *argv[]){
	char line[LINESIZE];
	char firstname[NAMESIZE];
	char lastname[NAMESIZE];
	int score;
	int command;
	char c;
	int scan;
	FILE *fp;
	
	if(argc != 2){
		printf("usage:%s filename\n", argv[0]);
		return 1;
	}
	
	if((fp = fopen(argv[1],"a+b")) == 0){
		perror("fopen");
		return 2;
	}
	
	while(1){
		printf("Welcome. Please enter commands.\n");
		
		if(!fgets(line,LINESIZE,stdin)){
			clearerr(stdin);
			break;
		}
		
		sscanf(line,"%s", line);
		scan = sscanf(line,"%d%c", &command ,&c);
		
		if(scan == 0 || scan == -1 || scan == 2){
			continue;
		}
		
		if(command > 0){
			display_record_n(fp,command);
			continue;
		}
		
		if(command < 0){
			display_records_from_n(fp,command*(-1));
			continue;
		}
		
		if(command == 0){
			if (get_records(firstname, lastname, &score)){	
				append_records(fp, firstname, lastname, score);				
				continue;
			}
			continue;		
		}
	}		
	
	if(fclose(fp) != 0){
		perror("fclose");
		return 3;
	}
	
	printf("Exit\n");
	return 0;	
}
