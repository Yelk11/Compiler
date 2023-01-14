#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

int is_next_word(lexer_T *lexer, char *str);


lexer_T *init_lexer(char *file_name)
{
    FILE *fp = fopen(file_name, "r");
    if (!fp)
        printf("Cannot open %s: %s", file_name, strerror(errno));
    lexer_T *lexer = calloc(1, sizeof(struct lexer_T));
    lexer->fp = fp;
    lexer->i = 0;
    lexer->c = fgetc(lexer->fp);
    return lexer;
}

token_T *lexer_next_token(lexer_T *lexer)
{
    lexer_skip_whitespace(lexer);
    lexer_skip_comment(lexer);
    while (lexer->c != '\0')
    {

        if (isalpha(lexer->c))
            return lexer_parse_id(lexer);
        if (isdigit(lexer->c))
            return lexer_parse_number(lexer);

        switch (lexer->c)
        {
        case '"':
            return lexer_parse_string(lexer);
        case '.':
            if(is_next_word(lexer, "...")){return lexer_advance_current(lexer, ELLIPSIS);}
            if(is_next_word(lexer, ".")){return lexer_advance_current(lexer, PERIOD);}
        case '>':
            if(is_next_word(lexer, ">>=")){return lexer_advance_current(lexer, RIGHT_ASSIGN);}
            if(is_next_word(lexer, ">>")){return lexer_advance_current(lexer, RIGHT_OP);}
            if(is_next_word(lexer, ">")){return lexer_advance_current(lexer, GREATER_THAN);}
            if(is_next_word(lexer, ">=")){return lexer_advance_current(lexer, GE_OP);}
        case '<':
            if(is_next_word(lexer, "<<=")){return lexer_advance_current(lexer, LEFT_ASSIGN);}
            if(is_next_word(lexer, "<<")){return lexer_advance_current(lexer, LEFT_OP);}
            if(is_next_word(lexer, "<=")){return lexer_advance_current(lexer, LE_OP);}
            if(is_next_word(lexer, "<")){return lexer_advance_current(lexer, LESS_THAN);}
        case '+':
            if(is_next_word(lexer, "+=")){return lexer_advance_current(lexer, ADD_ASSIGN);}
            if(is_next_word(lexer, "++")){return lexer_advance_current(lexer, INC_OP);}
            if(is_next_word(lexer, "+")){return lexer_advance_current(lexer, PLUS);}
        case '-':
            if(is_next_word(lexer, "-=")){return lexer_advance_current(lexer, SUB_ASSIGN);}
            if(is_next_word(lexer, "--")){return lexer_advance_current(lexer, DEC_OP);}
            if(is_next_word(lexer, "->")){return lexer_advance_current(lexer, PTR_OP);}
            if(is_next_word(lexer, "-")){return lexer_advance_current(lexer, MINUS);}
        case '*':
            if(is_next_word(lexer, "*=")){return lexer_advance_current(lexer, MUL_ASSIGN);}
            if(is_next_word(lexer, "*")){return lexer_advance_current(lexer, MULTIPLY);}
        case '/':
            if(is_next_word(lexer, "/=")){return lexer_advance_current(lexer, DIV_ASSIGN);}
            if(is_next_word(lexer, "/")){return lexer_advance_current(lexer, DIVIDE);}
        case '%':
            if(is_next_word(lexer, "%=")){return lexer_advance_current(lexer, MOD_ASSIGN);}
            if(is_next_word(lexer, "%")){return lexer_advance_current(lexer, PERCENT);}
        case '&':
            if(is_next_word(lexer, "&=")){return lexer_advance_current(lexer, AND_ASSIGN);}
            if(is_next_word(lexer, "&&")){return lexer_advance_current(lexer, AND_OP);}
            if(is_next_word(lexer, "&")){return lexer_advance_current(lexer, AMPERSAND);}
        case '^':
            if(is_next_word(lexer, "^=")){return lexer_advance_current(lexer, XOR_ASSIGN);}
            if(is_next_word(lexer, "^")){return lexer_advance_current(lexer, CARET);}
        case '|':
            if(is_next_word(lexer, "|=")){return lexer_advance_current(lexer, OR_ASSIGN);}
            if(is_next_word(lexer, "||")){return lexer_advance_current(lexer, OR_OP);}
            if(is_next_word(lexer, "|")){return lexer_advance_current(lexer, VERTICAL_BAR);}
        case '=':
            if(is_next_word(lexer, "==")){return lexer_advance_current(lexer, EQ_OP);}
            if(is_next_word(lexer, "=")){return lexer_advance_current(lexer, EQUALS);}
        case '!':
            if(is_next_word(lexer, "!=")){return lexer_advance_current(lexer, NE_OP);} 
            if(is_next_word(lexer, "!")){return lexer_advance_current(lexer, EXCLAMATION_MARK);}
        case ';':
            if(is_next_word(lexer, ";")){return lexer_advance_current(lexer, SEMICOLON);} 
        case '{':
            if(is_next_word(lexer, "{")){return lexer_advance_current(lexer, L_BRACE);} 
        case '}':
            if(is_next_word(lexer, "}")){return lexer_advance_current(lexer, R_BRACE);} 
        case ',':
            if(is_next_word(lexer, ",")){return lexer_advance_current(lexer, COMMA);} 
        case ':':
            if(is_next_word(lexer, ":")){return lexer_advance_current(lexer, COLON);} 
        case '(':
            if(is_next_word(lexer, "(")){return lexer_advance_current(lexer, L_PARENTHESIS);} 
        case ')':
            if(is_next_word(lexer, ")")){return lexer_advance_current(lexer, R_PARENTHESIS);} 
        case '[':
            if(is_next_word(lexer, "[")){return lexer_advance_current(lexer, L_BRACKET);} 
        case ']':
            if(is_next_word(lexer, "]")){return lexer_advance_current(lexer, R_BRACKET);} 
        case '~':
            if(is_next_word(lexer, "~")){return lexer_advance_current(lexer, TILDE);}
        case '?':
            if(is_next_word(lexer, "?")){return lexer_advance_current(lexer, QUESTION_MARK);}
        case '\0':
            break; // breaks to EOF token
        case '\xff':
            break;
            // default:
            //     printf("[Lexer]: Unexpected character `%c` (%d)\n", lexer->c, (int)lexer->c);
            //     exit(1);
            //     break;
        }

        lexer_advance(lexer);
    }
    return init_token(0, END_OF_FILE);
}

