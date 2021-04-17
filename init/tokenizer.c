#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer.h"


static TokenObject*
new_token(void)
{
   TokenObject *tok = (TokenObject *) malloc(sizeof(TokenObject));
   
   if (tok == NULL)
      return NULL;

   tok->word = (char *) malloc(sizeof(tok->word)+1);
   tok->lineno = 0;
   tok->colstart = 0;
   tok->colend = 0;
   tok->type = 0;

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
    int lc = 0;
    int c = 0;
    char curchr;
    int curtype;
    TokenObject *tok = new_token();
    
    next_state(source, &c, &curchr, &curtype);
    while (c < strlen(source))
    {
        switch (curtype) {
            case STRING:
                do {
                    add_char(tok->word, curchr);
                    next_state(source, &c, &curchr, &curtype);
                    tok->colend++;
                } while (curtype == STRING || curtype == NUMBER);
                tok->colend--;
                break;

            case NUMBER:
                do {
                    add_char(tok->word, curchr);
                    next_state(source, &c, &curchr, &curtype);
                    tok->colend++;
                } while (curtype == NUMBER);
                tok->colend--;
                break;

            case COMMA:
            case LSQB:
            case RSQB:
                add_char(tok->word, curchr);
                next_state(source, &c, &curchr, &curtype);
                break;

            case WHITESPACE:
                next_state(source, &c, &curchr, &curtype);
                tok->colstart++;
                tok->colend++;
                continue;

            case NEWLINE:
                //add_char(tok->word, curchr);
                next_state(source, &c, &curchr, &curtype);
                lineno++;
                break;

            case NAN:
                exit(1);

        }

        if (tok) {
            printf("%d\t%s\t%d\t%d\n", tok->lineno, tok->word, tok->colstart, tok->colend);

            int buffer_colend = tok->colend + 1;

            tok = new_token();
            tok->colstart = buffer_colend;
            tok->colend = tok->colstart;
            tok->lineno = lineno;

            lc++;
        }
    }
    
    return tokens;
}
