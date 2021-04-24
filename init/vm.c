#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "vm.h"

#define MEMORY_SIZE 0xF
#define INSTRUCTION_SIZE 0x8

static unsigned int CODE_SEGMENT_OFFSET = 220;
static unsigned int DATA_SEGMENT_BEGIN = 221;

static short int RAX[INSTRUCTION_SIZE];
static short int RBX[INSTRUCTION_SIZE];
static short int RCX[INSTRUCTION_SIZE];
static short int RDX[INSTRUCTION_SIZE];
static short int RIR[INSTRUCTION_SIZE];
static short int RAR[INSTRUCTION_SIZE];
static short int RDR[INSTRUCTION_SIZE];
static short int RTR[INSTRUCTION_SIZE];
static short int RPC[INSTRUCTION_SIZE];

static short int MEMORY[MEMORY_SIZE][INSTRUCTION_SIZE];

#define MEMDUMP                                     \
    for (int i = 0; i < CODE_SEGMENT_OFFSET; i++) { \
        printf("%0X\t", i);                         \
        for (int j = 0; j < 8; j++)                 \
            printf("%d", MEMORY[i][j]);             \
        printf("\n");                               \
    }

static void
hex2bin(short int hex, short int buffer[8])
{
    char chr_hex[2];
    char ch1;
    char ch2;
    
    sprintf(chr_hex, "%0x", hex);

    if (strlen(chr_hex) == 1) {
        ch1 = '0';
        ch2 = chr_hex[0];
    } else {
        ch1 = chr_hex[0];
        ch2 = chr_hex[1];
    }

    switch (ch1) {
    case '0':
        buffer[0] = 0;
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 0;
        break;
    case '1':
        buffer[0] = 0;
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 1;
        break;
    case '2':
        buffer[0] = 0;
        buffer[1] = 0;
        buffer[2] = 1;
        buffer[3] = 0;
        break;
    case '3':
        buffer[0] = 0;
        buffer[1] = 0;
        buffer[2] = 1;
        buffer[3] = 1;
        break;
    case '4':
        buffer[0] = 0;
        buffer[1] = 1;
        buffer[2] = 0;
        buffer[3] = 0;
        break;
    case '5':
        buffer[0] = 0;
        buffer[1] = 1;
        buffer[2] = 0;
        buffer[3] = 1;
        break;
    case '6':
        buffer[0] = 0;
        buffer[1] = 1;
        buffer[2] = 1;
        buffer[3] = 0;
        break;
    case '7':
        buffer[0] = 0;
        buffer[1] = 1;
        buffer[2] = 1;
        buffer[3] = 1;
        break;
    case '8':
        buffer[0] = 1;
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 0;
        break;
    case '9':
        buffer[0] = 1;
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 1;
        break;
    case 'A':
    case 'a':
        buffer[0] = 1;
        buffer[1] = 0;
        buffer[2] = 1;
        buffer[3] = 0;
        break;
    case 'B':
    case 'b':
        buffer[0] = 1;
        buffer[1] = 0;
        buffer[2] = 1;
        buffer[3] = 1;
        break;
    case 'C':
    case 'c':
        buffer[0] = 1;
        buffer[1] = 1;
        buffer[2] = 0;
        buffer[3] = 0;
        break;
    case 'D':
    case 'd':
        buffer[0] = 1;
        buffer[1] = 1;
        buffer[2] = 0;
        buffer[3] = 1;
        break;
    case 'E':
    case 'e':
        buffer[0] = 1;
        buffer[1] = 1;
        buffer[2] = 1;
        buffer[3] = 0;
        break;
    case 'F':
    case 'f':
        buffer[0] = 1;
        buffer[1] = 1;
        buffer[2] = 1;
        buffer[3] = 1;
        break;
    }

    switch (ch2) {
    case '0':
        buffer[4] = 0;
        buffer[5] = 0;
        buffer[6] = 0;
        buffer[7] = 0;
        break;
    case '1':
        buffer[4] = 0;
        buffer[5] = 0;
        buffer[6] = 0;
        buffer[7] = 1;
        break;
    case '2':
        buffer[4] = 0;
        buffer[5] = 0;
        buffer[6] = 1;
        buffer[7] = 0;
        break;
    case '3':
        buffer[4] = 0;
        buffer[5] = 0;
        buffer[6] = 1;
        buffer[7] = 1;
        break;
    case '4':
        buffer[4] = 0;
        buffer[5] = 1;
        buffer[6] = 0;
        buffer[7] = 0;
        break;
    case '5':
        buffer[4] = 0;
        buffer[5] = 1;
        buffer[6] = 0;
        buffer[7] = 1;
        break;
    case '6':
        buffer[4] = 0;
        buffer[5] = 1;
        buffer[6] = 1;
        buffer[7] = 0;
        break;
    case '7':
        buffer[4] = 0;
        buffer[5] = 1;
        buffer[6] = 1;
        buffer[7] = 1;
        break;
    case '8':
        buffer[4] = 1;
        buffer[5] = 0;
        buffer[6] = 0;
        buffer[7] = 0;
        break;
    case '9':
        buffer[4] = 1;
        buffer[5] = 0;
        buffer[6] = 0;
        buffer[7] = 1;
        break;
    case 'A':
    case 'a':
        buffer[4] = 1;
        buffer[5] = 0;
        buffer[6] = 1;
        buffer[7] = 0;
        break;
    case 'B':
    case 'b':
        buffer[4] = 1;
        buffer[5] = 0;
        buffer[6] = 1;
        buffer[7] = 1;
        break;
    case 'C':
    case 'c':
        buffer[4] = 1;
        buffer[5] = 1;
        buffer[6] = 0;
        buffer[7] = 0;
        break;
    case 'D':
    case 'd':
        buffer[4] = 1;
        buffer[5] = 1;
        buffer[6] = 0;
        buffer[7] = 1;
        break;
    case 'E':
    case 'e':
        buffer[4] = 1;
        buffer[5] = 1;
        buffer[6] = 1;
        buffer[7] = 0;
        break;
    case 'F':
    case 'f':
        buffer[4] = 1;
        buffer[5] = 1;
        buffer[6] = 1;
        buffer[7] = 1;
        break;
    }
}

