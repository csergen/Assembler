#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "node.h"

static TokenNode *s_token;

static void
next_token()
{
    if (s_token->next)
        s_token = s_token->next;
    else
    {
        s_token = NULL;
    }
}

static int
label_stmt()
{
    // DEC:          DEC   :
    //               token token->next
    if (s_token->type == STRING)
    {
        if (s_token->next->type == COLON)
        {
            next_token(); // :
            return 1;
        }
    }

    return 0;
}

static int
opcode_stmt()
{
    if (strcmp(s_token->word, "HRK") == 0 || strcmp(s_token->word, "TOP") == 0 || strcmp(s_token->word, "CIK") == 0 || strcmp(s_token->word, "CRP") == 0 || strcmp(s_token->word, "BOL") == 0 || strcmp(s_token->word, "DEG") == 0 || strcmp(s_token->word, "VE") == 0 || strcmp(s_token->word, "VEYA") == 0 || strcmp(s_token->word, "SS") == 0 || strcmp(s_token->word, "SSD") == 0 || strcmp(s_token->word, "SN") == 0 || strcmp(s_token->word, "SP") == 0)
        return 1;

    return 0;
}

static int
register_stmt()
{
    if (s_token->type == STRING)
    {
        if (strcmp(s_token->word, "AX") == 0 || strcmp(s_token->word, "BX") == 0 || strcmp(s_token->word, "CX") == 0 || strcmp(s_token->word, "DX") == 0)
        {
            next_token();
            return 1;
        }
    }

    return 0;
}

static int
memory_stmt()
{
    if (s_token->type == LSQB)
    {
        next_token();

        if (s_token->type == NUMBER)
        {
            next_token();

            if (s_token->type == RSQB)
            {
                next_token();
                return 1;
            }
        }
    }

    return 0;
}

static int
field_stmt(void)
{
    if (ISTERMINAL(s_token->type))
    {
        if (label_stmt())
        {
            next_token(); // \n
            if (s_token->type == NEWLINE)
                return 1;
        }

        if (s_token->type == STRING)
        {
            if (!opcode_stmt())
                return 0;
        }

        next_token(); // AX, BX, [ADDRESS]

        if (s_token->type == LSQB) // HRK [ADDRESS] ...
        {
            if (!memory_stmt())
                return 0;
        }
        else if (!register_stmt()) // HRK AX, BX, CX ....
            return 0;

        if (s_token->type == COMMA)
        {
            next_token();

            if (s_token->type == NUMBER)
                next_token();
            else if (s_token->type == LSQB)
            {
                if (!memory_stmt())
                    return 0;
            }
            else if (!register_stmt())
                return 0;
        }

        if (s_token->type == NEWLINE || s_token->type == ENDMARKER) // \n, \0
            return 1;
        else
            return 0;
    }

    return 0;
}

static void
program_stmt(void)
{
    while (s_token)
    {
        if (s_token->type == NEWLINE)
            next_token();
        else if (field_stmt())
            next_token();
        else
        {
            printf("error: ");
            printf("%s", s_token->word);
            exit(1);
        }

        if (s_token && ISNONTERMINAL(s_token->type))
            exit(1);
    }
}

void parse(TokenNode *tk)
{
    s_token = tk;
    program_stmt();
    /*
    while (s_token)
    {
        printf("%s\t%d\t%d\t%d\n", s_token->word, s_token->colstart, s_token->colend, s_token->type);
        next_token();
    }*/
}