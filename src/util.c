#include "util.h"
#include <stdio.h>
#include "lexer.h"

void print(char* str)
{
    printf("%s\n",str);
}

void warning_print(char* str, int line, int col)
{
    printf("WARNING: %s\n",str);
}

void error_print(char* str, lexer_T* lexer)
{
    printf("ERROR: %s token: %c line: %d col: %d\n", str, lexer->c, lexer->line, lexer->col);
}