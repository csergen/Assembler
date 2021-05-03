#include <stdio.h>
#include <stdint.h>

#include "util.h"

#include "tokenizer.h"
#include "vm.h"

#include "assemble.h"


static bool in_table(struct adsym *table[255], uint8_t size, char *ch)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(table[i]->symbol, ch) == 0)
            return true;
    }

    return false;
}

static void add_table(struct adsym* table[255], uint8_t* offset, struct adsym* buffer)
{
    if (*offset < 256)
    {
        memcpy(&table[*offset], &buffer, sizeof(buffer));
        (*offset)++;
    }
}

char *assemble(TokenNode *tk)
{
    TokenNode *ftk = tk;
    TokenNode *stk = tk;

    uint8_t lc = DATA_SEGMENT_BEGIN;
    uint8_t pc = CODE_SEGMENT_BEGIN;

    char *output_hex = "output.hex";

    //StreamObject *streamObject_outputhex = open_stream(output_hex, "w");

    struct adsym *address_symbol_table[255];

    uint8_t c = 0;

    // FIRST PASS for VARIABLE, VARIABLE CALL, LABEL, LABEL CALL DEFINITION
    while (ftk)
    {
        struct adsym *a1 = malloc(sizeof(a1));

        if (ftk->type == LABEL)
        {
            // variable definition ( v1: 10 )
            if (ftk->next->type == COLON && ftk->next->next->type != NEWLINE)
            {
                // variable is not in the table
                if (!in_table(address_symbol_table, c, ftk->word))
                {
                    // v1
                    a1->symbol = ftk->word;

                    // v1, :, N
                    ftk = ftk->next->next;

                    // 1, 2, .. n
                    a1->data = atoi(ftk->word);

                    // M[DATA_SEGMENT_BEGIN]
                    a1->address = lc;
                    lc++;

                    add_table(address_symbol_table, &c, a1);
                } 
                // variable repetition
                else 
                {
                    printf("Error: %s is repeated\n", ftk->word);
                    exit(EXIT_FAILURE);
                }
            }
            // pointer call definition ( ex: HRK AX, [v1] )
            else if (ftk->next->type == RSQB)
            {
                // search 'ftk->word' in address-symbol table

                //printf("[%s]\n", ftk->word);
                if (!in_table(address_symbol_table, c, ftk->word))
                {
                    printf("%s is not in the table\n");
                    exit(EXIT_FAILURE);
                }
                ftk = ftk->next->next;
            }
            // label definition ( ex: LOP: )
            else if (ftk->next->type == COLON && ftk->next->next->type == NEWLINE)
            {
                // search 'LABEL' in address-symbol table
                if (!in_table(address_symbol_table, c, ftk->word))
                {
                    a1->symbol = ftk->word;
                    a1->data = 0;
                    a1->address = pc;

                    add_table(address_symbol_table, &c, a1);
                }
                ftk = ftk->next;
            }
            // label call definition ( ex: SS LOP )
            else if (ftk->next->type == NEWLINE || ftk->next->type == ENDMARKER)
            {
            }
        }
        else
        {
            free(a1);

            // encode Instruction
            while (ftk->next && ftk->type != NEWLINE)
            {
                printf("%s\t%d\n", ftk->word, pc);
                ftk = ftk->next;

                // one memory block has 8 bit therefore pc 2 increased.
                if (ftk->type == NEWLINE)
                    pc+=2;


            }
        }

        ftk = ftk->next;
    }

    printf("====\n");
    for (int i = 0; i < c; i++)
    {
        printf("%s\t%d\t%02X\n", address_symbol_table[i]->symbol, address_symbol_table[i]->data, h(address_symbol_table[i]->address));
    }

    char *inst = malloc(sizeof(char) * 9);

    return output_hex;
}