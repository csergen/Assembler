#include "util.h"
#include "assembler.h"
#include "args.h"

int main(int argc, char **argv) 
{
    char* m_file_path = argparse(argc, argv);

    StreamObject *streamObject = open_stream(m_file_path, "r");
    char *f_source = read_stream(streamObject);

    printf("%s", f_source);

    free(f_source);
    close_stream(streamObject);
    return 0;
}
