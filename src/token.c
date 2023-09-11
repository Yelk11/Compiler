#include "token.h"
#include <stdlib.h>


token_T* init_token(char* tokenText, int tokenKind)
{
    token_T* token = calloc(1, sizeof(token_T));
    token->text = tokenText;
    token->kind = tokenKind;
    return token;
}
token_T* checkIfKeyword(char* tokenText)
{
    for(int i=EOF; i<=GTEQ; i++) {
        if(tokenText == get_token_name(i) && i >= 100 && i < 200)
        {
            token_T* token = init_token(tokenText, i);
            return token;
        }
    }
    return NULL;
}
        

char* get_token_name(int token)
{
    switch(token)
    {
        case EOF:
            return "EOF";
            break;	
        case NEWLINE:
            return "NEWLINE";
            break;
	    case NUMBER:
            return "NUMBER";
            break;
	    case IDENT:
            return "IDENT";
            break;
	    case STRING:
            return "STRING";
            break;
	    case LABEL:
            return "LABEL";
            break;
	    case GOTO:
            return "GOTO";
            break;
	    case PRINT:
            return "PRINT";
            break;
	    case INPUT:
            return "INPUT";
            break;
	    case LET:
            return "LET";
            break;
	    case IF:
            return "IF";
            break;
	    case THEN:
            return "THEN";
            break;
	    case ENDIF:
            return "ENDIF";
            break;
	    case WHILE:
            return "WHILE";
            break;
	    case REPEAT:
            return "REPEAT";
            break;
	    case ENDWHILE:
            return "ENDWHILE";
            break;
	    case EQ:
            return "EQ";
            break;
	    case PLUS:
            return "PLUS";
            break;
	    case MINUS:
            return "MINUS";
            break;
	    case ASTERISK:
            return "ASTERISK";
            break;
	    case SLASH:
            return "SLASH";
            break;
	    case EQEQ:
            return "EQEQ";
            break;
	    case NOTEQ:
            return "NOTEQ";
            break;
	    case LT:
            return "LT";
            break;
	    case LTEQ:
            return "LTEQ";
            break;
	    case GT:
            return "GT";
            break;
	    case GTEQ:
            return "GTEQ";
            break;
    }
    return "TOKEN_UKNOWN";
}