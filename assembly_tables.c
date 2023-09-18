#include <string.h>
#include "assembly_tables.h"

char *registers[REG_SIZE] = {R_0, R_1, R_2, R_3, R_4, R_5, R_6, R_7};

char *instructs[INSTRUCTS_SIZE] = {DATA_I, STRING_I, ENTRY_I, EXTERN_I};

char *operations_0[OPERATION_0_SIZE] = {RTS_O, STOP_O};
char *operations_1[OPERATION_1_SIZE] = {CLR_O, NOT_O, INC_O, DEC_O, JMP_O, BNE_O, JSR_O, RED_O, PRN_O};
char *operations_2[OPERATION_2_SIZE] = {MOV_O, CMP_O, ADD_O, SUB_O, LEA_O};


/* Check if a string exists in a given array. Return index + 1 if found, FALSE otherwise. */
int is_in(char *strings_arr[], char *str_to_search, int arr_len) {
    int index = 0;

    while (index < arr_len) {
        if (!strcmp(strings_arr[index], str_to_search)) {
            return index + 1;
        }
        index++;
    }

    return FALSE;
}

/* Determine the type of the given string. */
int is_who(char *str) {
    if (is_in(registers, str, REG_SIZE)) {
        return REGISTER;
    }

    if (is_in(instructs, str, INSTRUCTS_SIZE)) {
        return INSTRUCT;
    }

    if (is_in(operations_0, str, OPERATION_0_SIZE)) {
        return OPER_0_ARG;
    }

    if (is_in(operations_1, str, OPERATION_1_SIZE)) {
        return OPER_1_ARG;
    }

    if (is_in(operations_2, str, OPERATION_2_SIZE)) {
        return OPER_2_ARG;
    }

    return FALSE;
}

/* Check if a string is a reserved assembly word. */
int is_saved_word(char *str) {
    return is_who(str) == FALSE ? FALSE : TRUE;
}

/* Retrieve the opcode for a given operation. */
int getOpcode(char *operation) {
    /*strcmp return 0 if the strings are the same*/
    if (!strcmp(operation, MOV_O)) {return MOV_OPC;}
    if (!strcmp(operation, CMP_O)) {return CMP_OPC;}
    if (!strcmp(operation, ADD_O)) {return ADD_OPC;}
    if (!strcmp(operation, SUB_O)) {return SUB_OPC;}
    if (!strcmp(operation, LEA_O)) {return LEA_OPC;}
    if (!strcmp(operation, CLR_O)) {return CLR_OPC;}
    if (!strcmp(operation, NOT_O)) {return NOT_OPC;}
    if (!strcmp(operation, INC_O)) {return INC_OPC;}
    if (!strcmp(operation, DEC_O)) {return DEC_OPC;}
    if (!strcmp(operation, JMP_O)) {return JMP_OPC;}
    if (!strcmp(operation, BNE_O)) {return BNE_OPC;}
    if (!strcmp(operation, JSR_O)) {return JSR_OPC;}
    if (!strcmp(operation, RED_O)) {return RED_OPC;}
    if (!strcmp(operation, PRN_O)) {return PRN_OPC;}
    if (!strcmp(operation, RTS_O)) {return RTS_OPC;}
    if (!strcmp(operation, STOP_O)) {return STOP_OPC;}

    /*in case none are valid*/
    return ERR;
}

/*Determine the addressing method of a given argument.*/
int getAddressingM(char *argument) {
    /* Check for immediate addressing */
    if ((argument[0] >= '0' && argument[0] <= '9') || argument[0] == '-') {
        return ADDRESS_IMMEDIATE;
    }

    /* Check for direct register addressing */
    if (argument[0] == '@' && argument[1] == 'r' && 
        argument[2] >= '0' && argument[2] <= '7' && 
        argument[3] == '\0') {
        return ADDRESS_M_REG_DIRECT;
    }

    /* If none of the above, it's direct addressing */
    return ADDRESS_M_DIRECT;
}


    