token_T* lexer_peek_next_token(lexer_T* lexer, int words){
    lexer_T temp_lexer;

    temp_lexer = *lexer;
    temp_lexer.fp = fdopen (dup (fileno (lexer->fp)), "r");
    // for(int i = 0; i < (words); i++){
    //     lexer_next_token(&lexer);
    // }
    return lexer_next_token(&temp_lexer);
}

int is_next_word(lexer_T *lexer, char *str)
{
    char *temp_str = calloc(strlen(str), sizeof(char));
    for (int i = 0; i < strlen(str); i++)
    {
        temp_str = strcat(temp_str, (char[]){lexer_peek(lexer, i), '\0'});
    }
    return strcmp(temp_str, str);
}

char lexer_peek(lexer_T *lexer, int offset)
{
    return fseek(lexer->fp, offset, SEEK_CUR);
}

void lexer_advance(lexer_T *lexer)
{

    if (lexer->c != '\0')
    {
        lexer->c = fgetc(lexer->fp);
    }
}

token_T *lexer_advance_current(lexer_T *lexer, int type)
{
    char *value = calloc(2, sizeof(char));
    value[0] = lexer->c;
    value[1] = '\0';

    token_T *token = init_token(value, type);
    lexer_advance(lexer);

    return token;
}

token_T *lexer_advance_with(lexer_T *lexer, token_T *token)
{
    lexer_advance(lexer);
    return token;
}

