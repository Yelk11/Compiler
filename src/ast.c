
#include "ast.h"
#include <stdlib.h>

decl *decl_create(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next)
{
    struct decl *d = malloc(sizeof(*d));
    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;
    return d;
}

stmt *stmt_create(enum stmt_t kind,
                  struct decl *decl, struct expr *init_expr,
                  struct expr *expr, struct expr *next_expr,
                  struct stmt *body, struct stmt *else_body,
                  struct stmt *next)
{
    struct stmt *s = malloc(sizeof(*s));
    s->kind = kind;
    s->decl = decl;
    s->init_expr = init_expr;
    s->expr = expr;
    s->next_expr = next_expr;
    s->body = body;
    s->else_body = else_body;
    s->next = next;
}

struct expr *expr_create(enum expr_t kind,
                         struct expr *L,
                         struct expr *R)
{
    struct expr* e = malloc(sizeof(*e));
    e->expr_t = kind;
    e->left = L;
    e->right = R;
}

expr *expr_create_name(const char *name){
    
}

expr *expr_create_integer_literal(int i);
expr *expr_create_boolean_literal(int b);
expr *expr_create_char_literal(char c);
expr *expr_create_string_literal(const char *str);
