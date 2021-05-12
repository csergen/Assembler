#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

static int
get_char_type(char curchr)
{
    if (isdigit(curchr))
        return NUMBER;
    else if (isalpha(curchr))
        return STRING;

    switch (curchr) {
    case ',':
        return COMMA;
    case '[':
        return LSQB;
    case ']':
        return RSQB;
    case ':':
        return COLON;
    case '-':
        return MINUS;
    case '\n':
    case '\r':
        return NEWLINE;
    case ' ':
        return WHITESPACE;
    }

    return NAN;
}

static void
next_state(char* source, int* c, char* curchr, int* curtype)
{
    *curchr = source[(*c)++];
    *curtype = get_char_type(*curchr);
}

static inline void
add_char(char* word, char curchr) { 
    int len = strlen(word);
    word[len] = curchr;
    word[len+1] = '\0';
}

static int
get_word_type(char* word)
{
    if (strlen(word) == 1)
        return get_char_type(word[0]);

    if (strcmp(word, "HRK") == 0
        || strcmp(word, "TOP") == 0
        || strcmp(word, "CIK") == 0
        || strcmp(word, "CRP") == 0
        || strcmp(word, "BOL") == 0
        || strcmp(word, "DEG") == 0
        || strcmp(word, "VE") == 0
        || strcmp(word, "VEYA") == 0
        || strcmp(word, "SS") == 0
        || strcmp(word, "SSD") == 0
        || strcmp(word, "SN") == 0
        || strcmp(word, "SP") == 0)
        return OPCODE;

    if (strcmp(word, "AX") == 0
        || strcmp(word, "BX") == 0
        || strcmp(word, "CX") == 0
        || strcmp(word, "DX") == 0)
        return REGISTER;

    int alpha_counter = 0;
    int digit_counter = 0;

    for (int i = 0; i < strlen(word); i++) 
    {
        if (word[i] == '-')
            digit_counter++;

        else if (isalpha(word[i]) != 0)
            alpha_counter++;

        else if (isdigit(word[i]) != 0)
            digit_counter++;
    }

    if (strlen(word) == digit_counter)
        return CONSTANT;

    if (isalpha(word[0]) && (alpha_counter + digit_counter == strlen(word)))
        return LABEL;

    return NAN;
}

TokenNode*
tokenize(char* source)
{
    int lineno = 0;
    int c = 0;
    char curchr = 0;
    int curtype = 0;
    TokenNode* root = new_token();
    TokenNode* tok = new_token();

    next_state(source, &c, &curchr, &curtype);
    while (c < strlen(source)) {
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
        case MINUS:
            do {
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

        default:
            add_char(tok->word, curchr);
            next_state(source, &c, &curchr, &curtype);
            break;
        }

        if (tok) {
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


    free(tok);
    TokenNode* end_token = new_token();
    end_token->type = ENDMARKER;
    memset(end_token->word, 0, sizeof(char)+1);
    add_token(root, end_token);

    root = root->next;
    return root;
}
