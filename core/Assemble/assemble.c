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

static void _3block(TokenNode **tk, char *ins, char *addr)
{
    while ((*tk)->type != NEWLINE && (*tk)->type != ENDMARKER)
    {
        if (strcmp((*tk)->word, "HRK") == 0)
            strcpy(ins, "0000");
        else if (strcmp((*tk)->word, "TOP") == 0)
            strcpy(ins, "0001");
        else if (strcmp((*tk)->word, "CRP") == 0)
            strcpy(ins, "0010");
        else if (strcmp((*tk)->word, "CIK") == 0)
            strcpy(ins, "0011");
        else if (strcmp((*tk)->word, "BOL") == 0)
            strcpy(ins, "0100");
        else if (strcmp((*tk)->word, "VE") == 0)
            strcpy(ins, "0101");
        else if (strcmp((*tk)->word, "VEYA") == 0)
            strcpy(ins, "0110");
        else
            exit(EXIT_FAILURE);

        *tk = (*tk)->next;

        if (strcmp((*tk)->word, "AX") == 0)
            strcat(ins, "00");
        else if (strcmp((*tk)->word, "BX") == 0)
            strcat(ins, "01");
        else if (strcmp((*tk)->word, "CX") == 0)
            strcat(ins, "10");
        else if (strcmp((*tk)->word, "DX") == 0)
            strcat(ins, "11");

        *tk = (*tk)->next->next;

        if ((*tk)->type == LSQB)
        {
            *tk = (*tk)->next;
            strcat(ins, "10");
            uint8_t tmp = strtol((*tk)->word, NULL, 16);
            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next->next;
        }
        else if ((*tk)->type == REGISTER)
        {
            strcat(ins, "01");
            uint8_t tmp;
            if (strcmp((*tk)->word, "AX") == 0)
                tmp = 0x00;
            if (strcmp((*tk)->word, "BX") == 0)
                tmp = 0x01;
            if (strcmp((*tk)->word, "CX") == 0)
                tmp = 0x02;
            if (strcmp((*tk)->word, "DX") == 0)
                tmp = 0x03;

            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next;
        }

        else if ((*tk)->type == NUMBER || (*tk)->type == CONSTANT)
        {
            strcat(ins, "11");

            uint8_t tmp = strtol((*tk)->word, NULL, 10);
            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next;
        }
    }
}

static void deg(TokenNode **tk, char *ins, char *addr)
{
    strcpy(ins, "0111");
    strcat(ins, "00");

    *tk = (*tk)->next;

    while ((*tk)->type != NEWLINE && (*tk)->type != ENDMARKER)
    {
        if ((*tk)->type == LSQB)
        {
            strcat(ins, "10");
            *tk = (*tk)->next;
            uint8_t tmp = strtol((*tk)->word, NULL, 16);
            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next->next;
        }
        else if ((*tk)->type == REGISTER)
        {
            strcat(ins, "01");
            uint8_t tmp;
            if (strcmp((*tk)->word, "AX") == 0)
                tmp = 0x00;
            if (strcmp((*tk)->word, "BX") == 0)
                tmp = 0x01;
            if (strcmp((*tk)->word, "CX") == 0)
                tmp = 0x02;
            if (strcmp((*tk)->word, "DX") == 0)
                tmp = 0x03;

            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next;
        }
        else if ((*tk)->type == NUMBER || (*tk)->type == CONSTANT)
        {
            strcat(ins, "11");
            uint8_t tmp = strtol((*tk)->word, NULL, 10);
            char *bin_tmp = BIN(tmp);
            strcpy(addr, bin_tmp);
            free(bin_tmp);
            *tk = (*tk)->next;
        }
    }
}

static void branch(TokenNode **tk, char *ins, char *addr)
{

    if (strcmp((*tk)->word, "SS") == 0)
        strcpy(ins, "1000");
    else if (strcmp((*tk)->word, "SSD") == 0)
        strcpy(ins, "1001");
    else if (strcmp((*tk)->word, "SN") == 0)
        strcpy(ins, "1010");
    else if (strcmp((*tk)->word, "SP") == 0)
        strcpy(ins, "1011");

    strcat(ins, "0000");

    *tk = (*tk)->next;

    if ((*tk)->type == ADDRESS)
    {
        uint8_t tmp = strtol((*tk)->word, NULL, 16);
        char *bin_tmp = BIN(tmp);
        strcpy(addr, bin_tmp);
        free(bin_tmp);
        *tk = (*tk)->next;
    }
}

