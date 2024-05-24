#include <lexer.h>
#include <token.h>
#include <ast.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parse.h"
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

int parse(lexer_T *lexer)
{
    node *node = init_node_pair(INT, "int", NULL, NULL);

    if (is_function_definition(*lexer, *node))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
primary_expression
    : IDENTIFIER
    | CONSTANT
    | STRING_LITERAL
    | '(' expression ')'
    ;
*/
int is_primary_expression(lexer_T lexer, node my_node)
{
    
    if (lexer_next_token(&lexer)->type == IDENTIFIER)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == CONSTANT)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == STRING_LITERAL)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

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
int is_postfix_expression(lexer_T lexer, node my_node)
{

    if (is_primary_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_postfix_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_BRACKET &&
             is_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_BRACKET)
    {
        return true;
    }
    else if (is_postfix_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else if (is_postfix_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_argument_expression_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else if (is_postfix_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == PERIOD &&
             lexer_next_token(&lexer)->type == IDENTIFIER)
    {
        return true;
    }
    else if (is_postfix_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == PTR_OP &&
             lexer_next_token(&lexer)->type == IDENTIFIER)
    {
        return true;
    }
    else if (is_postfix_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == INC_OP)
    {
        return true;
    }
    else if (is_postfix_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == DEC_OP)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
argument_expression_list
    : assignment_expression
    | argument_expression_list ',' assignment_expression
    ;
*/
int p_is_argument_expression_list(lexer_T lexer, node my_node)
{
    return false; // TODO implement is_argument_expression_list
}
int is_argument_expression_list(lexer_T lexer, node my_node)
{
    if (is_assignment_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_argument_expression_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COMMA &&
             is_assignment_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
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

int is_unary_expression(lexer_T lexer, node my_node)
{
    if (is_postfix_expression(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == INC_OP &&
             is_unary_expression(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == DEC_OP &&
             is_unary_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_unary_expression(lexer, my_node) &&
             is_cast_expression(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == SIZEOF &&
             is_unary_expression(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == SIZEOF &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_type_name(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

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
int is_unary_operator(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == AMPERSAND)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == MULTIPLY)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == PLUS)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == MINUS)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == TILDE)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == EXCLAMATION_MARK)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
cast_expression
    : unary_expression
    | '(' type_name ')' cast_expression
    ;
*/

int is_cast_expression(lexer_T lexer, node my_node)
{
    if (is_unary_expression(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_type_name(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_cast_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression
    | multiplicative_expression '/' cast_expression
    | multiplicative_expression '%' cast_expression
    ;
*/
int is_multiplicative_expression(lexer_T lexer, node my_node)
{
    if (is_cast_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_multiplicative_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == MULTIPLY &&
             is_cast_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_multiplicative_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == DIVIDE &&
             is_cast_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_multiplicative_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == PERCENT &&
             is_cast_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;
*/
int is_additive_expression(lexer_T lexer, node my_node)
{
    if (is_multiplicative_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_additive_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == PLUS &&
             is_multiplicative_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_additive_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == MINUS &&
             is_multiplicative_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
shift_expression
    : additive_expression
    | shift_expression LEFT_OP additive_expression
    | shift_expression RIGHT_OP additive_expression
    ;
*/

int is_shift_expression(lexer_T lexer, node my_node)
{
    if (is_additive_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_shift_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == LEFT_OP &&
             is_additive_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_shift_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == RIGHT_OP &&
             is_additive_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
relational_expression
    : shift_expression
    | relational_expression '<' shift_expression
    | relational_expression '>' shift_expression
    | relational_expression LE_OP shift_expression
    | relational_expression GE_OP shift_expression
    ;
*/
int is_relational_expression(lexer_T lexer, node my_node)
{
    if (is_shift_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_relational_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == LESS_THAN &&
             is_shift_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_relational_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == GREATER_THAN &&
             is_shift_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_relational_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == LE_OP &&
             is_shift_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_relational_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == GE_OP &&
             is_shift_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
equality_expression
    : relational_expression
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;
*/
int is_equality_expression(lexer_T lexer, node my_node)
{
    if (is_relational_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_equality_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == EQ_OP &&
             is_relational_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_equality_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == NE_OP &&
             is_relational_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
and_expression
    : equality_expression
    | and_expression '&' equality_expression
    ;
*/
int is_and_expression(lexer_T lexer, node my_node)
{
    if (is_equality_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_and_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == AMPERSAND &&
             is_equality_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression
    ;
*/
int is_exclusive_or_expression(lexer_T lexer, node my_node)
{
    if (is_and_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_exclusive_or_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == CARET &&
             is_and_expression(lexer, my_node))
    {
        return true;
    }
    else
    {

        return false;
    }
}
/*
inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression
    ;
*/
int is_inclusive_or_expression(lexer_T lexer, node my_node)
{
    if (is_exclusive_or_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_inclusive_or_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == VERTICAL_BAR &&
             is_exclusive_or_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
logical_and_expression
    : inclusive_or_expression
    | logical_and_expression AND_OP inclusive_or_expression
    ;
*/
int is_logical_and_expression(lexer_T lexer, node my_node)
{
    if (is_inclusive_or_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_logical_and_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == AND_OP &&
             is_inclusive_or_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
logical_or_expression
    : logical_and_expression
    | logical_or_expression OR_OP logical_and_expression
    ;
*/
int is_logical_or_expression(lexer_T lexer, node my_node)
{
    if (is_logical_and_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_logical_or_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == OR_OP &&
             is_logical_and_expression(lexer, my_node))
    {
        return false;
    }
    else
    {
        return false;
    }
}
/*
conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression
    ;
*/
int is_conditional_expression(lexer_T lexer, node my_node)
{
    if (is_logical_or_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_logical_or_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == QUESTION_MARK &&
             is_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COLON &&
             is_conditional_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression
    ;
*/
int is_assignment_expression(lexer_T lexer, node my_node)
{
    if (is_conditional_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_unary_expression(lexer, my_node) &&
             is_assignment_operator(lexer, my_node) &&
             is_assignment_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
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
int is_assignment_operator(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == EQUALS)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == MUL_ASSIGN)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == DIV_ASSIGN)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == MOD_ASSIGN)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == ADD_ASSIGN)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == SUB_ASSIGN)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == LEFT_ASSIGN)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == RIGHT_ASSIGN)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == AND_ASSIGN)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == XOR_ASSIGN)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == OR_ASSIGN)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
expression
    : assignment_expression
    | expression ',' assignment_expression
    ;
*/
int is_expression(lexer_T lexer, node my_node)
{
    if (is_assignment_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COMMA &&
             is_assignment_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
constant_expression
    : conditional_expression
    ;
*/
int is_constant_expression(lexer_T lexer, node my_node)
{
    if (is_conditional_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
declaration
    : declaration_specifiers ';'
    | declaration_specifiers init_declarator_list ';'
    ;
*/
int is_declaration(lexer_T lexer, node my_node)
{
    if (is_declaration_specifiers(lexer, my_node) &&
        lexer_next_token(&lexer)->type == SEMICOLON)
    {
        return true;
    }
    else if (is_declaration_specifiers(lexer, my_node) &&
             is_init_declarator_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == SEMICOLON)
    {
        return true;
    }
    else
    {

        return false;
    }
}
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
int is_declaration_specifiers(lexer_T lexer, node my_node)
{
    if (is_storage_class_specifier(lexer, my_node))
    {
        return true;
    }
    else if (is_storage_class_specifier(lexer, my_node) &&
             is_declaration_specifiers(lexer, my_node))
    {
        return true;
    }
    else if (is_type_specifier(lexer, my_node))
    {
        return true;
    }
    else if (is_type_specifier(lexer, my_node) &&
             is_declaration_specifiers(lexer, my_node))
    {
        return true;
    }
    else if (is_type_qualifier(lexer, my_node))
    {
        return true;
    }
    else if (is_type_qualifier(lexer, my_node) &&
             is_declaration_specifiers(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
init_declarator_list
    : init_declarator
    | init_declarator_list ',' init_declarator
    ;
*/
int p_is_init_declarator_list(lexer_T lexer, node my_node)
{
    return false; // TODO imeplement p_is_init_declarator_list
}
int is_init_declarator_list(lexer_T lexer, node my_node)
{
    if (is_init_declarator(lexer, my_node))
    {
        return true;
    }
    else if (p_is_init_declarator_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COMMA &&
             is_init_declarator(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
init_declarator
    : declarator
    | declarator '=' initializer
    ;
*/
int is_init_declarator(lexer_T lexer, node my_node)
{
    if (is_declarator(lexer, my_node))
    {
        return true;
    }
    else if (is_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == EQUALS &&
             is_initializer(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
storage_class_specifier
    : TYPEDEF
    | EXTERN
    | STATIC
    | AUTO
    | REGISTER
    ;
*/
int is_storage_class_specifier(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == TYPEDEF)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == EXTERN)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == STATIC)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == AUTO)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == REGISTER)
    {
        return true;
    }
    else
    {
        return false;
    }
}
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
int is_type_specifier(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == VOID)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == CHAR)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == SHORT)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == INT)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == LONG)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == FLOAT)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == DOUBLE)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == SIGNED)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == UNSIGNED)
    {
        return true;
    }
    else if (is_struct_or_union_specifier(lexer, my_node))
    {
        return true;
    }
    else if (is_enum_specifier(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
struct_or_union_specifier
    : struct_or_union IDENTIFIER '{' struct_declaration_list '}'
    | struct_or_union '{' struct_declaration_list '}'
    | struct_or_union IDENTIFIER
    ;
*/
int is_struct_or_union_specifier(lexer_T lexer, node my_node)
{
    if (is_struct_or_union(lexer, my_node) &&
        lexer_next_token(&lexer)->type == IDENTIFIER &&
        lexer_next_token(&lexer)->type == L_BRACE &&
        is_struct_declaration_list(lexer, my_node) &&
        lexer_next_token(&lexer)->type == R_BRACE)
    {
        return true;
    }
    else if (is_struct_or_union(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_BRACE &&
             is_struct_declaration_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_BRACE)
    {
        return true;
    }
    else if (is_struct_or_union(lexer, my_node) &&
             lexer_next_token(&lexer)->type == IDENTIFIER)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
struct_or_union
    : STRUCT
    | UNION
    ;
*/
int is_struct_or_union(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == STRUCT)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == UNION)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
struct_declaration_list
    : struct_declaration
    | struct_declaration_list struct_declaration
    ;
*/
int p_is_struct_declaration_list(lexer_T lexer, node my_node)
{
    return false; // TODO implement p_is_struct_declaration_list
}
int is_struct_declaration_list(lexer_T lexer, node my_node)
{
    if (is_struct_declaration(lexer, my_node))
    {
        return true;
    }
    else if (p_is_struct_declaration_list(lexer, my_node) &&
             is_struct_declaration(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
struct_declaration
    : specifier_qualifier_list struct_declarator_list ';'
    ;
*/
int is_struct_declaration(lexer_T lexer, node my_node)
{
    if (is_specifier_qualifier_list(lexer, my_node) &&
        is_struct_declarator_list(lexer, my_node) &&
        lexer_next_token(&lexer)->type == SEMICOLON)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
specifier_qualifier_list
    : type_specifier specifier_qualifier_list
    | type_specifier
    | type_qualifier specifier_qualifier_list
    | type_qualifier
    ;
*/
int p_is_specifier_qualifier_list(lexer_T lexer, node my_node)
{
    return false; // TODO implement p_is_specifier_qualifier_list
}
int is_specifier_qualifier_list(lexer_T lexer, node my_node)
{
    if (is_type_specifier(lexer, my_node) &&
        p_is_specifier_qualifier_list(lexer, my_node))
    {
        return true;
    }
    else if (is_type_specifier(lexer, my_node))
    {
        return true;
    }
    else if (is_type_qualifier(lexer, my_node) &&
             p_is_specifier_qualifier_list(lexer, my_node))
    {
        return true;
    }
    else if (is_type_qualifier(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
struct_declarator_list
    : struct_declarator
    | struct_declarator_list ',' struct_declarator
    ;
*/
int p_is_struct_declarator_list(lexer_T lexer, node my_node)
{
    return false; // TODO implement p_is_struct_declarator_list
}
int is_struct_declarator_list(lexer_T lexer, node my_node)
{
    if (is_struct_declarator(lexer, my_node))
    {
        return true;
    }
    else if (p_is_struct_declarator_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COMMA &&
             is_struct_declarator(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
struct_declarator
    : declarator
    | ':' constant_expression
    | declarator ':' constant_expression
    ;
*/
int is_struct_declarator(lexer_T lexer, node my_node)
{
    if (is_declarator(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == COLON &&
             is_constant_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COLON &&
             is_constant_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
enum_specifier
    : ENUM '{' enumerator_list '}'
    | ENUM IDENTIFIER '{' enumerator_list '}'
    | ENUM IDENTIFIER
    ;
*/
int is_enum_specifier(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == ENUM &&
        lexer_next_token(&lexer)->type == L_BRACE &&
        is_enumerator_list(lexer, my_node) &&
        lexer_next_token(&lexer)->type == R_BRACE)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == ENUM &&
             lexer_next_token(&lexer)->type == IDENTIFIER &&
             lexer_next_token(&lexer)->type == L_BRACE &&
             is_enumerator_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_BRACE)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == ENUM &&
             lexer_next_token(&lexer)->type == IDENTIFIER)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
enumerator_list
    : enumerator
    | enumerator_list ',' enumerator
    ;
*/
int p_is_enumerator_list(lexer_T lexer, node my_node)
{
    return false; // TODO implement p_is_enumerator_list
}
int is_enumerator_list(lexer_T lexer, node my_node)
{
    if (is_enumerator(lexer, my_node))
    {
        return true;
    }
    if (p_is_enumerator_list(lexer, my_node) &&
        lexer_next_token(&lexer)->type == COMMA &&
        is_enumerator(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
enumerator
    : IDENTIFIER
    | IDENTIFIER '=' constant_expression
    ;
*/
int is_enumerator(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == IDENTIFIER)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == IDENTIFIER &&
             lexer_next_token(&lexer)->type == EQUALS &&
             is_constant_expression(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
type_qualifier
    : CONST
    | VOLATILE
    ;
*/
int is_type_qualifier(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == CONST)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == VOLATILE)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
declarator
    : pointer direct_declarator
    | direct_declarator
    ;
*/
int is_declarator(lexer_T lexer, node my_node)
{
    if (is_pointer(lexer, my_node) &&
        is_direct_declarator(lexer, my_node))
    {
        return true;
    }
    else if (is_direct_declarator(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
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
int is_direct_declarator(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == IDENTIFIER)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return false;
    }
    else if (is_direct_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_BRACKET &&
             is_constant_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_BRACKET)
    {
        return true;
    }
    else if (is_direct_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_BRACKET &&
             lexer_next_token(&lexer)->type == R_BRACKET)
    {
        return true;
    }
    else if (is_direct_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_parameter_type_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else if (is_direct_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_identifier_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else if (is_direct_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
pointer
    : '*'
    | '*' type_qualifier_list
    | '*' pointer
    | '*' type_qualifier_list pointer
    ;
*/
int is_pointer(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == MULTIPLY)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == MULTIPLY &&
             is_type_qualifier_list(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == MULTIPLY &&
             is_pointer(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == MULTIPLY &&
             is_type_qualifier_list(lexer, my_node) &&
             is_pointer(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
type_qualifier_list
    : type_qualifier
    | type_qualifier_list type_qualifier
    ;

*/
int p_is_type_qualifier_list(lexer_T lexer, node my_node)
{
    return false; // implement p_is_type_qualifier_list
}
int is_type_qualifier_list(lexer_T lexer, node my_node)
{
    if (is_type_qualifier(lexer, my_node))
    {
        return true;
    }
    else if (p_is_type_qualifier_list(lexer, my_node) &&
             is_type_qualifier(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
parameter_type_list
    : parameter_list
    | parameter_list ',' ELLIPSIS
    ;
*/
int is_parameter_type_list(lexer_T lexer, node my_node)
{
    if (is_parameter_list(lexer, my_node))
    {
        return true;
    }
    else if (is_parameter_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COMMA &&
             lexer_next_token(&lexer)->type == ELLIPSIS)
    {
        return true;
    }
    else
    {
        return true;
    }
}
/*
parameter_list
    : parameter_declaration
    | parameter_list ',' parameter_declaration
    ;
*/
int p_is_parameter_list(lexer_T lexer, node my_node)
{
    return false; // TODO implement p_is_parameter_list
}
int is_parameter_list(lexer_T lexer, node my_node)
{
    if (is_parameter_declaration(lexer, my_node))
    {
        return true;
    }
    else if (p_is_parameter_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COMMA &&
             is_parameter_declaration(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
parameter_declaration
    : declaration_specifiers declarator
    | declaration_specifiers abstract_declarator
    | declaration_specifiers
    ;
*/
int is_parameter_declaration(lexer_T lexer, node my_node)
{
    if (is_declaration_specifiers(lexer, my_node) &&
        is_declarator(lexer, my_node))
    {
        return true;
    }
    else if (is_declaration_specifiers(lexer, my_node) &&
             is_abstract_declarator(lexer, my_node))
    {
        return true;
    }
    else if (is_declaration_specifiers(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
identifier_list
    : IDENTIFIER
    | identifier_list ',' IDENTIFIER
    ;
*/
int p_is_identifier_list(lexer_T lexer, node my_node)
{
    return false; // TODO implement p_is_identifier_list
}
int is_identifier_list(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == IDENTIFIER)
    {
        return true;
    }
    else if (p_is_identifier_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COMMA &&
             lexer_next_token(&lexer)->type == IDENTIFIER)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
type_name
    : specifier_qualifier_list
    | specifier_qualifier_list abstract_declarator
    ;
*/
int is_type_name(lexer_T lexer, node my_node)
{
    if (is_specifier_qualifier_list(lexer, my_node))
    {
        return true;
    }
    else if (is_specifier_qualifier_list(lexer, my_node) &&
             is_abstract_declarator(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
abstract_declarator
    : pointer
    | direct_abstract_declarator
    | pointer direct_abstract_declarator
    ;
*/
int is_abstract_declarator(lexer_T lexer, node my_node)
{
    if (is_pointer(lexer, my_node))
    {
        return true;
    }
    else if (is_direct_abstract_declarator(lexer, my_node))
    {
        return true;
    }
    else if (is_pointer(lexer, my_node) &&
             is_direct_abstract_declarator(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
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
int is_direct_abstract_declarator(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == L_PARENTHESIS &&
        is_abstract_declarator(lexer, my_node) &&
        lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_BRACKET &&
             lexer_next_token(&lexer)->type == R_BRACKET)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_BRACKET &&
             is_constant_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_BRACKET)
    {
        return true;
    }
    else if (is_direct_abstract_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_BRACKET &&
             lexer_next_token(&lexer)->type == R_BRACKET)
    {
        return true;
    }
    else if (is_direct_abstract_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_BRACKET &&
             is_constant_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_BRACKET)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_parameter_type_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else if (is_direct_abstract_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else if (is_direct_abstract_declarator(lexer, my_node) &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_parameter_type_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
initializer
    : assignment_expression
    | '{' initializer_list '}'
    | '{' initializer_list ',' '}'
    ;
*/
int is_initializer(lexer_T lexer, node my_node)
{
    if (is_assignment_expression(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_BRACE &&
             is_initializer_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_BRACE)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_BRACE &&
             is_initializer_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COMMA &&
             lexer_next_token(&lexer)->type == R_BRACE)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
initializer_list
    : initializer
    | initializer_list ',' initializer
    ;
*/
int p_is_initializer_list(lexer_T lexer, node my_node)
{
    return false; // TODO implement p_is_initializer_list
}
int is_initializer_list(lexer_T lexer, node my_node)
{
    if (is_initializer(lexer, my_node))
    {
        return true;
    }
    else if (p_is_initializer_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COMMA &&
             is_initializer(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
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
int is_statement(lexer_T lexer, node my_node)
{
    if (is_labeled_statement(lexer, my_node))
    {
        return true;
    }
    else if (is_compound_statement(lexer, my_node))
    {
        return true;
    }
    else if (is_expression_statement(lexer, my_node))
    {
        return true;
    }
    else if (is_selection_statement(lexer, my_node))
    {
        return true;
    }
    else if (is_iteration_statement(lexer, my_node))
    {
        return true;
    }
    else if (is_jump_statement(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
labeled_statement
    : IDENTIFIER ':' statement
    | CASE constant_expression ':' statement
    | DEFAULT ':' statement
    ;
*/
int is_labeled_statement(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == IDENTIFIER &&
        lexer_next_token(&lexer)->type == COLON &&
        is_statement(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == CASE &&
             is_conditional_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == COLON &&
             is_statement(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == DEFAULT &&
             lexer_next_token(&lexer)->type == COLON &&
             is_statement(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
compound_statement
    : '{' '}'
    | '{' statement_list '}'
    | '{' declaration_list '}'
    | '{' declaration_list statement_list '}'
    ;
*/
int is_compound_statement(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == L_PARENTHESIS &&
        lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_statement_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_declaration_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_declaration_list(lexer, my_node) &&
             is_statement_list(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
declaration_list
    : declaration
    | declaration_list declaration
    ;
*/
int p_is_declaration_list(lexer_T lexer, node my_node)
{
    return false; // TODO implement is declation list
}
int is_declaration_list(lexer_T lexer, node my_node)
{
    if (is_declaration(lexer, my_node))
    {
        return true;
    }
    else if (p_is_declaration_list(lexer, my_node) &&
             is_declaration(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
statement_list
    : statement
    | statement_list statement
    ;
*/
int p_is_statement_list(lexer_T lexer, node my_node)
{
    return false; // TODO implement p_is_statement_list
}
int is_statement_list(lexer_T lexer, node my_node)
{
    if (is_statement(lexer, my_node))
    {
        return true;
    }
    else if (p_is_statement_list(lexer, my_node) &&
             is_statement(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
expression_statement
    : ';'
    | expression ';'
    ;
*/
int is_expression_statement(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == SEMICOLON)
    {
        return true;
    }
    else if (is_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == SEMICOLON)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
selection_statement
    : IF '(' expression ')' statement
    | IF '(' expression ')' statement ELSE statement
    | SWITCH '(' expression ')' statement
    ;
*/
int is_selection_statement(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == IF &&
        lexer_next_token(&lexer)->type == L_PARENTHESIS &&
        is_expression(lexer, my_node) &&
        lexer_next_token(&lexer)->type == R_PARENTHESIS &&
        is_statement(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == IF &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS &&
             is_statement(lexer, my_node) &&
             lexer_next_token(&lexer)->type == ELSE &&
             is_statement(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == SWITCH &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS &&
             is_statement(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
iteration_statement
    : WHILE '(' expression ')' statement
    | DO statement WHILE '(' expression ')' ';'
    | FOR '(' expression_statement expression_statement ')' statement
    | FOR '(' expression_statement expression_statement expression ')' statement
    ;
*/
int is_iteration_statement(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == WHILE &&
        lexer_next_token(&lexer)->type == L_PARENTHESIS &&
        is_expression(lexer, my_node) &&
        lexer_next_token(&lexer)->type == R_PARENTHESIS &&
        is_statement(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == DO &&
             is_statement(lexer, my_node) &&
             lexer_next_token(&lexer)->type == WHILE &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS &&
             lexer_next_token(&lexer)->type == SEMICOLON)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == FOR &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_expression_statement(lexer, my_node) &&
             is_expression_statement(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS &&
             is_statement(lexer, my_node))
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == FOR &&
             lexer_next_token(&lexer)->type == L_PARENTHESIS &&
             is_expression_statement(lexer, my_node) &&
             is_expression_statement(lexer, my_node) &&
             is_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == R_PARENTHESIS &&
             is_statement(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
jump_statement
    : GOTO IDENTIFIER ';'
    | CONTINUE ';'
    | BREAK ';'
    | RETURN ';'
    | RETURN expression ';'
    ;
*/
int is_jump_statement(lexer_T lexer, node my_node)
{
    if (lexer_next_token(&lexer)->type == GOTO &&
        lexer_next_token(&lexer)->type == IDENTIFIER &&
        lexer_next_token(&lexer)->type == SEMICOLON)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == CONTINUE &&
             lexer_next_token(&lexer)->type == SEMICOLON)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == BREAK &&
             lexer_next_token(&lexer)->type == SEMICOLON)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == RETURN &&
             lexer_next_token(&lexer)->type == SEMICOLON)
    {
        return true;
    }
    else if (lexer_next_token(&lexer)->type == RETURN &&
             is_expression(lexer, my_node) &&
             lexer_next_token(&lexer)->type == SEMICOLON)

    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
translation_unit
    : external_declaration
    | translation_unit external_declaration
    ;
*/
int is_translation_unit(lexer_T lexer, node my_node)
{
    if (is_external_declaration(lexer, my_node))
    {
        return true;
    }
    else if (is_translation_unit(lexer, my_node) &&
             is_external_declaration(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
external_declaration
    : function_definition
    | declaration
    ;
*/
int is_external_declaration(lexer_T lexer, node my_node)
{
    if (is_function_definition(lexer, my_node))
    {
        return true;
    }
    else if (is_declaration(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
function_definition
    : declaration_specifiers declarator declaration_list compound_statement
    | declaration_specifiers declarator compound_statement
    | declarator declaration_list compound_statement
    | declarator compound_statement
    ;
*/
int is_function_definition(lexer_T lexer, node my_node)
{
    if (is_declaration_specifiers(lexer, my_node) &&
        is_declarator(lexer, my_node) &&
        is_declaration_list(lexer, my_node) &&
        is_compound_statement(lexer, my_node))
    {
        return true;
    }
    else if (is_declaration_specifiers(lexer, my_node) &&
             is_declarator(lexer, my_node) &&
             is_compound_statement(lexer, my_node))
    {
        return true;
    }
    else if (is_declarator(lexer, my_node) &&
             is_declaration_list(lexer, my_node) &&
             is_compound_statement(lexer, my_node))
    {
        return true;
    }
    else if (is_declarator(lexer, my_node) &&
             is_compound_statement(lexer, my_node))
    {
        return true;
    }
    else
    {
        return false;
    }
}