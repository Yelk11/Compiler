
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node *init_node(node_type type, char *value)
{
    node *n = calloc(1, sizeof(struct node));
    n->type = type;
    n->value.str = value;
    return n;
}

node *init_node_pair(node_type type,char* value, node *left, node *right)
{
    node *n = calloc(1, sizeof(struct node));
    n->type = type;
    n->value.pair.left = left;
    n->value.pair.right = right;
    return n;
}



void print_ast(node* n, int indent){
    if(n == NULL){
        return;
    }
    if(indent > 0){
        char space[indent+1];
        memset(space, ' ', indent * sizeof(char));
        memset(space+indent, '\0', sizeof(char));
        printf("%s", space);
    }
    printf("%s",n->value.str);
    struct node *left, *right;
    left = n->value.pair.left;
    right = n->value.pair.right;
    print_ast(left, indent + 2);
    print_ast(right, indent);
}