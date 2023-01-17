#ifndef PARSE_H
#define PARSE_H
#include <lexer.h>
#include <token.h>
#include <ast.h>
#include <stdbool.h>
/*
%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%start translation_unit
%%
*/

int parse(lexer_T* lexer);

/*
primary_expression
    : IDENTIFIER
    | CONSTANT
    | STRING_LITERAL
    | '(' expression ')'
    ;
*/
int is_primary_expression(lexer_T lexer, node my_node);

/*
postfix_expression
    : primary_expression
    | postfix_expression '[' expression ']'
    | postfix_expression '(' ')'
    | postfix_expression '(' argument_expression_list ')'
    | postfix_expression '.' IDENTIFIER
    | postfix_expression PTR_OP IDENTIFIER
    | postfix_expression INC_OP
    | postfix_expression DEC_OP
    ;
*/

int is_postfix_expression(lexer_T lexer, node my_node);

/*
argument_expression_list
    : assignment_expression
    | argument_expression_list ',' assignment_expression
    ;
*/
int is_argument_expression_list(lexer_T lexer, node my_node);
/*
unary_expression
    : postfix_expression
    | INC_OP unary_expression
    | DEC_OP unary_expression
    | unary_operator cast_expression
    | SIZEOF unary_expression
    | SIZEOF '(' type_name ')'
    ;
*/

int is_unary_expression(lexer_T lexer, node my_node);

/*
unary_operator
    : '&'
    | '*'
    | '+'
    | '-'
    | '~'
    | '!'
    ;
*/
int is_unary_operator(lexer_T lexer, node my_node);
/*
cast_expression
    : unary_expression
    | '(' type_name ')' cast_expression
    ;
*/

int is_cast_expression(lexer_T lexer, node my_node);

/*
multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression
    | multiplicative_expression '/' cast_expression
    | multiplicative_expression '%' cast_expression
    ;
*/
int is_multiplicative_expression(lexer_T lexer, node my_node);

/*
additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;
*/
int is_additive_expression(lexer_T lexer, node my_node);
/*
shift_expression
    : additive_expression
    | shift_expression LEFT_OP additive_expression
    | shift_expression RIGHT_OP additive_expression
    ;
*/

int is_shift_expression(lexer_T lexer, node my_node);

