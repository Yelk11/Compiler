#include "lexer.h"
#include "token.h"

int main(){
    lexer_T* lexer = init_lexer("test.c");
    token_T* token = lexer_next_token(lexer);
    printf("%s : %s\n",type_to_string(token->type), token->value);

    while(lexer->c != '\xFF'){
        token = lexer_next_token(lexer);
        printf("%s : %s\n",type_to_string(token->type), token->value);
    }
    return 0;
}
