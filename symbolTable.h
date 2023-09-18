#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string.h>
#include <stdlib.h>

enum boolean {false=0,true=1};

/* Updated location definitions */
#define CODE_LOC 1
#define DATA_LOC 2
#define NONE_LOC 3

/* Updated type definitions */
#define EXTERNAL_TYPE 1
#define ENTRY_TYPE 2
#define NONE_TYPE 3

/* Label size remains the same */
#define MAX_LABEL 32

struct node{
    char *name;
    int val;
    int location;
    int type;
    struct node *next;
};

typedef struct node *labelnode;

/* function prototypes: */
void addlabelnocheck(labelnode *headptr, char *name, int val, int location, int type);
void addlable(labelnode *headptr, char *name, int val, int location, int type);
int tablecontainslabel(labelnode head, char *lablename);
labelnode getlabel(labelnode head, char *lablename);
void freetable(labelnode head);

#endif /*SYMBOL_TABLE_H*/

    


    

