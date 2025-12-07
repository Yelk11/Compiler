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

node_T* parse(lexer_T* lexer)
{
    /* create a root node for the AST and let existing parsing
       functions attach children to it */
    node_T *root = init_node(STATEMENT_NODE, init_token("<ROOT>", 0));

    /* start parsing from the top-level; existing functions expect
       a parent node pointer to attach to */
    (void)is_function_definition(lexer, root);

    return root;
}

/*
primary_expression
    : IDENTIFIER
    | CONSTANT
    | STRING_LITERAL
    | '(' expression ')'
    ;
*/
int is_primary_expression(lexer_T* lexer, node_T* my_node)
{
    if (lexer_peek_next_token(lexer,0)->type == L_PARENTHESIS)
    {
        lexer_next_token(lexer);
        if(is_expression(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
    }else if (lexer_peek_next_token(lexer,0)->type == STRING_LITERAL)
    {
        token_T* token = lexer_next_token(lexer);
        node_literal(my_node, token);
        return true;
    }else if (lexer_peek_next_token(lexer,0)->type == CONSTANT)
    {
        token_T* token = lexer_next_token(lexer);
        node_literal(my_node, token);
        return true;
    }else if (lexer_peek_next_token(lexer,0)->type == IDENTIFIER)
    {
        token_T* token = lexer_next_token(lexer);
        node_identifier(my_node, token);
        return true;
    } 
   return false;
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

/*
postfix_expression
    : primary_expression p_postfix_expression
p_postfix_expression
    | '[' expression ']' p_postfix_expression
    | '(' ')' p_postfix_expression
    | '(' argument_expression_list ')' p_postfix_expression
    | '.' IDENTIFIER p_postfix_expression
    | PTR_OP IDENTIFIER p_postfix_expression
    | INC_OP p_postfix_expression
    | DEC_OP p_postfix_expression
    | --nothing--
    ;
*/

int p_is_postfix_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == DEC_OP)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(p_is_postfix_expression(lexer,my_node))
        {
            return true;
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == INC_OP)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(p_is_postfix_expression(lexer,my_node))
        {
            return true;
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == PTR_OP)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
        {
            token_T* token = lexer_next_token(lexer);
            node_identifier(my_node, token);
            if(p_is_postfix_expression(lexer,my_node))
            {
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == PERIOD)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
        {
            token_T* token = lexer_next_token(lexer);
            node_identifier(my_node, token);
            if(p_is_postfix_expression(lexer,my_node))
            {
                return true;
            }
        }
    }
    else if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    {
        lexer_next_token(lexer);
        if(is_argument_expression_list(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                if(p_is_postfix_expression(lexer,my_node))
                {
                    return true;
                }
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    {
        lexer_next_token(lexer);
        if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if(p_is_postfix_expression(lexer,my_node))
            {
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == L_BRACKET)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_expression(lexer, my_node))
        {
            if(p_is_postfix_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_postfix_expression(lexer_T* lexer, node_T* my_node)
{
    if(is_primary_expression(lexer, my_node))
    {
        if(p_is_postfix_expression(lexer, my_node))
        {
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
/*
argument_expression_list
    : assignment_expression p_argument_expression_list
    ;
p_argument_expression_list
    | ',' assignment_expression p_argument_expression_list
    | --nothing--
    ;
*/
int p_is_argument_expression_list(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == COMMA)
    {
        lexer_next_token(lexer);
        if(is_assignment_expression(lexer, my_node))
        {
            if(p_is_argument_expression_list(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_argument_expression_list(lexer_T* lexer, node_T* my_node)
{
    if(is_assignment_expression(lexer, my_node))
    {
        if(p_is_argument_expression_list(lexer, my_node))
        {
            return true;
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
    | SIZEOF unary_expression
    | SIZEOF '(' type_name ')'
    ;
*/
int is_unary_expression(lexer_T* lexer, node_T* my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == SIZEOF)
    {
        lexer_next_token(lexer);
        if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if(is_type_name(lexer, my_node))
            {
                if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
        }else if(is_unary_expression(lexer, my_node))
        {
            return true;
        }
    }else if (is_unary_operator(lexer, my_node))
    {
        lexer_next_token(lexer);
        if(is_cast_expression(lexer, my_node))
        {
            return true;
        }
    }else if (lexer_peek_next_token(lexer, 0)->type == DEC_OP)
    {
        lexer_next_token(lexer);
        if(is_unary_expression(lexer, my_node))
        {
            return true;
        }
    }else if (lexer_peek_next_token(lexer, 0)->type == INC_OP)
    {
        lexer_next_token(lexer);
        if(is_unary_expression(lexer, my_node))
        {
            return true;
        }
    }else if (is_postfix_expression(lexer, my_node))
    {
        return true;
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
int is_unary_operator(lexer_T* lexer, node_T* my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == EXCLAMATION_MARK)
    {
        lexer_next_token(lexer);
        return true;
    }else if (lexer_peek_next_token(lexer, 0)->type == TILDE)
    {
        lexer_next_token(lexer);
        return true;
    }else if (lexer_peek_next_token(lexer, 0)->type == MINUS)
    {
        lexer_next_token(lexer);
        return true;
    }else if (lexer_peek_next_token(lexer, 0)->type == PLUS)
    {
        lexer_next_token(lexer);
        return true;
    }else if (lexer_peek_next_token(lexer, 0)->type == MULTIPLY)
    {
        lexer_next_token(lexer);
        return true;
    }else if (lexer_peek_next_token(lexer, 0)->type == AMPERSAND)
    {
        lexer_next_token(lexer);
        return true;
    }
    return false;
}
/*
cast_expression
    : unary_expression
    | '(' type_name ')' cast_expression
    ;
*/

int is_cast_expression(lexer_T* lexer, node_T* my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    {
        lexer_next_token(lexer);
        if(is_type_name(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                if(is_cast_expression(lexer, my_node))
                {
                    return true;
                }
            }
        }
    }else if (is_unary_expression(lexer, my_node))
    {
        return true;
    }
    return false;
}

/*
multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression
    | multiplicative_expression '/' cast_expression
    | multiplicative_expression '%' cast_expression
    ;
*/
/*
multiplicative_expression
    : cast_expression p_multiplicative_expression
    ;
p_multiplicative_expression
    | '*' cast_expression p_multiplicative_expression
    | '/' cast_expression p_multiplicative_expression
    | '%' cast_expression p_multiplicative_expression
    | --nothing--
    ;
*/
int p_is_multiplicative_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == PERCENT)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_cast_expression(lexer, my_node))
        {
            if(p_is_multiplicative_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == DIVIDE)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_cast_expression(lexer, my_node))
        {
            if(p_is_multiplicative_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == MULTIPLY)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_cast_expression(lexer, my_node))
        {
            if(p_is_multiplicative_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_multiplicative_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_cast_expression(lexer, my_node))
    {
        if(p_is_multiplicative_expression(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}

/*
additive_expression
    : multiplicative_expression p_additive_expression
    ;
p_additive_expression
    | '+' multiplicative_expression p_additive_expression
    | '-' multiplicative_expression p_additive_expression
    | --nothing--
    ;
*/
int p_is_additive_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == MINUS)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_multiplicative_expression(lexer, my_node))
        {
            if(p_is_additive_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == PLUS)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_multiplicative_expression(lexer, my_node))
        {
            if(p_is_additive_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_additive_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_multiplicative_expression(lexer, my_node))
    {
        if(p_is_additive_expression(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
shift_expression
    : additive_expression
    | shift_expression LEFT_OP additive_expression
    | shift_expression RIGHT_OP additive_expression
    ;
*/
/*
shift_expression
    : additive_expression
    ;
p_shift_expression
    | LEFT_OP additive_expression p_shift_expression
    | RIGHT_OP additive_expression p_shift_expression
    | --nothing--
    ;
*/
int p_is_shift_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == RIGHT_OP)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_additive_expression(lexer, my_node))
        {
            if(p_is_shift_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == LEFT_OP)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_additive_expression(lexer, my_node))
        {
            if(p_is_shift_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_shift_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_additive_expression(lexer, my_node))
    {
        if(p_is_shift_expression(lexer, my_node))
        {
            return true;
        }
    }
    return false;
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
/*
relational_expression
    : shift_expression p_relational_expression
    ;
p_relational_expression
    | '<' shift_expression p_relational_expression
    | '>' shift_expression p_relational_expression
    | LE_OP shift_expression p_relational_expression
    | GE_OP shift_expression p_relational_expression
    | --nothing--
    ;
*/
int p_is_relational_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == GE_OP)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_shift_expression(lexer, my_node))
        {
            if(p_is_relational_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == LE_OP)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_shift_expression(lexer, my_node))
        {
            if(p_is_relational_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else 
    if(lexer_peek_next_token(lexer, 0)->type == GREATER_THAN)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_shift_expression(lexer, my_node))
        {
            if(p_is_relational_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == LESS_THAN)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_shift_expression(lexer, my_node))
        {
            if(p_is_relational_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else {
        return true;
    }
    return false;
}
int is_relational_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_shift_expression(lexer, my_node))
    {
        if(p_is_relational_expression(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
equality_expression
    : relational_expression
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;
*/
/*
equality_expression
    : relational_expression p_equality_expression
    ;
p_equality_expression
    | EQ_OP relational_expression p_equality_expression
    | NE_OP relational_expression p_equality_expression
    | --nothing--
    ;
*/
int p_is_equality_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == NE_OP)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_relational_expression(lexer, my_node))
        {
            if(p_is_equality_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == EQ_OP)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_relational_expression(lexer, my_node))
        {
            if(p_is_equality_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_equality_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_relational_expression(lexer, my_node))
    {
        if(p_is_equality_expression(lexer, my_node))
        {
            return true;
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
/*
and_expression
    : equality_expression p_and_expression
    ;
p_and_expression
    | '&' equality_expression p_and_expression
    | --nothing--
    ;
*/
int p_is_and_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == AMPERSAND)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(p_is_and_expression(lexer, my_node))
        {
            return true;
        }
    }else{
        return true;
    }
    return false;
}
int is_and_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_equality_expression(lexer, my_node))
    {
        if(p_is_and_expression(lexer, my_node))
        {
            return true;
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
/*
exclusive_or_expression
    : and_expression p_exclusive_or_expression
    ;
p_exclusive_or_expression
    | '^' and_expression p_exclusive_or_expression
    | --nothing--
    ;
*/
int p_is_exclusive_or_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == CARET)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_and_expression(lexer, my_node))
        {
            if(p_is_exclusive_or_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_exclusive_or_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_and_expression(lexer, my_node))
    {
        if(p_is_exclusive_or_expression(lexer, my_node))
        {
            return true;
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
/*
inclusive_or_expression
    : exclusive_or_expression p_inclusive_or_expression
    ;
p_inclusive_or_expression
    | '|' exclusive_or_expression p_inclusive_or_expression
     | --nothing--
    ;
*/
int p_is_inclusive_or_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == VERTICAL_BAR)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_exclusive_or_expression(lexer, my_node))
        {
            if(p_is_inclusive_or_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_inclusive_or_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_exclusive_or_expression(lexer, my_node))
    {
        if(p_is_inclusive_or_expression(lexer, my_node))
        {
            return true;
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
/*
logical_and_expression
    : inclusive_or_expression p_logical_and_expression
    ;
p_logical_and_expression
    | AND_OP inclusive_or_expression p_logical_and_expression
    ; --nothing--
*/
int p_is_logical_and_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == AND_OP)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_inclusive_or_expression(lexer, my_node))
        {
            if(p_is_logical_and_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
int is_logical_and_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_inclusive_or_expression(lexer, my_node))
    {
        if(p_is_logical_and_expression(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
logical_or_expression
    : logical_and_expression p_logical_or_expression
    ;
p_logical_or_expression
    | OR_OP logical_and_expression p_logical_or_expression
    ; nothing
*/
int p_is_logical_or_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == OR_OP)
    {
        node_operator(my_node, lexer_next_token(lexer));
        if(is_logical_and_expression(lexer, my_node))
        {
            if(p_is_logical_or_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}

int is_logical_or_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_logical_and_expression(lexer, my_node))
    {
        if(p_is_logical_or_expression(lexer, my_node))
        {
            return true;
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
int is_conditional_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_logical_or_expression(lexer, my_node))
    {
        if(lexer_peek_next_token(lexer, 0)->type == QUESTION_MARK)
        {
            node_control_flow(my_node, lexer_next_token(lexer));
            if(is_expression(lexer, my_node))
            {
                if(lexer_peek_next_token(lexer, 0)->type == COLON)
                {
                    node_control_flow(my_node, lexer_next_token(lexer));
                    if(is_conditional_expression(lexer, my_node))
                    {
                        return true;
                    }
                }
            }
        }else{
            return true;
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
int is_assignment_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_unary_expression(lexer, my_node))
    {
        /* check and consume assignment operators here and create nodes */
        int t = lexer_peek_next_token(lexer, 0)->type;
        if(t == EQUALS || t == MUL_ASSIGN || t == DIV_ASSIGN || t == MOD_ASSIGN ||
           t == ADD_ASSIGN || t == SUB_ASSIGN || t == LEFT_ASSIGN || t == RIGHT_ASSIGN ||
           t == AND_ASSIGN || t == XOR_ASSIGN || t == OR_ASSIGN)
        {
            node_expression(my_node, lexer_next_token(lexer));
            if(is_assignment_expression(lexer, my_node))
            {
                return true;
            }
        }
    }
    else if (is_conditional_expression(lexer, my_node))
    {
        return true;
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
int is_assignment_operator(lexer_T* lexer, node_T* my_node)
{
    int t = lexer_peek_next_token(lexer, 0)->type;
    if (t == EQUALS || t == MUL_ASSIGN || t == DIV_ASSIGN || t == MOD_ASSIGN ||
        t == ADD_ASSIGN || t == SUB_ASSIGN || t == LEFT_ASSIGN || t == RIGHT_ASSIGN ||
        t == AND_ASSIGN || t == XOR_ASSIGN || t == OR_ASSIGN)
    {
        node_expression(my_node, lexer_next_token(lexer));
        return true;
    }
    return false;
}
/*
expression
    : assignment_expression
    | expression ',' assignment_expression
    ;
*/
/*
expression
    : assignment_expression p_expression
    ;
p_expression
    | ',' assignment_expression p_expression
    ;nothing
*/
int p_is_expression(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == COMMA)
    {
        node_expression(my_node, lexer_next_token(lexer));
        if(is_assignment_expression(lexer, my_node))
        {
            if(p_is_expression(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_expression(lexer_T* lexer, node_T* my_node)
{
    if (is_assignment_expression(lexer, my_node))
    {
        if(p_is_expression(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
constant_expression
    : conditional_expression
    ;
*/
int is_constant_expression(lexer_T* lexer, node_T* my_node)
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
int is_declaration(lexer_T* lexer, node_T* my_node)
{
    if(is_declaration_specifiers(lexer, my_node))
    {
        if(is_init_declarator_list(lexer, my_node)){
            if(lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
            {
                return true;
            }
        }else if(lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
        {
            lexer_next_token(lexer);
            return true;
        }
    }
    return false;
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
int is_declaration_specifiers(lexer_T* lexer, node_T* my_node)
{
    if(is_storage_class_specifier(lexer, my_node))
    {
        if(is_declaration_specifiers(lexer, my_node))
        {
            return true;
        }else{
            return true;
        }
    } else if(is_type_specifier(lexer, my_node)){
        
        if(is_declaration_specifiers(lexer, my_node))
        {
            return true;
        }else{
            return true;
        }
    }else if(is_storage_class_specifier(lexer, my_node)){
        
        if(is_declaration_specifiers(lexer, my_node))
        {
            return true;
        }else{
            return true;
        }
    }else{
        return false;
    }
    return true;
}
/*
init_declarator_list
    : init_declarator
    | init_declarator_list ',' init_declarator
    ;
*/
/*
init_declarator_list
    : init_declarator p_init_declarator_list
    ;
p_init_declarator_list
    | ',' init_declarator p_init_declarator_list
    ; nothing
*/
int p_is_init_declarator_list(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == COMMA)
    {
        lexer_next_token(lexer);
        if(is_init_declarator(lexer, my_node))
        {
            if(p_is_init_declarator_list(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_init_declarator_list(lexer_T* lexer, node_T* my_node)
{
    if (is_init_declarator(lexer, my_node))
    {
        if(p_is_init_declarator_list(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
init_declarator
    : declarator
    | declarator '=' initializer
    ;
*/
int is_init_declarator(lexer_T* lexer, node_T* my_node)
{
    if (is_declarator(lexer, my_node))
    {
        if(lexer_peek_next_token(lexer, 0)->type == EQUALS)
        {
            lexer_next_token(lexer);
            if(is_initializer(lexer, my_node))
            {
                return true;
            }
        }
    }else if (is_declarator(lexer, my_node))
    {
        return true;
    }
    return false;
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
int is_storage_class_specifier(lexer_T* lexer, node_T* my_node)
{
    if (lexer_peek_next_token(lexer,0)->type == TYPEDEF)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer,0)->type== EXTERN)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer,0)->type == STATIC)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer,0)->type == AUTO)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (lexer_peek_next_token(lexer,0)->type == REGISTER)
    {
        lexer_next_token(lexer);
        return true;
    }
    return false;
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
int is_type_specifier(lexer_T* lexer, node_T* my_node)
{
    token_T* token = lexer_peek_next_token(lexer, 0);
    if (token->type== VOID)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (token->type == CHAR)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (token->type== SHORT)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (token->type == INT)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (token->type == LONG)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (token->type == FLOAT)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (token->type == DOUBLE)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (token->type == SIGNED)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (token->type == UNSIGNED)
    {
        lexer_next_token(lexer);
        return true;
    }
    else if (is_enum_specifier(lexer, my_node))
    {
        return true;
    }
    else if (is_struct_or_union_specifier(lexer, my_node))
    {
        return true;
    }
    return false;
}
/*
struct_or_union_specifier
    : struct_or_union IDENTIFIER '{' struct_declaration_list '}'
    | struct_or_union '{' struct_declaration_list '}'
    | struct_or_union IDENTIFIER
    ;
*/
int is_struct_or_union_specifier(lexer_T* lexer, node_T* my_node)
{
    if(is_struct_or_union(lexer, my_node))
    {
        if(lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
        {
            token_T* token = lexer_next_token(lexer);
            node_expression(my_node, token);
            if(lexer_peek_next_token(lexer, 0)->type == L_BRACE)
            {
                lexer_next_token(lexer);
                if(is_struct_declaration_list(lexer, my_node))
                {
                    if(lexer_peek_next_token(lexer, 0)->type == R_BRACE)
                    {
                        lexer_next_token(lexer);
                        return true;
                    }
                }
            }else{
                return true;
            }
        }else if(lexer_peek_next_token(lexer, 0)->type == L_BRACE)
        {
            lexer_next_token(lexer);
            if(is_struct_declaration_list(lexer, my_node))
            {
                if(lexer_peek_next_token(lexer, 0)->type == R_BRACE)
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
struct_or_union
    : STRUCT
    | UNION
    ;
*/
int is_struct_or_union(lexer_T* lexer, node_T* my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == STRUCT)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == UNION)
    {
        return true;
    }
    return false;
}
/*
struct_declaration_list
    : struct_declaration
    | struct_declaration_list struct_declaration
    ;
*/
/*
struct_declaration_list
    : struct_declaration p_struct_declaration_list
    ;
p_struct_declaration_list
    | struct_declaration p_struct_declaration_list
    ; nothing
*/
int p_is_struct_declaration_list(lexer_T* lexer, node_T* my_node)
{
    if(is_struct_declaration(lexer, my_node))
    {
        if(p_is_struct_declaration_list(lexer, my_node))
        {
            return true;
        }
    }else{
        return true;
    }
    return false;
}
int is_struct_declaration_list(lexer_T* lexer, node_T* my_node)
{
    if (is_struct_declaration(lexer, my_node))
    {
        if(p_is_struct_declaration_list(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
struct_declaration
    : specifier_qualifier_list struct_declarator_list ';'
    ;
*/
int is_struct_declaration(lexer_T* lexer, node_T* my_node)
{
    if (is_specifier_qualifier_list(lexer, my_node))
    {
        if(is_struct_declarator_list(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
    }
    return false;
}
/*
specifier_qualifier_list
    : type_specifier specifier_qualifier_list
    | type_specifier
    | type_qualifier specifier_qualifier_list
    | type_qualifier
    ;
*/
int is_specifier_qualifier_list(lexer_T* lexer, node_T* my_node)
{
    if(is_type_qualifier(lexer, my_node))
    {
        if(is_specifier_qualifier_list(lexer, my_node))
        {
            return true;
        }else{
            return false;
        }
    }else if (is_type_specifier(lexer, my_node))
    {
        if(is_specifier_qualifier_list(lexer, my_node))
        {
            return true;
        }else{
            return false;
        }
    }
    return false;
}
/*
struct_declarator_list
    : struct_declarator
    | struct_declarator_list ',' struct_declarator
    ;
*/
/*
struct_declarator_list
    : struct_declarator p_struct_declarator_list
    ;
p_struct_declarator_list
    | ',' struct_declarator p_struct_declarator_list
    ; nothing
*/

int p_is_struct_declarator_list(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == COMMA)
    {
        lexer_next_token(lexer);
        if(p_is_struct_declarator_list(lexer, my_node))
        {
            return true;
        }
    }else{
        return true;
    }
    return false;
}
int is_struct_declarator_list(lexer_T* lexer, node_T* my_node)
{
    if (is_struct_declarator(lexer, my_node))
    {
        if(p_is_struct_declarator_list(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
struct_declarator
    : declarator
    | ':' constant_expression
    | declarator ':' constant_expression
    ;
*/
int is_struct_declarator(lexer_T* lexer, node_T* my_node)
{
    if (is_declarator(lexer, my_node))
    {
        if(lexer_peek_next_token(lexer, 0)->type == COLON)
        {
            lexer_next_token(lexer);
            if(is_constant_expression(lexer, my_node))
            {
                return true;
            }
        }else{
            return true;
        }
    }
    else if (lexer_peek_next_token(lexer, 0)->type == COLON)
    {
        if(is_constant_expression(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
enum_specifier
    : ENUM '{' enumerator_list '}'
    | ENUM IDENTIFIER '{' enumerator_list '}'
    | ENUM IDENTIFIER
    ;
*/
int is_enum_specifier(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == ENUM)
    {
        lexer_next_token(lexer);
        if(lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
        {
            token_T* token = lexer_next_token(lexer);
            node_expression(my_node, token);
            if(lexer_peek_next_token(lexer, 0)->type == L_BRACE)
            {
                lexer_next_token(lexer);
                if(is_enumerator_list(lexer, my_node))
                {
                    if(lexer_peek_next_token(lexer, 0)->type == R_BRACE)
                    {
                        lexer_next_token(lexer);
                        return true;
                    }
                }
            }else{
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == L_BRACE)
    {
        if(lexer_peek_next_token(lexer, 0)->type == L_BRACE)
        {
            lexer_next_token(lexer);
            if(is_enumerator_list(lexer, my_node))
            {
                if(lexer_peek_next_token(lexer, 0)->type == R_BRACE)
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
enumerator_list
    : enumerator
    | enumerator_list ',' enumerator
    ;
*/
/*
enumerator_list
    : enumerator p_enumerator_list
    ;
p_enumerator_list
    | ',' enumerator p_enumerator_list
    ; nothing
*/
int p_is_enumerator_list(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == COMMA)
    {
        lexer_next_token(lexer);
        if(is_enumerator(lexer, my_node))
        {
            if(p_is_enumerator_list(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_enumerator_list(lexer_T* lexer, node_T* my_node)
{
    if (is_enumerator(lexer, my_node))
    {
        if(p_is_enumerator_list(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
enumerator
    : IDENTIFIER
    | IDENTIFIER '=' constant_expression
    ;
*/
int is_enumerator(lexer_T* lexer, node_T* my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
    {
        token_T* token = lexer_next_token(lexer);
        node_expression(my_node, token);
        if(lexer_peek_next_token(lexer, 0)->type == EQUALS)
        {
            lexer_next_token(lexer);
            if(is_constant_expression(lexer, my_node))
            {
                return true;
            }
        }else{
            return true;
        }
    }
    return false;
}
/*
type_qualifier
    : CONST
    | VOLATILE
    ;
*/
int is_type_qualifier(lexer_T* lexer, node_T* my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == CONST)
    {
        return true;
    }
    else if (lexer_peek_next_token(lexer, 0)->type == VOLATILE)
    {
        return true;
    }
    return false;
}
/*
declarator
    : pointer direct_declarator
    | direct_declarator
    ;
*/
int is_declarator(lexer_T* lexer, node_T* my_node)
{
    if(is_pointer(lexer, my_node)){
        if(is_direct_declarator(lexer, my_node)){
            return true;
        }
    }else if(is_direct_declarator(lexer, my_node)){
        return true;
    }
    return false;
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
/*
direct_declarator
    : IDENTIFIER p_direct_declarator
    | '(' declarator ')' p_direct_declarator
    ;
p_direct_declarator
    | '[' constant_expression ']' p_direct_declarator
    | '[' ']' p_direct_declarator
    | '(' parameter_type_list ')' p_direct_declarator
    | '(' identifier_list ')' p_direct_declarator
    | '(' ')' p_direct_declarator
    ; nothing
*/
int p_is_direct_declarator(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    {
        lexer_next_token(lexer);
        if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if(p_is_direct_declarator(lexer,my_node))
            {
                return true;
            }
        }
        else if(is_identifier_list(lexer, my_node))
        {
            token_T* token = lexer_peek_next_token(lexer, 0);
            node_expression(my_node, token);
            if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                if(p_is_direct_declarator(lexer,my_node))
                {
                    return true;
                }
            }
        }else if(is_parameter_type_list(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                if(p_is_direct_declarator(lexer,my_node))
                {
                    return true;
                }
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == L_BRACKET)
    {
        lexer_next_token(lexer);
        if(lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
        {
            lexer_next_token(lexer);
            if(p_is_direct_declarator(lexer,my_node))
            {
                return true;
            }
        }else if(is_constant_expression(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
            {
                lexer_next_token(lexer);
                if(p_is_direct_declarator(lexer,my_node))
                {
                    return true;
                }
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_direct_declarator(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    {
        if(is_declarator(lexer, my_node))
        {
            lexer_next_token(lexer);
            if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                if(p_is_direct_declarator(lexer, my_node))
                {
                    return true;
                }
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
    {
        lexer_next_token(lexer);
        node_expression(my_node,IDENTIFIER);
        if(p_is_direct_declarator(lexer, my_node))
        {
            return true;
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
int is_pointer(lexer_T* lexer, node_T* my_node)
{
    if (lexer_peek_next_token(lexer, 0)->type == MULTIPLY)
    {
        lexer_next_token(lexer);
        if(is_type_qualifier_list(lexer, my_node))
        {
            if(is_pointer(lexer, my_node))
            {
                return true;
            }
            else
            {
                return true;
            }
        }else if(is_pointer(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
type_qualifier_list
    : type_qualifier
    | type_qualifier_list type_qualifier
    ;
*/
/*
type_qualifier_list
    : type_qualifier p_type_qualifier_list
    ;
p_type_qualifier_list
    | type_qualifier p_type_qualifier_list
    ; nothing
*/
int p_is_type_qualifier_list(lexer_T* lexer, node_T* my_node)
{
    if(is_type_qualifier(lexer, my_node))
    {
        if(p_is_type_qualifier_list(lexer, my_node))
        {
            return true;
        }
    }else{
        return true;
    }
    return false;
}
int is_type_qualifier_list(lexer_T* lexer, node_T* my_node)
{
    if (is_type_qualifier(lexer, my_node))
    {
        if(p_is_type_qualifier_list(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
parameter_type_list
    : parameter_list
    | parameter_list ',' ELLIPSIS
    ;
*/
int is_parameter_type_list(lexer_T* lexer, node_T* my_node)
{
    if (is_parameter_list(lexer, my_node))
    {
        if(lexer_peek_next_token(lexer, 0)->type == COMMA)
        {
            lexer_next_token(lexer);
            if(lexer_peek_next_token(lexer, 0)->type == ELLIPSIS)
            {
                lexer_next_token(lexer);
                return true;
            }
        }else{
            return true;
        }
    }
    return false;
}
/*
parameter_list
    : parameter_declaration
    | parameter_list ',' parameter_declaration
    ;
*/
/*
parameter_list
    : parameter_declaration p_parameter_list
    ;
p_parameter_list
    | ',' parameter_declaration p_parameter_list
    ; nothing
*/
int p_is_parameter_list(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == COMMA)
    {
        lexer_next_token(lexer);
        if(is_parameter_declaration(lexer, my_node))
        {
            if(p_is_parameter_list(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_parameter_list(lexer_T* lexer, node_T* my_node)
{
    if (is_parameter_declaration(lexer, my_node))
    {
        if(p_is_parameter_list(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
parameter_declaration
    : declaration_specifiers declarator
    | declaration_specifiers abstract_declarator
    | declaration_specifiers
    ;
*/

int is_parameter_declaration(lexer_T* lexer, node_T* my_node)
{
    if(is_declaration_specifiers(lexer, my_node))
    {
        if(is_abstract_declarator(lexer, my_node))
        {
            return true;
        }else if(is_declarator(lexer, my_node))
        {
            return true;
        }else{
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
/*
identifier_list
    : IDENTIFIER p_identifier_list
    ;
p_identifier_list 
    | ',' IDENTIFIER p_identifier_list
    ; nothing
*/
int p_is_identifier_list(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == COMMA)
    {
        lexer_next_token(lexer);
        if(lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
        {
            lexer_next_token(lexer);
            node_expression(my_node,IDENTIFIER);
            if(p_is_identifier_list(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_identifier_list(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
    {
        lexer_next_token(lexer);
        node_expression(my_node,IDENTIFIER);
        if(p_is_identifier_list(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
type_name
    : specifier_qualifier_list
    | specifier_qualifier_list abstract_declarator
    ;
*/
int is_type_name(lexer_T* lexer, node_T* my_node)
{
    if (is_specifier_qualifier_list(lexer, my_node))
    {
        if(is_abstract_declarator(lexer, my_node))
        {
            return true;
        }else{
            return true;
        }
    }
    return false;
}
/*
abstract_declarator
    : pointer
    | direct_abstract_declarator
    | pointer direct_abstract_declarator
    ;
*/
int is_abstract_declarator(lexer_T* lexer, node_T* my_node)
{
    if (is_pointer(lexer, my_node))
    {
        if (is_direct_abstract_declarator(lexer, my_node))
        {
            return true;
        }else{
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
/*
direct_abstract_declarator
    : '(' abstract_declarator ')' p_direct_abstract_declarator
    | '[' ']' p_direct_abstract_declarator
    | '[' constant_expression ']' p_direct_abstract_declarator
    | '(' ')' p_direct_abstract_declarator
    | '(' parameter_type_list ')' p_direct_abstract_declarator
    ;
p_direct_abstract_declarator
    | '[' ']' p_direct_abstract_declarator
    | '[' constant_expression ']' p_direct_abstract_declarator
    | '(' ')' p_direct_abstract_declarator
    | '(' parameter_type_list ')' p_direct_abstract_declarator
    ; nothing
*/
int p_is_direct_abstract_declarator(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    {
        lexer_next_token(lexer);
        if(is_parameter_type_list(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                if(p_is_direct_abstract_declarator(lexer, my_node))
                {
                    return true;
                }
            }
        }else if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if(p_is_direct_abstract_declarator(lexer, my_node))
            {
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == L_BRACKET)
    {
        lexer_next_token(lexer);
        if(is_constant_expression(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
            {
                lexer_next_token(lexer);
                if(p_is_direct_abstract_declarator(lexer, my_node))
                {
                    return true;
                }
            }
        }else if(lexer_peek_next_token(lexer, 0)->type == R_BRACKET)
        {
            lexer_next_token(lexer);
            if(p_is_direct_abstract_declarator(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_direct_abstract_declarator(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
    {
        lexer_next_token(lexer);
        if(is_parameter_list(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                if(p_is_direct_abstract_declarator(lexer, my_node))
                {
                    return true;
                }
            }
        }else if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if(p_is_direct_abstract_declarator(lexer, my_node))
            {
                return true;
            }
        }else if(is_abstract_declarator(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
            {
                lexer_next_token(lexer);
                if(p_is_direct_abstract_declarator(lexer, my_node))
                {
                    return true;
                }
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == L_BRACKET)
    {
        lexer_next_token(lexer);
        if(is_constant_expression(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == L_BRACKET)
            {
                lexer_next_token(lexer);
                if(p_is_direct_abstract_declarator(lexer, my_node))
                {
                    return true;
                }
            }
        }else if(lexer_peek_next_token(lexer, 0)->type == L_BRACKET)
        {
            lexer_next_token(lexer);
            if(p_is_direct_abstract_declarator(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
initializer
    : assignment_expression
    | '{' initializer_list '}'
    | '{' initializer_list ',' '}'
    ;
*/
int is_initializer(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == L_BRACE)
    {
        lexer_next_token(lexer);
        if(is_initializer_list(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == COMMA)
            {
                lexer_next_token(lexer);
                if(lexer_peek_next_token(lexer, 0)->type == R_BRACE)
                {
                    lexer_next_token(lexer);
                    return true;
                }
            }
            else if(lexer_peek_next_token(lexer, 0)->type == R_BRACE)
            {
                lexer_next_token(lexer);
                return true;
            }
        }
    }else if(is_assignment_expression(lexer, my_node))
    {
        return true;
    }
    return false;
}
/*
initializer_list
    : initializer
    | initializer_list ',' initializer
    ;
*/
/*
initializer_list
    : initializer p_initializer_list
    ;
p_initializer_list 
    | ',' initializer p_initializer_list
    ; nothing
*/
int p_is_initializer_list(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == COMMA)
    {
        lexer_next_token(lexer);
        if(is_initializer(lexer, my_node))
        {
            if(p_is_initializer_list(lexer, my_node))
            {
                return true;
            }
        }
    }else{
        return true;
    }
    return false;
}
int is_initializer_list(lexer_T* lexer, node_T* my_node)
{
    if(is_initializer(lexer, my_node))
    {
        if(p_is_initializer_list(lexer, my_node))
        {
            return true;
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
int is_statement(lexer_T* lexer, node_T* my_node)
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
int is_labeled_statement(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == DEFAULT)
    {
        lexer_next_token(lexer);
        if(lexer_peek_next_token(lexer, 0)->type == COLON)
        {
            lexer_next_token(lexer);
            if(is_statement(lexer, my_node))
            {
                return true;
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == CASE)
    {
        lexer_next_token(lexer);
        if(is_constant_expression(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == COLON)
            {
                lexer_next_token(lexer);
                if(is_statement(lexer, my_node))
                {
                    return true;
                }
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
    {
        lexer_next_token(lexer);
        node_expression(my_node,IDENTIFIER);
        if(lexer_peek_next_token(lexer, 0)->type == COLON)
        {
            lexer_next_token(lexer);
            if(is_statement(lexer, my_node))
            {
                return true;
            }
        }
    }
    return false;
}
/*
compound_statement
    : '{' '}'
    | '{' statement_list '}'
    | '{' declaration_list '}'
    | '{' declaration_list statement_list '}'
    ;
*/
int is_compound_statement(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == L_BRACE){
        lexer_next_token(lexer);
        if(is_statement_list(lexer, my_node)){
            if(lexer_peek_next_token(lexer, 0)->type == R_BRACE){
                lexer_next_token(lexer);
                return true;
            }
        }else if(is_declaration_list(lexer, my_node)){
            if(is_statement_list(lexer, my_node)){
                if(lexer_peek_next_token(lexer, 0)->type == R_BRACE){
                    lexer_next_token(lexer);
                    return true;
                }
            }else if(lexer_peek_next_token(lexer, 0)->type == R_BRACE){
                lexer_next_token(lexer);
                return true;
            }
        }else if(lexer_peek_next_token(lexer, 0)->type == R_BRACE){
            lexer_next_token(lexer);
            return true;
        }
    }
    return false;
}
/*
declaration_list
    : declaration
    | declaration_list declaration
    ;
*/
/*
declaration_list
    : declaration p_declaration_list
    ;
p_declaration_list 
    | declaration p_declaration_list
    ; nothing
*/
int p_is_declaration_list(lexer_T* lexer, node_T* my_node)
{
    if (is_declaration(lexer, my_node))
    {
        if(p_is_declaration_list(lexer, my_node))
        {
            return true;
        }
    }else{
        return true;
    }
    return false;
}
int is_declaration_list(lexer_T* lexer, node_T* my_node)
{
    if (is_declaration(lexer, my_node))
    {
        if(p_is_declaration_list(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
statement_list
    : statement
    | statement_list statement
    ;
*/
/*
statement_list
    : statement p_statement_list
    ;
p_statement_list 
    | statement p_statement_list
    ; nothing
*/
int p_is_statement_list(lexer_T* lexer, node_T* my_node)
{
    if(is_statement(lexer, my_node))
    {
        if(p_is_statement_list(lexer, my_node))
        {
            return true;
        }
    }else{
        return true;
    }
    return false;
}
int is_statement_list(lexer_T* lexer, node_T* my_node)
{
    if(is_statement(lexer, my_node))
    {
        if(p_is_statement_list(lexer, my_node))
        {
            return true;
        }
    }
    return false;
}
/*
expression_statement
    : ';'
    | expression ';'
    ;
*/
int is_expression_statement(lexer_T* lexer, node_T* my_node)
{
    if (is_expression(lexer, my_node))
    {
        if(lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
        {
            lexer_next_token(lexer);
            return true;
        }
    }else if (lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
    {
        lexer_next_token(lexer);
        return true;
    }
    return false;
}
/*
selection_statement
    : IF '(' expression ')' statement
    | IF '(' expression ')' statement ELSE statement
    | SWITCH '(' expression ')' statement
    ;
*/
int is_selection_statement(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == SWITCH)
    {
        lexer_next_token(lexer);
        if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if(is_expression(lexer, my_node))
            {
                if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    if(is_statement(lexer, my_node))
                    {
                        return true;
                    }
                }
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == IF)
    {
        lexer_next_token(lexer);
        if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if(is_expression(lexer, my_node))
            {
                if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    if(is_statement(lexer, my_node))
                    {
                        return true;
                    }else if(lexer_peek_next_token(lexer, 0)->type == ELSE){
                        lexer_next_token(lexer);
                        if(is_statement(lexer, my_node))
                        {
                            return true;
                        }
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
    | FOR '(' expression_statement expression_statement ')' statement
    | FOR '(' expression_statement expression_statement expression ')' statement
    ;
*/
int is_iteration_statement(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == FOR)
    {
        lexer_next_token(lexer);
        if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if(is_expression_statement(lexer, my_node))
            {
                if(is_expression_statement(lexer, my_node))
                {
                    if(is_expression(lexer, my_node))
                    {
                        if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                        {
                            lexer_next_token(lexer);
                            if(is_statement(lexer, my_node))
                            {
                                return true;
                            }
                        }
                    }else if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                    {
                        lexer_next_token(lexer);
                        if(is_statement(lexer, my_node))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == DO)
    {
        if(is_statement(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == WHILE)
            {
                lexer_next_token(lexer);
                if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    if(is_expression(lexer, my_node))
                    {
                        if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                        {
                            lexer_next_token(lexer);
                            if(lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
                            {
                                lexer_next_token(lexer);
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == WHILE)
    {
        lexer_next_token(lexer);
        if(lexer_peek_next_token(lexer, 0)->type == L_PARENTHESIS)
        {
            lexer_next_token(lexer);
            if(is_expression(lexer, my_node))
            {
                if(lexer_peek_next_token(lexer, 0)->type == R_PARENTHESIS)
                {
                    lexer_next_token(lexer);
                    if(is_statement(lexer, my_node))
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
jump_statement
    : GOTO IDENTIFIER ';'
    | CONTINUE ';'
    | BREAK ';'
    | RETURN ';'
    | RETURN expression ';'
    ;
*/
int is_jump_statement(lexer_T* lexer, node_T* my_node)
{
    if(lexer_peek_next_token(lexer, 0)->type == RETURN)
    {
        lexer_next_token(lexer);
        if(is_expression(lexer, my_node))
        {
            if(lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
            {
                return true;
            }
        }else if(lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
        {
            return true;
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == BREAK)
    {
        if(lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
        {
            return true;
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == CONTINUE)
    {
        if(lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
        {
            return true;
        }
    }else if(lexer_peek_next_token(lexer, 0)->type == GOTO)
    {
        if(lexer_peek_next_token(lexer, 0)->type == IDENTIFIER)
        {
            // node_expression(my_node, token);
            if(lexer_peek_next_token(lexer, 0)->type == SEMICOLON)
            {
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
/*
translation_unit
    : external_declaration p_translation_unit
    ;
p_translation_unit
    | external_declaration p_translation_unit
    ; nothing
*/
int p_is_translation_unit(lexer_T* lexer, node_T* my_node)
{
    if(is_external_declaration(lexer, my_node))
    {
        if(p_is_translation_unit(lexer, my_node))
        {
            return true;
        }
    }else{
        return true;
    }
    return false;
}
int is_translation_unit(lexer_T* lexer, node_T* my_node)
{
    if(is_external_declaration(lexer, my_node))
    {
        if(p_is_translation_unit(lexer, my_node))
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
int is_external_declaration(lexer_T* lexer, node_T* my_node)
{
    if (is_declaration(lexer, my_node))
    {
        return true;
    }
    else if (is_function_definition(lexer, my_node))
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
int is_function_definition(lexer_T* lexer, node_T* my_node)
{
    if(is_declarator(lexer, my_node))
    {
        if(is_compound_statement(lexer, my_node))
        {
            return true;
        }else if(is_declaration_list(lexer, my_node))
        {
            if(is_compound_statement(lexer, my_node))
            {
                return true;
            }
        }
    }else if(is_declaration_specifiers(lexer, my_node)){
        if(is_declarator(lexer, my_node)){
            if (is_compound_statement(lexer, my_node))
            {
                return true;
            }else if(is_declaration_list(lexer, my_node)){
                if(is_compound_statement(lexer, my_node)){
                    return true;
                }
            }
        }
    }
    return false;
}