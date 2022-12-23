#include <stdlib.h>
#include <string.h>

typedef struct Exp
{
    enum
    {
        integer_exp,
        string_exp,
        variable_exp,
        binary_exp,
        unary_exp,
        call_exp,
        projection_exp,
        record_exp
    } tag;
    union
    {
        int integerExp;
        char *stringExp;
        char *variableExp;
        struct
        {
            char *operator;
            struct Exp *left;
            struct Exp *right;
        } binaryExp;
        struct
        {
            char *operator;
            struct Exp *operand;
        } unaryExp;
        struct
        {
            char *name;
            struct Exp_list *arguments;
        } callExp;
        struct
        {
            struct Exp *record;
            char *attribute;
        } projectionExp;
        struct rec
        {
            char *attribute;
            struct Exp *value;
            struct rec *next;
        } recordExp;
    } op;
} ast;

ast *make_integerExp(char *integer)
{
    ast *e = (ast *)malloc(sizeof(ast));
    e->tag = integer_exp;
    e->op.integerExp = atoi(integer);
    return e;
};

ast *make_stringExp(char *string)
{
    ast *e = (ast *)malloc(sizeof(ast));
    e->tag = string_exp;
    e->op.stringExp = string;
    return e;
};



ast *make_binaryExp(char *operator, ast * left, ast *right)
{
    ast *e = (ast *)malloc(sizeof(ast));
    e->tag = binary_exp;
    e->op.binaryExp.operator= operator;
    e->op.binaryExp.left = left;
    e->op.binaryExp.right = right;
    return e;
};
