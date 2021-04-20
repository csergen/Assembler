#include <stdio.h>
#include <stdlib.h>

#include "node.h"

/*
#include "parser.h"
#include "tokenizer.h"

static int s_current_iter;
static TokenObject** s_tokens;
static TokenObject* s_current_token;
static int s_size;

static void
next_token()
{
    if (s_current_iter < s_size) {
        s_current_token = s_tokens[s_current_iter];
        s_current_iter++;
    } else {
        s_current_token = NULL;
    }
}

static void
label_stmt(void)
{
    next_token();

    if (s_current_token == COLON)
        next_token();
}

static void 
opcode_stmt()
{

}

static void 
register_stmt()
{

}

static void
field_stmt(void)
{
    if (s_current_token == STRING) 
    {

    }
}

static void
program_stmt(void)
{
    while (s_current_iter < s_size) {
        if (ISNEWLINE(s_current_token->type)) {

        } else if (ISTERMINAL(s_current_token->type)) {
            field_stmt();
        } else {
            printf("ERROR");
            exit(1);
        }

        next_token();
    }
}
*/

void parse(TokenNode* tk)
{

}