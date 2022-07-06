#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#include "token.h"

typedef struct lexer_T{
    FILE* fp;
    char c;
    unsigned int i;
}lexer_T;

lexer_T* init_lexer(char* src);

token_T* lexer_next_token(lexer_T* lexer);

char lexer_peek(lexer_T* lexer, int offset);

void lexer_advance(lexer_T* lexer);

token_T* lexer_advance_current(lexer_T* lexer, int type);

token_T* lexer_advance_with(lexer_T* lexer, token_T* token);

void lexer_skip_whitespace(lexer_T* lexer);

void lexer_skip_comment(lexer_T* lexer);

void lexer_skip_comment_block(lexer_T* lexer);

token_T* lexer_parse_id(lexer_T* lexer);

token_T* lexer_parse_number(lexer_T* lexer);

token_T* lexer_parse_string(lexer_T* lexer);

#endif