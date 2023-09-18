#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include "firstPass.h"
#include "memoryWord.h"
#include "symbolTable.h"
#include "assembly_tables.h"


/*function prototypes/declarations:*/
int secondPass(FILE *fd, MemWord instruct_img[], labelnode *sym_table);
int oper_handle_II(char *line_after_op, char *operation, char *optional_label, MemWord image[], int free_index, labelnode *sym_table);



#endif /*SECOND_PASS_H*/

    

