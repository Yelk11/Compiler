#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

token_T *next(lexer_T *lexer)
{
    token_T* token;
    token = lexer->token;
    advance(lexer);
    return token;
}

lexer_T *init_lexer(char *file_name)
{
    lexer_T *lexer = calloc(1, sizeof(struct lexer_T));

    FILE *fp = fopen(file_name, "r");
    lexer->c = fgetc(fp);
    lexer->length = 1;
    
    return lexer;
}

lexer_T *advance(lexer_T *lexer)
{
    if(lexer->c == ' ')
    {
        skip_whitespace(lexer);
        lexer = advance(lexer);
    }
    return lexer;
}

lexer_T *skip_whitespace(lexer_T *lexer)
{
    return NULL;
}

int append(char *s, size_t size, char c)
{
    if (strlen(s) + 1 >= size)
    {
        return 1;
    }
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
    return 0;
}
