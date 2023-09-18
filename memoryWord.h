#ifndef MEMORY_WORD_H
#define MEMORY_WORD_H

typedef long MemWord;

/* A, R, E fields are now represented with 2 bits */
enum field_ARE {
    ABSOLUTE = 0,      /* 00 for A */
    RELOCATABLE = 2,   /* 10 for R */
    EXTERNAL = 1       /* 01 for E */
};

/*start using addresses from 100th*/
#define START_ADDRESS 100
/* Set the maximum data and instruction sizes */
#define MAX_DATA 9999  /* WAS 512 MADE CHANGE HERE $$$$$$ */
#define MAX_INSTRUCT 9999 /* WAS 512 MADE CHANGE HERE $$$$$$ */


/*function declarations:*/
MemWord fir_word(int sour_method, int opcode, int dest_method);
MemWord additional_word(int val, int which_addressing, int is_extern);
MemWord additional_word_registers(int source_reg_num, int dest_reg_num);


#endif /*MEMORY_WORD_H*/

    


    

