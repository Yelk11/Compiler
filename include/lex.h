#ifndef LEX_H
#define LEX_H

#include "token.h"

#include <stdio.h>

typedef struct lexer_T{
    FILE* fp;
    char curChar;
    int curPos;
}lexer_T;

lexer_T *init_lexer(FILE *fp);

int nextChar(lexer_T* lexer);

char peek(lexer_T* lexer);

void lex_abort(char* message);

void skipWhitespace(lexer_T* lexer);

void skipComment(lexer_T* lexer);

token_T* getToken(lexer_T* lexer);

#endif