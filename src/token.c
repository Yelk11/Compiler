#include <stdlib.h>
#include <stdio.h>

#include "token.h"

token_T *init_token(char *value, int type)
{
    token_T *token = calloc(1, sizeof(struct token_T));
    token->value = value;
    token->type = type;
    return token;
}

char *type_to_string(int token)
{

    switch (token)
    {
    case TOKEN_ID:
        return "TOKEN_ID";
    case TOKEN_EQUALS:
        return "TOKEN_EQUALS";
    case TOKEN_LPAREN:
        return "TOKEN_LPAREN";
    case TOKEN_RPAREN:
        return "TOKEN_RPAREN";
    case TOKEN_LBRACE:
        return "TOKEN_LBRACE";
    case TOKEN_RBRACE:
        return "TOKEN_RBRACE";
    case TOKEN_LBRACKET:
        return "TOKEN_LBRACKET";
    case TOKEN_RBRACKET:
        return "TOKEN_RBRACKET";
    case TOKEN_COLON:
        return "TOKEN_COLON";
    case TOKEN_COMMA:
        return "TOKEN_COMMA";
    case TOKEN_LT:
        return "TOKEN_LT";
    case TOKEN_GT:
        return "TOKEN_GT";
    case TOKEN_ARROW_RIGHT:
        return "TOKEN_ARROW_RIGHT";
    case TOKEN_INT:
        return "TOKEN_INT";
    case TOKEN_STRING:
        return "TOKEN_STRING";
    case TOKEN_STATEMENT:
        return "TOKEN_STATEMENT";
    case TOKEN_SEMI:
        return "TOKEN_SEMI";
    case TOKEN_PLUS:
        return "TOKEN_PLUS";
    case TOKEN_MINUS:
        return "TOKEN_MINUS";
    case TOKEN_DIV:
        return "TOKEN_DIV";
    case TOKEN_MUL:
        return "TOKEN_MUL";
    case TOKEN_EOF:
        return "TOKEN_EOF";
    default:
        return "TOKEN_NOT_VALID";
    }
}