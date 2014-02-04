#include <stdio.h>

//Global jump table

#define numEntries 3
int (*jumptable[numEntries])(int a);
//forward declarations

int dispatch(int tableEntry);
int file_load(int a);
int file_save(int a);
int memory_load(int a);
void setup();

void setup()
{
   jumptable[0] =file_load;
   jumptable[1] = file_save;
   jumptable[2] = memory_load;
}

int file_load(int a)
{
    printf("Loading file\n");
    return 0;
}
int file_save(int a)
{
    printf("Saving file\n");
    return 0;
}
int memory_load(int a)
{
    printf("Loading memory");
    return 0;
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
    int table_entry;
    setup();
    printf("Please enter the table entry number :");
    scanf("%d", &table_entry);
    dispatch(table_entry);
    return 0;
}
