#ifndef Lexer_H
#define Lexer_H

#include "token.h"
#include <stdio.h>


typedef struct Lexer_T{
    char* src;
    size_t src_size;
    char c;
    unsigned int i;
}Lexer_T;

Lexer_T *init_lexer(char *src);

void lexer_next(Lexer_T *lexer);

char lexer_peek(Lexer_T *lexer, int offset);

Token_T *lexer_advance_with(Lexer_T *lexer, Token_T *token);

Token_T *lexer_advance_current(Lexer_T *lexer, int type);

Token_T *lexer_parse_id(Lexer_T *lexer);

Token_T *lexer_parse_number(Lexer_T *lexer);

void lexer_skip_whitespace(Lexer_T *lexer);

void lexer_skip_comment(Lexer_T *lexer);

Token_T* lexer_parse_string(Lexer_T *lexer);

Token_T *lexer_next_token(Lexer_T *lexer);



#endif