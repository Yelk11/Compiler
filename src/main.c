#include "lex.h"
#include <stdio.h>

int main(){
    char* source = "LET foobar = 123";
	lexer_T* lexer = init_lexer(source);

	while (peek(lexer) != '\0')
	{
        printf("%c",lexer->curChar);
		nextChar(lexer);
    }
}



