    #ifndef ASSEMBLY_TABLES_H
#define ASSEMBLY_TABLES_H

#define TRUE 1
#define FALSE 0
#define ERR (-1)

/* opcodes */
#define MOV_OPC 0
#define CMP_OPC 1
#define ADD_OPC 2
#define SUB_OPC 3
#define NOT_OPC 4
#define CLR_OPC 5
#define LEA_OPC 6
#define INC_OPC 7
#define DEC_OPC 8
#define JMP_OPC 9
#define BNE_OPC 10
#define RED_OPC 11
#define PRN_OPC 12
#define JSR_OPC 13
#define RTS_OPC 14
#define STOP_OPC 15

/* instructions */
#define INSTRUCTS_SIZE 4
#define DATA_I ".data"
#define STRING_I ".string"
#define ENTRY_I ".entry"
#define EXTERN_I ".extern"

/* registers */
#define REG_SIZE 8
#define R_0 "@r0"
#define R_1 "@r1"
#define R_2 "@r2"
#define R_3 "@r3"
#define R_4 "@r4"
#define R_5 "@r5"
#define R_6 "@r6"
#define R_7 "@r7"

/* operations with 0 operands */
#define OPERATION_0_SIZE 2
#define RTS_O "rts"
#define STOP_O "stop"

/* operations with 1 operand */
#define OPERATION_1_SIZE 9
#define CLR_O "clr"
#define NOT_O "not"
#define INC_O "inc"
#define DEC_O "dec"
#define JMP_O "jmp"
#define BNE_O "bne"
#define JSR_O "jsr"
#define RED_O "red"
#define PRN_O "prn"

/* operations with 2 operands */
#define OPERATION_2_SIZE 5
#define MOV_O "mov"
#define CMP_O "cmp"
#define ADD_O "add"
#define SUB_O "sub"
#define LEA_O "lea"

/* maximum lengths */
#define MAX_LINE_LEN 81 /*including '\n'*/
#define MAX_LABEL_LEN 32 /*including '\0'*/
#define MAX_INSTRCUT_N_OPER 8 /* .string is the longest*/

/* addressing methods */
enum addressing_methods { ADDRESS_IMMEDIATE = 1, ADDRESS_M_DIRECT = 3, ADDRESS_M_REG_DIRECT = 5 }; /* 1, 3, 5 */
/* types */
enum type { REGISTER = 1, INSTRUCT, OPER_0_ARG, OPER_1_ARG, OPER_2_ARG };

/* function prototypes */
int is_in(char *strings_arr[], char *str_to_search, int arr_len);
int is_who(char *str);
int is_saved_word(char *str);
int getOpcode(char *operation);
int getAddressingM(char *argument);

#endif  /*ASSEMBLY_TABLES_H*/

    

