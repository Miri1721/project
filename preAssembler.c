#include "preAssembler.h"


/*
preProcessFile function preprocesses the provided '.am' file, by doing the following: it identifies the macro definitions and stored the macros, replaces their occurence in the text(code), the result is written into a new '.as' file. the functions also handles memory allocations and deallocations needed for the process:
*/
void preprocessFile(const char* fileName){
    Macro macros[MAX_MACROS];
    int macroCount = 0;
    char line[MAX_LINE_LENGTH];
    FILE *inputFile = NULL, *outputFile = NULL;
    char inputFileName[FILENAME_MAX], outputFileName[FILENAME_MAX];
    char modifiedLine[MAX_LINE_LENGTH];
    int i;
    char *tempLine;
    /*prepare input and output file names:*/
    strcpy(inputFileName, fileName);
    if(strcmp(&inputFileName[strlen(inputFileName)-3], ".as") != 0){
        strcat(inputFileName, ".as");
    }
    strcpy(outputFileName, fileName);
    if(strcmp(&outputFileName[strlen(outputFileName)-3], ".as") == 0){
        outputFileName[strlen(outputFileName)-3]='\0';
    }
    strcat(outputFileName, ".am");

    /*Open input and output files*/
    inputFile = fopen(inputFileName, "r");
    if(!inputFile){
        perror(inputFileName);
        return;
    }
    outputFile = fopen(outputFileName, "w");
    if(!outputFile){
        perror(outputFileName);
        fclose(inputFile);
        return;
    }

    printf("=== PROCESSING ===\n");

    while (fgets(line, sizeof(line), inputFile)) {
        printf("processing line: %s", line);

        /*If this is a macro definition line, store the macro*/
        if (isMacroDefinitionStart(line)) {
            if (macroCount >= MAX_MACROS) {
                printf("Exceeded maximum number of macros!\n");
                exit(1);
            }

            /*Extract and store macro name and body*/
            macros[macroCount].name = getMacroName(line);
            macros[macroCount].body = (char*)malloc(sizeof(char) * MAX_LINE_LENGTH * MAX_LINES_IN_MACRO);
            if(!macros[macroCount].body){
                printf("error: failed to allocate memory for macro body\n");
                return;
            }
            macros[macroCount].body[0]='\0';

            while (fgets(line, sizeof(line), inputFile) && !isMacroDefinitionEnd(line)) {
    /* Remove leading whitespaces from the line before storing it */	tempLine = line;
    		while (isspace(*tempLine)) {
        		tempLine++;
   	         }
   	     strcat(macros[macroCount].body, tempLine);
            }

            macroCount++;
            /* Skip over lines containing only whitespace or newline */
        do {
            if (!fgets(line, sizeof(line), inputFile)) {
                break;  /*Exit the loop if EOF is reached or an error occurs*/
            }
        } while (isWhitespaceOrEmpty(line));

        /* Move the file pointer back by the length of the line so that it can be processed in the next iteration */
        fseek(inputFile, -strlen(line), SEEK_CUR);
        continue;	
      }
        strcpy(modifiedLine, line);

        for ( i = 0; i < macroCount; i++) {
    /*Replace any macro usages in the line*/
    while(isMacroUsage(modifiedLine, macros[i].name)) {
        char *tmp = replaceString(modifiedLine, macros[i].name, macros[i].body);
        if (!tmp) {
            printf("Memory allocation error!\n");
            exit(1);
        }
        strcpy(modifiedLine, tmp);
        free(tmp);
    }
}



        /*Write the possibly modified line to the output file*/
        fputs(modifiedLine, outputFile);
    }

    /* Free the allocated memory for macros and close the files*/
    freeMacros(macros, macroCount);
    fclose(inputFile);
    fclose(outputFile);
}


/*helping functions, they handle macros:*/
int isWhitespaceOrEmpty(const char *line){
    while (*line) {
        if (!isspace(*line)) {
            return 0;  /* Not a whitespace-only line */
        }
        line++;
    }
    return 1;  /* Only whitespace characters or empty line */
}

void freeMacros(Macro *macros, int macroCount) {
    int i;
    for (i = 0; i < macroCount; i++) {
        free(macros[i].name);
        free(macros[i].body);
    }
}
/* This function checks if the provided line starts with the macro start keyword */
int isMacroDefinitionStart(const char *line) {
    /*Remove any leading whitespace*/
    while (isspace(*line)) {
        line++;
    }
    /*Check if the line starts with the MACRO_START keyword*/
    return strncmp(line, MACRO_START, strlen(MACRO_START)) == 0;
}

/* This function checks if the provided line starts with the macro end keyword */
int isMacroDefinitionEnd(const char *line) {
    /*Remove any leading whitespace*/
    while (isspace(*line)) {
        line++;
    }
    /*Check if the line starts with the MACRO_END keyword*/
    return strncmp(line, MACRO_END, strlen(MACRO_END)) == 0;
}

