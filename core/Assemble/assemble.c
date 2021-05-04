#include <stdio.h>
#include <stdint.h>

#include "util.h"

#include "tokenizer.h"
#include "vm.h"

#include "assemble.h"

#define ADDRESS 0x20

static bool in_table(struct adsym *table[255], uint8_t size, char *ch)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(table[i]->symbol, ch) == 0)
            return true;
    }

    return false;
}

static struct adsym *get_from_table(struct adsym *table[255], uint8_t size, char *ch)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(table[i]->symbol, ch) == 0)
            return table[i];
    }

    return NULL;
}

static void add_table(struct adsym *table[255], uint8_t *offset, struct adsym *buffer)
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
    TokenNode *fftk = ftk;

    uint8_t lc = DATA_SEGMENT_BEGIN;
    uint8_t pc = CODE_SEGMENT_BEGIN;

    uint8_t data_offset = 0;

    char *output_hex = "output.hex";

    StreamObject *sobj = open_stream(output_hex, "w");

    struct adsym *address_symbol_table[255];

    uint8_t c = 0;

    // ##################### FIRST PASS #######################
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
                    data_offset++;

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

            while (ftk->next && ftk->type != NEWLINE)
            {
                //printf("%s\t%d\n", ftk->word, pc);
                ftk = ftk->next;

                if (ftk->type == LABEL)
                {
                    struct adsym *tmp = get_from_table(address_symbol_table, c, ftk->word);

                    if (tmp)
                    {
                        memset(ftk->word, 0, sizeof(char) * 2);
                        sprintf(ftk->word, "%02X", h(tmp->address));
                        ftk->type = ADDRESS;
                    }
                }

                // one memory block has 8 bit therefore pc 2 increased.
                if (ftk->type == NEWLINE)
                    pc += 2;
            }
        }

        ftk = ftk->next;
    }

    printf("========ADDRESS SYMBOL TABLE==========\n");
    for (int i = 0; i < c; i++)
    {
        printf("%s\t%d\t%02X\n",
               address_symbol_table[i]->symbol, address_symbol_table[i]->data, h(address_symbol_table[i]->address));
    }
    printf("======================================\n");

    // ################## SECOND PASS ##################
    ftk = fftk;
    while (ftk)
    {
        if (ftk->type == LABEL)
        {
            if (ftk->next->type == NEWLINE)
            {
                struct adsym *tmp = get_from_table(address_symbol_table, c, ftk->word);

                if (tmp)
                {
                    memset(ftk->word, 0, sizeof(char) * 2);
                    sprintf(ftk->word, "%02X", h(tmp->address));
                    ftk->type = ADDRESS;
                }
                else
                {
                    printf("%s is not in the table.", ftk->word);
                    exit(EXIT_FAILURE);
                }
            }
        }
        ftk = ftk->next;
    }

    // DATA SEGMENT
    for (uint8_t i = 0; i < data_offset; i++)
    {
        char ch[2];
        sprintf(ch, "%02X", h(address_symbol_table[i]->address));
        write_stream(sobj, ch);
        sprintf(ch, "%02X", h(address_symbol_table[i]->data));
        write_stream(sobj, ch);
        write_stream(sobj, "\n");
    }

    write_stream(sobj, "$\n");

    // CODE SEGMENT
    ftk = fftk;
    while (ftk)
    {
        if (ftk->type == LABEL)
        {
            if (ftk->next->type == COLON)
            {
                while (ftk->type != NEWLINE)
                    ftk = ftk->next;
            }
        }
        else
        {
            char *inst = malloc(8);
            char *dt = malloc(9);

            if (ftk->type != NEWLINE && ftk->type != ENDMARKER)
            {
                if (ftk->type == OPCODE)
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
                    ftk = ftk->next;
                }


                if (ftk->type == REGISTER)
                {
                    if (strcmp(ftk->word, "AX") == 0)
                        strcat(inst, "00");
                    if (strcmp(ftk->word, "BX") == 0)
                        strcat(inst, "01");
                    if (strcmp(ftk->word, "CX") == 0)
                        strcat(inst, "10");
                    if (strcmp(ftk->word, "DX") == 0)
                        strcat(inst, "11");


                    ftk = ftk->next->next;

                    if (ftk->type == LSQB)
                    {
                        strcat(inst, "10");
                        ftk = ftk->next;
                        uint8_t tmp = strtol(ftk->word, NULL, 16);
                        char *bin_tmp = BIN(tmp);
                        strcpy(dt, bin_tmp);
                        free(bin_tmp);
                        ftk = ftk->next->next;
                    }
                    else if (ftk->type == NUMBER || ftk->type == CONSTANT)
                    {
                        strcat(inst, "11");
                        uint8_t tmp = strtol(ftk->word, NULL, 16);
                        char *bin_tmp = BIN(tmp);
                        strcpy(dt, bin_tmp);
                        free(bin_tmp);
                        ftk = ftk->next;
                    }
                    else if (ftk->type == REGISTER)
                    {
                        strcat(inst, "01");
                        uint8_t tmp;
                        if (strcmp(ftk->word, "AX") == 0)
                            tmp = 0x00;
                        if (strcmp(ftk->word, "BX") == 0)
                            tmp = 0x01;
                        if (strcmp(ftk->word, "CX") == 0)
                            tmp = 0x02;
                        if (strcmp(ftk->word, "DX") == 0)
                            tmp = 0x03;

                        char *bin_tmp = BIN(tmp);
                        strcpy(dt, bin_tmp);
                        free(bin_tmp);
                        ftk = ftk->next;
                    }
                }
                else if (ftk->type == ADDRESS)
                {
                    strcat(inst, "00");
                    strcat(inst, "00");
                    uint8_t tmp = strtol(ftk->word, NULL, 16);
                    char *bin_tmp = BIN(tmp);
                    strcpy(dt, bin_tmp);
                    free(bin_tmp);
                }

                uint8_t tmp1 = h(HEX(inst));
                uint8_t tmp2 = h(HEX(dt));

                char w1[2];
                sprintf(w1, "%02X", tmp1);
                write_stream(sobj, w1);

                sprintf(w1, "%02X", tmp2);
                write_stream(sobj, w1);

                write_stream(sobj, "\n");
                free(inst);
                free(dt);
            }
        }

        ftk = ftk->next;
    }
    close_stream(sobj);
    return output_hex;
}
