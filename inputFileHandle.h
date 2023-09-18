#ifndef INPUT_FILE_HANDLE_H
#define INPUT_FILE_HANDLE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "assembly_tables.h"

extern char *operations_0[], *operations_1[], *operations_2[], *instructs[], *registers[];

extern int linenum;

/*type*/
#define OPER 5
#define INST 6

#define MAX_QUOTES 2
#define STRING_COLON '\t'

/*macros*/
#define print_err(string) printf("Error, line %d: " string ".\n", linenum);

/*return TRUE if the string holds chars other then spaces, and FALSE if the string is holding spaces only*/
#define holds_ex_text(string) (!(string) || !(remove_space(string))) ? FALSE : TRUE


/*return FALSE if the string not containing two commas (",,"), TRUE if it does*/
#define holds_dou_commas(string) (!(strstr(string, ",,"))) ? FALSE : TRUE


/*function prototypes:*/
int how_many_times(char *string, char c);
int only_letters_digits(char *string);
int get_argument(char *line_after_op, char *store_in);
int get_inst_or_op(char *line, char *store_in);
char *holds_label(char *line, char *store_label_in);
char *get_string(char *string);
int scan_line(char *store_in, FILE * fd);
char *remove_space (char *string);
int holds_space(char *string);
int valid_addressing (char *operation, int src_add_method, int dest_add_method);
int valid_num (char *num);
int how_many_args(char *operation);


#endif /*INPUT_FILE_HANDLE_H*/


    

