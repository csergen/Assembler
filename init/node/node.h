#ifndef __INIT_NODE_H__
#define __INIT_NODE_H__

typedef struct node
{
    char *word;
    int type;
    int colstart;
    int colend;
    int lineno;
    struct node *next;
} TokenNode;


TokenNode *new_token(void);
void add_token(TokenNode *, TokenNode *);
#endif
