#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "assembly_tables.h"
#include "memoryWord.h"
#include "symbolTable.h"
#include "inputFileHandle.h"

#define BITS_IN_WORD 12
#define START_ADDRESS 100

/*macro to get register number after the '@' and the 'r' chars*/
#define getRegNum(regstr) (regstr[2] - '0')



/*function declarations:*/
int InstructlineDeciphering(char *ptrlineafterinstruct, char *instruct, char *opt_label, MemWord image[], int *dc, labelnode *symbolstable);
int operationDeciphering(char *ptrafterline, char *operation, char *opt_label, MemWord image[], int *ic, labelnode *symbolstable);
int firstPass(FILE *asfile, MemWord data_img[], MemWord instruct_img[], labelnode *symbolstable, int *ic, int *dc);

#endif /*FIRST_PASS_H*/

    

