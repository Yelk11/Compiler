#include "lexer.h"
#include "token.h"
#include "parse.h"
#include <stdlib.h>


int main(int argc, char *argv[])
{
    lexer_T *lexer;
    char *file_name;
    if (argc > 1)
    {
        file_name = argv[1];
    }
    else
    {
        file_name = "test.c";
    }
    
    

    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen(file_name, "rb");
    if (!fp)
        perror("blah.txt"), exit(1);

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



    printf("%d\n",parse(lexer));
    // token_T* token;
    // while(1){
        
    //     token = lexer_next_token(lexer);
    //     printf("%s : %s\n",type_to_string(token->type), token->value);
    //     if (token->type == END_OF_FILE){break;}
    // }

    free(buffer);
    return 0;
}
