#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define LINESIZE 256
#define BUFSIZE 1024
#define CHECK(PRED) printf("%s...%s\n",(PRED) ? "passed" : "FAILED" , #PRED)
#define TYPE_N  1
#define TYPE_N_TO_END  2
#define TYPE_N_TO_M  3
int range_type(const char s[]){
	size_t i;
	int counter = 0;
	if(s[0] == '\0'){
		return 0;
	}
	for(i = 0; s[i] != '\0' ; i++){
		if(!isdigit(s[i])){
			if(s[i] == ':'){
				counter++;
			}else{
				return 0;
			} 			
		}
	}
	if(counter > 1){
		return 0;
	}
	if(counter == 0){
		return 1;
	}
	if(s[i-1] == ':'){
		return 2;
	}
	return 3;
}

int range_type2(const char s[]){
	int type;
	int pos;
	char b[100];
	int scanvalue;
	char c1;
	char c2;
	
	if(!isdigit(s[0]))
		return 0;
	scanvalue = sscanf(s,"%d%c%d%s",&type,&c1,&pos,b);

	if(scanvalue <= 0 || scanvalue == 4){
		return 0;
	}
	if(scanvalue == 2){
		if(c1 != ':'){
			return 0;
		}
		if(sscanf(s,"%d%c%c",&type,&c1,&c2) == 3)
			return 0;
	}
	if(scanvalue == 3){
		sscanf(s,"%d%c%c",&type,&c1,&c2);
		if(c1 != ':' || !isdigit(c2))
			return 0;
	}	
	
	return scanvalue;		
}

int set_range(const char range[],char to_print[]){
	int type;
	int pos;
	int counter;
	size_t i;
	type = range_type(range);
	if(type == 0){
		return 0;
	}
	
	sscanf(range,"%d",&pos);
	if(pos < 0){
		return 0;
	}
	if(pos >= 1024){
		return 1;
	}
	
	if(type == 1){
		
		to_print[pos - 1] = 1;
		return 1;
	}
	
	if(type == 2){
		for(i = pos - 1;to_print[i] != '\0';i++)
			to_print[i] = 1;
		return 1;
	}
	
	if(type == 3){		
		sscanf(range,"%d:%d",&pos,&counter);
		if(pos < 0 || counter < 0){
			return 0;
		}
		if(pos >= 1024){
			return 1;
		}
		for(i = pos - 1;i < pos + counter - 1; i++){
			to_print[i] = 1;
		}
		return 1;
	}		
}


int main(void){

	char a[BUFSIZE] = {0};	
	
	CHECK(range_type("2") == 1);
	CHECK(range_type("3:") == 2);
	CHECK(range_type("3:5") == 3);
	CHECK(range_type("2s") == 0);
	CHECK(range_type("y:s") == 0);
	CHECK(range_type("") == 0);
	CHECK(range_type("3r5") == 0);
	CHECK(range_type("3:2d") == 0);
	
 	set_range("2", a);
	set_range("5:3",a);
	CHECK(a[0] == 0);
	CHECK(a[1] == 1);
	CHECK(a[1] == a[4]);
	CHECK(a[5] == 1);
	CHECK(a[5] == a[6]);
	CHECK(a[7] == 0);
	CHECK(a[1000] == 0);	
}
