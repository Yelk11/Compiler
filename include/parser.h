#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"
#include "ast.h"

/*
seems to be really helpful
https://www3.nd.edu/~dthain/compilerbook/chapter6.pdf

*/

typedef struct parser_T
{
  lexer_T* lexer;
  token_T* token;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

token_T* parser_eat(parser_T* parser);

// ast_T* parse(lexer_T* lexer);

// ast_T* make_compound();

// ast_T* make_binary();

#endif