void lexer_skip_whitespace(lexer_T *lexer)
{
    while (lexer->c == 13 || lexer->c == 10 || lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n')
        lexer_advance(lexer);
}

void lexer_skip_comment(lexer_T *lexer)
{
    if (lexer->c == '/')
    {
        lexer_advance(lexer);
        if (lexer->c == '/')
        {
            while (lexer->c != '\n')
            {
                lexer_advance(lexer);
            }
            lexer_advance(lexer);
        }
        else if (lexer->c == '*')
        {
            lexer_advance(lexer);
            while(lexer->c != '*' && lexer_peek(lexer, 1) != '/'){
                lexer_advance(lexer);
            }
            lexer_advance(lexer);
        }
    }
    lexer_skip_whitespace(lexer);
}

token_T *lexer_parse_id(lexer_T *lexer)
{
    char *value = calloc(1, sizeof(char));
    while (isalpha(lexer->c))
    {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        lexer_advance(lexer);
    }
    if (strcmp(value, "auto") == 0)
        return init_token(value, AUTO);

    

    if (strcmp(value, "auto") == 0)
        return init_token(value, AUTO);
    if (strcmp(value, "break") == 0)
        return init_token(value, BREAK);
    if (strcmp(value, "case") == 0)
        return init_token(value, CASE);
    if (strcmp(value, "char") == 0)
        return init_token(value, CHAR);
    if (strcmp(value, "const") == 0)
        return init_token(value, CONST);
    if (strcmp(value, "continue") == 0)
        return init_token(value, CONTINUE);
    if (strcmp(value, "default") == 0)
        return init_token(value, DEFAULT);
    if (strcmp(value, "do") == 0)
        return init_token(value, DO);
    if (strcmp(value, "double") == 0)
        return init_token(value, DOUBLE);
    if (strcmp(value, "else") == 0)
        return init_token(value, ELSE);
    if (strcmp(value, "enum") == 0)
        return init_token(value, ENUM);
    if (strcmp(value, "extern") == 0)
        return init_token(value, EXTERN);
    if (strcmp(value, "float") == 0)
        return init_token(value, FLOAT);
    if (strcmp(value, "for") == 0)
        return init_token(value, FOR);
    if (strcmp(value, "goto") == 0)
        return init_token(value, GOTO);
    if (strcmp(value, "if") == 0)
        return init_token(value, IF);
    if (strcmp(value, "int") == 0)
        return init_token(value, INT);
    if (strcmp(value, "long") == 0)
        return init_token(value, LONG);
    if (strcmp(value, "register") == 0)
        return init_token(value, REGISTER);
    if (strcmp(value, "return") == 0)
        return init_token(value, RETURN);
    if (strcmp(value, "short") == 0)
        return init_token(value, SHORT);
    if (strcmp(value, "signed") == 0)
        return init_token(value, SIGNED);
    if (strcmp(value, "sizeof") == 0)
        return init_token(value, SIZEOF);
    if (strcmp(value, "static") == 0)
        return init_token(value, STATIC);
    if (strcmp(value, "struct") == 0)
        return init_token(value, STRUCT);
    if (strcmp(value, "switch") == 0)
        return init_token(value, SWITCH);
    if (strcmp(value, "typedef") == 0)
        return init_token(value, TYPEDEF);
    if (strcmp(value, "union") == 0)
        return init_token(value, UNION);
    if (strcmp(value, "unsigned") == 0)
        return init_token(value, UNSIGNED);
    if (strcmp(value, "void") == 0)
        return init_token(value, VOID);
    if (strcmp(value, "volatile") == 0)
        return init_token(value, VOLATILE);
    if (strcmp(value, "while") == 0)
        return init_token(value, WHILE);
    if (strcmp(value, "constant") == 0)
        return init_token(value, CONSTANT);
    
    return init_token(value, IDENTIFIER);
}

token_T *lexer_parse_number(lexer_T *lexer)
{
    char *value = calloc(1, sizeof(char));

    while (isdigit(lexer->c))
    {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        lexer_advance(lexer);
    }

    return init_token(value, INT);
}

token_T *lexer_parse_string(lexer_T *lexer)
{
    char *value = calloc(1, sizeof(char));

    lexer_advance(lexer);

    while (lexer->c != '"')
    {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(value, STRING_LITERAL);
}

token_T *lexer_parse_char(lexer_T *lexer)
{
    char *value = calloc(1, sizeof(char));
    lexer_advance(lexer);
    *value = lexer->c;
    lexer_advance(lexer);
    return init_token(value, CHAR);
}