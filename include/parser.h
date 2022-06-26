#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"

typedef struct Parser_T
{
  Lexer_T* lexer;
  Token_T* token;
} Parser_T;

Parser_T* init_parser(Lexer_T* lexer);

Token_T* parser_eat(Parser_T* parser, int type);

AST_T* parser_parse(Parser_T* parser);

AST_T* parser_parse_factor(Parser_T* parser);

AST_T* parser_parse_term(Parser_T* parser);

AST_T* parser_parse_expr(Parser_T* parser);

AST_T* parser_parse_statement(Parser_T* parser);

AST_T* parser_parse_list(Parser_T* parser);

AST_T* parser_parse_string(Parser_T* parser);

AST_T* parser_parse_compound(Parser_T* parser);
#endif