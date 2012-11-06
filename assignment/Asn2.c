#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#define BUFSIZE 1024
#define CHECK(PRED) printf("%s...%s\n",(PRED) ? "passed" : "FAILED" , #PRED)
/*return value for range type N*/
#define TYPE_N  1
/*return value for range type N:*/
#define TYPE_N_TO_END  2
/*return value for range type N:M*/
#define TYPE_N_TO_M  3
/*command option type for C*/
#define C_OPTION 1
/*command option type for f*/
#define F_OPTION 2
/*default delimiter for f option*/
#define DELIMITER ','

/*print out error message and usage.*/
void usage(const char *s) {
    const char us[] = "usage: cut -cLIST/-fLIST [-n] [--] FilePath";
    printf("%s\n", s);
    printf("%s\n", us);
}

/*test if the commond is valid.
If it is not valid, return 0.
If it is valid, set up option type ,negation and file list.
If the option is c type ,return 1;
If the option is f type ,return 2;
 */
int get_valid_com(const int n, const char *ar[], char *option, int *negate, int *files) {
    int ccount = 0;
    int fcount = 0;
    int retval = 0;
    int i;
    for (i = 1; i < n && ar[i][0] == '-' && ar[i][1] != '-'; i++) {
        if (ar[i][1] == 'c') {
            ccount++;
            strcpy(option, ar[i]);
            retval = C_OPTION;
        } else if (ar[i][1] == 'f') {
            fcount++;
            strcpy(option, ar[i]);
            retval = F_OPTION;
        } else if (ar[i][1] == 'n') {
            (*negate)++;
        } else {
            usage("unknowed option.");
            return 0;
        }
    }
    if (ccount > 1) {
        usage("-c option specified more than once.");
        return 0;
    }
    if (fcount > 1) {
        usage("-f option specified more than once.");
    }
    if (ccount > 0 && fcount > 0) {
        usage("can’t specify both -c and -f options.");
        return 0;
    }
    if (i == n) {
        usage("no files specified.");
        return 0;
    }
    if (ccount == 0 && fcount == 0) {
        usage("either the -c or -f option (but not both) must be specified.");
        return 0;
    }
    *files = ((ar[i][1] == '-') ? i + 1 : i);
    return retval;
}

/*test the range type,
return 0 for invalid range type.
return 1 for range type N
return 2 for range type N:
return 3 for range type N:M
 */
int range_type(const char s[]) {
    size_t i;
    int counter = 0;
    if (!isdigit(s[0])) {
        return 0;
    }
    for (i = 0; s[i] != '\0'; i++) {
        if (!isdigit(s[i])) {
            if (s[i] == ':') {
                counter++;
            } else {
                return 0;
            }
        }
    }
    if (counter > 1) {
        return 0;
    }
    if (counter == 0) {
        return 1;
    }
    if (s[i - 1] == ':') {
        return 2;
    }
    return 3;
}

int set_each_range(const char range[], int to_print[]) {
    int type;
    int pos;
    int counter;
    int i;
    type = range_type(range);
    if (type == 0) {
        printf("invalid option range %s\n", range);
        return 0;
    }

    sscanf(range, "%d:%d", &pos, &counter);

    if (pos >= 1024) {
        return 1;
    }
    if (type == 1) {
        to_print[pos - 1] = 1;
        return 1;
    }
    if (type == 2) {
        for (i = pos - 1; i < 1024; i++) {
            to_print[i] = 1;
        }
        return 1;
    }
    for (i = pos - 1; i < pos + counter - 1 && i < 1024; i++) {
        to_print[i] = 1;
    }
    return 1;
}

char *get_range(char *range, char *option) {
    while (*option != ',' && *option != '\0')
        *range++ = *option++;
    *range = '\0';
    return (*option == ',') ? option + 1 : option;
}

int set_range(char *option, int *to_print, const int negate) {
    char range[BUFSIZE];
    size_t i;
    option = option + 2;

    do {
        option = get_range(range, option);
        if (!set_each_range(range, to_print)) {
            return 0;
        }
    } while (*option != '\0');

    if (negate > 0) {
        for (i = 0; i < BUFSIZE; i++) {
            to_print[i] = (to_print[i] - 1) * -1;
        }
    }
    return 1;
}

void display_c(FILE *fp, const int *to_print) {
    char line[BUFSIZE];
    size_t i;
    while (1) {
        if (!fgets(line, BUFSIZE, fp)) {
            clearerr(fp);
            break;
        }
        for (i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
            if (to_print[i] == 1) {
                putchar(line[i]);
            }
        }
        printf("\n");
    }
}

void display_f(FILE *fp, const int *to_print) {
    char line[BUFSIZE];
    size_t i;
    char *start;
    char *end;
    int comma;
    while (1) {
        if (!fgets(line, BUFSIZE, fp)) {
            clearerr(fp);
            break;
        }
        start = line;
        end = strchr(start, DELIMITER);
        comma = 0;
        for (i = 0; end != NULL; i++) {
            if (to_print[i] == 1) {
                if (comma == 1) {
                    putchar(DELIMITER);
                }
                while (start < end)
                    putchar(*start++);
                comma = 1;
            } else {
                start = end;
            }
            start++;
            end = strchr(start, DELIMITER);
        }
        if (to_print[i] == 1) {
            putchar(DELIMITER);
            while (*start != '\n' && *start != '\0') {
                putchar(*start++);
            }
        }
        printf("\n");
    }
}

int main(const int argc, const char *argv[]) {
    char option[BUFSIZE];
    int to_print[BUFSIZE];
    int negate = 0;
    int fpos;
    int type;
    int i;
    FILE *fp;

    type = get_valid_com(argc, argv, option, &negate, &fpos);
    if (type == 0)
        return 1;
    if (!set_range(option, to_print, negate)) {
        return 2;
    }
    for (i = fpos; i < argc; i++) {
        if ((fp = fopen(argv[i], "r+b")) == 0) {
            perror("fopen");
            return 3;
        }
        if (type == 1)
            display_c(fp, to_print);
        else
            display_f(fp, to_print);
        if (fclose(fp) != 0) {
            perror("fclose");
            return 4;
        }
    }
    return 0;
}

