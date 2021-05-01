#ifndef __CORE_ARGS_H__
#define __CORE_ARGS_H__

#include <limits.h>

char *argparse(int argc, char **argv)
{
    char *_filepath = malloc(sizeof(char) * FILENAME_MAX);

    if (argc > 2)
    {
        perror("Too many argument");
        exit(EXIT_FAILURE);
    }
    else if (argc == 2)
    {
        strcpy(_filepath, argv[1]);
    }
    else
    {
        printf("Enter file path: ");
        scanf("%s", _filepath);
        //fscanf(stdin, "%s", _filepath);
    }

    return _filepath;
}

#endif
