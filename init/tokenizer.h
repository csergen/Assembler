#ifndef __INIT_TOKENIZER_H__
#define __INIT_TOKENIZER_H__

#define NAN         0x00
#define NUMBER      0x01
#define STRING      0x02
#define COMMA	    0x03
#define LSQB	    0x04
#define RSQB        0x05
#define NEWLINE     0x06
#define ENDMARKER   0x07
#define WHITESPACE  0x08

typedef struct 
{
   char* word;
   int lineno;
   int colstart;
   int colend;
   int type;   
} TokenObject;


static TokenObject* new_token(void);
const int tokenize(char*, TokenObject**);

#endif
