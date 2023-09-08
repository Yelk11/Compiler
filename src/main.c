#include "lex.h"
#include "token.h"
#include <stdio.h>

int main(){
    char* source = "+- */";

    lexer_T* lexer = init_lexer(source);

    token_T* token = getToken(lexer);
    while (token->kind != EOF)
    {
        printf("%s\n",get_token_name(token->kind));
        token = getToken(lexer);
    }
}



