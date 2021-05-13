#ifndef __CORE_COLOR_H__
#define __CORE_COLOR_H__

#ifdef _WIN32
#define RED
#define GRN
#define WHITE
#define BLUE
#define BGRY
#define BRED
#define RESET

#else
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define WHITE "\x1B[37m"
#define BLUE "\x1B[36m"
#define GRY "\x1B[30m"
#define BGRY "\x1B[40m"
#define BRED "\x1B[41m"
#define RESET "\033[0m"
#endif

#endif
