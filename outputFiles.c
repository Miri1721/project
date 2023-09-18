    #include "outputFiles.h"

/*function to convert data into a base64 string*/
void to_base64(int num, char *buff){
    char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char holder[3] = {0};
    int index1, index2;

    /*Get the two base64 characters representing the 12-bit number*/
    index1 = (num >> 6) & 0x3F;
    index2 = num & 0x3F;

    holder[0] = base64Chars[index1];
    holder[1] = base64Chars[index2];

    strcpy(buff, holder);
}

/*
 the function creates object file and entry & external if necessary only(if there are symbols of .entry type and data words of .extern type*/
void output_files_creation(MemWord data_img[], MemWord instruct_img[], labelnode sym_table, int ic, int dc, char *obj_name, char *external_name, char *entry_name) {
    FILE *object, *entry, *external;
    labelnode current;
    int entry_flag, external_flag, i;
    /*long for_debug;*/

    entry_flag = external_flag = 0;

    object = fopen(obj_name, "w");
    if (!object) {
        printf("Failed to open the file");
        return;
    }
    fprintf(object, "%d %d\n", ic, dc); /*headline*/
    for (i = 0; i < ic; i++) {
    char word_content[3] = {0};
    /*for_debug = instruct_img[i + START_ADDRESS];*/
    to_base64(instruct_img[i+START_ADDRESS], word_content);
    fprintf(object, "%s\n", word_content);
}

for (i = 0; i < dc; i++) {
    char word_content[3] = {0};
    /*for_debug = data_img[i];*/
    to_base64(data_img[i], word_content);
    fprintf(object, "%s\n", word_content);
}
   /*for (i = 0; i < dc; i++) {
    to_base64(data_img[i], word_content);
    fprintf(object, "%s\n", word_content);
   }*/

    fclose(object);

    external = fopen(external_name, "w");
    if (!external) {
        printf("Failed to open the file");
        return;
    }

    current = sym_table;
    while (current) {
        if (current->type == EXTERNAL_TYPE) {
            external_flag = 1;
            fprintf(external, "%s\t%d\n", current->name, current->val);
        }
        current = current->next;
    }
    fclose(external);
    if (!external_flag) {
        remove(external_name);
    }

    entry = fopen(entry_name, "w");
    if (!entry) {
        printf("Failed to open the file");
        return;
    }

    current = sym_table;
    while (current) {
        if (current->type == ENTRY_TYPE) {
            entry_flag = 1;
            fprintf(entry, "%s\t%d\n", current->name, current->val);
        }
        current = current->next;
    }
    fclose(entry);
    if (!entry_flag) {
        remove(entry_name);
    }
}
  


    

