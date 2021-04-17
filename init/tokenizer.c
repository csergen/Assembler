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
    strncat(word, &curchr, 1);
}

static inline int
get_word_type(char *word)
{
    if (strlen(word) == 1)
        return get_char_type(word[0]);

    int alpha_counter = 0;
    int digit_counter = 0;

    for (int i = 0; i < strlen(word); i++) {
        if (isalpha(word[i]) != 0)
            alpha_counter++;

        if (isdigit(word[i]) != 0)
            digit_counter++;
    }

    if (strlen(word) == digit_counter)
        return NUMBER;

    if (isalpha(word[0]) && (alpha_counter + digit_counter == strlen(word)))
        return STRING;
    
    return NAN;
}


const int
tokenize(char *source, TokenObject **tokens)
{
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
                add_char(tok->word, curchr);
                next_state(source, &c, &curchr, &curtype);
                lineno++;
                break;

            case NAN:
                return NAN;

        }

        if (tok) {
            tokens[lc] = malloc(sizeof(tok));

            tokens[lc]->word = malloc(sizeof(tok->word)+1);
            strcpy(tokens[lc]->word, tok->word);

            tokens[lc]->lineno = lineno;
            tokens[lc]->colstart = tok->colstart;
            tokens[lc]->colend = tok->colend;
            tokens[lc]->type = get_word_type(tokens[lc]->word);

            tok = new_token();
            tok->colstart = tokens[lc]->colend + 1;
            tok->colend = tok->colstart;
            tok->lineno = lineno;
            tok->type = 0;

            lc++;
        }
    }
    
    free(tok->word);
    free(tok);
    return lc;
}
