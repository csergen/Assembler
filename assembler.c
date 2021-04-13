#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <util.h>
#include <assembler.h>


int main(int argc, char **argv) 
{
    char m_file_path[FILENAME_MAX] = "";

    if (argc > 2)
    {
        perror("Too many argument");
        exit(EXIT_FAILURE);
    } else if (argc == 2)
    {
        strcpy(m_file_path, argv[1]);
    } else {
        printf("Enter file path: ");
        fscanf(stdin, "%s", m_file_path);
    }

    StreamObject *streamObject = open_stream(m_file_path, "r");
    char *f_source = read_stream(streamObject);

    printf("%s", f_source);

    free(f_source);
    close_stream(streamObject);
    return 0;
}
