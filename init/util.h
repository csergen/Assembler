#ifndef _INIT_UTIL_H_
#define _INIT_UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

typedef struct 
{
      FILE *stream;
      char* path;
      u_int32_t size;
      bool status;
      char* mode;
} StreamObject;

StreamObject *open_stream(char *_filepath, char *_mode);

bool close_stream(StreamObject *_streamObject);

bool write_stream(StreamObject *_streamObject, char *const text_);

char *read_stream(StreamObject *_streamObject);

#endif
