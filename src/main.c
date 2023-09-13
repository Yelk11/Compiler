#include "lex.h"
#include "token.h"
#include "parse.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h> 




int main(int argc, char *argv[]){
    printf("Teeny Tiny Compiler\n");

    if (argc != 2)
    {
        printf("Error: Compiler needs source file as argument.\n");
        exit(1);
    }
    FILE* fp = fopen(argv[1], "r");

    
    char* source_ptr = "";
    strcpy(source_ptr, source);

    // Initialize the lexer and parser.
    lexer_T* lexer = init_lexer(source_ptr);
    parse_T* parser = init_parser(lexer);

    program(parser); // Start the parser.
    printf("Parsing completed.\n");
}



