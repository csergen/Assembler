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

    char *output_lst = "output.lst";
    char *output_hex = "output.hex";

    StreamObject *streamObject_outputlst = open_stream(output_lst, "w");
    StreamObject *streamObject_outputhex = open_stream(output_hex, "w");

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
            }
            pc+=2;

            // one memory block has 8 bit therefore pc 2 increased.
        }

        ftk = ftk->next;
    }

    printf("====\n");
    for (int i = 0; i < c; i++)
    {
        printf("%s\t%d\t%02X\n", address_symbol_table[i]->symbol, address_symbol_table[i]->data, h(address_symbol_table[i]->address));
    }

    char *inst = malloc(sizeof(char) * 9);

    /*// first pass
    while (ftk)
    {
        if (ftk->type != NEWLINE)
        {
            if (ftk->type == LABEL)
            {
                if (ftk->next->type != COLON)
                {
                    perror("this is not label");
                    exit(EXIT_FAILURE);
                }

                ftk = ftk->next->next; // immediate, \n

                // variable definition
                if (ftk->type == NUMBER || ftk->type == CONSTANT)
                {
                }
                // label definition
                else if (ftk->type == NEWLINE)
                {
                }
            }
            else if (ftk->type == OPCODE)
            {
                if (strcmp(ftk->word, "HRK") == 0)
                    strcpy(inst, "0000");
                else if (strcmp(ftk->word, "TOP") == 0)
                    strcpy(inst, "0001");
                else if (strcmp(ftk->word, "CRP") == 0)
                    strcpy(inst, "0010");
                else if (strcmp(ftk->word, "CIK") == 0)
                    strcpy(inst, "0011");
                else if (strcmp(ftk->word, "BOL") == 0)
                    strcpy(inst, "0100");
                else if (strcmp(ftk->word, "VE") == 0)
                    strcpy(inst, "0101");
                else if (strcmp(ftk->word, "VEYA") == 0)
                    strcpy(inst, "0110");
                else if (strcmp(ftk->word, "DEG") == 0)
                    strcpy(inst, "0111");
                else if (strcmp(ftk->word, "SS") == 0)
                    strcpy(inst, "1000");
                else if (strcmp(ftk->word, "SSD") == 0)
                    strcpy(inst, "1001");
                else if (strcmp(ftk->word, "SN") == 0)
                    strcpy(inst, "1010");
                else if (strcmp(ftk->word, "SP") == 0)
                    strcpy(inst, "1011");
                else
                    exit(EXIT_FAILURE);

                // HRK, TOP, ... <reg>
                // DEG <im>, <mem>
                // SS <label>
                ftk = ftk->next;

                if (ftk->type == REGISTER && ftk->next->type == COMMA)
                {
                    if (strcmp(ftk->word, "AX") == 0)
                        strcat(inst, "00");
                    else if (strcmp(ftk->word, "BX") == 0)
                        strcat(inst, "01");
                    else if (strcmp(ftk->word, "CX") == 0)
                        strcat(inst, "10");
                    else if (strcmp(ftk->word, "DX") == 0)
                        strcat(inst, "11");

                    ftk = ftk->next->next;

                    if (ftk->type == NUMBER || ftk->type == CONSTANT)
                        strcat(inst, "11");
                    else if (ftk->type == REGISTER)
                        strcat(inst, "01");
                    else if (ftk->type == LSQB)
                        strcat(inst, "10");
                }
                else
                {
                    strcat(inst, "00"); // this is not value of AX register

                    if (ftk->type == NUMBER || ftk->type == CONSTANT)
                        strcat(inst, "11"); // this is immediate mode
                    else if (ftk->type == REGISTER)
                        strcat(inst, "01"); // this is register mode
                    else if (ftk->type == LSQB)
                        strcat(inst, "10"); // this is memory mode
                    else if (ftk->type == LABEL)
                        strcat(inst, "00"); // this is branch mode
                }

                char op[4] = {inst[0], inst[1], inst[2], inst[3]};
                int8_t i = HEX(op);
                int8_t hex_inst = HEX(inst);
                bool st = false;

                for (int c = 0; c < INSTRUCTION_OFFSET; c++)
                {
                    if (reserved_instruction_table[i][c] == hex_inst)
                    {
                        st = true;
                        break;
                    }
                }

                if (!st)
                {
                    perror("something went wrong");
                    exit(EXIT_FAILURE);
                }

                printf("%02X\n", strtol(inst, NULL, 2));
            }

            ftk = ftk->next;
        }
    }
    */

    close_stream(streamObject_outputhex);
    close_stream(streamObject_outputlst);
    return output_hex;
}