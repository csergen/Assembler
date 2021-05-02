#include <stdio.h>
#include <stdint.h>

#include "util.h"

#include "tokenizer.h"
#include "vm.h"

#include "assemble.h"

char *assemble(TokenNode *tk)
{
  TokenNode *ftk = tk;
  TokenNode *stk = tk;

  uint8_t lc = DATA_SEGMENT_BEGIN;
  uint8_t pc = CODE_SEGMENT_BEGIN;

  char *output_file = "output.hex";
  StreamObject *streamObject = open_stream(output_file, "w");

  char *inst = malloc(sizeof(char) * 9);

  // first pass
  while (ftk)
  {
    if (ftk->type != NEWLINE)
    {
      if (ftk->type == LABEL)
      {
        if (ftk->next->type != COLON) {
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
        int8_t i = strtol(op, NULL, 2);
        int8_t hex_inst = strtol(inst, NULL, 2);
        bool st = false;

        for (int c = 0; c < INSTRUCTION_OFFSET; c++)
        {
          if (reserved_instruction_table[i][c] == hex_inst) {
            st = true;
            break;
          }
        }

        if (!st) {
          perror("something went wrong");
          exit(EXIT_FAILURE);
        }

        printf("%02X\n", strtol(inst, NULL, 2));

      }
    }
  }

  close_stream(streamObject);
  return output_file;
}