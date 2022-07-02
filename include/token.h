#ifndef TOKEN_H
#define TOKEN_H

typedef struct token_T
{
    char *value;
    enum
    {
        TOKEN_ID,
        TOKEN_EQUALS,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_LBRACKET,
        TOKEN_RBRACKET,
        TOKEN_COLON,
        TOKEN_COMMA,
        TOKEN_LT,
        TOKEN_GT,
        TOKEN_ARROW_RIGHT,
        TOKEN_INT,
        TOKEN_STRING,
        TOKEN_STATEMENT,
        TOKEN_SEMI,
        TOKEN_PLUS,
        TOKEN_MINUS,
        TOKEN_DIV,
        TOKEN_MUL,
        TOKEN_EOF,
    } type;

} token_T;


token_T* init_token(char* value, int type);
char* type_to_string(token_T* token);
#endif