#ifndef PRE_ASSEMBLER_H /*prevend double declaration*/
#define PRE_ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_LINE_LENGTH 80 /*ADD HERE*/
#define MAX_MACROS 100
#define MAX_LINES_IN_MACRO 100
#define MACRO_START "mcro"
#define MACRO_END "endmcro"


typedef struct Macro{	
	char* name;
	char* body;
} Macro;

/*functions declarations:*/
void freeMacros(Macro *macros, int macroCount);
int isWhitespaceOrEmpty(const char *line);
void  parseMacroDefinition(FILE *inputFile, char *line, Macro* macro);
char* getMacroName(const char *line);
char* getMacroBody(FILE *inputFile);
char* replaceString(const char *str, const char *oldSubstr, const char *newSubstr);
int isMacroDefinitionEnd(const char *line);
int isMacroDefinitionStart(const char *line);
int isMacroUsage(const char *line, const char *macroName);
char* my_strdup(const char* s);
void preprocessFile(const char* fileName);


#endif /*PRE_ASSEMBLER_H*/
