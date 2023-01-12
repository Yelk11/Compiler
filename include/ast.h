#ifndef AST_H
#define AST_H

#include "token.h"

typedef struct ast_T
{

    struct decl
    {
        char *name;
        struct type *type;
        struct expr *value;
        struct stmt *code;
        struct decl *next;
    }decl;
    struct stmt
    {
        enum stmt_t
        {
            STMT_DECL,
            STMT_EXPR,
            STMT_IF_ELSE,
            STMT_FOR,
            STMT_PRINT,
            STMT_RETURN,
            STMT_BLOCK
        }stmt_t;
        enum stmt_t kind;
        struct decl *decl;
        struct expr *init_expr;
        struct expr *expr;
        struct expr *next_expr;
        struct stmt *body;
        struct stmt *else_body;
        struct stmt *next;
    }stmt;
    struct exp
    {
    }exp;
} ast_T;

#endif