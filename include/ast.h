#ifndef AST_H
#define AST_H

#include "token.h"

typedef enum node_type
{
    OPERATOR_NODE,
    DECLARATION_NODE,
    EXPRESSION_NODE
} node_type;

typedef struct node node;

typedef struct pair_value
{
    node *right;
    node *left;
} pair_value;

typedef char *string_constant;

// Union only allows for one of the variables to be populated
union node_value
{
    pair_value pair;
    string_constant str;
} node_value;

typedef struct node
{
    node_type type;
    union node_value value;
} node;

node *init_node(node_type type, char *value);
node *init_node_pair(node_type type, char* value, node *left, node *right);

#endif