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
    } decl;
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
        } stmt_t;
        enum stmt_t kind;
        struct decl *decl;
        struct expr *init_expr;
        struct expr *expr;
        struct expr *next_expr;
        struct stmt *body;
        struct stmt *else_body;
        struct stmt *next;
    } stmt;
    struct expr
    {
        enum expr_t
        {
            EXPR_ADD,
            EXPR_SUB,
            EXPR_MUL,
            EXPR_DIV,
            EXPR_NAME,
            EXPR_INTEGER_LITERAL,
            EXPR_STRING_LITERAL
        } expr_t;
        enum expr_t kind;
        struct expr* left;
        struct expr* right;
        const char *name;
        int integer_value;
        const char* string_literal;
    }expr;

} ast_T;

#endif