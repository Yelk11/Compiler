#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#include "token.h"

typedef struct lexer_T{
    char* str;
    char c;
    int length;
    FILE* fp;
    struct token_T* token;
}lexer_T;

token_T* next(lexer_T* lexer);

lexer_T* init_lexer(char* file_name);
lexer_T* advance(lexer_T* lexer);
lexer_T* skip_whitespace(lexer_T* lexer);


#endif