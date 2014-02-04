#include <stdio.h>

//Global jump table

#define numEntries 5
int (*jumptable[numEntries])(int arg1,int arg2);
//forward declarations

int dispatch(int tableEntry, int arg1, int arg2);
void setup ();
int add(int arg1, int arg2);
int subtract(int arg1, int arg2);
int multiple(int arg1, int arg2);
int divide(int arg1, int arg2);
int compare(int arg1, int arg2);

void setup()
{
   jumptable[0] = add;
   jumptable[1] = subtract;
   jumptable[2] = multiple;
   jumptable[3] = divide;
   jumptable[4] = compare;
}

int add(int arg1, int arg2)
{
    return arg1 + arg2;
}
int subtract(int arg1, int arg2)
{
    return arg1 - arg2;
}
int multiple(int arg1, int arg2)
{
    return arg1 * arg2;
}

int divide(int arg1, int arg2)
{
    return arg1 / arg2;
}

int compare(int arg1, int arg2)
{
    return arg1 > arg2 ? 1 : 0;

}

int dispatch( int tableEntry, int arg1, int arg2)
{
    int (* func)(int arg1, int arg2);
    if((tableEntry < 0) || (tableEntry >= numEntries))
    {
       fprintf(stderr,"Table entry not found\n");
       return -1;
     }

    //return jumptable[tableEntry]();
    func = jumptable[tableEntry];
    return func(arg1, arg2);
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
    printf("Result : %d \n" , dispatch(table_entry, arg1, arg2));
    return 0;
}
