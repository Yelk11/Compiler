
Introduction to Parsing for Compilers

At its core the parser's job is to take an input stream of charectors and do three operations on that stream. The first is to break the stream into tokens, or small groups of charectors seperated by a delimiter; commonly a space. The next operation is to give the tokens more context to what they are. The lexer attaches datatype to the token, like if it is a parenthesis or addition sign. Last, the parser will assemble the tokens into a abrstract syntax tree, or AST. The AST is a tree datastructure that holds the form of an expression. 



Introduction to Lexical Analysis

Lexical Analysis is the practice of braking down a sequence of charectors into tokens.




Implementation of Lexer and Tokenizer

The lexer and tokenzer work hand in hand. Tokens are really structs that hold the raw value and the token type that is assigned to it by the lexer, and the lexer handles the rest of the operation.

The lexer has two main functions:

void Next()
Next() moves the lxer forword one token.

char Peak(int offset)
Peak() shows the next Nth charector in the stream, where N = index + offset.


Implementation of AST

E Expression
T Term
F Factor

E -> T + E
E -> T - E
E -> T

T -> F * T
T -> F / T
T -> F

F -> ID
F -> Integer
F -> (E) -F












Refrences
Recursive Descent Parsing
https://www.youtube.com/watch?v=SToUyjAsaFk&t=1122s




