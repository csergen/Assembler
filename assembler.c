#include "util.h"
#include "args.h"

#include "node.h"

#include "tokenizer.h"
#include "parser.h"

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
    //Tokenizer output
    while (tk) {
        printf("%s\t%d\t%d\t%d\n", tk->word, tk->colstart, tk->colend, tk->type);
        tk = tk->next;
    }

    parse(tk);

    free(tk);
    free(m_file_path);
    free(f_source);

    close_stream(streamObject);
    return 0;
}
