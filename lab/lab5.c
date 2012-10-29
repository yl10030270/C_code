#include <stdio.h>
#include <ctype.h>
#include <string.h>

unsigned num_digits(unsigned long n){
	unsigned long i = 1;
	unsigned num = 0;
	while(n/i > 0){
		i *= 10; 
		num++;
	}
	return num;
}

unsigned long reverse(unsigned long n){
	unsigned long numofdigits;
	unsigned long reversednum = 0;
	unsigned long zeros = 1;
	unsigned long tempn = n;
	unsigned long i;
	numofdigits = num_digits(n);
	for(i = 1; i < numofdigits; i++){
		zeros *= 10;
	}
	for(i = 1; i <= numofdigits; i++){		
		reversednum += (tempn % 10 ) * zeros;
		tempn = tempn / 10;
		zeros /= 10;  
	}
	return reversednum;
}

int all_digits_odd(long unsigned n){
	long unsigned tempn = n;
	if(tempn / 10lu == 0){
		return 0;
	}
	
	while(tempn / 10lu != 0){
			if ((tempn % 10lu) % 2lu == 0){
				return 0;
			}
			tempn /= 10lu; 
		}
	return 1;
}

unsigned long reversible(long unsigned n){
	long unsigned i;
	long unsigned tempn;
	long unsigned num = 0;
	for(i = 1lu; i <= n; i++){
		if (i % 10lu == 0){
			continue;
		}
		tempn = i + reverse(i);	
		if(all_digits_odd(tempn)){
			/*printf("%lu , %lu \n", i, reverse(i));*/
			num++;
		}
	}
	return num;
}

int main(void){
	printf("%u \n" , num_digits(0ul));
	printf("%lu \n" , reverse(12ul));
	printf("%lu \n", reversible(1000000ul));
}