/*this function chekcs whether the given line of code contains the usage of a specific macro definition*/
int isMacroUsage(const char *line, const char *macroName){
	char *pos, *temp;
	size_t len = strlen(macroName);
	pos= strstr(line, macroName);
	printf("check if line '%s' contains macro usage of '%s'\n", line, macroName);
	
	
	while(pos){
		temp = pos + len;/*temp points to the next character after macroName in line*/
		/*check if macroName is a whole word, i.e. its not a part of another word*/
		if((pos==line || isspace(*(pos-1)) || ispunct(*(pos-1))) && /*before macroName*/
		(*temp== '\0' || isspace(*temp)|| ispunct(*temp))){/*after macroName*/
			return 1;
	         }
		 pos = strstr(temp, macroName);/*find next occurence of macroName*/
	}
	return 0;/*false*/
}
/*this function pareses a line of code that starts with macro definition, it extracts the macro name and body from the given line and stores them into a macro structure*/
void parseMacroDefinition(FILE *inputFile, char *line, Macro *macro){
	(*macro).name = getMacroName(line);
	(*macro).body = getMacroBody(inputFile);
}
/*this function retrieves the name of a macro from a line of code that contains a macro definition, it returns the name of a macro as a dynamically allocated string*/
char* getMacroName(const char *line){
	
	char *macroName=(char*)malloc(sizeof(char) * MAX_LINE_LENGTH);
	printf("extracting macro name from line: %s", line);
	if(!macroName){
		printf("error: failed to allocate memory for macro name.\n");
		return NULL;
	}
	if(sscanf(line, "%*s %s", macroName) != 1){
		printf("failed to parse macro name from line: %s\n", line);
		return NULL;
	}	
	return macroName;
}
/*this functin reads the body of a macro from a file, until he reaches "endmcro"*/
char* getMacroBody(FILE *inputFile){
	
    char line[MAX_LINE_LENGTH];
    size_t currentSize = MAX_LINE_LENGTH;
    char *macroBody = (char*)malloc(sizeof(char) * currentSize);
    if(!macroBody){
        printf("error: failed to allocate memory for macro body.\n");
        return NULL;
    }
    macroBody[0] = '\0'; /*Initialize to empty string*/

    while(fgets(line, MAX_LINE_LENGTH, inputFile) != NULL){
        if(strstr(line, MACRO_END) == line){
            break;
        }
        /*If buffer is not large enough, resize it*/
        if(strlen(macroBody) + strlen(line) >= currentSize){
            currentSize *= 2; /*Double the size*/
            macroBody = realloc(macroBody, currentSize);
            if(!macroBody){
                printf("error: failed to allocate memory for macro body.\n");
                return NULL;
            }
        }
        strcat(macroBody, line);
    }
    return macroBody;
}

/*
replaceString function: Replace all occurrences of a substring in a string
str: String to be modified
oldSubstr: Substring to be replaced
newSubstr: New substring to replace the old one
Returns: Modified string as a dynamically allocated string
 */
char* replaceString(const char *str, const char *oldSubstr, const char *newSubstr) {
    size_t newSize = strlen(str) + strlen(newSubstr) + 1;  /* Initially, just a rough estimate */
    char indentation[MAX_LINE_LENGTH] = "";
    int spacesCount = 0;
    size_t length;
    char* pos;
    const char* tmpStr = str;
    int isMultilineMacro = strchr(newSubstr, '\n') != NULL;  /* Check if the macro is multiline */
    char* processedNewSubstr;
    char* lineStart, *lineEnd;
    char* result;
    
    /* Determine the indentation of the line calling the macro */
    while (str[spacesCount] == ' ' || str[spacesCount] == '\t') {
        indentation[spacesCount] = str[spacesCount];
        spacesCount++;
    }
    indentation[spacesCount] = '\0';

    /* Allocate memory for the processed new substring */
    processedNewSubstr = (char*)malloc(newSize);
    if(!processedNewSubstr) {
        printf("Memory allocation error!\n");
        return NULL;
    }
    processedNewSubstr[0] = '\0';  /* Initialize to an empty string */

    /* If it's a multiline macro, process the new substring line by line and add indentation */
if (isMultilineMacro) {
    int isFirstLine = 1;
    lineStart = my_strdup(newSubstr);
    while ((lineEnd = strchr(lineStart, '\n'))) {
        *lineEnd = '\0';
        if (!isFirstLine) {
            strcat(processedNewSubstr, indentation);
        }
        strcat(processedNewSubstr, lineStart);
        
        lineStart = lineEnd + 1;
        if (*lineStart) {  /* Only add a newline if there are more lines to process */
            strcat(processedNewSubstr, "\n");
        }
        
        isFirstLine = 0;
    }

    if (*lineStart) {  /* Handle any remaining part of the new substring */
        if (!isFirstLine) {
            strcat(processedNewSubstr, indentation);
        }
        strcat(processedNewSubstr, lineStart);
    }
} else {
    strcpy(processedNewSubstr, newSubstr);  /* For single-line macros, don't add extra indentation */
}

    /* Replace all occurrences of oldSubstr with the processed new substring */
    tmpStr = str;
    length = strlen(tmpStr) + strlen(processedNewSubstr) + 1;
    result = (char*)malloc(length);
    if (!result) {
        printf("Memory allocation error!\n");
        free(processedNewSubstr);
        return NULL;
    }
    result[0] = '\0';
    pos = strstr(tmpStr, oldSubstr);
    while (pos) {
        strncat(result, tmpStr, pos - tmpStr);
        strcat(result, processedNewSubstr);
        tmpStr = pos + strlen(oldSubstr);
        pos = strstr(tmpStr, oldSubstr);
    }
    strcat(result, tmpStr);

    free(processedNewSubstr);
    return result;
}

/*since strdup isnt a part of any of the C90 libraries, we'll write one of our own funcitons my_strdup
The function: Duplicates a string
s: String to be duplicated
returns a pointer to the duplicated string
*/
char* my_strdup(const char* s){
	size_t len = strlen(s) +1;
	char* new_str = malloc(len);
	if(new_str == NULL){
		/*print an error massage and exit*/
		fprintf(stderr, "error: malloc failed in my_strdup\n");
		return NULL;
	}
	/*copy string to new memory location and pointer to the copy returned:*/
	return memcpy(new_str, s, len);
}

