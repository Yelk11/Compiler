#include "parser.h"
#include "token.h"
#include "ast.h"
#include <stdlib.h>

parser_T *init_parser(lexer_T *lexer)
{
    parser_T *parser = calloc(1, sizeof(struct parser_T));
    parser->lexer = lexer;
    parser->token = lexer_next_token(parser->lexer);
    return parser;
}

token_T *parser_eat(parser_T *parser)
{
    parser->token = lexer_next_token(parser->lexer);

    return parser->token;
}
ast_T* parse(lexer_T* lexer){
    return make_compound(lexer);
}

ast_T* make_compound(lexer_T* lexer){
    return NULL;
}