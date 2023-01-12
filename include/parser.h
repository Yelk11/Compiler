#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"
#include "ast.h"

/*
About Abstract Syntax Tree:
https://www3.nd.edu/~dthain/compilerbook/chapter6.pdf
*/

typedef struct parser_T
{
  lexer_T *lexer;
  token_T *token;
} parser_T;

parser_T *init_parser(lexer_T *lexer);

token_T *parser_eat(parser_T *parser);

void *parse(lexer_T *lexer);

expr *make_primary_expression(lexer_T *lexer);

expr *make_postfix_expression(lexer_T *lexer);

expr *make_argument_experssion_list(lexer_T *lexer);

expr *make_unary_expression(lexer_T *lexer);

expr *make_unary_operator(lexer_T *lexer);

expr *make_cast_expression(lexer_T *lexer);

expr *make_multiplicative_expression(lexer_T *lexer);

expr *make_additive_expression(lexer_T *lexer);

expr* make_shift_expression(lexer_T* lexer);

expr* make_relational_expression(lexer_T* lexer);

expr* make_equality_expression(lexer_T* lexer);

expr* make_and_expression(lexer_T* lexer);

expr* make_exclusive_or_expression(lexer_T* lexer);

expr* make_inclusive_or_expression(lexer_T* lexer);

expr* make_logical_and_expression(lexer_T* lexer);

expr* make_logical_or_expression(lexer_T* lexer);

expr* make_conditional_expression(lexer_T* lexer);

expr* make_assignment_expression(lexer_T* lexer);

expr* make_assignment_operator(lexer_T* lexer);

expr *make_expression(lexer_T *lexer);

expr* make_constant_expression(lexer_T* lexer);

decl* make_declaration(lexer_T* lexer);

decl* make_declaration_specifiers(lexer_T* lexer);

decl* make_init_declarator_list(lexer_T* lexer);

decl* make_init_declarator(lexer_T* lexer);

decl* make_storage_class_specifier(lexer_T* lexer);

decl* make_type_specifier(lexer_T* lexer);

decl* make_struct_or_union_specifier(lexer_T* lexer);

decl* make_struct_or_union(lexer_T* lexer);

decl* make_struct_declaration_list(lexer_T* lexer);

decl* make_struct_declaration(lexer_T* lexer);

decl* make_specifier_qualifier_list(lexer_T* lexer);

decl* make_struct_declarator_list(lexer_T* lexer);

decl* make_struct_declarator(lexer_T* lexer);

decl* make_enum_specifier(lexer_T* lexer);

decl* make_enumerator_list(lexer_T* lexer);

decl* make_enumerator(lexer_T* lexer);

decl* make_type_qualifier(lexer_T* lexer);

decl* make_declarator(lexer_T* lexer);

decl* make_direct_declarator(lexer_T* lexer);

decl* make_pointer(lexer_T* lexer);

decl* make_type_qualifier_list(lexer_T* lexer);

decl* make_parameter_type_list(lexer_T* lexer);

decl* make_parameter_list(lexer_T* lexer);

decl* make_parameter_declaration(lexer_T* lexer);

decl* make_identifier_list(lexer_T* lexer);

decl* make_type_name(lexer_T* lexer);

decl* make_abstract_declarator(lexer_T* lexer);

decl* make_direct_abstract_declarator(lexer_T* lexer);

decl* make_initializer(lexer_T* lexer);

decl* make_initializer_list(lexer_T* lexer);

stmt* make_statement(lexer_T* lexer);

stmt* make_labeled_statement(lexer_T* lexer);

stmt* make_compound_statement(lexer_T* lexer);

decl* make_declaration_list(lexer_T* lexer);

stmt* make_statement_list(lexer_T* lexer);

stmt* make_expression_statement(lexer_T* lexer);

decl* make_selection_statement(lexer_T* lexer);

stmt* make_iteration_statement(lexer_T* lexer);

stmt* make_jump_statement(lexer_T* lexer);

decl* make_translation_unit(lexer_T* lexer);

decl* make_external_declaration(lexer_T* lexer);

decl* make_function_definition(lexer_T* lexer);

#endif