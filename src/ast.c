
#include "ast.h"
#include <stdlib.h>


// declaration
decl *init_decl(char *name, struct type *type,
                    struct expr *value, struct stmt *code, struct decl *next)
{
    decl* node = calloc(1, sizeof(struct decl));
    node->name = name;
    node->type = type;
    node->value = value;
    node->code = code;
    node->next = next;
    return node;
}

// statement
stmt *init_stmt(enum stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr,
                    struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next)
{
    stmt* node = calloc(1, sizeof(struct stmt));
    node->kind = kind;
    node->decl = decl;
    node->init_expr = init_expr;
    node->expr = expr;
    node->next_expr = next_expr;
    node->body = body;
    node->else_body = else_body;
    node->next = next;
    return node;
}

// expression
expr *init_expr(enum expr_t kind, struct expr *left, struct expr *right, const char *name,
                    int integer_value, const char *string_literal)
{
    expr* node = calloc(1, sizeof(struct expr));
    node->kind = kind;
    node->left = left;
    node->right = right;
    node->name = name;
    node->integer_value = integer_value;
    node->string_literal = string_literal;
    return node;
}
