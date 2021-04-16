#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer.h"


static TokenObject*
new_token(char* word,
          int lineno,
          int colstart,
          int colend,
          int type)
{
   TokenObject *tok = (TokenObject *) malloc(sizeof(TokenObject));
   
   if (tok == NULL)
      return NULL;

   tok->word = (char *) malloc(sizeof(word)+1);
   strcpy(tok->word, word);

   tok->lineno = lineno;
   tok->colstart = colstart;
   tok->colend = colend;
   tok->type = type;

   return tok;
}

static inline int
get_char_type(char curchr)
{
    if (isdigit(curchr))
        return NUMBER;
    else if (isalpha(curchr))
        return STRING;

    switch (curchr)
    {
        case ',':
            return COMMA;
        case '[':
            return LSQB;
        case ']':
            return RSQB;
        case '\n':
            return NEWLINE;
        case ' ':
            return WHITESPACE;
        case '\0':
            return ENDMARKER;
    }

    return NAN;
}

static inline void
next_state(char *source, int *c, char *curchr, int *curtype)
{
    *curchr = source[(*c)++];
    *curtype = get_char_type(*curchr);
}

static inline void
add_char(char *word, char curchr)
{
    char *buffer = (char *) malloc(sizeof(word) * sizeof(curchr) + 1);

    strcpy(buffer, word);
    strcat(buffer, &curchr);
    memcpy(word, buffer, strlen(buffer) + strlen(word) + 1);
    free(buffer);
}

static inline int
get_word_type(char *word)
{
    return NAN;
}


TokenObject*
tokenize(char *source)
{
    TokenObject* tokens = (TokenObject*) malloc(sizeof(TokenObject));
    int lineno = 0;
    int colstart = 0;
    int colend = 0;
    int lc = 0;
    int c = 0;
    char curchr;
    int curtype;
    char* word = malloc(sizeof(word));
    
    next_state(source, &c, &curchr, &curtype);
    while (c <= strlen(source))
    {
        switch (curtype) {
            case STRING:
                do {
                    add_char(word, curchr);
                    next_state(source, &c, &curchr, &curtype);
                    colend++;
                } while (curtype == STRING || curtype == NUMBER);
                colend--;
                break;
            
            case NEWLINE:
            case WHITESPACE:
                printf("%s\t%d\t%d\t%c\n", word, colstart, colend, curchr);
                next_state(source, &c, &curchr, &curtype);
                memset(word, 0, strlen(word));
                colend += 2;
                colstart = colend;
                break;
        }
    }
    
    free(word);   
    return tokens;
}
