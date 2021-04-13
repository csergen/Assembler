#include "util.h"

/**
 * @brief This function opens the file.
 * 
 * @param _filepath 
 * @param _mode 
 * @return StreamObject* 
 */
StreamObject *open_stream(char *_filepath, char *_mode)
{
    StreamObject *temp_streamObject = malloc(sizeof(StreamObject));
    temp_streamObject->path = _filepath;
    temp_streamObject->mode = _mode;
    temp_streamObject->stream = fopen(temp_streamObject->path,
                                      temp_streamObject->mode);
    struct stat st;
    stat(temp_streamObject->path, &st);
    temp_streamObject->size = st.st_size;

    if (!temp_streamObject->stream)
    {
        perror("File Does Not Exist");
        exit(EXIT_FAILURE);
    }

    if (temp_streamObject->size == 0)
    {
        perror("File is empty");
        exit(EXIT_FAILURE);
    }

    temp_streamObject->status = true;
    return temp_streamObject;
}

/**
 * @brief This function closes the file.
 * 
 * @param _streamObject 
 * @return boolean
 */
bool close_stream(StreamObject *_streamObject)
{
    int status = false;
    if (_streamObject->status)
    {
        status = fclose(_streamObject->stream) ? false : true;
        if (status) free(_streamObject);
    }
    else
        fprintf(stdout, "File is already closed");

    return status;
}

bool write_stream(StreamObject *_streamObject, char *const text_)
{
    return true;
}

char *read_stream(StreamObject *_streamObject)
{
    if (!_streamObject->status
        || _streamObject->size == 0)
        return NULL;

    char *m_destination = malloc(sizeof(m_destination) * _streamObject->size);
    int m_line_counter = 0;
    char m_line;

    while ((m_line=fgetc(_streamObject->stream)) != EOF)
        m_destination[m_line_counter++] = m_line;

    return m_destination;
}
