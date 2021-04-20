#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer.h"

static int
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
    case ':':
        return COLON;
    case '\n':
        return NEWLINE;
    case ' ':
        return WHITESPACE;
    default:
        return OFFSET;
    }

    return NAN;
}

static void
next_state(char *source, int *c, char *curchr, int *curtype)
{
    *curchr = source[(*c)++];
    if (curchr == NULL)
        *curchr = '\n';
    *curtype = get_char_type(*curchr);
}

static inline void
add_char(char *word, char curchr) { strncat(word, &curchr, 1); }

static int
get_word_type(char *word)
{
    if (strlen(word) == 1)
        return get_char_type(word[0]);

    int alpha_counter = 0;
    int digit_counter = 0;

    for (int i = 0; i < strlen(word); i++)
    {
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

TokenNode*
tokenize(char *source)
{
    int lineno = 0;
    int lc = 0;
    int c = 0;
    char curchr;
    int curtype;
    //TokenObject *tok = new_token();
    TokenNode *root = new_token();
    TokenNode *tok = new_token();

    next_state(source, &c, &curchr, &curtype);
    while (c < strlen(source))
    {
        switch (curtype)
        {
        case STRING:
            do
            {
                add_char(tok->word, curchr);
                next_state(source, &c, &curchr, &curtype);
                tok->colend++;
            } while (curtype == STRING || curtype == NUMBER);
            tok->colend--;
            break;

        case NUMBER:
            do
            {
                add_char(tok->word, curchr);
                next_state(source, &c, &curchr, &curtype);
                tok->colend++;
            } while (curtype == NUMBER);
            tok->colend--;
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

        case LSQB:
        case RSQB:
        case COLON:
        case COMMA:
        case NAN:
            add_char(tok->word, curchr);
            next_state(source, &c, &curchr, &curtype);
            break;
        
        case OFFSET:
            return NULL;
        }

        if (tok)
        {
            tok->lineno = lineno;
            tok->type = get_word_type(tok->word);
            add_token(root, tok);
            
            int buffer_colend = tok->colend;

            tok->next = new_token();
            tok = tok->next;
            tok->colstart = buffer_colend + 1;
            tok->colend = tok->colstart;
            tok->lineno = lineno;
            tok->type = 0;
        }
    }

    free(tok->word);
    free(tok);

    return root;
}
