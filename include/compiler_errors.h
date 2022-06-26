#ifndef COMPILER_ERRORS_H
#define COMPILER_ERRORS_H
#include "ast.h"
void assert_call_matches_signature(AST_T *call_node, AST_T *func_node);
#endif