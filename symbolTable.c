#include "symbolTable.h"


/*the function: directly adds a new label node to the beginning of the provided label list,it creates a new node, assigns the given data, and places it at the beginning of the label list.*/
void addlabelnocheck(labelnode *headptr, char *name, int val, int location, int type){
    labelnode head;
    labelnode newnode;
    head = *headptr;
    newnode = malloc(sizeof(struct node));
    newnode->val = val;
    newnode->location = location;
    newnode->type = type;
    newnode->name = malloc(MAX_LABEL);
    strcpy(newnode->name, name);

    if (head == NULL) {
        newnode->next = NULL;
        *headptr = newnode;
    }
    else {
        newnode->next = head;
        *headptr = newnode;
    }
}
/*the function: adds a new label node to the label list if it doesn't already exist, it checks if the label already exists in the list. If not, it calls addlabelnocheck to add the label.*/
void addlable(labelnode *headptr, char *name, int val, int location, int type){
    if (tablecontainslabel(*headptr, name))
        return;
    addlabelnocheck(headptr, name, val, location, type);
}
/*the function: checks if a given label name exists in the label list.*/
int tablecontainslabel(labelnode head, char *lablename){
    labelnode temp;
    temp = head;
    while(temp) {
        if (strcmp(temp->name, lablename) == 0)
            return 1; /* replaced true with 1 */
        temp = temp->next;
    }
    return 0; /* replaced false with 0 */
}
/*the function: fetches the node of a specific label from the label list.*/
labelnode getlabel(labelnode head, char *lablename){
    labelnode temp;
     temp = head;
    while(temp) {
        if (strcmp(temp->name, lablename) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}
/* the function: frees up the memory allocated for the label list.*/
void freetable(labelnode head) {
    labelnode p;
    while (head) {
        p = head;
        head = head->next;
        free(p->name);  /* free the allocated label name */
        free(p);
    }
}


    

