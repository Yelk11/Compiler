#include "ast.h"
#include "list.h"
#include <stdlib.h>


AST_T* init_ast(int type){
    AST_T* ast = calloc(1, sizeof(struct AST_T));
    ast->type = type;

    if(type == AST_COMPOUND){
        ast->children = init_list(sizeof(struct AST_T*));
    }

    ast->stack_frame = (void*)0;
    ast->multiplier = 1;
    ast->dtype = DATA_TYPE_UNKNOWN;
    
    return ast;
}