#include "token.h"
#include <stdlib.h>


token_T* init_token(char tokenText, int tokenKind)
{
    token_T* token = calloc(1, sizeof(token_T));
    token->text = tokenText;
    token->kind = tokenKind;
    return token;
}
