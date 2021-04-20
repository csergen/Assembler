#include "util.c"

int main()
{
    char *m_file_path = "../test.asm";

    StreamObject *streamObject = open_stream(m_file_path, "r"); // true if this is not NULL

    bool stream_status1 = close_stream(streamObject);
    bool stream_status2 = close_stream(streamObject);
    
    if (stream_status1 == 0 && stream_status2 == 1)
        return 0;

    return 1;
}
