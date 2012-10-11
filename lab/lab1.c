#include <stdio.h>
#include <stddef.h>
#define CHECK(PRED) printf("%s...%s\n",(PRED) ? "passed" : "FAILED" , #PRED)
int array_min(const int a[], size_t n){
	int min = a[0];
	size_t i;
	for( i = 0; i < n; i++ ){
		if (min > a[i]) {
			min = a[i];
		}
	}
	return min;
}

size_t array_index_of_first_min(const int a[], size_t n){
	size_t indexoffirst = 0;
	size_t i;
	int min = a[0];
	for( i = 0; i < n; i++ ){
		if (min > a[i]) {
			min = a[i];
			indexoffirst = i;
		}
	}
	return indexoffirst;
}

size_t array_index_of_last_min(const int a[], size_t n){
	size_t indexoflast = 0;
	size_t i;
	int min = a[0];
	for( i = 0; i < n; i++ ){
		if (min >= a[i]) {
			min = a[i];
			indexoflast = i;
		}
	}
	return indexoflast;
}

int array_count_min(const int a[],size_t n){
	size_t i;
	int min = a[0];
	int count = 0;
	for( i = 0; i < n; i++ ){
		if (min == a[i]){
			count ++;
		}
		if (min > a[i]) {
			min = a[i];
			count = 1;
		}	
	}
	return count;
}

int main(void){
	
	int a[] = {-1, -2, 5, -2, 4,-2 ,7, 4};
	int b[] = {0,1,2,3,4,5,4,3,2,1,0};
	int c[] = {-1,2, 3, 4, -2, -2, -4};
	
	CHECK(array_min(a,8) == -2);
	CHECK(array_index_of_first_min(a,8) == 1);
	CHECK(array_index_of_last_min(a,8) == 5);
	CHECK(array_count_min(a,8) == 3);	
	
	CHECK(array_min(b,11) == 0);
	CHECK(array_index_of_first_min(b,11) == 0);
	CHECK(array_index_of_last_min(b,11) == 10);
	CHECK(array_count_min(b,11) == 2);	
	
	CHECK(array_min(c,7) == -4);
	CHECK(array_index_of_first_min(c,7) == 6);
	CHECK(array_index_of_last_min(c,7) == 6);
	CHECK(array_count_min(c,7) == 1);
}