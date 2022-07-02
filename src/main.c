#include "lexer.h"




int main(){
    lexer_T* lexer = init_lexer("test.c");
    // printf("%s", type_to_string(next(lexer)));
    type_to_string(next(lexer));

    return 0;
}
