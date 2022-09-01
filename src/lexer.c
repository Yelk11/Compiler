#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

lexer_T *init_lexer(char *file_name)
{
    FILE *fp = fopen(file_name, "r");
    if (!fp)
        printf("Cannot open %s: %s", file_name, strerror(errno));
    lexer_T *lexer = calloc(1, sizeof(struct lexer_T));
    lexer->fp = fp;
    lexer->i = 0;
    lexer->c = fgetc(lexer->fp);
    return lexer;
}

token_T *lexer_next_token(lexer_T *lexer)
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
        case '=':
        {
            if (lexer_peek(lexer, 1) == '>')
                return lexer_advance_with(lexer, lexer_advance_with(lexer, init_token("->", TOKEN_ARROW_RIGHT)));
            return lexer_advance_with(lexer, init_token("=", TOKEN_EQUALS));
        }
        break;
        case '(':
            return lexer_advance_current(lexer, TOKEN_LPAREN);
        case ')':
            return lexer_advance_current(lexer, TOKEN_RPAREN);
        case '{':
            return lexer_advance_current(lexer, TOKEN_LBRACE);
        case '}':
            return lexer_advance_current(lexer, TOKEN_RBRACE);
        case '[':
            return lexer_advance_current(lexer, TOKEN_LBRACKET);
        case ']':
            return lexer_advance_current(lexer, TOKEN_RBRACKET);
        case ':':
            return lexer_advance_current(lexer, TOKEN_COLON);
        case ',':
            return lexer_advance_current(lexer, TOKEN_COMMA);
        case '<':
            return lexer_advance_current(lexer, TOKEN_LT);
        case '>':
            return lexer_advance_current(lexer, TOKEN_GT);
        case ';':
            return lexer_advance_current(lexer, TOKEN_SEMI);
        case '+':
            return lexer_advance_current(lexer, TOKEN_PLUS);
        case '-':
            return lexer_advance_current(lexer, TOKEN_MINUS);
        case '/':
            return lexer_advance_current(lexer, TOKEN_DIV);
        case '*':
            return lexer_advance_current(lexer, TOKEN_MUL);
        case '"':
            return lexer_parse_string(lexer);
        case '\0':
            break; // breaks to EOF token
        default:
            printf("[Lexer]: Unexpected character `%c` (%d)\n", lexer->c, (int)lexer->c);
            exit(1);
            break;
        }
    }
    return init_token(0, TOKEN_EOF);
}

char lexer_peek(lexer_T *lexer, int offset)
{
    return fseek(lexer->fp, offset, SEEK_CUR);
}

void lexer_advance(lexer_T *lexer)
{

    if (lexer->c != '\0')
    {
        lexer->c = fgetc(lexer->fp);
    }
}

token_T *lexer_advance_current(lexer_T *lexer, int type)
{
    char *value = calloc(2, sizeof(char));
    value[0] = lexer->c;
    value[1] = '\0';

    token_T *token = init_token(value, type);
    lexer_advance(lexer);

    return token;
}

token_T *lexer_advance_with(lexer_T *lexer, token_T *token)
{
    lexer_advance(lexer);
    return token;
}

void lexer_skip_whitespace(lexer_T *lexer)
{
    while (lexer->c == 13 || lexer->c == 10 || lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n')
        lexer_advance(lexer);
}

void lexer_skip_comment(lexer_T *lexer) //TODO finish /*
{
    if (lexer->c == '/')
    {
        lexer_advance(lexer);
        if (lexer->c == '/')
        {
            while (lexer->c != '\n')
            {
                lexer_advance(lexer);
            }
        }
        else if (lexer->c == '*')
        {
            lexer_skip_comment_block(lexer);
        }
    }
}

void lexer_skip_comment_block(lexer_T* lexer)
{
    printf("Hello");
}

token_T *lexer_parse_id(lexer_T *lexer)
{
    int Token_Type = TOKEN_ID;
    char *value = calloc(1, sizeof(char));
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

token_T *lexer_parse_number(lexer_T *lexer)
{
    char *value = calloc(1, sizeof(char));

    while (isdigit(lexer->c))
    {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        lexer_advance(lexer);
    }

    return init_token(value, TOKEN_INT);
}

token_T *lexer_parse_string(lexer_T *lexer)
{
    char *value = calloc(1, sizeof(char));

    lexer_advance(lexer);

    while (lexer->c != '"')
    {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(value, TOKEN_STRING);
}
