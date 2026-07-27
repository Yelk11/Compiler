#include "util.h"
#include <stdio.h>

void print(char* str)
{
    printf("%s\n",str);
}

void warning_print(char* str, int line, int col)
{
    printf("WARNING: %s\n",str);
}

void error_print(char* str, int line, int col)
{
    printf("ERROR: %s\n",str);
}