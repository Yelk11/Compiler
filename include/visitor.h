#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include "list.h"
#include "stack_frame.h"

typedef struct Visitor_T
{
  AST_T* object;
} Visitor_T;

Visitor_T* init_visitor();

AST_T* visitor_visit(Visitor_T* visitor, AST_T* node, List_T* list, Stack_frame_T* stack_frame);

AST_T* visitor_visit_compound(Visitor_T* visitor, AST_T* node, List_T* list, Stack_frame_T* stack_frame);
AST_T* visitor_visit_assignment(Visitor_T* visitor, AST_T* node, List_T* list, Stack_frame_T* stack_frame);
AST_T* visitor_visit_variable(Visitor_T* visitor, AST_T* node, List_T* list, Stack_frame_T* stack_frame);
AST_T* visitor_visit_function(Visitor_T* visitor, AST_T* node, List_T* list, Stack_frame_T* stack_frame);
AST_T* visitor_visit_call(Visitor_T* visitor, AST_T* node, List_T* list, Stack_frame_T* stack_frame);
AST_T* visitor_visit_int(Visitor_T* visitor, AST_T* node, List_T* list, Stack_frame_T* stack_frame);
AST_T* visitor_visit_string(Visitor_T* visitor, AST_T* node, List_T* list, Stack_frame_T* stack_frame);
AST_T* visitor_visit_binop(Visitor_T* visitor, AST_T* node, List_T* list, Stack_frame_T* stack_frame);
AST_T* visitor_visit_statement_return(Visitor_T* visitor, AST_T* node, List_T* list, Stack_frame_T* stack_frame);
AST_T* visitor_visit_access(Visitor_T* visitor, AST_T* node, List_T* list, Stack_frame_T* stack_frame);
#endif