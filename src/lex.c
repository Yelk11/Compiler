#include "lex.h"
#include <string.h>
#include <stdlib.h>

lexer_T* init_lexer(char* source)
{
    lexer_T* lexer = calloc(1, sizeof(lexer_T));
    char *newstr = malloc(strlen(source) + 4);
    strcpy(newstr, source);
    strcat(newstr, "\n\n");
    lexer->source = newstr; // Source code to lex as a string. Append a newline to simplify lexing/parsing the last token/statement.
    lexer->curChar = ' ';   // Current character in the string.
    lexer->curPos = -1;    // Current position in the string.
    nextChar(lexer);
    return lexer;
}


// Process the next character.
void nextChar(lexer_T* lexer)
{
    lexer->curPos += 1;
    if (lexer->curPos >= strlen(lexer->source))
    {
        lexer->curChar = '\0';  // EOF
    }
    else
    {
        lexer->curChar = lexer->source[lexer->curPos];
    }
}


// Return the lookahead character.
char peek(lexer_T* lexer)
{
    if (lexer->curPos + 1 >= strlen(lexer->source))
    {
        return '\0';
    }
    return lexer->source[lexer->curPos+1];
}


// Invalid token found, print error message and exit.
void lex_abort(char* message)
{

}
    
// Skip whitespace except newlines, which we will use to indicate the end of a statement.
void skipWhitespace(lexer_T* lexer)
{

}
    
// Skip comments in the code.
void skipComment(lexer_T* lexer)
{
    
}

// Return the next token.
void getToken(lexer_T* lexer)
{
    
}