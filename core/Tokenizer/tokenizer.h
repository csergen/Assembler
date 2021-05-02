#ifndef __CORE_TOKENIZER_TOKENIZER_H__
#define __CORE_TOKENIZER_TOKENIZER_H__

#define NAN         0
#define NUMBER      1
#define STRING      2
#define COMMA	      3
#define LSQB	      4
#define RSQB        5
#define COLON       6
#define MINUS       7
#define DOT         8
#define NEWLINE     9
#define ENDMARKER   10
#define WHITESPACE  11
#define OPCODE      12
#define REGISTER    13
#define LABEL       14
#define CONSTANT    15
#define OFFSET      16

#define ISEOF(x)            ((x) == ENDMARKER)
#define ISTERMINAL(x)       ((x) < OFFSET)
#define ISNONTERMINAL(x)    ((x) >= OFFSET)
#define ISWHITESPACE(x)     ((x) == WHITESPACE)
#define ISNEWLINE(x)        ((x) == NEWLINE)

#include "node.h"

TokenNode* tokenize(char*);

#endif
