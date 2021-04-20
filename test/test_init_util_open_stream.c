#include "util.c"

int main()
{
    char *m_file_path_1 = "test.asm";
    char *m_file_path_2 = "../test.asm";

    StreamObject *streamObject1 = open_stream(m_file_path_1, "r"); // true if this is NULL
    StreamObject *streamObject2 = open_stream(m_file_path_2, "r"); // true if this is not NULL

    if (streamObject1 == NULL && streamObject2 != NULL)
        return 0;

    return 1;
}
