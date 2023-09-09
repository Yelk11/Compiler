#include "lex.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


lexer_T *init_lexer(char *source)
{
    lexer_T *lexer = calloc(1, sizeof(lexer_T));
    char *newstr = malloc(strlen(source) + 4);
    strcpy(newstr, source);
    strcat(newstr, "\n\n");
    lexer->source = newstr; // Source code to lex as a string. Append a newline to simplify lexing/parsing the last token/statement.
    lexer->curChar = ' ';   // Current character in the string.
    lexer->curPos = -1;     // Current position in the string.
    nextChar(lexer);
    return lexer;
}

// Process the next character.
void nextChar(lexer_T *lexer)
{
    lexer->curPos += 1;
    if (lexer->curPos >= strlen(lexer->source))
    {
        lexer->curChar = '\0'; // EOF
    }
    else
    {
        lexer->curChar = lexer->source[lexer->curPos];
    }
}

// Return the lookahead character.
char peek(lexer_T *lexer)
{
    if (lexer->curPos + 1 >= strlen(lexer->source))
    {
        return '\0';
    }
    return lexer->source[lexer->curPos + 1];
}

// Invalid token found, print error message and exit.
void lex_abort(char *message)
{
    printf("Lexing error: %s", message);
    exit(0);
}

// Skip whitespace except newlines, which we will use to indicate the end of a statement.
void skipWhitespace(lexer_T *lexer)
{
    while (lexer->curChar == ' '|| lexer->curChar == '\t' || lexer->curChar == '\r')
    {
        nextChar(lexer);
    }

}

// Skip comments in the code.
void skipComment(lexer_T *lexer)
{
}

// Return the next token.
token_T* getToken(lexer_T *lexer)
{
    skipWhitespace(lexer);
    token_T *token = NULL;
    // Check the first character of this token to see if we can decide what it is.
    // If it is a multiple character operator (e.g., !=), number, identifier, or keyword then we will process the rest.
    if (lexer->curChar == '+')
    {
        token = init_token(&lexer->curChar, PLUS);
    }
    else if (lexer->curChar == '-')
    {
        token = init_token(&lexer->curChar, MINUS);
    }
    else if (lexer->curChar == '*')
    {
        token = init_token(&lexer->curChar, ASTERISK);
    }
    else if (lexer->curChar == '/')
    {
        token = init_token(&lexer->curChar, SLASH);
    }
    else if (lexer->curChar == '=')
    {
        // Check whether this token is = or ==
        if (peek(lexer) == '=')
        {
            nextChar(lexer);
            token = init_token("==", EQEQ);
        }
        else
        {
            token = init_token("=", EQ);
        }
            
    }
    else if(lexer->curChar == '>')
    {
        // Check whether this is token is > or >=
        if (peek(lexer) == '=')
        {
            nextChar(lexer);
            token = init_token(">=", GTEQ);
        }
        else
        {
            token = init_token(">", GT);
        }
            
    }
    else if(lexer->curChar == '<')
    {
        // Check whether this is token is < or <=
        if( peek(lexer) == '=')
        {
            nextChar(lexer);
            token = init_token("<=", LTEQ);
        }
        else
        {
            token = init_token("<", LT);
        }
    }
    else if(lexer->curChar == '!')
    {
        if (peek(lexer) == '=')
        {
            nextChar(lexer);
            token = init_token("!=", NOTEQ);
        }   
        else
        {
            char str[20] = "Expected !=, got !";
            str[18] = peek(lexer);
            lex_abort(str);
        }
                
    }
            
    else if (lexer->curChar == '\n')
    {
        token = init_token(&lexer->curChar, NEWLINE);
    }
    else if (lexer->curChar == '\0')
    {
        token = init_token(" ", EOF);
    }
    else
    {
        char str[15] = "Uknown token";
        strcpy(str, &lexer->curChar);
        lex_abort(str);
    }

    nextChar(lexer);
    return token;
}