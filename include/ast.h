#ifndef AST_H
#define AST_H

#include "token.h"



typedef enum {
    TYPE_VOID,
    TYPE_BOOLEAN,
    TYPE_CHARACTER,
    TYPE_INTEGER,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_FUNCTION
} type_t;

struct type {
    type_t kind;
    struct type *subtype;
    struct param_list *params;
};

struct param_list {
    char *name;
    struct type *type;
    struct param_list *next;
};

typedef struct decl
{
    char *name;
    struct type *type;
    struct expr *value;
    struct stmt *code;
    struct decl *next;
} decl;

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

typedef struct stmt
{

    enum stmt_t kind;
    struct decl *decl;
    struct expr *init_expr;
    struct expr *expr;
    struct expr *next_expr;
    struct stmt *body;
    struct stmt *else_body;
    struct stmt *next;
} stmt;

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

typedef struct expr
{
    enum expr_t kind;
    struct expr *left;
    struct expr *right;
    const char *name;
    int integer_value;
    const char *string_literal;
} expr;

decl* init_decl(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next);

stmt* init_stmt(enum stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr,
                    struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next);

expr* init_expr(enum expr_t kind, struct expr *left, struct expr *right, const char *name,
                    int integer_value, const char *string_literal);
#endif