#ifndef UTIL_H
#define UTIL_H
#include "lexer.h"

void print(char* str);

void warning_print(char* str, int line, int col);

void error_print(char* str, lexer_T *lexer);


#endif