#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void gen(node_T* node)
{
    switch(node->node_type)
    {
        case EXPRESSION_NODE:
            break;
        case STATEMENT_NODE:
            break;
        case DECLARATION_NODE:
            break;
        case TYPE_NODE:
            break;
        case CONTROL_FLOW_NODE:
            break;
        default:
            break;
    }
}
