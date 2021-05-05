#ifndef __CORE_UTIL_UTIL_H__
#define __CORE_UTIL_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/stat.h>




typedef struct 
{
      FILE *stream;
      char* path;
      int size;
      bool status;
      char* mode;
} StreamObject;

StreamObject *open_stream(char *_filepath, char *_mode);
bool close_stream(StreamObject *_streamObject);
void write_stream(StreamObject *_streamObject, char *const text_);
char *read_stream(StreamObject *_streamObject);

#endif