/*
relational_expression
    : shift_expression
    | relational_expression '<' shift_expression
    | relational_expression '>' shift_expression
    | relational_expression LE_OP shift_expression
    | relational_expression GE_OP shift_expression
    ;
*/
int is_relational_expression(lexer_T lexer, node my_node);
/*
equality_expression
    : relational_expression
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;
*/
int is_equality_expression(lexer_T lexer, node my_node);
/*
and_expression
    : equality_expression
    | and_expression '&' equality_expression
    ;
*/
int is_and_expression(lexer_T lexer, node my_node);
/*
exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression
    ;
*/
int is_exclusive_or_expression(lexer_T lexer, node my_node);
/*
inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression
    ;
*/
int is_inclusive_or_expression(lexer_T lexer, node my_node);
/*
logical_and_expression
    : inclusive_or_expression
    | logical_and_expression AND_OP inclusive_or_expression
    ;
*/
int is_logical_and_expression(lexer_T lexer, node my_node);
/*
logical_or_expression
    : logical_and_expression
    | logical_or_expression OR_OP logical_and_expression
    ;
*/
int is_logical_or_expression(lexer_T lexer, node my_node);
/*
conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression
    ;
*/
int is_conditional_expression(lexer_T lexer, node my_node);
/*
assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression
    ;
*/
int is_assignment_expression(lexer_T lexer, node my_node);
/*
assignment_operator
    : '='
    | MUL_ASSIGN
    | DIV_ASSIGN
    | MOD_ASSIGN
    | ADD_ASSIGN
    | SUB_ASSIGN
    | LEFT_ASSIGN
    | RIGHT_ASSIGN
    | AND_ASSIGN
    | XOR_ASSIGN
    | OR_ASSIGN
    ;
*/
int is_assignment_operator(lexer_T lexer, node my_node);
/*
expression
    : assignment_expression
    | expression ',' assignment_expression
    ;
*/
int is_expression(lexer_T lexer, node my_node);
/*
constant_expression
    : conditional_expression
    ;
*/
int is_constant_expression(lexer_T lexer, node my_node);
/*
declaration
    : declaration_specifiers ';'
    | declaration_specifiers init_declarator_list ';'
    ;
*/
int is_declaration(lexer_T lexer, node my_node);
/*
declaration_specifiers
    : storage_class_specifier
    | storage_class_specifier declaration_specifiers
    | type_specifier
    | type_specifier declaration_specifiers
    | type_qualifier
    | type_qualifier declaration_specifiers
    ;
*/
int is_declaration_specifiers(lexer_T lexer, node my_node);
/*
init_declarator_list
    : init_declarator
    | init_declarator_list ',' init_declarator
    ;
*/
int is_init_declarator_list(lexer_T lexer, node my_node);
/*
init_declarator
    : declarator
    | declarator '=' initializer
    ;
*/
int is_init_declarator(lexer_T lexer, node my_node);
/*
storage_class_specifier
    : TYPEDEF
    | EXTERN
    | STATIC
    | AUTO
    | REGISTER
    ;
*/
int is_storage_class_specifier(lexer_T lexer, node my_node);
/*
type_specifier
    : VOID
    | CHAR
    | SHORT
    | INT
    | LONG
    | FLOAT
    | DOUBLE
    | SIGNED
    | UNSIGNED
    | struct_or_union_specifier
    | enum_specifier
    | TYPE_NAME // TODO NOT SUPPORTED
    ;
*/
int is_type_specifier(lexer_T lexer, node my_node);
/*
struct_or_union_specifier
    : struct_or_union IDENTIFIER '{' struct_declaration_list '}'
    | struct_or_union '{' struct_declaration_list '}'
    | struct_or_union IDENTIFIER
    ;
*/
int is_struct_or_union_specifier(lexer_T lexer, node my_node);
/*
struct_or_union
    : STRUCT
    | UNION
    ;
*/
int is_struct_or_union(lexer_T lexer, node my_node);
/*
struct_declaration_list
    : struct_declaration
    | struct_declaration_list struct_declaration
    ;
*/
int is_struct_declaration_list(lexer_T lexer, node my_node);
/*
struct_declaration
    : specifier_qualifier_list struct_declarator_list ';'
    ;
*/
int is_struct_declaration(lexer_T lexer, node my_node);
/*
specifier_qualifier_list
    : type_specifier specifier_qualifier_list
    | type_specifier
    | type_qualifier specifier_qualifier_list
    | type_qualifier
    ;
*/
int is_specifier_qualifier_list(lexer_T lexer, node my_node);
/*
struct_declarator_list
    : struct_declarator
    | struct_declarator_list ',' struct_declarator
    ;
*/
int is_struct_declarator_list(lexer_T lexer, node my_node);
/*
struct_declarator
    : declarator
    | ':' constant_expression
    | declarator ':' constant_expression
    ;
*/
int is_struct_declarator(lexer_T lexer, node my_node);
/*
enum_specifier
    : ENUM '{' enumerator_list '}'
    | ENUM IDENTIFIER '{' enumerator_list '}'
    | ENUM IDENTIFIER
    ;
*/
int is_enum_specifier(lexer_T lexer, node my_node);
/*
enumerator_list
    : enumerator
    | enumerator_list ',' enumerator
    ;
*/
int is_enumerator_list(lexer_T lexer, node my_node);
/*
enumerator
    : IDENTIFIER
    | IDENTIFIER '=' constant_expression
    ;
*/
int is_enumerator(lexer_T lexer, node my_node);
/*
type_qualifier
    : CONST
    | VOLATILE
    ;
*/
int is_type_qualifier(lexer_T lexer, node my_node);
/*
declarator
    : pointer direct_declarator
    | direct_declarator
    ;
*/
int is_declarator(lexer_T lexer, node my_node);
/*
direct_declarator
    : IDENTIFIER
    | '(' declarator ')'
    | direct_declarator '[' constant_expression ']'
    | direct_declarator '[' ']'
    | direct_declarator '(' parameter_type_list ')'
    | direct_declarator '(' identifier_list ')'
    | direct_declarator '(' ')'
    ;
*/
int is_direct_declarator(lexer_T lexer, node my_node);
/*
pointer
    : '*'
    | '*' type_qualifier_list
    | '*' pointer
    | '*' type_qualifier_list pointer
    ;
*/
int is_pointer(lexer_T lexer, node my_node);
/*
type_qualifier_list
    : type_qualifier
    | type_qualifier_list type_qualifier
    ;

*/
int is_type_qualifier_list(lexer_T lexer, node my_node);
/*
parameter_type_list
    : parameter_list
    | parameter_list ',' ELLIPSIS
    ;
*/
int is_parameter_type_list(lexer_T lexer, node my_node);
/*
parameter_list
    : parameter_declaration
    | parameter_list ',' parameter_declaration
    ;
*/
int is_parameter_list(lexer_T lexer, node my_node);
/*
parameter_declaration
    : declaration_specifiers declarator
    | declaration_specifiers abstract_declarator
    | declaration_specifiers
    ;
*/
int is_parameter_declaration(lexer_T lexer, node my_node);
/*
identifier_list
    : IDENTIFIER
    | identifier_list ',' IDENTIFIER
    ;
*/
int is_identifier_list(lexer_T lexer, node my_node);
/*
type_name
    : specifier_qualifier_list
    | specifier_qualifier_list abstract_declarator
    ;
*/
int is_type_name(lexer_T lexer, node my_node);
/*
abstract_declarator
    : pointer
    | direct_abstract_declarator
    | pointer direct_abstract_declarator
    ;
*/
int is_abstract_declarator(lexer_T lexer, node my_node);
/*
direct_abstract_declarator
    : '(' abstract_declarator ')'
    | '[' ']'
    | '[' constant_expression ']'
    | direct_abstract_declarator '[' ']'
    | direct_abstract_declarator '[' constant_expression ']'
    | '(' ')'
    | '(' parameter_type_list ')'
    | direct_abstract_declarator '(' ')'
    | direct_abstract_declarator '(' parameter_type_list ')'
    ;
*/
int is_direct_abstract_declarator(lexer_T lexer, node my_node);
/*
initializer
    : assignment_expression
    | '{' initializer_list '}'
    | '{' initializer_list ',' '}'
    ;
*/
int is_initializer(lexer_T lexer, node my_node);
/*
initializer_list
    : initializer
    | initializer_list ',' initializer
    ;
*/
int is_initializer_list(lexer_T lexer, node my_node);
/*
statement
    : labeled_statement
    | compound_statement
    | expression_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    ;
*/
int is_statement(lexer_T lexer, node my_node);
/*
labeled_statement
    : IDENTIFIER ':' statement
    | CASE constant_expression ':' statement
    | DEFAULT ':' statement
    ;
*/
int is_labeled_statement(lexer_T lexer, node my_node);
/*
compound_statement
    : '{' '}'
    | '{' statement_list '}'
    | '{' declaration_list '}'
    | '{' declaration_list statement_list '}'
    ;
*/
int is_compound_statement(lexer_T lexer, node my_node);
/*
declaration_list
    : declaration
    | declaration_list declaration
    ;
*/
int is_declaration_list(lexer_T lexer, node my_node);
/*
statement_list
    : statement
    | statement_list statement
    ;
*/
int is_statement_list(lexer_T lexer, node my_node);
/*
expression_statement
    : ';'
    | expression ';'
    ;
*/
int is_expression_statement(lexer_T lexer, node my_node);
/*
selection_statement
    : IF '(' expression ')' statement
    | IF '(' expression ')' statement ELSE statement
    | SWITCH '(' expression ')' statement
    ;
*/
int is_selection_statement(lexer_T lexer, node my_node);
/*
iteration_statement
    : WHILE '(' expression ')' statement
    | DO statement WHILE '(' expression ')' ';'
    | FOR '(' expression_statement expression_statement ')' statement
    | FOR '(' expression_statement expression_statement expression ')' statement
    ;
*/
int is_iteration_statement(lexer_T lexer, node my_node);
/*
jump_statement
    : GOTO IDENTIFIER ';'
    | CONTINUE ';'
    | BREAK ';'
    | RETURN ';'
    | RETURN expression ';'
    ;
*/
int is_jump_statement(lexer_T lexer, node my_node);
/*
translation_unit
    : external_declaration
    | translation_unit external_declaration
    ;
*/
int is_translation_unit(lexer_T lexer, node my_node);
/*
external_declaration
    : function_definition
    | declaration
    ;
*/
int is_external_declaration(lexer_T lexer, node my_node);
/*
function_definition
    : declaration_specifiers declarator declaration_list compound_statement
    | declaration_specifiers declarator compound_statement
    | declarator declaration_list compound_statement
    | declarator compound_statement
    ;
*/
int is_function_definition(lexer_T lexer, node my_node);

#endif