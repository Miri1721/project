#include "inputFileHandle.h"
#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"
#include "outputFiles.h"

/*initializes a string that will represent a given file name + a given extension*/
#define init_(name, file_name, extension){\
name = malloc(strlen(file_name) + strlen(extension) + 1);\
strcpy(name, file_name);\
strcat(name, extension);\
}

/*frees everything that needs to be freed in every iteration at main*/
#define freeiter() {\
free(as_name);\
free(obj_name);\
free(externals_name);\
free(entry_name);\
freetable(symbol_table);\
}

#define MAX_EXTENSION 4 /*.am + '\0'*/

int main(int argc, const char *argv[]){
    char *as_name, *obj_name, *externals_name, *entry_name;
    int file_index, iteration_check;

    as_name = obj_name = externals_name = entry_name = NULL;

    if (argc < 2){
        printf("assembly files have not provided, provide the files names as command line arguments without extension");
        exit(0);
    }
    /*each iteration of loop handling a single file*/
    for (file_index = 1; file_index < argc; file_index++){
        FILE *fd;
        int ic, dc;
        labelnode symbol_table = NULL;
        MemWord data_img[MAX_DATA] = {0}, instruct_img[MAX_INSTRUCT] = {0};

        ic = 100;
        dc = 0;
         /* Preprocess the file to generate .am file with macros withdrawn */
         preprocessFile(argv[file_index]);
        as_name = malloc(strlen(argv[file_index] + MAX_EXTENSION + 1)); /*+1 for '\0'*/
        strcpy(as_name, argv[file_index]); /*without .am*/

        if(!(fd = fopen(strcat(as_name, ".am"), "r"))){
            printf("Cannot open file \"%s\", moving to the next file\n", as_name);
            freeiter()
            continue;
        }

        /*strings holding output files names*/
        init_(obj_name, argv[file_index], ".ob")
        init_(externals_name, argv[file_index], ".ext")
        init_(entry_name, argv[file_index], ".ent")

         putchar('\n');
        printf("SCANNING FILE \"%s\" ...\n", as_name);
        iteration_check = firstPass(fd, data_img, instruct_img, &symbol_table, &ic, &dc);
        printf("==FIRST PASS==\n");
        printf("printed AFTER first pass From here \n");

        /* End of debug print statements */
        if (iteration_check == ERR){
            freeiter()
            continue;
        }

        rewind(fd);
        iteration_check = secondPass(fd, instruct_img, &symbol_table);
        
        if (iteration_check == ERR){
            freeiter()
            continue;
        }
        fclose(fd); /*done with the file*/

        printf("The file is scanned in its entirety without errors.\nStarting the creation of the output files \"%s\", \"%s\" & \"%s\"...\n", obj_name, externals_name, entry_name);
        ic -= START_ADDRESS;
        output_files_creation(data_img, instruct_img, symbol_table, ic, dc, obj_name, externals_name, entry_name);
        printf("==OUTPUT FILES== \n");
        printf("Files creation was successfully completed !\n");

        freeiter()
    }
    putchar('\n');
    return 0;
}


    

