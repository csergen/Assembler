#include "util.h"
#include "tokenizer.h"
#include "assembler.h"
#include "args.h"


int main(int argc, char **argv) 
{
    char* m_file_path = argparse(argc, argv);

    StreamObject *streamObject = open_stream(m_file_path, "r");
    char *f_source = read_stream(streamObject);
    
    TokenObject **tokens = (TokenObject**) malloc(sizeof(TokenObject*)*streamObject->size);
    const int token_size = tokenize(f_source, tokens);
    
    /* // Tokenizer output
    for (int i = 0; i < token_size; i++)
    {
        if (tokens[i]->type == NEWLINE) 
            tokens[i]->word = "\\n";
        printf("%d\t%s\t%d\t%d\t%d\n", tokens[i]->lineno, tokens[i]->word, tokens[i]->colstart, tokens[i]->colend, tokens[i]->type);
    }
    */

    for (int i = 0; i < streamObject->size; i++) 
        free(tokens[i]);
    free(tokens);

    free(m_file_path);
    free(f_source);

    close_stream(streamObject);
    return 0;
}
