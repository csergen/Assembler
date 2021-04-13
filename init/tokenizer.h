#ifndef __INIT_TOKENIZER_H__
#define __INIT_TOKENIZER_H__

#define NUMBER      0x00
#define STRING      0x01
#define NEWLINE     0x02
#define ENDMARKER   0x03
#define INDENT      0x04
#define DEDENT      0x05
#define OFFSET      0x06


#define ISEOF(x)            ((x) == ENDMARKER)
#define ISTERMINAL(x)       ((x) < OFFSET)
#define ISNONTERMINAL(x)    ((x) >= OFFSET)
#define ISWHITESPACE(x)     ((x) == ENDMARKER || \
                             (x) == NEWLINE   || \
                             (x) == INDENT    || \
                             (x) == DEDENT)

#endif