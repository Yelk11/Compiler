#include "parser.h"
#include "token.h"
#include "ast.h"
#include <stdlib.h>
/*
decl* init_decl(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next);

stmt* init_stmt(enum stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr,
                    struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next);

expr* init_expr(enum expr_t kind, struct expr *left, struct expr *right, const char *name,
                    int integer_value, const char *string_literal);
*/
parser_T *init_parser(lexer_T *lexer)
{
    parser_T *parser = calloc(1, sizeof(struct parser_T));
    parser->lexer = lexer;
    parser->token = lexer_next_token(parser->lexer);
    return parser;
}

token_T *parser_eat(parser_T *parser)
{
    parser->token = lexer_next_token(parser->lexer);

    return parser->token;
}

/*
primary_expression
    : IDENTIFIER
    | CONSTANT <-- not supported yet
    | STRING_LITERAL
    | '(' expression ')'
    ;
*/
expr *make_primary_expression(lexer_T *lexer)
{

    token_T *token = lexer_next_token(lexer);

    switch (token->type)
    {
    case TOKEN_ID:
        return init_expr(EXPR_NAME, NULL, NULL, type_to_string(token->type), 0, token->value);
    case TOKEN_STRING:
        return init_expr(EXPR_STRING_LITERAL, NULL, NULL, type_to_string(token->type), 0, token->value);
    case TOKEN_LPAREN:
        return make_expression(lexer);
    default:
        printf("%s not supported", token->value);
        return NULL;
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

expr *make_postfix_expression(lexer_T *lexer)
{
    return NULL;
}

/*
argument_expression_list
    : assignment_expression
    | argument_expression_list ',' assignment_expression
    ;

*/

expr *make_argument_experssion_list(lexer_T *lexer)
{
    return NULL;
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
expr *make_unary_expression(lexer_T *lexer)
{
    return NULL;
}
/*
unary_operator <-- expression?
    : '&'
    | '*'
    | '+'
    | '-'
    | '~'
    | '!'
    ;
*/
expr *make_unary_operator(lexer_T *lexer)
{
    return NULL;
}
/*
cast_expression
    : unary_expression
    | '(' type_name ')' cast_expression
    ;
*/
expr *make_cast_expression(lexer_T *lexer)
{
    return NULL;
}
/*
multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression
    | multiplicative_expression '/' cast_expression
    | multiplicative_expression '%' cast_expression
    ;
*/
expr *make_multiplicative_expression(lexer_T *lexer)
{
    return NULL;
}
/*
additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;
*/
expr *make_additive_expression(lexer_T *lexer)
{
    return NULL;
}

/*
shift_expression
    : additive_expression
    | shift_expression LEFT_OP additive_expression
    | shift_expression RIGHT_OP additive_expression
    ;

*/
expr* make_shift_expression(lexer_T* lexer)
{
    return NULL;
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
expr* make_relational_expression(lexer_T* lexer)
{
    return NULL;
}
/*
equality_expression
    : relational_expression
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;

*/
expr* make_equality_expression(lexer_T* lexer)
{
    return NULL;
}
/*

and_expression
    : equality_expression
    | and_expression '&' equality_expression
    ;

*/
expr* make_and_expression(lexer_T* lexer)
{
    return NULL;
}
/*

exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression
    ;
*/
expr* make_exclusive_or_expression(lexer_T* lexer) // TODO
{
    return NULL;
}
/*


inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression
    ;

*/
expr* make_inclusive_or_expression(lexer_T* lexer)
{
    return NULL;
}
/*

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression AND_OP inclusive_or_expression
    ;


*/
expr* make_logical_and_expression(lexer_T* lexer)
{
    return NULL;
}
/*
logical_or_expression
    : logical_and_expression
    | logical_or_expression OR_OP logical_and_expression
    ;
*/
expr* make_logical_or_expression(lexer_T* lexer)
{
    return NULL;
}
/*
conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression
    ;

*/
expr* make_conditional_expression(lexer_T* lexer)
{
    return NULL;
}
/*

assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression
    ;

*/
expr* make_assignment_expression(lexer_T* lexer)
{
    return NULL;
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
expr* make_assignment_operator(lexer_T* lexer)
{
    return NULL;
}
/*

expression
    : assignment_expression
    | expression ',' assignment_expression
    ;

*/
expr *make_expression(lexer_T *lexer)
{
    return NULL;
}

/*

constant_expression
    : conditional_expression
    ;

*/
expr* make_constant_expression(lexer_T* lexer)
{
    return NULL;
}
/*

declaration
    : declaration_specifiers ';'
    | declaration_specifiers init_declarator_list ';'
    ;

*/
decl* make_declaration(lexer_T* lexer)
{
    return NULL;
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
decl* make_declaration_specifiers(lexer_T* lexer)
{
    return NULL;
}
/*

init_declarator_list
    : init_declarator
    | init_declarator_list ',' init_declarator
    ;

*/
decl* make_init_declarator_list(lexer_T* lexer)
{
    return NULL;
}
/*

init_declarator
    : declarator
    | declarator '=' initializer
    ;
*/
decl* make_init_declarator(lexer_T* lexer)
{
    return NULL;
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
decl* make_storage_class_specifier(lexer_T* lexer)
{
    return NULL;
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
    | TYPE_NAME
    ;
*/
decl* make_type_specifier(lexer_T* lexer)
{
    return NULL;
}
/*
struct_or_union_specifier
    : struct_or_union IDENTIFIER '{' struct_declaration_list '}'
    | struct_or_union '{' struct_declaration_list '}'
    | struct_or_union IDENTIFIER
    ;
*/
decl* make_struct_or_union_specifier(lexer_T* lexer)
{
    return NULL;
}
/*
struct_or_union
    : STRUCT
    | UNION
    ;
*/
decl* make_struct_or_union(lexer_T* lexer)
{
    return NULL;
}
/*
struct_declaration_list
    : struct_declaration
    | struct_declaration_list struct_declaration
    ;
*/
decl* make_struct_declaration_list(lexer_T* lexer)
{
    return NULL;
}
/*
struct_declaration
    : specifier_qualifier_list struct_declarator_list ';'
    ;
*/
decl* make_struct_declaration(lexer_T* lexer)
{
    return NULL;
}
/*
specifier_qualifier_list
    : type_specifier specifier_qualifier_list
    | type_specifier
    | type_qualifier specifier_qualifier_list
    | type_qualifier
    ;
*/
decl* make_specifier_qualifier_list(lexer_T* lexer)
{
    return NULL;
}
/*
struct_declarator_list
    : struct_declarator
    | struct_declarator_list ',' struct_declarator
    ;
*/
decl* make_struct_declarator_list(lexer_T* lexer)
{
    return NULL;
}
/*
struct_declarator
    : declarator
    | ':' constant_expression
    | declarator ':' constant_expression
    ;
*/
decl* make_struct_declarator(lexer_T* lexer)
{
    return NULL;
}
/*
enum_specifier
    : ENUM '{' enumerator_list '}'
    | ENUM IDENTIFIER '{' enumerator_list '}'
    | ENUM IDENTIFIER
    ;
*/
decl* make_enum_specifier(lexer_T* lexer)
{
    return NULL;
}
/*
enumerator_list
    : enumerator
    | enumerator_list ',' enumerator
    ;
*/
decl* make_enumerator_list(lexer_T* lexer)
{
    return NULL;
}
/*
enumerator
    : IDENTIFIER
    | IDENTIFIER '=' constant_expression
    ;
*/
decl* make_enumerator(lexer_T* lexer)
{
    return NULL;
}
/*
type_qualifier
    : CONST
    | VOLATILE
    ;
*/
decl* make_type_qualifier(lexer_T* lexer)
{
    return NULL;
}
/*
declarator
    : pointer direct_declarator
    | direct_declarator
    ;
*/
decl* make_declarator(lexer_T* lexer)
{
    return NULL;
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
decl* make_direct_declarator(lexer_T* lexer)
{
    return NULL;
}
/*
pointer
    : '*'
    | '*' type_qualifier_list
    | '*' pointer
    | '*' type_qualifier_list pointer
    ;
*/
decl* make_pointer(lexer_T* lexer)
{
    return NULL;
}
/*
type_qualifier_list
    : type_qualifier
    | type_qualifier_list type_qualifier
    ;

*/
decl* make_type_qualifier_list(lexer_T* lexer)
{
    return NULL;
}
/*
parameter_type_list
    : parameter_list
    | parameter_list ',' ELLIPSIS
    ;
*/
decl* make_parameter_type_list(lexer_T* lexer)
{
    return NULL;
}
/*
parameter_list
    : parameter_declaration
    | parameter_list ',' parameter_declaration
    ;
*/
decl* make_parameter_list(lexer_T* lexer)
{
    return NULL;
}
/*
parameter_declaration
    : declaration_specifiers declarator
    | declaration_specifiers abstract_declarator
    | declaration_specifiers
    ;
*/
decl* make_parameter_declaration(lexer_T* lexer)
{
    return NULL;
}
/*
identifier_list
    : IDENTIFIER
    | identifier_list ',' IDENTIFIER
    ;
*/
decl* make_identifier_list(lexer_T* lexer)
{
    return NULL;
}
/*
type_name
    : specifier_qualifier_list
    | specifier_qualifier_list abstract_declarator
    ;
*/
decl* make_type_name(lexer_T* lexer)
{
    return NULL;
}
/*
abstract_declarator
    : pointer
    | direct_abstract_declarator
    | pointer direct_abstract_declarator
    ;
*/
decl* make_abstract_declarator(lexer_T* lexer)
{
    return NULL;
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
decl* make_direct_abstract_declarator(lexer_T* lexer)
{
    return NULL;
}
/*
initializer
    : assignment_expression
    | '{' initializer_list '}'
    | '{' initializer_list ',' '}'
    ;
*/
decl* make_initializer(lexer_T* lexer)
{
    return NULL;
}
/*
initializer_list
    : initializer
    | initializer_list ',' initializer
    ;
*/
decl* make_initializer_list(lexer_T* lexer)
{
    return NULL;
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
stmt* make_statement(lexer_T* lexer)
{
    return NULL;
}
/*
labeled_statement
    : IDENTIFIER ':' statement
    | CASE constant_expression ':' statement
    | DEFAULT ':' statement
    ;
*/
stmt* make_labeled_statement(lexer_T* lexer)
{
    return NULL;
}
/*
compound_statement
    : '{' '}'
    | '{' statement_list '}'
    | '{' declaration_list '}'
    | '{' declaration_list statement_list '}'
    ;
*/
stmt* make_compound_statement(lexer_T* lexer)
{
    return NULL;
}
/*
declaration_list
    : declaration
    | declaration_list declaration
    ;
*/
decl* make_declaration_list(lexer_T* lexer)
{
    return NULL;
}
/*
statement_list
    : statement
    | statement_list statement
    ;
*/
stmt* make_statement_list(lexer_T* lexer)
{
    return NULL;
}
/*
expression_statement
    : ';'
    | expression ';'
    ;
*/
stmt* make_expression_statement(lexer_T* lexer)
{
    return NULL;
}
/*
selection_statement
    : IF '(' expression ')' statement
    | IF '(' expression ')' statement ELSE statement
    | SWITCH '(' expression ')' statement
    ;
*/
decl* make_selection_statement(lexer_T* lexer)
{
    return NULL;
}
/*
iteration_statement
    : WHILE '(' expression ')' statement
    | DO statement WHILE '(' expression ')' ';'
    | FOR '(' expression_statement expression_statement ')' statement
    | FOR '(' expression_statement expression_statement expression ')' statement
    ;
*/
stmt* make_iteration_statement(lexer_T* lexer)
{
    return NULL;
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
stmt* make_jump_statement(lexer_T* lexer)
{
    return NULL;
}
/*
translation_unit
    : external_declaration
    | translation_unit external_declaration
    ;
*/
decl* make_translation_unit(lexer_T* lexer)
{
    return NULL;
}
/*
external_declaration
    : function_definition
    | declaration
    ;
*/
decl* make_external_declaration(lexer_T* lexer)
{
    return NULL;
}
/*
function_definition
    : declaration_specifiers declarator declaration_list compound_statement
    | declaration_specifiers declarator compound_statement
    | declarator declaration_list compound_statement
    | declarator compound_statement
    ;
*/
decl* make_function_definition(lexer_T* lexer)
{
    return NULL;
}
