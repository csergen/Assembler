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

#define MEMDUMP                                   \
    for (int i = 0; i < CODE_SEGMENT_OFFSET; i++) \
    {                                             \
        printf("%0X\t", i);                       \
        for (int j = 0; j < 8; j++)               \
            printf("%d", MEMORY[i][j]);           \
        printf("\n");                             \
    }

#define REGDUMP                       \
    printf("RAX: 0x%0x(%d)\t", HEX(RAX), HEX(RAX)); \
    printf("RBX: 0x%0x(%d)\t", HEX(RBX), HEX(RBX)); \
    printf("RCX: 0x%0x(%d)\t", HEX(RCX), HEX(RCX)); \
    printf("RDX: 0x%0x(%d)\n", HEX(RDX), HEX(RDX)); \
    printf("RIR: 0x%0x(%d)\t", HEX(RIR), HEX(RIR)); \
    printf("RAR: 0x%0x(%d)\t", HEX(RAR), HEX(RAR)); \
    printf("RDR: 0x%0x(%d)\t", HEX(RDR), HEX(RDR)); \
    printf("RTR: 0x%0x(%d)\n", HEX(RTR), HEX(RTR)); \
    printf("RPC: 0x%0x(%d)\n", HEX(RPC), HEX(RPC));

// UTIL
static void
BIN(short int hex, short int buffer[8])
{
    char chr_hex[2];
    char ch1;
    char ch2;

    sprintf(chr_hex, "%0x", hex);

    if (strlen(chr_hex) == 1)
    {
        ch1 = '0';
        ch2 = chr_hex[0];
    }
    else
    {
        ch1 = chr_hex[0];
        ch2 = chr_hex[1];
    }

    switch (ch1)
    {
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

    switch (ch2)
    {
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
HEX(short int buffer[8])
{
    char other[8];

    for (int i = 0; i < 8; i++)
        other[i] = buffer[i] + '0';

    return strtol(other, NULL, 2);
}

static void
set(short int _dest[8],
    short int _src)
{
    short int _bin_src[8];
    BIN(_src, _bin_src);

    for (int i = 0; i < 8; i++)
        _dest[i] = _bin_src[i];
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
    short int temp = HEX(_other);
    temp++;

    BIN(temp, _other);
}

static void and (short int _src1[8], short int _src2[8])
{
    for (int i = 7; i >= 0; i--)
        _src1[i] = _src1[i] & _src2[i];
}

static void or (short int _src1[8], short int _src2[8])
{
    for (int i = 7; i >= 0; i--)
        _src1[i] = _src1[i] | _src2[i];
}

static void not(short int _src[8])
{
    for (int i = 7; i >= 0; i--)
        _src[i] = _src[i] ? 0 : 1;
}

//############# FETCH-DECODE-EXECUTE CYCLE ###############
static void
fetch_decode_execute()
{
    /* ======================== FETCH ========================== */
    // for OPCODE, REGISTER and MODE
    set(RAR, HEX(RPC));              // RAR <-- RPC
    set(RIR, HEX(MEMORY[HEX(RAR)])); // RIR <-- M[RAR]
    increase(RPC);                   // RPC <-- RPC + 1

    // for ADDRESS
    set(RAR, HEX(RPC));              // RAR <-- RPC
    set(RAR, HEX(MEMORY[HEX(RAR)])); // RAR <-- M[RAR]
    increase(RPC);                   // RPC <-- RPC

    /* ======================== DECODE ========================== */
    //OPCODE(4)
    unsigned char *opcode_char = (unsigned char *)malloc(5);
    unsigned char *register_char = (unsigned char *)malloc(3);
    unsigned char *mode_char = (unsigned char *)malloc(3);
    
 
    short int opcode_hex;
    short int register_hex;
    short int mode_hex;

    // OPCODE(4)
    opcode_char[0] = RIR[0] + '0';
    opcode_char[1] = RIR[1] + '0';
    opcode_char[2] = RIR[2] + '0';
    opcode_char[3] = RIR[3] + '0';
    opcode_char[4] = '\0';
    opcode_hex = strtol(opcode_char, NULL, 2);
    free(opcode_char);

    // DESTINATION REGISTER(2)
    register_char[0] = RIR[4] + '0';
    register_char[1] = RIR[5] + '0';
    register_char[2] = '\0';
    register_hex = strtol(register_char, NULL, 2);
    free(register_char);

    // MODE(2)
    mode_char[0] = RIR[6] + '0';
    mode_char[1] = RIR[7] + '0';
    mode_char[3] = '\0';
    mode_hex = strtol(mode_char, NULL, 2);
    free(mode_char);

       /* ======================== EXECUTE ========================== */
    short int src_hex;
    switch (mode_hex)
    {
    // register
    case MR:
        printf("\nselected mode: MR\n");
        src_hex = HEX(RAR);
        switch (src_hex)
        {
        case AX:
            printf("select AX, RDR <-- RAX: %0x\n", HEX(RAX));
            set(RDR, HEX(RAX));
            break;
        case BX:
            printf("select BX, RDR <-- RBX: %0x\n", HEX(RBX));
            set(RDR, HEX(RBX));
            break;
        case CX:
            printf("select CX, RDR <-- RCX: %0x\n", HEX(RCX));
            set(RDR, HEX(RCX));
            break;
        case DX:
            printf("select DX, RDR <-- RDX: %0x\n", HEX(RDX));
            set(RDR, HEX(RDX));
            break;
        }
        break;
    case MI:
        printf("\nselected mode: MI\n");
        set(RDR, HEX(RAR));
        break;
    case MM:
    case MB:
        set(RDR, HEX(MEMORY[HEX(RAR)]));
        break;
    }

    switch (opcode_hex)
    {
    case HRK:
        switch (register_hex)
        {
        case AX:
            set(RAX, HEX(RDR));
            break;
        case BX:
            set(RBX, HEX(RDR));
            break;
        case CX:
            set(RCX, HEX(RDR));
            break;
        case DX:
            set(RDX, HEX(RDR));
            break;
        }
        break;
    case TOP:
        switch (register_hex)
        {
        case AX:
            // RAX + RDR
            set(RAX, HEX(RAX)+HEX(RDR));
            break;
        case BX:
            // RBX + RDR
            set(RBX, HEX(RBX)+HEX(RDR));
            break;
        case CX:
            // RCX + RDR;
            set(RCX, HEX(RCX)+HEX(RDR));
            break;
        case DX:
            // RDX + RDR
            set(RDX, HEX(RDX)+HEX(RDR));
            break;
        }
        break;
    case CRP:
        switch (register_hex)
        {
        case AX:
            // RAX * RDR
            set(RAX, HEX(RAX)*HEX(RDR));
            break;
        case BX:
            // RBX * RDR
            set(RBX, HEX(RBX)*HEX(RDR));
            break;
        case CX:
            // RCX * RDR;
            set(RCX, HEX(RCX)*HEX(RDR));
            break;
        case DX:
            // RDX * RDR
            set(RDX, HEX(RDX)*HEX(RDR));
            break;
        }
        break;
    case CIK:
        switch (register_hex)
        {
        case AX:
            // RAX - RDR
            set(RAX, HEX(RAX)-HEX(RDR));
            break;
        case BX:
            // RBX - RDR
            set(RBX, HEX(RBX)-HEX(RDR));
            break;
        case CX:
            // RCX - RDR:
            set(RCX, HEX(RCX)-HEX(RDR));
            break;
        case DX:
            // RDX - RDR
            set(RDX, HEX(RDX)-HEX(RDR));
            break;
        }
        break;
    case BOL:
        switch (register_hex)
        {
        case AX:
            // RAX / RDR
            set(RAX, HEX(RAX)/HEX(RDR));
            break;
        case BX:
            // RBX / RDR
            set(RBX, HEX(RBX)/HEX(RDR));
            break;
        case CX:
            // RCX / RDR:
            set(RCX, HEX(RCX)/HEX(RDR));
            break;
        case DX:
            // RDX / RDR
            set(RDX, HEX(RDX)/HEX(RDR));
            break;
        }
        break;

    }

}
//#######################################################

static void
init()
{
    reset(RAX);
    reset(RBX);
    reset(RCX);
    reset(RDX);
    reset(RIR);
    reset(RDR);
    reset(RAR);
    reset(RTR);

    CODE_SEGMENT_OFFSET = HEX(RPC);
    DATA_SEGMENT_BEGIN = CODE_SEGMENT_OFFSET + 1;

    set(RPC, 0x1);
}

static void
run()
{
    while (HEX(RPC) <= CODE_SEGMENT_OFFSET)
    {
        printf("\e[1;1H\e[2J");
        REGDUMP
        getc(stdin);

        fetch_decode_execute();
    }
}

void load_program(char *src_file)
{
    StreamObject *streamObject = open_stream(src_file, "r");
    char buffer[3];
    short int hex;

    reset(RPC);
    increase(RPC);

    while (fgets(buffer, sizeof(buffer), streamObject->stream))
    {
        if (*buffer != '\n')
        {
            hex = strtol(buffer, NULL, 16);
            BIN(hex, MEMORY[HEX(RPC)]);
            increase(RPC);
        }
    }

    init();
    run();
}
