#include <stdio.h>

//Global jump table

#define numEntries 5
int (*jumptable[numEntries])(void);
//forward declarations

int dispatch(int tableEntry);
void setup ();
int add();
int subtract();
int multiple();
int divide();
int compare();
void push(int , int);
void pop(int*, int*);

//stack
int space1;
int space2;

void push(int arg1, int arg2)
{
    space1 = arg1;
    space2 = arg2;
}

void pop(int* arg1, int* arg2)
{
    *arg1 = space1;
    *arg2 = space2;
}

void setup()
{
   jumptable[0] = add;
   jumptable[1] = subtract;
   jumptable[2] = multiple;
   jumptable[3] = divide;
   jumptable[4] = compare;
}

int add()
{
    int arg1, arg2;
    pop(&arg1, &arg2);
    return arg1 + arg2;
}
int subtract()
{
    int arg1, arg2;
    pop(&arg1, &arg2);    
    return arg1 - arg2;
}
int multiple()
{
	int arg1, arg2;
	pop(&arg1, &arg2);    
	return arg1 * arg2;
}

int divide()
{
	int arg1, arg2;
        pop(&arg1, &arg2);    
	return arg1 / arg2;
}

int compare()
{
	int arg1, arg2;
        pop(&arg1, &arg2);    
	return arg1 > arg2 ? 1 : 0;

}

int dispatch( int tableEntry)
{
    int (* func)();
    if((tableEntry < 0) || (tableEntry >= numEntries))
    {
       fprintf(stderr,"Table entry not found\n");
       return -1;
     }

    //return jumptable[tableEntry]();
    func = jumptable[tableEntry];  
    return func();
}

int main(int argc, char * argv[])
{
    int table_entry, arg1, arg2;
    setup();
    printf("Please enter the table entry number :\n");
    scanf("%d", &table_entry);
    printf("Please enter the first argument :\n");
    scanf("%d", &arg1);
    printf("Please enter the second argument :\n");
    scanf("%d", &arg2);
    push(arg1,arg2);
    printf("Result : %d \n" , dispatch(table_entry));
    return 0;
}
