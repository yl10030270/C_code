#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define NAMESIZE 20
#define LINESIZE 256
#define BLOCK 2

typedef struct {
    char last[NAMESIZE]; /* last name */
    char first[NAMESIZE]; /* first name */
} name;

typedef struct {
    name name;
    int score;
} record;

typedef struct {
    record **data; /*points to dynamic array of pointers */
    size_t nalloc; /*number of pointers allocated */
    size_t nused; /*number of pointers used */
} record_list;



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
    size_t i;
    for (i = 0; i < a.nused; i++) {
        printf("%s %s %d \n", a.data[i]->name.last, a.data[i]->name.first, a.data[i]->score);
    }
}


/*--------------------------------------------------------------*/



void list_init(record_list *list){
	list->data = 0;
    list->nalloc = 0;
    list->nused = 0;
}

void list_destroy(record_list *list){
	 size_t i;
	 for (i = 0; i < list->nused; i++) {
        free(list->data[i]);
    }
    free(list->data);
}

int list_insert(record_list *list, const record *rec){
    record **tmp;
    if (list->nalloc == list->nused) {
        tmp = realloc(list->data, (list->nalloc + BLOCK) * sizeof (record *));
        if (tmp == 0) {
            fprintf(stderr, "unable to realloc record_list");
            return 0;
        }
        list->data = tmp;
        list->nalloc += BLOCK;
    }

    list->data[list->nused] = malloc(sizeof (record));
    if (list->data[list->nused] == 0) {
        fprintf(stderr, "unable to malloc list->data[%d]", (int) list->nused);
        return 0;
    }
    strcpy(list->data[list->nused]->name.first, rec->name.first);
    strcpy(list->data[list->nused]->name.last, rec->name.last);
    list->data[list->nused]-> score = rec->score;
    list->nused++;
    return 1;
}

int cmp_poss_s(const void *p, const void *q) {
    const record *pp = *(record **)p;
    const record *qq = *(record **)q;
    printf("looking at %s %s & %s %s\n", pp->name.last,pp->name.first ,qq->name.last, qq->name.first);
    printf("looking at %d & %d\n", pp->score, qq->score);
    return (pp->score - qq->score);
}

int main(void) {
    size_t i;
    record_list rl;   
    record buffer;
 	list_init(&rl);
 	
 	for(i = 0; i < 6;i++){
 		input_record(&buffer);
 		/*strcpy(testarray[i].name.first, buffer.name.first);
   		strcpy(testarray[i].name.last, buffer.name.last);
   		testarray[i].score = buffer.score;
   		printf("%d = %d\n",i, testarray[i].score);*/
 		list_insert(&rl,&buffer);
 	}
 	
 	printf("------------------%d----------------------\n", (int)sizeof(record *));
    /*for (i = 0; i < 6; i++) {
        printf("%s %s %d \n", testarray[i].name.last, testarray[i].name.first, testarray[i].score);
    }*/
    print_records(rl);
 	printf("------------------%d----------------------\n" ,(int)sizeof(record));
 	printf("---%s--%s--%d---------\n",rl.data[0]->name.last, rl.data[0]->name.first, rl.data[0]->score);
 	qsort(rl.data, rl.nused, sizeof(record*), cmp_poss_s);  
 	print_records(rl);
 	/*for (i = 0; i < 6; i++) {
        printf("%s %s %d \n", testarray[i].name.last, testarray[i].name.first, testarray[i].score);
    }*/
 	return 1;
}
