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
        free(temp_streamObject);
        return 0;
    }

    if (temp_streamObject->size == 0)
        perror("File is empty");

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
    int status = 1;

    if (_streamObject == NULL)
        fprintf(stdout, "Stream object is not exist");
    else if (_streamObject->status)
    {
        status = fclose(_streamObject->stream) ? 1 : 0;
        if (status == 0) 
        { 
            _streamObject->stream = NULL;
            _streamObject->path = NULL;
            _streamObject->size = 0;
            _streamObject->status = NULL;
            _streamObject->mode = NULL;

            free(_streamObject->stream);
            free(_streamObject->mode);
            free(_streamObject);
            //_streamObject = NULL;
        }
    }
    else
        fprintf(stdout, "File is already closed");

    return status;
}

void write_stream(StreamObject *_streamObject, char *const text_)
{
    fprintf(_streamObject->stream, "%s", text_);
}

char *read_stream(StreamObject *_streamObject)
{
    if (!_streamObject->status
        || _streamObject->size == 0)
        return NULL;

    char *m_destination = malloc(sizeof(char) * _streamObject->size);
    int m_line_counter = 0;
    char m_line;

    while ((m_line=fgetc(_streamObject->stream)) != EOF)
        m_destination[m_line_counter++] = m_line;

    return m_destination;
}
