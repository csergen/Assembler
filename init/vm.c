#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vm.h"
#include "util.h"

#define MEMORY_SIZE 0xF
#define INSTRUCTION_SIZE 0x8


static struct _register
{   
    short int D[INSTRUCTION_SIZE];
} RAX, RBX, RCX, RDX, RIR, RAR, RDR, RTR, RPC;

static bool ZF = 0;
static bool CF = 1;

static short int MEMORY[MEMORY_SIZE];


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