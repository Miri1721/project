#include "memoryWord.h"
#include "assembly_tables.h"

/*the purpose of the function is to create the first memory word based on the given source method, opcode, and destination method.*/
MemWord fir_word(int sour_method, int opcode, int dest_method) {
    MemWord word;
    word = 0;
    word |= ABSOLUTE; /* all first words are absolute, bits 0-1*/
    word |= (dest_method << 2); /*bits 2-4*/
    word |= (opcode << 5); /*bits 5-8*/
    word |= (sour_method << 9); /*bits 9-11*/
    return word;
}
/* the purpose is to generate an additional memory word based on a value, its addressing method, and whether it is external.*/
MemWord additional_word(int val, int which_addressing, int is_extern) {
    MemWord word;
    word = 0;

    /* all extern addresses are 1*/
    if (is_extern) {
        word |= EXTERNAL;
        return word;
    }

    word = (val << 2); /* bits 2-11 */
    word |= which_addressing == ADDRESS_M_DIRECT ? RELOCATABLE : ABSOLUTE;
    return word;
}
/*the purpose of the function is to construct a memory word for operations involving two registers.*/
MemWord additional_word_registers(int source_reg_num, int dest_reg_num){
    MemWord word;
    word = 0;
    word |= ABSOLUTE; /*bits 0-1*/
    word |= (dest_reg_num << 2); /* bits 2-6 */
    word |= (source_reg_num << 7); /*bits 7-11*/
    return word;
}


    

