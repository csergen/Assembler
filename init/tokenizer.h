#ifndef __INIT_TOKENIZER_H__
#define __INIT_TOKENIZER_H__

#define NAN         0x00
#define NUMBER      0xA1
#define STRING      0xA2
#define COMMA	      0xA3
#define LSQB	      0xA4
#define RSQB        0xA5
#define NEWLINE     0xA6
#define ENDMARKER   0xA7
#define WHITESPACE  0xA8
#define COLON       0xA9
#define OFFSET      0xAA

#define ISEOF(x)            ((x) == ENDMARKER)
#define ISTERMINAL(x)       ((x) < OFFSET)
#define ISNONTERMINAL(x)    ((x) >= OFFSET)
#define ISWHITESPACE(x)     ((x) == WHITESPACE)
#define ISNEWLINE(x)        ((x) == NEWLINE)

#include "node.h"

TokenNode* tokenize(char*);

#endif
