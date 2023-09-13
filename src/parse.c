#include "parse.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

parse_T* init_parser(lexer_T* lexer)
{
    parse_T *parser = calloc(1, sizeof(parse_T));
    parser->lexer = lexer;
    parser->curToken = NULL;
    parser->peekToken = NULL;
    nextToken(parser);
    nextToken(parser);
    return parser;
}

// Return true if the current token matches.
int checkToken(token_T* curToken, int kind)
{
    return kind == curToken->kind;
}

// Return true if the next token matches.
int checkPeek(token_T* peekToken, int kind)
{
    return kind == peekToken->kind;
}

// Try to match current token. If not, error. Advances the current token.
void match(parse_T* parser, int kind)
{
    if(!checkToken(parser->curToken, kind))
    {
        char* error_message = "Expected ";
        strcat(error_message, get_token_name(kind));
        strcat(error_message, "got ");
        strcat(error_message, get_token_name(parser->curToken->kind));
        parse_abort(error_message);
    }
            
    nextToken(parser);
}

// Advances the current token.
void nextToken(parse_T* parser)
{
    parser->curToken = parser->peekToken;
    parser->peekToken = getToken(parser->lexer);
    // No need to worry about passing the EOF, lexer handles that.
}

void parse_abort(char* message)
{
    char* str = "Error: ";
    strcat(str,message);
    printf("%s", str);
    exit(1);
}

// Production rules.

// program ::= {statement}
void program(parse_T* parser)
{
    printf("PROGRAM");

    // Since some newlines are required in our grammar, need to skip the excess.
    while(checkToken(parser->curToken, NEWLINE))
    {
        nextToken(parser);
    }

    // Parse all the statements in the program.
    while(!checkToken(parser->curToken, EOF))
    {
        statement(parser);
    }
        
}

// One of the following statements...
void statement(parse_T* parser)
{
    // Check the first token to see what kind of statement this is.

    // "PRINT" (expression | string)
    if(checkToken(parser->curToken, PRINT))
    {
        printf("STATEMENT-PRINT");
        nextToken(parser);

        if(checkToken(parser->curToken, STRING))
        {
            // Simple string.
            nextToken(parser);
        }
        else
        {
            // Expect an expression.
            expression(parser);
        }
            
    }
    // "IF" comparison "THEN" {statement} "ENDIF"
    else if(checkToken(parser->curToken, IF))
    {
        printf("STATEMENT-IF");
        nextToken(parser);
        comparison(parser);

        match(parser, THEN);
        nl(parser);

        // Zero or more statements in the body.
        while(checkToken(parser->curToken, ENDIF))
        {
            statement(parser);
        }

        match(parser, ENDIF);
    }
    // "WHILE" comparison "REPEAT" {statement} "ENDWHILE"
    else if(checkToken(parser->curToken, WHILE))
    {
        printf("STATEMENT-WHILE");
        nextToken(parser);
        comparison(parser);

        match(parser, REPEAT);
        nl(parser);

        // Zero or more statements in the loop body.
        while(!checkToken(parser->curToken, ENDWHILE))
        {
            statement(parser);
        }
        match(parser, ENDWHILE);
    }
    // "LABEL" ident
    else if(checkToken(parser->curToken, LABEL))
    {
        printf("STATEMENT-LABEL");
        nextToken(parser);
        match(parser, IDENT);
    }
    // "GOTO" ident
    else if (checkToken(parser->curToken, GOTO))
    {
        printf("STATEMENT-GOTO");
        nextToken(parser);
        match(parser, IDENT);
    }
    // "LET" ident "=" expression
    else if (checkToken(parser->curToken, LET))
    {
        printf("STATEMENT-LET");
        nextToken(parser);
        match(parser, IDENT);
        match(parser, EQ);
        expression(parser);
    }
    // "INPUT" ident
    else if (checkToken(parser->curToken, INPUT))
    {
        printf("STATEMENT-INPUT");
        nextToken(parser);
        match(parser, IDENT);
    }
    // This is not a valid statement. Error!
    else
    {
        char* error_message = "Invalid statement at " ;
        strcat(error_message, parser->curToken->text);
        strcat(error_message, " (");
        strcat(error_message, get_token_name(parser->curToken->kind));
        strcat(error_message, ") ");
        parse_abort(error_message);
    }
    // Newline.
    nl(parser);
}

// nl ::= '\n'+
void nl(parse_T* parser)
{
    printf("NEWLINE");
    
    // Require at least one newline.
    match(parser, NEWLINE);
    // But we will allow extra newlines too, of course.
    while(checkToken(parser->curToken, NEWLINE))
    {
        nextToken(parser);
    }

}

// comparison ::= expression (("==" | "!=" | ">" | ">=" | "<" | "<=") expression)+
void comparison(parse_T* parser)
{
    printf("COMPARISON");

    expression(parser);
    // Must be at least one comparison operator and another expression.
    if(isComparisonOperator(parser))
    {
        nextToken(parser);
        expression(parser);
    }   
    else
    {
        char* error_message = "Expected comparison operator at: ";
        strcat(error_message, parser->curToken->text);
        parse_abort(error_message);
    }
    // Can have 0 or more comparison operator and expressions.
    while(isComparisonOperator(parser))
    {
        nextToken(parser);
        expression(parser);
    }
        
}

// Return true if the current token is a comparison operator.
int isComparisonOperator(parse_T* parser)
{
    return checkToken(parser->curToken, GT) || 
        checkToken(parser->curToken, GTEQ) || 
        checkToken(parser->curToken, LT) || 
        checkToken(parser->curToken, LTEQ) || 
        checkToken(parser->curToken, EQEQ) || 
        checkToken(parser->curToken, NOTEQ);
}

// expression ::= term {( "-" | "+" ) term}
void expression(parse_T* parser)
{
    printf("EXPRESSION");

    term(parser);
    // Can have 0 or more +/- and expressions.
    while (checkToken(parser->curToken, PLUS) || checkToken(parser->curToken, MINUS))
    {
        nextToken(parser);
        term(parser);
    } 
}

// term ::= unary {( "/" | "*" ) unary}
void term(parse_T* parser)
{
    printf("TERM");

    unary(parser);
    // Can have 0 or more *// and expressions.
    while(checkToken(parser->curToken, ASTERISK) || checkToken(parser->curToken, SLASH))
    {
        nextToken(parser);
        unary(parser);
    }
}



// unary ::= ["+" | "-"] primary
void unary(parse_T* parser)
{
    printf("UNARY");

    // Optional unary +/-
    if(checkToken(parser->curToken, PLUS) || checkToken(parser->curToken, MINUS))
    {
        nextToken(parser);
    }      
    primary(parser);
}

// primary ::= number | ident
void primary(parse_T* parser)
{
    char* error_message = "PRIMARY (";
    strcat(error_message, parser->curToken->text);
    strcat(error_message, ")");
    printf("%s", error_message);

    if(checkToken(parser->curToken, NUMBER))
    {
        nextToken(parser);
    }
    else if(checkToken(parser->curToken, IDENT))
    {
        nextToken(parser);
    }
    else
    {
        // Error!
        char* error_message = "Unexpected token at ";
        strcat(error_message, parser->curToken->text);
        parse_abort(error_message);
    }
        
}