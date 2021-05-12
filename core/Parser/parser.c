#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "node.h"
#include "color.h"

static TokenNode *s_token;
static char *src;

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
  if (s_token->type == LABEL)
  {
    if (s_token->next->type == COLON)
    {
      if (s_token->next->next->type == NEWLINE || s_token->next->next->type == ENDMARKER)
      {
        next_token(); // :
        next_token(); // \n , \0
        return 1;
      }
    }
  }

  return 0;
}

static int
variable_stmt()
{
  if (s_token->type == LABEL)
  {
    if (s_token->next->type == COLON)
    {
      if (s_token->next->next->type == NUMBER || s_token->next->next->type == CONSTANT)
      {
        next_token(); // :
        next_token(); // number
        next_token(); // \n
        return 1;
      }
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

    if (s_token->type == LABEL)
    {

      if (s_token->next->type == RSQB) {
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
      return 1;
    else if (variable_stmt())
      return 1;

    if (s_token->type != OPCODE)
      return 0;

    if (strcmp(s_token->word, "DEG") == 0)
    {
      next_token();
      if (s_token->type == REGISTER)
      {
      }
      else if (s_token->type == LSQB)
      {
        if (!memory_stmt())
          return 0;
      }
      else if (s_token->type == NUMBER || s_token->type == CONSTANT)
      {
      }
      else
        return 0;
    }
    else if (strcmp(s_token->word, "SS") == 0 || strcmp(s_token->word, "SSD") == 0 || strcmp(s_token->word, "SN") == 0 || strcmp(s_token->word, "SP") == 0)
    {
      next_token();
      if (s_token->type != LABEL)
        return 0;
    }
    else
    {
      next_token();
      if (s_token->type != REGISTER)
        return 0;
      next_token();
    }

    if (s_token->type == COMMA)
    {
      next_token();

      if (s_token->type == NUMBER || s_token->type == CONSTANT)
      {
      }
      else if (s_token->type == LSQB)
      {
        if (memory_stmt() == 0)
          return 0;
      }
      else if (s_token->type != REGISTER)
        return 0;
    }

    next_token();

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
    else if (s_token->type == ENDMARKER)
      return;
    else if (field_stmt())
    {
    }
    else
    {
      perror(RED "Error " RESET);

      int row = 0;
      for (int i = 0; i < strlen(src); i++)
      {
        if (src[i] == '\n')
          row++;

        if (row == s_token->lineno)
          printf(BRED "%c" RESET, src[i]);
        else
          printf(BGRY "%c" RESET, src[i]);
      }
      exit(EXIT_FAILURE);
    }

    if (s_token && ISNONTERMINAL(s_token->type))
      exit(EXIT_FAILURE);
  }
}

void parse(TokenNode *tk, char *sr)
{
  s_token = tk;
  src = sr;
  program_stmt();
  printf(GRN "%s" RESET, sr);
}