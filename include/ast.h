#ifndef AST_H
#define AST_H

#include "token.h"


/*
good documentation on ASTs
https://ruslanspivak.com/lsbasi-part7/
*/
typedef enum node_type
{
    /*
    Literal Nodes: Represent constant values.
        IntegerLiteral, FloatLiteral, CharLiteral, StringLiteral.
    Identifier Nodes: Represent variable names or function names.
        Identifier.
    Binary Expression Nodes: Represent binary operations.
        Add, Subtract, Multiply, Divide, Modulo, And, Or, Xor, ShiftLeft, ShiftRight, Equal, NotEqual, LessThan, GreaterThan, LessEqual, GreaterEqual.
    Unary Expression Nodes: Represent unary operations.
        UnaryMinus, UnaryPlus, Not, BitwiseNot, AddressOf, Dereference, SizeOf.
    Assignment Nodes: Represent assignment operations.
        Assign, AddAssign, SubtractAssign, MultiplyAssign, DivideAssign, ModuloAssign, AndAssign, OrAssign, XorAssign, ShiftLeftAssign, ShiftRightAssign.
    Function Call Nodes: Represent function calls.
        FunctionCall.
    Member Access Nodes: Represent access to struct members.
        MemberAccess, PointerMemberAccess.
    Array Subscript Nodes: Represent array indexing.
        ArraySubscript.
    */
    EXPRESSION_NODE,
    /*
    Declaration Nodes: Represent variable and function declarations.
        VariableDeclaration, FunctionDeclaration.
    Expression Statement Nodes: Represent expressions used as statements.
        ExpressionStatement.
    Return Nodes: Represent return statements.
        Return.
    If Nodes: Represent conditional statements.
        If, IfElse.
    Switch Nodes: Represent switch statements.
        Switch.
    Case Nodes: Represent case labels in switch statements.
        Case, Default.
    While Nodes: Represent while loops.
        While.
    Do-While Nodes: Represent do-while loops.
        DoWhile.
    For Nodes: Represent for loops.
        For.
    Break Nodes: Represent break statements.
        Break.
    Continue Nodes: Represent continue statements.
        Continue.
    Compound Statement Nodes: Represent a block of statements enclosed in braces.
        CompoundStatement.
    */
    STATEMENT_NODE,
    /*
    Variable Declaration Nodes: Represent variable declarations.
        VariableDeclaration.
    Function Declaration Nodes: Represent function declarations.
        FunctionDeclaration.
    Struct Declaration Nodes: Represent struct type declarations.
        StructDeclaration.
    Union Declaration Nodes: Represent union type declarations.
        UnionDeclaration.
    Enum Declaration Nodes: Represent enumeration type declarations.
        EnumDeclaration.
    */
    DECLARATION_NODE,
    /*
    Primitive Type Nodes: Represent basic types.
        IntType, FloatType, CharType, VoidType, DoubleType, ShortType, LongType, SignedType, UnsignedType.
    Pointer Type Nodes: Represent pointer types.
        PointerType.
    Array Type Nodes: Represent array types.
        ArrayType.
    Function Type Nodes: Represent function types.
        FunctionType.
    Struct Type Nodes: Represent struct types.
        StructType.
    Union Type Nodes: Represent union types.
        UnionType.
    Enum Type Nodes: Represent enum types.
        EnumType.
    */
    TYPE_NODE,
    /*
    If-Else Nodes: Represent if-else conditional structures.
        If, IfElse.
    Switch Nodes: Represent switch-case structures.
        Switch, Case, Default.
    Loop Nodes: Represent loops.
        While, DoWhile, For.
    */
    CONTROL_FLOW_NODE,

} n_type;

// typedef struct node node;

// typedef struct pair_value
// {
//     node *right;
//     node *left;
// } pair_value;

// typedef char *string_constant;

// // Union only allows for one of the variables to be populated
// union node_value
// {
//     pair_value pair;
//     string_constant str;
// } node_value;

// typedef struct node
// {
//     node_type type;
//     union node_value value;
// } node;






typedef struct node_T {
    struct node_T* right;
    struct node_T* left;
    enum node_type node_type;
    struct token_T* token;
}node_T;

node_T *init_node(n_type type, token_T* token);
// node_T *init_node_pair(n_type* type, char *value, node_T *left, node_T *right);

node_T* node_expression(node_T* parent, token_T* token);
node_T* node_statement(node_T* parent, token_T* token);
node_T* node_declaration(node_T* parent, token_T* token);
node_T* node_type(node_T* parent, token_T* token);
node_T* node_control_flow(node_T* parent, token_T* token);
#endif