char *assemble(TokenNode *tk)
{
    TokenNode *ftk = tk;
    TokenNode *fftk = ftk;
    uint8_t lc = DATA_SEGMENT_BEGIN;
    uint8_t pc = CODE_SEGMENT_BEGIN;
    uint8_t data_offset = 0;
    char *output_hex = "a.obj";
    StreamObject *sobj = open_stream(output_hex, "w");
    struct adsym *address_symbol_table[255];
    uint8_t c = 0;

    // ##################### FIRST PASS #######################
    while (ftk)
    {
        struct adsym *a1 = malloc(sizeof(struct adsym));

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
                    printf("%s is not in the table\n", ftk->word);
                    exit(EXIT_FAILURE);
                }
                ftk = ftk->next->next;
            }
            // label definition ( ex: LOP: )
            else if (ftk->next->type == COLON && (ftk->next->next->type == NEWLINE || ftk->next->next->type == ENDMARKER))
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

    /*
    printf("========ADDRESS SYMBOL TABLE==========\n");
    for (int i = 0; i < c; i++)
        printf("%s\t%d\t%02X\n", address_symbol_table[i]->symbol, address_symbol_table[i]->data, h(address_symbol_table[i]->address));
    printf("======================================\n");
    */

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
        char ch[3];
        sprintf(ch, "%02X", h(address_symbol_table[i]->address));
        write_stream(sobj, ch);
        sprintf(ch, "%02X", h(address_symbol_table[i]->data));
        write_stream(sobj, ch);
        write_stream(sobj, "\n");
    }

    write_stream(sobj, "$\n");

    // CODE SEGMENT
    ftk = fftk;
    char *ins = malloc(9 * sizeof(char));
    char *addr = malloc(9 * sizeof(char));

    while (ftk)
    {
        if (ftk->type == LABEL)
        {
            if (ftk->next->type == COLON)
            {
                while (ftk->type != NEWLINE && ftk->type != ENDMARKER)
                    ftk = ftk->next;
            }
        }
        else
        {
            while (ftk->type != NEWLINE && ftk->type != ENDMARKER)
            {
                if (ftk->type == OPCODE)
                {
                    if (strcmp(ftk->word, "DEG") == 0)
                        deg(&ftk, ins, addr);
                    else if (strcmp(ftk->word, "SS") == 0 || strcmp(ftk->word, "SSD") == 0 || strcmp(ftk->word, "SN") == 0 || strcmp(ftk->word, "SP") == 0)
                        branch(&ftk, ins, addr);
                    else
                        _3block(&ftk, ins, addr);
                }

                if (ftk->type == NEWLINE || ftk->type == ENDMARKER)
                {
                    char ch[3];
                    uint8_t tmp1 = h(HEX(ins));
                    sprintf(ch, "%02X", tmp1);

                    uint8_t tmp2 = h(HEX(addr));

                    int i = ch[0];
                    switch (i)
                    {
                    case 'A':
                    case 'a':
                        i = 10;
                        break;
                    case 'B':
                    case 'b':
                        i = 11;
                        break;
                    default:
                        i -= '0';
                        break;
                    }
                    bool b = 0;

                    //printf("%s %s\t%02X %02X\n", ins, addr, tmp1, tmp2);
                    for (int j = 0; j < INSTRUCTION_OFFSET; j++)
                    {
                        uint8_t t = h(reserved_instruction_table[i][j]);
                        if (t == h(tmp1))
                            b = 1;
                    }

                    if (b == 0)
                    {
                        printf("error: not in the reserved table");
                        exit(EXIT_FAILURE);
                    }

                    char w1[3];
                    sprintf(w1, "%02X", tmp1);
                    write_stream(sobj, w1);

                    sprintf(w1, "%02X", tmp2);
                    write_stream(sobj, w1);

                    write_stream(sobj, "\n");
                    memset(ins, 0, sizeof(char*));
                    memset(addr, 0, sizeof(char*));
                }
            }
        }

        ftk = ftk->next;
    }

    free(ins);
    free(addr);
    close_stream(sobj);
    return output_hex;
}
