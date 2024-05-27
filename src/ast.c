
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node_T *init_node(n_type type, token_T* token)
{
    node_T *n = calloc(1, sizeof(struct node_T));
    n->node_type = type;
    n->token = token;
    n->left = NULL;
    n->right = NULL;
    return n;
}

// node_T *init_node_pair(node_type type,char* value, node_T *left, node_T *right)
// {
//     node_T *n = calloc(1, sizeof(struct node_T));
//     n->type = type;
//     n->value.pair.left = left;
//     n->value.pair.right = right;
//     return n;
// }

node_T* node_expression(node_T* parent, token_T* token)
{
    node_T* node = init_node(EXPRESSION_NODE, token);
    if(parent->left == NULL)
    {
        parent->left = node;
    }else{
        parent->right = node;
    }
    return node;
}
node_T* node_statement(node_T* parent, token_T* token)
{
    node_T* node = init_node(STATEMENT_NODE, token);
    if(parent->left == NULL)
    {
        parent->left = node;
    }else{
        parent->right = node;
    }
    return node;
}
node_T* node_declaration(node_T* parent, token_T* token)
{
    node_T* node = init_node(DECLARATION_NODE, token);
    if(parent->left == NULL)
    {
        parent->left = node;
    }else{
        parent->right = node;
    }
    return node;
}
node_T* node_type(node_T* parent, token_T* token)
{
    node_T* node = init_node(TYPE_NODE, token);
    if(parent->left == NULL)
    {
        parent->left = node;
    }else{
        parent->right = node;
    }
    return node;
}
node_T* node_control_flow(node_T* parent, token_T* token)
{
    node_T* node = init_node(CONTROL_FLOW_NODE, token);
    if(parent->left == NULL)
    {
        parent->left = node;
    }else{
        parent->right = node;
    }
    return node;
}

// void print_ast(node_T* n, int indent){
//     if(n == NULL){
//         return;
//     }
//     if(indent > 0){
//         char space[indent+1];
//         memset(space, ' ', indent * sizeof(char));
//         memset(space+indent, '\0', sizeof(char));
//         printf("%s", space);
//     }
//     printf("%s",n->value.str);
//     struct node *left, *right;
//     left = n->value.pair.left;
//     right = n->value.pair.right;
//     print_ast(left, indent + 2);
//     print_ast(right, indent);
// }