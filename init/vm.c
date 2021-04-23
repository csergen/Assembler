/*

    load_program(out.hex);
    run_program();

    
    hrk: set(R, DR)
    top: set(R, R+DR)
    crp: set(R, R*DR)
    cik: set(R, R-DR)
    bol: set(R, R/DR)
    ve: set(R, R&DR)
    veya: set(R, R|DR)
    deg: set(R, !DR)
    ss: 
        if (ZF == 0)
            set(PC, DR)
    ssd:
        if (ZF != 0)
            set(PC, DR)
    sn:
        set(PC, DR) if negative

    execute:
        match S:
            case HRK:
                hrk();
            case TOP:
                top();
            case CRP:
                crp();
            case CIK:
                cik();
            case BOL:
                bol();
            case VE:
                ve();
            case VEYA:
                veya();
            case DEG:
                deg();
            case SS:
                ss();
            case SSD:
                ssd();
            case SN:
                sn();
            case SP:
                sp();
    decode:
        S = IR[0-3] // opcode
        R = IR[4-5] // register
        M = IR[6-7] // mode

        match M:
            case MM:
                set(AR, PC)
                set(AR, M[AR])
                set(DR, M[AR])
            
            case MI:
                set(AR, PC)
                set(DR, M[AR])
            
            case MR:
                set(AR, PC)
                set(DR, M[AR])

            case MB:
                set(AR, PC)
                set(AR, M[AR])
                set(DR, M[AR])

    fetch:
        set(AR, PC); // dest, src
        set(IR, M[AR])
        inc(PC);

    load_program:
        loop: M[PC++] = getline(src) is not EOF goto loop

    run_program:
        clear(ALL_REGISTER)

        for (;;)
            fetch()
            decode()
            execute()
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "vm.h"

#define MEMORY_SIZE 0xF
#define INSTRUCTION_SIZE 0x8

#define INSTRUCTION_MEMORY_OFFSET 220

static short int RAX;
static short int RBX;
static short int RCX;
static short int RDX;
static short int RIR;
static short int RAR;
static short int RDR;
static short int RTR;
static short int RPC = 1;

static bool ZF = 0;
static bool CF = 1;

static short int MEMORY[MEMORY_SIZE];

#define MEMORY_DUMP              \
    for (int i = 0; i < 10; i++) \
        printf("%0X\n", MEMORY[i]);
#define REGISTER_DUMP printf("AX: (0x%0X)\tBX: (0x%0X)\nCX: (0x%0X)\tDX: (0x%0X)\nIR: (0x%0X)\tAR: (0x%0X)\nDR: (0x%0X)\tTR: (0x%0X)\nPC: (0x%0X)\tZF: (0x%0x)\tCF: (0x%0x)\n", \
    RAX, RBX, RCX, RDX, RIR, RAR, RDR, RTR, RPC, ZF, CF);

static void hrk(short int reg, short int mode)
{
}

static void top(short int reg, short int mode)
{
}

static void crp(short int reg, short int mode)
{
}

static void cik(short int reg, short int mode)
{
}

static void bol(short int reg, short int mode)
{
}

static void ve(short int reg, short int mode)
{
}

static void veya(short int reg, short int mode)
{
}

static void deg(short int reg, short int mode)
{
}

static void ss()
{
}

static void ssd()
{
}

static void sn()
{
}

static void sp()
{
}

static void
fetch()
{
    RAR = RPC;
    RIR = MEMORY[RAR];
    RPC++;
}

static short int*
decode()
{
    short int* d = malloc(sizeof(short int));

    short int opcode;
    short int reg;
    short int mode;

    unsigned char buffer[2];
    sprintf(buffer, "%0X", RIR);

    if (buffer) {
        int i = 0;

        if (strlen(buffer) == 1) {
            opcode = 0;
        } else {
            switch (buffer[i]) {
            case 'A':
                opcode = 10;
                break;
            case 'B':
                opcode = 11;
                break;
            default:
                opcode = buffer[i] - '0';
                break;
            }
            i++;
        }

        switch (buffer[i]) {
        case '0':
            reg = 0;
            mode = 0;
            break;
        case '1':
            reg = 0;
            mode = 1;
            break;
        case '2':
            reg = 0;
            mode = 2;
            break;
        case '3':
            reg = 0;
            mode = 3;
            break;
        case '4':
            reg = 1;
            mode = 0;
            break;
        case '5':
            reg = 1;
            mode = 1;
            break;
        case '6':
            reg = 1;
            mode = 2;
            break;
        case '7':
            reg = 1;
            mode = 3;
            break;
        case '8':
            reg = 2;
            mode = 0;
            break;
        case '9':
            reg = 2;
            mode = 1;
            break;
        case 'A':
            reg = 2;
            mode = 2;
            break;
        case 'B':
            reg = 2;
            mode = 3;
            break;
        case 'C':
            reg = 3;
            mode = 0;
            break;
        case 'D':
            reg = 3;
            mode = 1;
            break;
        case 'E':
            reg = 3;
            mode = 2;
            break;
        case 'F':
            reg = 3;
            mode = 3;
            break;
        }
    }

    switch (mode) {
    case MM:
        RAR = RPC;
        RAR = MEMORY[RAR];
        RDR = MEMORY[RAR];
        break;
    case MI:
        RAR = RPC;
        RDR = MEMORY[RAR];
        break;
    case MR:
        RAR = RPC;
        RDR = MEMORY[RAR];
        break;
    case MB:
        RAR = RPC;
        RAR = MEMORY[RAR];
        RDR = MEMORY[RAR];
        break;
    }

    d[0] = opcode;
    d[1] = reg;
    d[2] = mode;

    RPC++;
    return d;
}

static void
execute(short int opcode, short int reg, short int mode)
{

    switch (opcode) {
    case 0:
        hrk(reg, mode);
        break;
    case 1:
        top(reg, mode);
        break;
    case 2:
        crp(reg, mode);
        break;
    case 3:
        cik(reg, mode);
        break;
    case 4:
        bol(reg, mode);
        break;
    case 5:
        ve(reg, mode);
        break;
    case 6:
        veya(reg, mode);
        break;
    case 7:
        deg(reg, mode);
        break;
    case 8:
        ss();
        break;
    case 9:
        ssd();
        break;
    case 10:
        sn();
        break;
    case 11:
        sp();
        break;
    }
}

static void
run()
{
    RAX = 0;
    RBX = 0;
    RCX = 0;
    RDX = 0;
    RIR = 0;
    RAR = 0;
    RDR = 0;
    RTR = 0;
    RPC = 1;

    short int* d;

    while (RPC < INSTRUCTION_MEMORY_OFFSET) {
        fetch();
        d = decode();
        execute(d[0], d[1], d[2]);
        printf("\e[1;1H\e[2J");
        REGISTER_DUMP
        getc(stdin);
    }
}

void load_program(char* src_file)
{
    StreamObject* streamObject = open_stream(src_file, "r");
    char* buffer = read_stream(streamObject);

    char hex[2];
    int c = 0;

    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] != '\n') {
            hex[c++] = buffer[i];
        } else {
            if (RPC < INSTRUCTION_MEMORY_OFFSET)
                MEMORY[RPC++] = (short int)strtol(hex, NULL, 16);
            c = 0;
        }
    }

    run();
}
