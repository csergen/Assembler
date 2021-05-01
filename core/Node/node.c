#include "node.h"
#include <stdlib.h>
#include <string.h>

TokenNode *new_token(void)
{
    TokenNode *temp = (TokenNode *)malloc(sizeof(TokenNode));

    temp->word = (char *)malloc(sizeof(char *) + 1);
    temp->type = 0;
    temp->colstart = 0;
    temp->colend = 0;
    temp->lineno = 0;
    temp->next = NULL;

    return temp;
}

void add_token(TokenNode *r, TokenNode *n)
{
    if (n != NULL)
    {
        TokenNode *temp = new_token();
        strcpy(temp->word, n->word);
        temp->type = n->type;
        temp->colstart = n->colstart;
        temp->colend = n->colend;
        temp->lineno = n->lineno;

        TokenNode *t = r;

        while (t->next)
            t = t->next;

        t->next = temp;
    }
}