static short int
bin2hex(short int buffer[8])
{
    char other[8];

    for (int i = 0; i < 8; i++)
        other[i] = buffer[i] + '0';

    return strtol(other, NULL, 2);
}

static void
set(short int _dest[8],
    short int _src[8])
{
    for (int i = 0; i < 8; i++)
        _dest[i] = _src[i];
}

static void
reset(short int _other[8])
{
    for (int i = 0; i < 8; i++)
        _other[i] = 0;
}

static void
increase(short int _other[8])
{
    short int temp = bin2hex(_other);
    temp++;
    hex2bin(temp, _other);
}

static void and(short int _src1[8], short int _src2[8])
{
    for (int i = 7; i >= 0; i--)
        _src1[i] = _src1[i] & _src2[i];
}

static void or(short int _src1[8], short int _src2[8])
{
    for (int i = 7; i >= 0; i--)
        _src1[i] = _src1[i] | _src2[i];
}

static void not(short int _src[8])
{
    for (int i = 7; i >= 0; i--)
        _src[i] = _src[i] ? 0 : 1;
}

void load_program(char* src_file)
{
    StreamObject* streamObject = open_stream(src_file, "r");
    char buffer[3];

    int i = 1;
    short int hex;
    reset(RPC);
    increase(RPC);
    while (fgets(buffer, sizeof(buffer), streamObject->stream)) {
        if (*buffer != '\n') {
            hex = strtol(buffer, NULL, 16);
            hex2bin(hex, MEMORY[bin2hex(RPC)]);
            increase(RPC);
        }
    }

    CODE_SEGMENT_OFFSET = bin2hex(RPC);
    DATA_SEGMENT_BEGIN = CODE_SEGMENT_OFFSET + 1;
    
    MEMDUMP
    //run();
}
