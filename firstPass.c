    #include "firstPass.h"


int linenum = 0;

/*firstPass over ps.am*/
int firstPass(FILE *asfile, MemWord data_img[], MemWord instruct_img[], labelnode *symbolstable, int *ic, int *dc) {
    labelnode current;
    int complete = TRUE;
    char line[MAX_LINE_LEN] = {0};
    char op_or_instruct[MAX_INSTRCUT_N_OPER], label[MAX_LABEL_LEN];

    linenum = 0;

    while (scan_line(line, asfile) != EOF) {
        int instruct_type;
        char *ptr_after_label, *ptr_after_op_instruct;

        memset(op_or_instruct, 0, MAX_INSTRCUT_N_OPER);
        memset(label, 0, MAX_LABEL_LEN);

        linenum++;
        /* skipping white spaces and ; */
        if (*line == '\0' || line[0] == ';') { continue; }

        ptr_after_label = holds_label(line, label);

        if (ptr_after_label == (char *)ERR) {
            complete = FALSE;
            continue;
        }

        instruct_type = get_inst_or_op(ptr_after_label, op_or_instruct);

        if (instruct_type == ERR) {
            complete = FALSE;
            continue;
        }

        ptr_after_op_instruct = ptr_after_label + strlen(op_or_instruct);
        ptr_after_op_instruct = remove_space(ptr_after_op_instruct);

        if (instruct_type == INST) {
            if ((InstructlineDeciphering(ptr_after_op_instruct, op_or_instruct, label, data_img, dc, symbolstable)) == ERR) {
                complete = FALSE;
                continue;
            }
            /*printf("InstructDeciphering, %ld \n", data_img[(*dc) - 1]);*/
        } else if (instruct_type == OPER) {
            if ((operationDeciphering(ptr_after_op_instruct, op_or_instruct, label, instruct_img, ic, symbolstable)) == ERR) {
                complete = FALSE;
                continue;
            }
        }

    }
    /* increase all labels marked with data attribute (for second pass) */
    current = *symbolstable;
    while (current) {
        if (current->location == DATA_LOC) {
            current->val += *ic;
        }
        current = current->next;
    }
    printf("printed From here \n");
    printf("operationDeciphering, %ld, %ld, %ld \n", instruct_img[100], instruct_img[101], instruct_img[102]);

    printf("\nSymbol Table after First Pass:\n");
    printf("---------------------------------\n");
    printf("Name\t\tAddress\t\tData/Code\n");
    current = *symbolstable;
    while (current) {
        printf("%s\t\t%d\t\t%s\n", current->name, current->val, current->location == DATA_LOC ? "Data" : "Code");
        current = current->next;
    }

    return (!complete) ? ERR : TRUE;

}

/*handling instruction command*/
int InstructlineDeciphering(char *ptrlineafterinstruct, char *instruct, char *opt_label, MemWord image[], int *dc, labelnode *symbolstable) {
    if (!holds_ex_text(ptrlineafterinstruct)){
        print_err("Instruction needs an operand!")
        return ERR;
    }

    /*check whether the instruction is .string or .data*/
    if (!(strcmp(instruct, STRING_I)) || !(strcmp(instruct, DATA_I))){
        if (tablecontainslabel(*symbolstable, opt_label)) {
            print_err(" The label already declared and saved in the symbol table");
            return ERR;
        }
        /*check whether there is a label, adding it to the symbol table*/
        if (opt_label && opt_label[0] != '\0'){
            addlable(symbolstable, opt_label, *dc, DATA_LOC, NONE_TYPE);
        }
        /*if it's .string*/
        if (!(strcmp(instruct, STRING_I))){
            int i = 0;
            char *string = get_string(ptrlineafterinstruct);
            /* if the parameter is invalid*/
            if (string == (char *) ERR) {
                return ERR;
            }
            /*insert the string into the image, char by char*/
            while(i < strlen(string)){
                image[i + (*dc)] = string[i];
                i++;
            }
            (*dc) += i;
            image[(*dc)++] = 0; /*COMMENTED OUT FOR THE ADDRESSED TO BE CORRECT IN SYMBOL TABLE*/
        }
        /* else it's .data*/
        else {
            long num;
            char *numstring, *linecopy;
            unsigned mask12 = 0, i;
            /*int with all first 11 bits (0-11) turned on, others off*/
            for (i = 0; i < BITS_IN_WORD; ++i){
                mask12 <<= 1;
                mask12 |= 1;
            }

            linecopy = malloc(strlen(ptrlineafterinstruct) + 1);
            /*creates a copy*/
            strcpy(linecopy, ptrlineafterinstruct);

            if ((linecopy[strlen(linecopy) - 1] == ',') || (linecopy[0] == ',')){
                print_err("Invalid comma located");
                return ERR;
            }

            if (holds_dou_commas(linecopy)){
                print_err("located two consecutive commas (,,) , which isn't valid!")
                return ERR;
            }

            numstring = strtok(linecopy, ",");

            while (numstring) {
                int sign;
                numstring = remove_space(numstring);
                /*check validation of the number*/
                if (!valid_num(numstring)){
                    free(linecopy);
                    return ERR;
                }
                /*move the sign bit from the last one to the 11th bit*/
                num = atoi(numstring);
                /*get bit of sign*/
                sign = (num & (1 << (sizeof(int) * 8 - 8))) ? 1 : 0;
                /*turn off all bits after the 11th*/
                num &= mask12;
                /*put the sign bit in place - at the 11th bit*/
                num |= sign << (BITS_IN_WORD - 1);
                /*add the number to image*/
                image[(*dc)++] = num;
                /*move to the next number*/
                numstring = strtok(NULL, ",");
            }
            free(linecopy);
        }
    }
    /* if label */
    else {
        char *checklabel;
        if (!ptrlineafterinstruct){
            print_err("no label after instruction")
            return ERR;
        }
        checklabel = malloc(strlen(ptrlineafterinstruct + 2));
        strcpy(checklabel, ptrlineafterinstruct);
        strcat(checklabel, ":c");
        if (holds_label(checklabel, ptrlineafterinstruct) == (char *) ERR){
            free(checklabel);
            return ERR;
        }
        /* extern */
        if (strcmp(instruct, EXTERN_I) == 0){
            labelnode temp;

            if ((temp = (getlabel(*symbolstable, ptrlineafterinstruct))) && temp->type != EXTERNAL_TYPE){
                print_err(" a declared label cannot be external ! ")
                free(checklabel);
                return ERR;
            } else {
                /*insert as external label with value 0 into the symbol table*/
                addlable(symbolstable, ptrlineafterinstruct, 0, NONE_LOC, EXTERNAL_TYPE);
            }
        }
        free(checklabel);
    }
    return TRUE;
}

