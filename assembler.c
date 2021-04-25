#include "util.h"
#include "args.h"

#include "node.h"

#include "tokenizer.h"
#include "parser.h"

#include "vm.h"


int main(int argc, char **argv)
{
    char *m_file_path = argparse(argc, argv);

    StreamObject *streamObject = open_stream(m_file_path, "r");
    if (streamObject == NULL)
        exit(EXIT_FAILURE);

    char *f_source = read_stream(streamObject);

    //TokenObject **tokens = (TokenObject**) malloc(sizeof(TokenObject*)*streamObject->size);
    //const int token_size = tokenize(f_source, tokens);
    TokenNode *tk = tokenize(f_source);
    TokenNode *tk_iter = tk;
    //Tokenizer output

    /*//parse(tk_iter);
    while (tk)
    {
        if (tk->type == NEWLINE)
            printf("\\n\t(%d)\n", tk->type);
        else
            printf("%s\t(%d)\n", tk->word, tk->type);
        tk = tk->next;
    }
    */
    
    load_program("../out2.hex");
    free(tk);
    //free(tk_iter);
    free(m_file_path);
    free(f_source);

    close_stream(streamObject);
    return 0;
}
