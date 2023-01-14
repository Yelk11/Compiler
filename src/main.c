#include "lexer.h"
#include "token.h"

int main(int argc, char* argv[]){
    lexer_T* lexer;
    if(argc > 1){
        lexer = init_lexer(argv[1]);
    }else{
        lexer = init_lexer("test.c");
    }
    


    for(int i = 0; i < 10; i++){
        printf("next word %s\n", lexer_peek_next_token(lexer, 1)->value);
        
    }



    // token_T* token;
    // token = lexer_next_token(lexer);
    // printf("%s : %s\n",type_to_string(token->type), token->value);

    // while(lexer->c != '\xFF'){
    //     token = lexer_next_token(lexer);
    //     printf("%s : %s\n",type_to_string(token->type), token->value);
    // }
    return 0;
}