/*handling operation command*/
int operationDeciphering(char *ptrafteroperand, char *operation, char *opt_label, MemWord image[], int *ic, labelnode *symbolstable) {
    int numargs;
    int temp_ic = (*ic);
    /*long for_debug_1, for_debug_2, for_debug_3;*/
    /*first deal with label*/
    if (opt_label && *opt_label != '\0'){
        /*there is a label*/
        if (tablecontainslabel(*symbolstable, opt_label)){
            print_err("label already declared")
            return ERR;
        }
        /*label does not exist in symbol table, so we insert it */
        addlable(symbolstable, opt_label, *ic, CODE_LOC, NONE_TYPE);
    }
    /* move after operation */
    numargs = how_many_args(operation); /*get number of arguments the operation takes*/

    /* case for operations which take no additional arguments, which are: rts, stop*/
    if (numargs == 0){
        if (holds_ex_text(ptrafteroperand)){
            print_err("extra text after operation, which takes no arguments")
        }
        /*source and destination addressing methods are 0 because no arguments*/
        image[temp_ic] = fir_word(0, getOpcode(operation), 0); /*sourceAddressing, Opcode, destAddressing */
        temp_ic++;
        (*ic)++;
    } else if (numargs >= 1){
        int addrM1, addrM2, state; /*Addressing methods for arguments, 'state' for checking the state of get_argument*/
        char arg1[MAX_LABEL_LEN], arg2[MAX_LABEL_LEN];

        addrM1 = addrM2 = 0;
        if (ptrafteroperand == NULL){
            print_err("operation takes at least one operand")
            return ERR;
        }
        if (holds_dou_commas(ptrafteroperand)){
            print_err("there's two consecutive commas (,,) which isn't allowed!")
            return ERR;
        }

        /*get first argument into arg1*/
        state = get_argument(ptrafteroperand, arg1);
        if (state == ERR){
            return ERR;
        }
        /*get arg1 Addressing method*/
        addrM1 = getAddressingM(arg1);

        if (numargs == 1){
            if (holds_ex_text(ptrafteroperand + strlen(arg1)) && (ptrafteroperand[strlen(ptrafteroperand) - 1] != '\n')){
                print_err("there's extra text after the command, case is: number of arguments == 1")
                return ERR;
            }
            /* check whether the Addressing method of the argument fits with the operation */
            if (valid_addressing(operation, -1, addrM1) == FALSE){
                print_err("Validation of addressing method went unsuccessfully. ")
                return ERR;
            }
            /*argument is alright and valid as well as the addressing method*/
        }
        /* two arguments */
        else {
            /* skip first argument because we already took argument 1 and it's addressing method*/
            ptrafteroperand += strlen(arg1);
            ptrafteroperand = remove_space(ptrafteroperand);

            if (ptrafteroperand == NULL){
                print_err("the operation takes two arguments, only one found.")
                return ERR;
            }
            /* skip the comma between the arguments*/
            if (ptrafteroperand[0] == ','){
                ptrafteroperand++;
            }
            /*get second argument into arg2*/
            state = get_argument(ptrafteroperand, arg2);
            if (state == ERR){
                return ERR;
            }
            /*validate addressing methods of BOTH arguments */
            addrM2 = getAddressingM(arg2);
            if (valid_addressing(operation, addrM1, addrM2) == FALSE){
                return ERR;
            }
            ptrafteroperand = remove_space(ptrafteroperand);
            if (holds_ex_text(ptrafteroperand + strlen(arg2))) {
                print_err("there's extra text after the command, case is: number of arguments == 2")
                return ERR;
            }

            /*both arguments alright and valid as well as the addressing methods*/
        }

        /* starting the process of building the memory words*/

        /*first handling the case of two registers*/
        if((addrM1 == ADDRESS_M_REG_DIRECT) && (addrM2 == ADDRESS_M_REG_DIRECT)){
            int reg1 = getRegNum(arg1);
            int reg2 = getRegNum(arg2);
            /*build first word*/
            image[temp_ic] = fir_word(addrM1, getOpcode(operation), addrM2);
            
            temp_ic++;
            (*ic)++;
            /*build second word*/
            image[temp_ic] = additional_word_registers(reg1, reg2);
            
            temp_ic++;
            (*ic)++;
        }
        else {
            /*arg1 is the destination argument in case we have only one argument*/
            if (numargs == 1) {
                /* build the first word */
                image[temp_ic] = fir_word(0, getOpcode(operation), addrM1);
                
                temp_ic++;
                (*ic)++;
                /*build additional words*/
                /*if the argument is register, we build another memory word with [0-1 A,R,E], [2-6 destination register number] [7-11 0] */
                if (addrM1 == ADDRESS_M_REG_DIRECT){
                    int regNum = getRegNum(arg1);
                    /*no source - 0*/
                    image[temp_ic] = additional_word_registers(0, regNum);
                    
                    temp_ic++;
                    (*ic)++;
                }
                else if (addrM1 == ADDRESS_IMMEDIATE){
                    /*it's a number, we should create additional word with the value (and A,R,E)*/
                    int num;
                    char *numstr = arg1;
                    /*validate the number, check if the string represent it only contains digits*/
                    if (!valid_num(numstr)){
                        return ERR;
                    }
                    /*get the number as int*/
                    num = atoi(numstr);
                    /*build word*/
                    image[temp_ic] = additional_word(num, ADDRESS_IMMEDIATE, 0);
                    
                    temp_ic++;
                    (*ic)++;
                }
                /*it's ADDRESS_DIRECT*/
                else {
                    /* here we are going to save space for later (second pass), because in ADDRESS_DIRECT we deal with labels addresses*/
                    image[temp_ic] = 0;
                    
                    temp_ic++;
                    (*ic)++;
                }

            }
            /*here, with two arguments, arg1 is the source, arg2 is the destination*/
            else if (numargs == 2){
                /*build the first word*/
                image[temp_ic] = fir_word(addrM1, getOpcode(operation), addrM2);
               
                temp_ic++;
                (*ic)++;

                /*handle the first argument - arg1, source argument*/
                if (addrM1 == ADDRESS_M_REG_DIRECT){
                    int regNum = getRegNum(arg1);
                    /*register is source now*/
                    image[temp_ic] = additional_word_registers(regNum, 0);
                    
                    temp_ic++;
                    (*ic)++;
                }
                else if (addrM1 == ADDRESS_IMMEDIATE){
                    /*it's a number, we should create additional word with the value (and A,R,E)*/
                    int num;
                    char *numstr = arg1;
                    /*validate the number, check if the string represent it only contains digits*/
                    if (!valid_num(numstr)){
                        return ERR;
                    }
                    /*get the number as int*/
                    num = atoi(numstr);
                    /*build word*/
                    image[temp_ic] = additional_word(num, ADDRESS_IMMEDIATE, 0);
                    
                    temp_ic++;
                    (*ic)++;
                }
                /* ADDRESS_DIRECT */
                else {
                    /*save space for second pass*/
                    image[temp_ic] = 0;
                    
                    temp_ic++;
                    (*ic)++;
                }

                /*handle the second argument - arg2, destination argument.*/
                if (addrM2 == ADDRESS_M_REG_DIRECT){
                    int regNum = getRegNum(arg2);
                    /*register is destination now*/
                    image[temp_ic] = additional_word_registers(0, regNum);
                   
                    temp_ic++;
                    (*ic)++;
                }
                else if (addrM2 == ADDRESS_IMMEDIATE){
                    int num;
                    char *numstr = arg2;
                    if (!valid_num(numstr)){
                        return ERR;
                    }
                    num = atoi(numstr);
                    /*build word*/
                    image[temp_ic] = additional_word(num, ADDRESS_IMMEDIATE, 0);
                    
                    temp_ic++;
                    (*ic)++;
                }
                /* ADDRESS_DIRECT */
                else {
                    /*save space for second pass*/
                    image[temp_ic] = 0;
                   
                    temp_ic++;
                    (*ic)++;
                }
            }
        }
    }
    /*every thing went through successfully*/
    return TRUE;
}


    

