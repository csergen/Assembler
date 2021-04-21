#ifndef __INIT_ARGS_H__
#define __INIT_ARGS_H__

#include <limits.h>

/**
 * @brief This function returns the argument value entered by the user
 *        [ ./Executable {argument} ]
 * 
 * @param argc
 * @param argv 
 * @return char* 
 */

const char *args_program_version = "";
const char *args_program_email = "";
static char doc[] = "";
static char args_doc[] = "";


struct arguments
{
    char *arg1;
    char **strings;
    enum { SOURCE_MODE, FOLDER_MODE } mode;
    int repeat_count;
};


char *argparse(int argc, char **argv)
{
    char *_filepath = malloc(sizeof(_filepath) * FILENAME_MAX);

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
        fscanf(stdin, "%s", _filepath);
    }

    return _filepath;
}

#endif
