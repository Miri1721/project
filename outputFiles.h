#ifndef OUTPUT_FILES_H
#define OUTPUT_FILES_H

#include <stdio.h>
#include <stdlib.h>
#include "memoryWord.h"
#include "symbolTable.h"


/*function declarations:*/
void to_base64(int num, char *buff);
void output_files_creation(MemWord data_img[], MemWord instruct_img[], labelnode sym_table, int ic, int dc, char *obj_name, char *external_name, char *entry_name);

  
#endif   /*OUTPUT_FILES_H*/

    


    

