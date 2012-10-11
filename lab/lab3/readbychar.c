#include <stdio.h>
#include <ctype.h>


void squeeze_spaces(void){
	int c;
	int spacecount = 0;
	while((c = getchar()) != EOF){
		if(isspace(c)){
			spacecount ++;
			if(spacecount <= 1){
			   putchar(c);
			}
		}else{
			putchar(c);
			spacecount = 0;			
		}	
	}
}

void format_words(void){
	int c;
	int counter = 1;
	while ((c = getchar()) != EOF){
		if(isspace(c) || c == '\'' || c == '\"'){
			counter++;
			putchar(c);		
		}else{
			if(counter = 1){
				putchar(toupper(c));
				counter = 0;
			}else{
				putchar(tolower(c));					
			}
		}
	}
		
}

int main(void){
	squeeze_spaces();
}
