#include "lexer.h"
#include "token.h"
#include "parse.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    lexer_T *lexer;
    char *file_name;
    int errnum;
    if (argc > 1)
    {
        file_name = argv[1];
    }
    else
    {
        file_name = "example/test.c";
    }

    long lSize;
    char *buffer;

    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror(errnum));

        exit(2);
    }
    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    /* allocate memory for entire content */
    buffer = calloc(1, lSize + 1);
    if (!buffer)
        fclose(fp), fputs("memory alloc fails", stderr), exit(1);

    /* copy the file into the buffer */
    if (1 != fread(buffer, lSize, 1, fp))
        fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);
    fclose(fp);
    /* do your work here, buffer is a string contains the whole text */

    lexer = init_lexer(buffer);

    token_T *token;
    while (1)
    {
        token = lexer_next_token(lexer);
        printf("%s : %s\n", type_to_string(token->type), token->value);
        if (token->type == END_OF_FILE)
        {
            break;
        }
    }

    free(buffer);
    return 0;
}
