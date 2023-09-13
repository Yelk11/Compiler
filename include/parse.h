#ifndef PARSE_H
#define PARSE_H


#include "lex.h"
#include "token.h"

typedef struct parse_T{
    lexer_T* lexer;
    token_T* curToken;
    token_T* peekToken;
}parse_T;

parse_T* init_parser(lexer_T* lexer);

int checkToken(token_T* curToken, int kind);

int checkPeek(token_T* peekToken, int kind);

void match(parse_T* parser, int kind);

void nextToken(parse_T* parse);

void parse_abort(char* message);

void program(parse_T* parse);

void statement(parse_T* parser);

void nl(parse_T* parse);

void comparison(parse_T* parser);

int isComparisonOperator(parse_T* parser);

void expression(parse_T* parser);

void term(parse_T* parser);

void unary(parse_T* parser);

void primary(parse_T* parser);

#endif