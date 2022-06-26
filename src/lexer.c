#include "lexer.h"
#include "utils.h"
#include "macros.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

Lexer_T* init_lexer(char* src)
{
  Lexer_T* lexer = calloc(1, sizeof(struct Lexer_T));
  lexer->src = src;
  lexer->src_size = strlen(src);
  lexer->i = 0;
  lexer->c = src[lexer->i];

  return lexer;
}

void lexer_advance(Lexer_T* lexer)
{
  if (lexer->i < lexer->src_size && lexer->c != '\0')
  {
    lexer->i += 1;
    lexer->c = lexer->src[lexer->i];
  }
}

char lexer_peek(Lexer_T* lexer, int offset)
{
  return lexer->src[MIN(lexer->i + offset, lexer->src_size)];
}

Token_T* lexer_advance_with(Lexer_T* lexer, Token_T* token)
{
  lexer_advance(lexer);
  return token;
}

Token_T* lexer_advance_current(Lexer_T* lexer, int type)
{
  char* value = calloc(2, sizeof(char));
  value[0] = lexer->c;
  value[1] = '\0';

  Token_T* token = init_token(value, type);
  lexer_advance(lexer);

  return token;
}

void lexer_skip_whitespace(Lexer_T* lexer)
{
  while (lexer->c == 13 || lexer->c == 10 || lexer->c == ' ' || lexer->c == '\t')
    lexer_advance(lexer);
}

void lexer_skip_comment(Lexer_T* lexer)
{
  if (lexer->c == '/')
  {
    if (lexer_peek(lexer, 1) == '/') 
    {
      while (lexer->c != '\n')
        lexer_advance(lexer);
    }
    else
    if (lexer_peek(lexer, 1) == '*')
    {
      while (1) {
        if (lexer->c == '*' && lexer_peek(lexer, 1) == '/')
        {
          lexer_advance(lexer);
          lexer_advance(lexer);
          break;
        }

        lexer_advance(lexer);
      }
    }
  }
  
  lexer_skip_whitespace(lexer);
}

Token_T* lexer_parse_id(Lexer_T* lexer)
{
  int Token_Type = TOKEN_ID;

  char* value = calloc(1, sizeof(char));

  while (isalpha(lexer->c))
  {
    value = realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, (char[]){lexer->c, 0});
    lexer_advance(lexer);
  }

  if (strcmp(value, "return") == 0)
    Token_Type = TOKEN_STATEMENT;

  return init_token(value, Token_Type);
}

Token_T* lexer_parse_number(Lexer_T* lexer)
{
  char* value = calloc(1, sizeof(char));

  while (isdigit(lexer->c))
  {
    value = realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, (char[]){lexer->c, 0});
    lexer_advance(lexer);
  }

  return init_token(value, TOKEN_INT);
}

Token_T* lexer_parse_string(Lexer_T* lexer)
{
  char* value = calloc(1, sizeof(char));
  
  lexer_advance(lexer);

  while (lexer->c != '"')
  {
    value = realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, (char[]){lexer->c, 0});
    lexer_advance(lexer);
  }
  
  lexer_advance(lexer);

  char* formatted = str_format(value);
  free(value);

  return init_token(formatted, TOKEN_STRING);
}

Token_T* lexer_next_token(Lexer_T* lexer)
{
  while (lexer->c != '\0')
  {
    lexer_skip_whitespace(lexer);
    lexer_skip_comment(lexer);

    if (isalpha(lexer->c))
      return lexer_parse_id(lexer);

    if (isdigit(lexer->c))
      return lexer_parse_number(lexer);

    switch (lexer->c)
    {
      case '=': {
        if (lexer_peek(lexer, 1) == '>') return lexer_advance_with(lexer, lexer_advance_with(lexer, init_token("=>", TOKEN_ARROW_RIGHT)));
        return lexer_advance_with(lexer, init_token("=", TOKEN_EQUALS));
      } break;
      case '(': return lexer_advance_current(lexer, TOKEN_LPAREN);
      case ')': return lexer_advance_current(lexer, TOKEN_RPAREN);
      case '{': return lexer_advance_current(lexer, TOKEN_LBRACE);
      case '}': return lexer_advance_current(lexer, TOKEN_RBRACE);
      case '[': return lexer_advance_current(lexer, TOKEN_LBRACKET);
      case ']': return lexer_advance_current(lexer, TOKEN_RBRACKET);
      case ':': return lexer_advance_current(lexer, TOKEN_COLON);
      case ',': return lexer_advance_current(lexer, TOKEN_COMMA);
      case '<': return lexer_advance_current(lexer, TOKEN_LT);
      case '>': return lexer_advance_current(lexer, TOKEN_GT);
      case ';': return lexer_advance_current(lexer, TOKEN_SEMI);
      case '+': return lexer_advance_current(lexer, TOKEN_PLUS);
      case '-': return lexer_advance_current(lexer, TOKEN_MINUS);
      case '/': return lexer_advance_current(lexer, TOKEN_DIV);
      case '*': return lexer_advance_current(lexer, TOKEN_MUL);
      case '"': return lexer_parse_string(lexer);
      case '\0': break;
      default: printf("[Lexer]: Unexpected character `%c` (%d)\n", lexer->c, (int)lexer->c); exit(1); break;
    }
  }

  return init_token(0, TOKEN_EOF);
}