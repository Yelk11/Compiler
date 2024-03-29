#ifndef TOKEN_H
#define TOKEN_H

typedef struct token_T
{
    char *value;
    enum
    {
        IDENTIFIER,
        AUTO,
        BREAK,
        CASE,
        CHAR,
        CONST,
        CONTINUE,
        DEFAULT,
        DO,
        DOUBLE,
        ELSE,
        ENUM,
        EXTERN,
        FLOAT,
        FOR,
        GOTO,
        IF,
        INT,
        LONG,
        REGISTER,
        RETURN,
        SHORT,
        SIGNED,
        SIZEOF,
        STATIC,
        STRUCT,
        SWITCH,
        TYPEDEF,
        UNION,
        UNSIGNED,
        VOID,
        VOLATILE,
        WHILE,
        CONSTANT,
        STRING_LITERAL,
        ELLIPSIS,
        RIGHT_ASSIGN,
        LEFT_ASSIGN,
        ADD_ASSIGN,
        SUB_ASSIGN,
        MUL_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN,
        AND_ASSIGN,
        XOR_ASSIGN,
        OR_ASSIGN,
        RIGHT_OP,
        LEFT_OP,
        INC_OP,
        DEC_OP,
        PTR_OP,
        AND_OP,
        OR_OP,
        LE_OP,
        GE_OP,
        EQ_OP,
        NE_OP,
        SEMICOLON,
        L_BRACE, //{
        R_BRACE, //}
        COMMA,
        COLON,
        EQUALS,
        L_PARENTHESIS,
        R_PARENTHESIS,
        L_BRACKET,
        R_BRACKET,
        PERIOD,
        AMPERSAND,
        EXCLAMATION_MARK,
        TILDE,
        MINUS,
        PLUS,
        MULTIPLY,
        DIVIDE,
        PERCENT,
        LESS_THAN,
        GREATER_THAN,
        CARET, // ^
        VERTICAL_BAR,
        QUESTION_MARK,
        END_OF_FILE,
    } type;
} token_T;

token_T *init_token(char *value, int type);
char *type_to_string(int token);

#endif