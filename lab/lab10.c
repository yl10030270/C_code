#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define NAMESIZE 20
#define LINESIZE 256

typedef struct{
	char last[NAMESIZE];
	char first[NAMESIZE];
} name;

typedef struct{
	name name;
	int score;
} record;

typedef struct node node;
struct node {
	record data;
	node *next;
};

typedef node *record_list;

/*String to lower case.*/
char * strtolower(char q[]) {
    char *p = q;
    for (; *p; p++)
        *p = tolower(*p);
    return q;
}

/*read and validate a single line input.
 * if it's valid input ,store it in the record that passed in. and return 1.
 * if it's invalid input . print out error message. and return -1. 
 */
int input_record(record *rec) {
    char line[LINESIZE];
    char firstname[LINESIZE];
    char lastname[LINESIZE];
    char scorestr[LINESIZE];
    int score;
    char c;
    int scan;
    int scanscore;
    if (!fgets(line, LINESIZE, stdin)) {
        clearerr(stdin);
        return 0;
    }
    scan = sscanf(line, "%s%s%s", firstname, lastname, scorestr);
    scanscore = sscanf(scorestr, "%d%c", &score, &c);
    if (scan <= 2) {
        fprintf(stderr, "invalid input, missing data.\n");
        return -1;
    }
    if (strlen(firstname) >= NAMESIZE || strlen(lastname) >= NAMESIZE) {
        fprintf(stderr, "invalid input, first/last name too long.\n");
        return -1;
    }
    if (scanscore != 1 || score < 0 || score > 100) {
        fprintf(stderr, "invalid score.\n");
        return -1;
    }
    strcpy(rec->name.first, strtolower(firstname));
    strcpy(rec->name.last, strtolower(lastname));
    rec->score = score;
    return 1;
}

/*print out all the record.
 * a[] - the starting address of the record.
 * n -  the number of records.
 */
void print_records(const record_list a) {
    record_list p;
    for(p = a; p != 0;p = p->next){
    	printf("%s %s %d \n", p->data.name.last, p->data.name.first, p->data.score);
    }
}


/*--------------------------------------------------------------*/


void list_init(record_list *list){
	*list = NULL;
}

int list_insert(record_list *list, const record *rec){
	int cmp;
	node **tracer;
	node *newnode = malloc(sizeof(node));
	if(newnode == 0)
		return 0;
	newnode->data = *rec;
	for(tracer = list; *tracer != 0; tracer = &(*tracer)->next){
		cmp = strcmp((*tracer)->data.name.last, newnode->data.name.last);
		if( cmp > 0)
			break;
		if(cmp == 0)
			if(strcmp((*tracer)->data.name.first, newnode->data.name.first) >= 0)
				break;
	}
	newnode->next = *tracer;
	*tracer = newnode;
	return 1; 						
}

void list_destroy(record_list *list){
	node **q;
	node **p;
	for(p = list; *p != NULL; p = q){
		q = &(*p)->next;
		free(*p);
	}
}

int main(void){
	size_t i;
    record_list head;   
    record buffer;
 	list_init(&head);
 	for(i = 0; i < 6;i++){
 		input_record(&buffer);
 		list_insert(&head,&buffer);
 	}
 	
	print_records(head);			
}
