#include <stdlib.h>
#include <string.h>

typedef struct Exp {
  enum { integer_exp, string_exp, variable_exp,
         binary_exp, unary_exp, call_exp,
         projection_exp, record_exp } tag;
  union { int                                      integerExp;
          string                                   stringExp;
          string                                   variableExp;
          struct { string           operator;
                   struct Exp*      left;
                   struct Exp*      right; }       binaryExp;
          struct { string           operator;
                   struct Exp*      operand; }     unaryExp;
          struct { string           name;
                   struct Exp_list* arguments; }   callExp;
          struct { struct Exp*  record;
                   string       attribute; }       projectionExp;
          struct rec { string       attribute;
                       struct Exp*  value;
                       struct rec*  next; }        recordExp;
      } op;
} ast;



ast* make_binaryExp ( string operator, ast* left, ast* right ) {
  ast* e = (ast*) malloc(sizeof(ast));
  e->tag = binary_exp;
  e->op.binary.operator = operator;
  e->op.binary.left = left;
  e->op.binary.right = right;
  return e;
};

ast* make_arithmaticExp ( string operator, ast* left, ast* right ) {
  ast* e = (ast*) malloc(sizeof(ast));
  e->tag = binary_exp;
  e->op.binary.operator = operator;
  e->op.binary.left = left;
  e->op.binary.right = right;
  return e;
};



