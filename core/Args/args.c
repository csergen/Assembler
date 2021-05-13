#include "args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *argparse(int argc, char **argv)
{
    char *buffer = malloc(sizeof(char) * FILENAME_MAX);

    if (argc > 2)
    {
        perror("Too many argument");
        exit(EXIT_FAILURE);
    }
    else if (argc == 2)
    {
        strcpy(buffer, argv[1]);
    }
    else
    {
        printf("Enter file path: ");
        scanf("%255[^\n]%*c", buffer);
    }
    return buffer;
}