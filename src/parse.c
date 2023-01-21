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
    node *my_node = init_node(INT, "int");

    if (is_function_definition(lexer, my_node))
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
int is_primary_expression(lexer_T *lexer, node *my_node)
{

    if (lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == CONSTANT)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == STRING_LITERAL)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    {
        lexer_next_token(lexer);
        if (is_expression(lexer, my_node))
        {
            if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
    }
    return false;
}

/*
postfix_expression
    : primary_expression
    | postfix_expression {'[' expression ']' || '(' {"" || argument_expression_list} ')' || '.' IDENTIFIER || PTR_OP IDENTIFIER || INC_OP || DEC_OP }
*/
int p_is_postfix_expression(lexer_T *lexer, node *my_node)
{

    return false;
}
int is_postfix_expression(lexer_T *lexer, node *my_node)
{

    if (is_primary_expression(lexer, my_node))
    {
        return true;
    }
    else if (p_is_postfix_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == L_BRACKET)
        {
            lexer_next_token(lexer);
            if (is_expression(lexer, my_node))
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
            else
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if (is_argument_expression_list(lexer, my_node))
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
            else
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == PERIOD)
        {
            lexer_next_token(lexer);
            if (lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == PTR_OP)
        {
            lexer_next_token(lexer);
            if (lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == INC_OP)
        {
            lexer_next_token(lexer);
            return true;
        }
        else if (lexer_peek_next_token(lexer, 0)->type == DEC_OP)
        {
            lexer_next_token(lexer);
            return true;
        }
    }
    return false;
}

/*
argument_expression_list
    : assignment_expression
    | argument_expression_list ',' assignment_expression
    ;
*/
int p_is_argument_expression_list(lexer_T *lexer, node *my_node)
{
    return false; // TODO implement is_argument_expression_list
}
int is_argument_expression_list(lexer_T *lexer, node *my_node)
{
    if (is_assignment_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_argument_expression_list(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == COMMA)
        {
            lexer_next_token(lexer);
            if (is_assignment_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
unary_expression
    : postfix_expression
    | INC_OP unary_expression
    | DEC_OP unary_expression
    | unary_operator cast_expression
    | SIZEOF { unary_expression || '(' type_name ')'}
    ;
*/

int p_is_unary_expression(lexer_T *lexer, node *my_node)
{

    return false;
}
/*
unary_expression
    : postfix_expression
    | INC_OP unary_expression
    | DEC_OP unary_expression
    | unary_operator cast_expression
    | SIZEOF { unary_expression || '(' type_name ')'}
    ;
*/
int is_unary_expression(lexer_T *lexer, node *my_node)
{
    if (is_postfix_expression(lexer, my_node))
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == INC_OP)
    {
        lexer_next_token(lexer);
        if (p_is_unary_expression(lexer, my_node))
        {
            return true;
        }
    }
    else if (lexer_peek_next_token(lexer, 0)->type == DEC_OP)
    {
        lexer_next_token(lexer);
        if (p_is_unary_expression(lexer, my_node))
        {
            return true;
        }
    }
    else if (is_unary_operator(lexer, my_node))
    {
        if (is_cast_expression(lexer, my_node))
        {
            return true;
        }
    }
    else if (lexer_peek_next_token(lexer, 0)->type == SIZEOF)
    {
        lexer_next_token(lexer);
        if (is_unary_expression(lexer, my_node))
        {
            return true;
        }
        else
        {
            if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
            {
                lexer_next_token(lexer);
                if (is_type_name(lexer, my_node))
                {
                    if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                    {
                        lexer_next_token(lexer);
                        return true;
                    }
                }
            }
        }
    }
    return false;
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
int is_unary_operator(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == AMPERSAND)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == MULTIPLY)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == PLUS)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == MINUS)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == TILDE)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == EXCLAMATION_MARK)
    {
        lexer_next_token(lexer);
        return true;
    }
    return false;
}
/*
cast_expression
    : unary_expression
    | '(' type_name ')' p_cast_expression
    ;
*/
int p_is_cast_expression(lexer_T *lexer, node *my_node)
{
    return false;
}
int is_cast_expression(lexer_T *lexer, node *my_node)
{
    if (is_unary_expression(lexer, my_node))
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    {
        if (is_type_name(lexer, my_node))
        {
            if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
            {
                if (p_is_cast_expression(lexer, my_node))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

/*
multiplicative_expression
    : cast_expression
    | multiplicative_expression {'*' || '/' || '%'} cast_expression
    ;
*/
int p_is_multiplicative_expression(lexer_T *lexer, node *my_node)
{

    return false;
}

int is_multiplicative_expression(lexer_T *lexer, node *my_node)
{
    if (is_cast_expression(lexer, my_node))
    {
        return true;
    }
    else if (p_is_multiplicative_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == MULTIPLY)
        {
            lexer_next_token(lexer);
            if (is_cast_expression(lexer, my_node))
            {
                return true;
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == DIVIDE)
        {
            lexer_next_token(lexer);
            if (is_cast_expression(lexer, my_node))
            {
                return true;
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == PERCENT)
        {
            lexer_next_token(lexer);
            if (is_cast_expression(lexer, my_node))
            {
                return true;
            }
        }
    }

    return false;
}

/*
additive_expression
    : multiplicative_expression
    | additive_expression {'+' || '-'} multiplicative_expression
    ;
*/
int p_is_additive_expression(lexer_T *lexer, node *my_node)
{
    return false;
}
int is_additive_expression(lexer_T *lexer, node *my_node)
{
    if (p_is_multiplicative_expression(lexer, my_node))
    {
        return true;
    }
    else if (p_is_additive_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == PLUS)
        {
            lexer_next_token(lexer);
            if (is_multiplicative_expression(lexer, my_node))
            {
                return true;
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == MINUS)
        {
            lexer_next_token(lexer);
            if (is_multiplicative_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
shift_expression
    : additive_expression
    | shift_expression {LEFT_OP || RIGHT_OP} additive_expression
    ;
*/
int p_is_shift_expression(lexer_T *lexer, node *my_node)
{
    return false;
}
int is_shift_expression(lexer_T *lexer, node *my_node)
{
    if (is_additive_expression(lexer, my_node))
    {
        return true;
    }
    else if (p_is_shift_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == LEFT_OP)
        {
            lexer_next_token(lexer);
            if (is_additive_expression(lexer, my_node))
            {
                return true;
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == RIGHT_OP)
        {
            lexer_next_token(lexer);
            if (is_additive_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}

/*
relational_expression
    : shift_expression
    | relational_expression {'<' || '>' || LE_OP || GE_OP} shift_expression
    ;
*/
int p_is_relational_expression(lexer_T *lexer, node *my_node)
{
    return false;
}
int is_relational_expression(lexer_T *lexer, node *my_node)
{
    if (is_shift_expression(lexer, my_node))
    {
        return true;
    }
    else if (p_is_relational_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == LESS_THAN)
        {
            lexer_next_token(lexer);
            if (is_shift_expression(lexer, my_node))
            {
                return true;
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == GREATER_THAN)
        {
            lexer_next_token(lexer);
            if (is_shift_expression(lexer, my_node))
            {
                return true;
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == LE_OP)
        {
            lexer_next_token(lexer);
            if (is_shift_expression(lexer, my_node))
            {
                return true;
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == GE_OP)
        {
            lexer_next_token(lexer);
            if (is_shift_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
equality_expression
    : relational_expression
    | equality_expression {EQ_OP || NE_OP} relational_expression
    ;
*/
int p_is_equality_expression(lexer_T *lexer, node *my_node)
{
    return false;
}

int is_equality_expression(lexer_T *lexer, node *my_node)
{
    if (is_relational_expression(lexer, my_node))
    {
        return true;
    }
    else if (p_is_equality_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == EQ_OP)
        {
            lexer_next_token(lexer);
            if (is_relational_expression(lexer, my_node))
            {
                return true;
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == NE_OP)
        {
            lexer_next_token(lexer);
            if (is_relational_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}

/*
and_expression
    : equality_expression
    | and_expression '&' equality_expression
    ;
*/
int p_is_and_expression(lexer_T *lexer, node *my_node)
{
    return false;
}

int is_and_expression(lexer_T *lexer, node *my_node)
{
    if (is_equality_expression(lexer, my_node))
    {
        return true;
    }
    else if (p_is_and_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == AMPERSAND)
        {
            if (is_equality_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression
    ;
*/
int p_is_exclusive_or_expression(lexer_T *lexer, node *my_node)
{
    return false;
}
int is_exclusive_or_expression(lexer_T *lexer, node *my_node)
{
    if (is_and_expression(lexer, my_node))
    {
        return true;
    }
    else if (p_is_exclusive_or_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == CARET)
        {
            lexer_next_token(lexer);
            if (is_and_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression
    ;
*/
int p_is_inclusive_or_expression(lexer_T *lexer, node *my_node)
{
    return false;
}
int is_inclusive_or_expression(lexer_T *lexer, node *my_node)
{
    if (is_exclusive_or_expression(lexer, my_node))
    {
        return true;
    }
    else if (p_is_inclusive_or_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == VERTICAL_BAR)
        {
            if (is_exclusive_or_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
logical_and_expression
    : inclusive_or_expression
    | logical_and_expression AND_OP inclusive_or_expression
    ;
*/
int p_is_logical_and_expression(lexer_T *lexer, node *my_node)
{
    return false;
}
int is_logical_and_expression(lexer_T *lexer, node *my_node)
{
    if (is_inclusive_or_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_inclusive_or_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == AND_OP)
        {
            lexer_next_token(lexer);
            if (is_inclusive_or_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
logical_or_expression
    : logical_and_expression
    | logical_or_expression OR_OP logical_and_expression
    ;
*/
int p_is_logical_or_expression(lexer_T *lexer, node *my_node)
{

    return false;
}
int is_logical_or_expression(lexer_T *lexer, node *my_node)
{
    if (is_logical_and_expression(lexer, my_node))
    {
        return true;
    }
    else if (p_is_logical_or_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == OR_OP)
        {
            lexer_next_token(lexer);
            if (is_logical_and_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression
    ;
*/
int p_is_conditional_expression(lexer_T *lexer, node *my_node)
{
    return false;
}
int is_conditional_expression(lexer_T *lexer, node *my_node)
{
    if (is_logical_or_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_logical_or_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == QUESTION_MARK)
        {
            lexer_next_token(lexer);
            if (is_expression(lexer, my_node))
            {
                if (lexer_peek_next_token(lexer, 0)->type == COLON)
                {
                    lexer_next_token(lexer);
                    if (p_is_conditional_expression(lexer, my_node))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
/*
assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression
    ;
*/
int p_is_assignment_expression(lexer_T *lexer, node *my_node)
{
    return false;
}
int is_assignment_expression(lexer_T *lexer, node *my_node)
{
    if (is_conditional_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_unary_expression(lexer, my_node))
    {
        if (is_assignment_operator(lexer, my_node))
        {
            if (p_is_assignment_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
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
int is_assignment_operator(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == EQUALS)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == MUL_ASSIGN)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == DIV_ASSIGN)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == MOD_ASSIGN)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == ADD_ASSIGN)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == SUB_ASSIGN)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == LEFT_ASSIGN)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == RIGHT_ASSIGN)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == AND_ASSIGN)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == XOR_ASSIGN)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == OR_ASSIGN)
    {
        lexer_next_token(lexer);
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
int p_is_expression(lexer_T *lexer, node *my_node)
{

    return false;
}
int is_expression(lexer_T *lexer, node *my_node)
{
    if (is_assignment_expression(lexer, my_node))
    {
        return true;
    }
    else if (p_is_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == COMMA)
        {
            if (is_assignment_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
constant_expression
    : conditional_expression
    ;
*/
int is_constant_expression(lexer_T *lexer, node *my_node)
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
int is_declaration(lexer_T *lexer, node *my_node)
{
    if (is_declaration_specifiers(lexer, my_node) &&
        lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
    {
        return true;
    }
    else if (is_declaration_specifiers(lexer, my_node) &&
             is_init_declarator_list(lexer, my_node) &&
             lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
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
    : storage_class_specifier {"" || declaration_specifiers}
    | type_specifier {"" || declaration_specifiers}
    | type_qualifier {"" || declaration_specifiers}
    ;
*/
int p_is_declaration_specifiers(lexer_T *lexer, node *my_node)
{

    return false;
}
int is_declaration_specifiers(lexer_T *lexer, node *my_node)
{
    if (is_storage_class_specifier(lexer, my_node))
    {
        if (p_is_declaration_specifiers(lexer, my_node))
        {
            return true;
        }
        else
        {
            return true;
        }
    }
    else if (is_type_specifier(lexer, my_node))
    {
        if (p_is_declaration_specifiers(lexer, my_node))
        {
            return true;
        }
        else
        {
            return true;
        }
    }
    else if (is_type_qualifier(lexer, my_node))
    {
        if (p_is_declaration_specifiers(lexer, my_node))
        {
            return true;
        }
        else
        {
            return true;
        }
    }
    return false;
}
/*
init_declarator_list
    : init_declarator
    | init_declarator_list ',' init_declarator
    ;
*/
int p_is_init_declarator_list(lexer_T *lexer, node *my_node)
{
    return false; // TODO imeplement p_is_init_declarator_list
}
int is_init_declarator_list(lexer_T *lexer, node *my_node)
{
    if (is_init_declarator(lexer, my_node))
    {
        return true;
    }
    else if (p_is_init_declarator_list(lexer, my_node) &&
             lexer_peek_next_token(lexer, 0)->type == COMMA &&
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
int is_init_declarator(lexer_T *lexer, node *my_node)
{
    if (is_declarator(lexer, my_node))
    {
        return true;
    }
    else if (is_declarator(lexer, my_node) &&
             lexer_peek_next_token(lexer, 0)->type == EQUALS &&
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
int is_storage_class_specifier(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == TYPEDEF)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == EXTERN)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == STATIC)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == AUTO)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == REGISTER)
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
int is_type_specifier(lexer_T *lexer, node *my_node)
{
    // print_lexer(lexer);
    // printf("type: %s\n",type_to_string( lexer_peek_next_token(lexer,1)->type));
    if (lexer_peek_next_token(lexer, 0)->type == VOID)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == CHAR)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == SHORT)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == INT)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == LONG)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == FLOAT)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == DOUBLE)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == SIGNED)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == UNSIGNED)
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
int is_struct_or_union_specifier(lexer_T *lexer, node *my_node)
{
    if (is_struct_or_union(lexer, my_node) &&
        lexer_peek_next_token(lexer, 0)->type == IDENTIFIER &&
        lexer_peek_next_token(lexer, 0)->type == L_BRACE &&
        is_struct_declaration_list(lexer, my_node) &&
        lexer_peek_next_token(lexer, 0)->type == R_BRACE)
    {
        return true;
    }
    else if (is_struct_or_union(lexer, my_node) &&
             lexer_peek_next_token(lexer, 0)->type == L_BRACE &&
             is_struct_declaration_list(lexer, my_node) &&
             lexer_peek_next_token(lexer, 0)->type == R_BRACE)
    {
        return true;
    }
    else if (is_struct_or_union(lexer, my_node) &&
             lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
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
int is_struct_or_union(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == STRUCT)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == UNION)
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
int p_is_struct_declaration_list(lexer_T *lexer, node *my_node)
{
    return false; // TODO implement p_is_struct_declaration_list
}
int is_struct_declaration_list(lexer_T *lexer, node *my_node)
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
int is_struct_declaration(lexer_T *lexer, node *my_node)
{
    if (is_specifier_qualifier_list(lexer, my_node) &&
        is_struct_declarator_list(lexer, my_node) &&
        lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
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
int p_is_specifier_qualifier_list(lexer_T *lexer, node *my_node)
{
    return false; // TODO implement p_is_specifier_qualifier_list
}
int is_specifier_qualifier_list(lexer_T *lexer, node *my_node)
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
int p_is_struct_declarator_list(lexer_T *lexer, node *my_node)
{
    return false; // TODO implement p_is_struct_declarator_list
}
int is_struct_declarator_list(lexer_T *lexer, node *my_node)
{
    if (is_struct_declarator(lexer, my_node))
    {
        return true;
    }
    else if (p_is_struct_declarator_list(lexer, my_node) &&
             lexer_peek_next_token(lexer, 0)->type == COMMA &&
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
int is_struct_declarator(lexer_T *lexer, node *my_node)
{
    if (is_declarator(lexer, my_node))
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == COLON &&
             is_constant_expression(lexer, my_node))
    {
        return true;
    }
    else if (is_declarator(lexer, my_node) &&
             lexer_peek_next_token(lexer, 0)->type == COLON &&
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
int is_enum_specifier(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == ENUM &&
        lexer_peek_next_token(lexer, 0)->type == L_BRACE &&
        is_enumerator_list(lexer, my_node) &&
        lexer_peek_next_token(lexer, 0)->type == R_BRACE)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == ENUM &&
             lexer_peek_next_token(lexer, 0)->type == IDENTIFIER &&
             lexer_peek_next_token(lexer, 0)->type == L_BRACE &&
             is_enumerator_list(lexer, my_node) &&
             lexer_peek_next_token(lexer, 0)->type == R_BRACE)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == ENUM &&
             lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
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
int p_is_enumerator_list(lexer_T *lexer, node *my_node)
{
    return false; // TODO implement p_is_enumerator_list
}
int is_enumerator_list(lexer_T *lexer, node *my_node)
{
    if (is_enumerator(lexer, my_node))
    {
        return true;
    }
    if (p_is_enumerator_list(lexer, my_node) &&
        lexer_peek_next_token(lexer, 0)->type == COMMA &&
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
int is_enumerator(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == IDENTIFIER &&
             lexer_peek_next_token(lexer, 0)->type == EQUALS &&
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
int is_type_qualifier(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == CONST)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == VOLATILE)
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
    : { pointer || ""} direct_declarator
    | direct_declarator
    ;
*/
int is_declarator(lexer_T *lexer, node *my_node)
{
    if (is_pointer(lexer, my_node))
    {
        if (is_direct_declarator(lexer, my_node))
        {
            return true;
        }
    }
    else
    {
        if (is_direct_declarator(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
direct_declarator
    : IDENTIFIER
    | '(' declarator ')'
    | direct_declarator { '[' {constant_expression || "" } ']' ||  '(' {parameter_type_list  ||  identifier_list || ""} ')' }
*/
int p_is_direct_declarator(lexer_T *lexer, node *my_node)
{

    return false;
}
int is_direct_declarator(lexer_T *lexer, node *my_node)
{

    if (lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    {
        if (is_declarator(lexer, my_node))
        {
            if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                return true;
            }
        }
    }
    else if (p_is_direct_declarator(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == L_BRACKET)
        {
            if (is_constant_expression(lexer, my_node))
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
                {
                    return true;
                }
            }
            else
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
                {
                    return true;
                }
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
        {
            if (is_parameter_type_list(lexer, my_node))
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
            else if (is_identifier_list(lexer, my_node))
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
            else
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
        }
    }
    return false;
}
/*
pointer
    : '*'
    | '*' type_qualifier_list
    | '*' pointer
    | '*' type_qualifier_list pointer
    ;
*/
int is_pointer(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == MULTIPLY)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == MULTIPLY &&
             is_type_qualifier_list(lexer, my_node))
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == MULTIPLY &&
             is_pointer(lexer, my_node))
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == MULTIPLY &&
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
int p_is_type_qualifier_list(lexer_T *lexer, node *my_node)
{
    return false; // implement p_is_type_qualifier_list
}
int is_type_qualifier_list(lexer_T *lexer, node *my_node)
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
int is_parameter_type_list(lexer_T *lexer, node *my_node)
{
    if (is_parameter_list(lexer, my_node))
    {
        return true;
    }
    else if (is_parameter_list(lexer, my_node) &&
             lexer_peek_next_token(lexer, 0)->type == COMMA &&
             lexer_peek_next_token(lexer, 0)->type == ELLIPSIS)
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
int p_is_parameter_list(lexer_T *lexer, node *my_node)
{
    return false; // TODO implement p_is_parameter_list
}
int is_parameter_list(lexer_T *lexer, node *my_node)
{
    if (is_parameter_declaration(lexer, my_node))
    {
        return true;
    }
    else if (p_is_parameter_list(lexer, my_node) &&
             lexer_peek_next_token(lexer, 0)->type == COMMA &&
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
    : declaration_specifiers {"" || declarator || abstract_declarator}
*/
int is_parameter_declaration(lexer_T *lexer, node *my_node)
{
    if (is_declaration_specifiers(lexer, my_node))
    {
        if (is_declarator(lexer, my_node))
        {
            return true;
        }
        else if (is_abstract_declarator(lexer, my_node))
        {
            return true;
        }
        else
        {
            return true;
        }
    }
    return false;
}
/*
identifier_list
    : IDENTIFIER
    | identifier_list ',' IDENTIFIER
    ;
*/
int p_is_identifier_list(lexer_T *lexer, node *my_node)
{

    return false;
}
int is_identifier_list(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
    {
        return true;
    }
    else if (p_is_identifier_list(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == COMMA)
        {
            if (lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
            {
                return true;
            }
        }
    }
    return false;
}
/*
type_name
    : specifier_qualifier_list {"" || abstract_declarator}
*/
int is_type_name(lexer_T *lexer, node *my_node)
{
    if (is_specifier_qualifier_list(lexer, my_node))
    {
        if (is_abstract_declarator(lexer, my_node))
        {
            return true;
        }
        else
        {
            return true;
        }
    }
    return false;
}
/*
abstract_declarator
    : pointer {"" || direct_abstract_declarator}
    | direct_abstract_declarator
    ;
*/
int is_abstract_declarator(lexer_T *lexer, node *my_node)
{
    if (is_pointer(lexer, my_node))
    {
        if (is_direct_abstract_declarator(lexer, my_node))
        {
            return true;
        }
        else
        {
            return true;
        }
    }
    else if (is_direct_abstract_declarator(lexer, my_node))
    {
        return true;
    }
    return false;
}
/*
direct_abstract_declarator
    : '(' {"" || abstract_declarator || parameter_type_list} ')'
    | '[' {"" || constant_expression} ']'
    | direct_abstract_declarator {'[' {"" || constant_expression} ']' || '(' {"" || parameter_type_list } ')'}
    ;
*/

int p_direct_abstract_declarator(lexer_T *lexer, node *my_node)
{
    // if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    // {
    //     lexer_next_token(lexer);
    //     if (is_parameter_type_list(lexer, my_node))
    //     {
    //         if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
    //         {
    //             lexer_next_token(lexer);
    //             return true;
    //         }
    //     }
    //     else if (is_abstract_declarator(lexer, my_node))
    //     {
    //         if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
    //         {
    //             lexer_next_token(lexer);
    //             return true;
    //         }
    //     }
    //     else
    //     {
    //         if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
    //         {
    //             lexer_next_token(lexer);
    //             return true;
    //         }
    //     }
    // }
    // else if (lexer_peek_next_token(lexer, 0)->type == L_BRACKET)
    // {
    //     lexer_next_token(lexer);
    //     if (is_constant_expression(lexer, my_node))
    //     {
    //         if (lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
    //         {
    //             lexer_next_token(lexer);
    //             return true;
    //         }
    //     }
    //     else
    //     {
    //         if (lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
    //         {
    //             lexer_next_token(lexer);
    //             return true;
    //         }
    //     }
    // }
    return false;
}
int is_direct_abstract_declarator(lexer_T *lexer, node *my_node)
{

    if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    {
        lexer_next_token(lexer);
        if (is_parameter_type_list(lexer, my_node))
        {
            if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
        else if (is_abstract_declarator(lexer, my_node))
        {
            if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
        else
        {
            if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
    }
    else if (lexer_peek_next_token(lexer, 0)->type == L_BRACKET)
    {
        lexer_next_token(lexer);
        if (is_constant_expression(lexer, my_node))
        {
            if (lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
        else
        {
            if (lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
    }
    else if (p_direct_abstract_declarator(lexer, my_node))
    {
        return true;
    }
    return false;
}
/*
initializer
    : assignment_expression
    | '{' initializer_list {"," || ""}'}'
    ;
*/
int is_initializer(lexer_T *lexer, node *my_node)
{
    if (is_assignment_expression(lexer, my_node))
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == L_BRACE)
    {
        lexer_next_token(lexer);
        if (is_initializer_list(lexer, my_node))
        {
            if (lexer_peek_next_token(lexer, 0)->type == COMMA)
            {
                lexer_next_token(lexer);
                if (lexer_peek_next_token(lexer, 0)->type == R_BRACE)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
            else
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_BRACE)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
        }
    }
    return false;
}
/*
initializer_list
    : initializer
    | initializer_list ',' initializer
    ;
*/
int p_is_initializer_list(lexer_T *lexer, node *my_node)
{
    // if (is_initializer(lexer, my_node))
    // {
    //     return true;
    // }
    return false;
}
int is_initializer_list(lexer_T *lexer, node *my_node)
{
    if (is_initializer(lexer, my_node))
    {
        return true;
    }
    else if (p_is_initializer_list(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == COMMA)
        {
            lexer_next_token(lexer);
            if (is_initializer(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
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
int is_statement(lexer_T *lexer, node *my_node)
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
    return false;
}
/*
labeled_statement
    : IDENTIFIER ':' statement
    | CASE constant_expression ':' statement
    | DEFAULT ':' statement
    ;
*/
int is_labeled_statement(lexer_T *lexer, node *my_node)
{

    if (lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
    {
        lexer_next_token(lexer);
        if (lexer_peek_next_token(lexer, 0)->type == COLON)
        {

            if (is_statement(lexer, my_node))
            {
                return true;
            }
        }
    }
    else if (lexer_peek_next_token(lexer, 0)->type == CASE)
    {
        lexer_next_token(lexer);
        if (is_conditional_expression(lexer, my_node))
        {
            if (lexer_peek_next_token(lexer, 0)->type == COLON)
            {
                lexer_next_token(lexer);
                if (is_statement(lexer, my_node))
                {
                    return true;
                }
            }
        }
    }
    else if (lexer_peek_next_token(lexer, 0)->type == DEFAULT)
    {
        lexer_next_token(lexer);
        if (lexer_peek_next_token(lexer, 0)->type == COLON)
        {
            lexer_next_token(lexer);
            if (is_statement(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
compound_statement
    : '{' {""|| statement_list || {declaration_list || {declaration_list statement_list}} '}'
    ;
*/
int is_compound_statement(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == L_BRACE)
    {
        lexer_next_token(lexer);
        if (is_statement_list(lexer, my_node))
        {

            if (lexer_peek_next_token(lexer, 0)->type == R_BRACE)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
        if (is_declaration_list(lexer, my_node))
        {
            if (is_statement_list(lexer, my_node))
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_BRACE)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
            else
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_BRACE)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
        }
        else if (lexer_peek_next_token(lexer, 0)->type == R_BRACE)
        {
            lexer_next_token(lexer);
            return true;
        }
    }

    return false;
}
/*
declaration_list
    : {""|| p_declaration_list} declaration
    ;
*/
int p_is_declaration_list(lexer_T *lexer, node *my_node)
{
    return false;
}
int is_declaration_list(lexer_T *lexer, node *my_node)
{
    if (!p_is_declaration_list(lexer, my_node)) // NOT p_is_declaration_list
    {
        if (is_declaration(lexer, my_node))
        {
            return true;
        }
    }
    else
    {
        if (is_declaration(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
statement_list
    : {""|| p_statement_list } statement
    ;
*/
int p_is_statement_list(lexer_T *lexer, node *my_node)
{
    // if (is_statement(lexer, my_node))
    // {
    //     return true;
    // }
    return false;
}
int is_statement_list(lexer_T *lexer, node *my_node)
{
    if (is_statement(lexer, my_node))
    {
        return true;
    }
    else if (p_is_statement_list(lexer, my_node))
    {
        return true;
    }
    return false;
}
/*
expression_statement
    : {"" || expression} ';'
    ;
*/
int is_expression_statement(lexer_T *lexer, node *my_node)
{
    if (is_expression(lexer, my_node))
    {
        if (lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
        {
            lexer_next_token(lexer);
            return true;
        }
    }
    else
    {
        if (lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
        {
            lexer_next_token(lexer);
            return true;
        }
    }
    return false;
}
/*
selection_statement
    : IF '(' expression ')' statement {""|| ELSE statement}
    | SWITCH '(' expression ')' statement
    ;
*/
int is_selection_statement(lexer_T *lexer, node *my_node)
{

    if (lexer_peek_next_token(lexer, 0)->type == IF)
    {
        lexer_next_token(lexer);
        if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if (is_expression(lexer, my_node))
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    if (is_statement(lexer, my_node))
                    {
                        if (lexer_peek_next_token(lexer, 0)->type != ELSE) // if NOT else or ""
                        {
                            return true;
                        }
                        else
                        {
                            lexer_next_token(lexer);
                            if (is_statement(lexer, my_node))
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (lexer_peek_next_token(lexer, 0)->type == SWITCH)
    {
        lexer_next_token(lexer);
        if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if (is_expression(lexer, my_node))
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    if (is_statement(lexer, my_node))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
/*
iteration_statement
    : WHILE '(' expression ')' statement
    | DO statement WHILE '(' expression ')' ';'
    | FOR '(' expression_statement expression_statement {expression || ""}')' statement
    ;
*/
int is_iteration_statement(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == WHILE)
    {
        lexer_next_token(lexer);
        if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if (is_expression(lexer, my_node))
            {
                if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    if (is_statement(lexer, my_node))
                    {
                        return true;
                    }
                }
            }
        }
    }
    else if (lexer_peek_next_token(lexer, 0)->type == DO)
    {
        lexer_next_token(lexer);
        if (is_statement(lexer, my_node))
        {
            if (lexer_peek_next_token(lexer, 0)->type == WHILE)
            {
                lexer_next_token(lexer);
                if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    if (is_expression(lexer, my_node))
                    {
                        if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                        {
                            lexer_next_token(lexer);
                            if (lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
                            {
                                lexer_next_token(lexer);
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (lexer_peek_next_token(lexer, 0)->type == FOR)
    {
        lexer_next_token(lexer);
        if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if (is_expression_statement(lexer, my_node))
            {
                if (is_expression_statement(lexer, my_node))
                {
                    if (is_expression(lexer, my_node))
                    {
                        if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                        {
                            lexer_next_token(lexer);
                            if (is_statement(lexer, my_node))
                            {
                                return true;
                            }
                        }
                    }
                    else
                    {
                        if (lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                        {
                            lexer_next_token(lexer);
                            if (is_statement(lexer, my_node))
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}
/*
jump_statement
    : GOTO IDENTIFIER ';'
    | CONTINUE ';'
    | BREAK ';'
    | RETURN {expression ||""}';'
    ;
*/
int is_jump_statement(lexer_T *lexer, node *my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == GOTO)
    {
        lexer_next_token(lexer);
        if (lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
        {
            lexer_next_token(lexer);
            if (lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
    }
    else if (lexer_peek_next_token(lexer, 0)->type == CONTINUE)
    {
        lexer_next_token(lexer);
        if (lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
        {
            lexer_next_token(lexer);
            return true;
        }
    }
    else if (lexer_peek_next_token(lexer, 0)->type == BREAK)
    {
        lexer_next_token(lexer);
        if (lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
        {
            lexer_next_token(lexer);
            return true;
        }
    }
    if (lexer_peek_next_token(lexer, 0)->type == RETURN)
    {
        lexer_next_token(lexer);
        if (is_expression(lexer, my_node))
        {
            if (lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
        else
        {
            if (lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
    }
    return false;
}
/*
translation_unit
    : external_declaration
    | translation_unit external_declaration
    ;
*/
int p_is_translation_unit(lexer_T *lexer, node *my_node)
{

    return false;
}
int is_translation_unit(lexer_T *lexer, node *my_node)
{
    if (is_external_declaration(lexer, my_node))
    {
        return true;
    }
    else if (p_is_translation_unit(lexer, my_node))
    {
        if (is_external_declaration(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
external_declaration
    : function_definition
    | declaration
    ;
*/
int is_external_declaration(lexer_T *lexer, node *my_node)
{
    if (is_function_definition(lexer, my_node))
    {
        return true;
    }
    else if (is_declaration(lexer, my_node))
    {
        return true;
    }
    return false;
}
/*
function_definition
    : declaration_specifiers declarator {declaration_list || ""} compound_statement
    | declarator {declaration_list || ""} compound_statement
    ;
*/
int is_function_definition(lexer_T *lexer, node *my_node)
{
    // GOAL
    if (is_declaration_specifiers(lexer, my_node))
    {
        // GOAL
        if (is_declarator(lexer, my_node))
        {
            if (is_declaration_list(lexer, my_node))
            {
                if (is_compound_statement(lexer, my_node))
                {
                    return true;
                }
            }
            else
            {
                if (is_compound_statement(lexer, my_node))
                {
                    return true;
                }
            }
        }
    }
    else if (is_declarator(lexer, my_node))
    {

        if (is_declaration_list(lexer, my_node))
        {
            if (is_compound_statement(lexer, my_node))
            {
                return true;
            }
        }
        else
        {
            if (is_compound_statement(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}