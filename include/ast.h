#ifndef AST_H
#define AST_H
#include "list.h"
#include "stack_frame.h"
#include "types.h"

struct VISITOR_T;

typedef struct AST_T
{
  enum
  {
    AST_COMPOUND,
    AST_FUNCTION,
    AST_CALL,
    AST_ASSIGNMENT = 99,
    AST_DEFINITION_TYPE,
    AST_VARIABLE,
    AST_STATEMENT_RETURN,
    AST_ACCESS,
    AST_INT,
    AST_STRING,
    AST_BINOP,
    AST_NOOP,
  } type;

  data_type dtype;
  
  List_T* children;
  char* name;
  char* string_value;
  struct AST_T* value;
  struct AST_T* left;
  struct AST_T* right;
  int op;
  int int_value;
  int id;
  int stack_index;
  int multiplier;
  struct AST_T* (*fptr)(struct VISITOR_T* visitor, struct AST_T* node, List_T* list);
  Stack_frame_T* stack_frame;
} AST_T;

AST_T* init_ast(int type);
#endif