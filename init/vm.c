#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "vm.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[36m"
#define RESET "\x1B[0m"

// MEMORY PROPERTY
#define MEMORY_SIZE 0xF
#define INSTRUCTION_SIZE 0x8

// MEMORY OFFSETS
static unsigned int CODE_SEGMENT_BEGIN = 0x01;
static unsigned int CODE_SEGMENT_END;

static unsigned int DATA_SEGMENT_BEGIN;
static unsigned int DATA_SEGMENT_END = 0xff;

// REGISTERS
static short int RAX = 0x00;
static short int RBX = 0x00;
static short int RCX = 0x00;
static short int RDX = 0x00;
static short int RIR = 0x00;
static short int RAR = 0x00;
static short int RDR = 0x00;
static short int RTR = 0x00;
static short int RPC = 0x01;

// MEMORY
static char MEMORY[MEMORY_SIZE][INSTRUCTION_SIZE+1];


// ##################### BIN to HEX | HEX TO BIN #####################
static char *BIN(const short int hex)
{
    char *binary = (char*)malloc(9);
    char *hex_buffer = malloc(3);

    sprintf(hex_buffer, "%0X", hex);

    int hex_len = strlen(hex_buffer);

    if (hex_len == 1)
    {
        char temp = hex_buffer[0];
        hex_buffer[1] = temp;
        hex_buffer[0] = '0';
        hex_len = strlen(hex_buffer);
    }

    char ch1 = hex_buffer[1];
    char ch2 = hex_buffer[0];

    char *bin1 = malloc(sizeof(char)*4);
    char *bin2 = malloc(sizeof(char)*4);

    switch (ch1)
    {
    case '0':
        bin1 = "0000";
        break;
    case '1':
        bin1 = "0001";
        break;
    case '2':
        bin1 = "0010";
        break;
    case '3':
        bin1 = "0011";
        break;
    case '4':
        bin1 = "0100";
        break;
    case '5':
        bin1 = "0101";
        break;
    case '6':
        bin1 = "0110";
        break;
    case '7':
        bin1 = "0111";
        break;
    case '8':
        bin1 = "1000";
        break;
    case '9':
        bin1 = "1001";
        break;
    case 'a':
    case 'A':
        bin1 = "1010";
        break;
    case 'b':
    case 'B':
        bin1 = "1011";
        break;
    case 'c':
    case 'C':
        bin1 = "1100";
        break;
    case 'd':
    case 'D':
        bin1 = "1101";
        break;
    case 'e':
    case 'E':
        bin1 = "1110";
        break;
    case 'f':
    case 'F':
        bin1 = "1111";
        break;
    }

    switch (ch2)
    {
    case '0':
        bin2 = "0000";
        break;
    case '1':
        bin2 = "0001";
        break;
    case '2':
        bin2 = "0010";
        break;
    case '3':
        bin2 = "0011";
        break;
    case '4':
        bin2 = "0100";
        break;
    case '5':
        bin2 = "0101";
        break;
    case '6':
        bin2 = "0110";
        break;
    case '7':
        bin2 = "0111";
        break;
    case '8':
        bin2 = "1000";
        break;
    case '9':
        bin2 = "1001";
        break;
    case 'a':
    case 'A':
        bin2 = "1010";
        break;
    case 'b':
    case 'B':
        bin2 = "1011";
        break;
    case 'c':
    case 'C':
        bin2 = "1100";
        break;
    case 'd':
    case 'D':
        bin2 = "1101";
        break;
    case 'e':
    case 'E':
        bin2 = "1110";
        break;
    case 'f':
    case 'F':
        bin2 = "1111";
        break;
    }

    strcpy(binary, bin2);
    strcat(binary, bin1);
    free(hex_buffer);
    binary[8] = '\0';

    return binary;
}

static short int HEX(const char *binary)
{
    return strtol(binary, NULL, 2);
}


// ######################### MEMORY DUMP ############################
static void MEMDUMP()
{
    printf(YELLOW"\n──────────Memory────────────────────────────────────────\n"RESET);

    int i, j = 1;
    printf("0\t%s\n", MEMORY[0]);

    for (i = 1; i <= CODE_SEGMENT_END+5; i+=2)
    {
        printf("%0X\t", i);
        if (RPC == i)
            printf(GRN"%s %s\n"RESET, MEMORY[j], MEMORY[j+1]);
        else
            printf("%s %s\n", MEMORY[j], MEMORY[j+1]);
        j+=2;
    }
}

// ########################### REGISTER DUMP ############################
static void REGDUMP() {
    printf(BLUE"\n──────Registers───────────────────────────────────────────\n"RESET);
    printf("AX\t%s\t%0x\t%d\n", BIN(RAX), RAX, RAX); 
    printf("BX\t%s\t%0x\t%d\n", BIN(RBX), RBX, RBX); 
    printf("CX\t%s\t%0x\t%d\n", BIN(RCX), RCX, RCX); 
    printf("DX\t%s\t%0x\t%d\n\n", BIN(RDX), RDX, RDX); 
    printf("IR\t%s\t%0x\t%d\n", BIN(RIR), RIR, RIR); 
    printf("AR\t%s\t%0x\t%d\n", BIN(RAR), RAR, RAR); 
    printf("DR\t%s\t%0x\t%d\n", BIN(RDR), RDR, RDR); 
    printf("TR\t%s\t%0x\t%d\n\n", BIN(RTR), RTR, RTR); 
    printf("PC\t%s\t%0x\t%d\n", BIN(RPC), RPC, RPC);
}


// ####################### FETCH & DECODE & EXECUTE ######################
static void ftdcex()
{   
    /*********** FETCH ***********/
    // opcode, register, mode
    RAR = RPC;
    RIR = HEX(MEMORY[RAR]);
    RPC++;

    // address
    RAR = RPC;
    RAR = HEX(MEMORY[RAR]);
    RPC++;

    /*********** DECODE ***********/
    char *ir = BIN(RIR);

    char *opcode_b = malloc(5);
    char *reg_b = malloc(3);
    char *mode_b = malloc(3);

    short int opcode;
    short int reg;
    short int mode;

    opcode_b[0] = ir[0];
    opcode_b[1] = ir[1];
    opcode_b[2] = ir[2];
    opcode_b[3] = ir[3];
    opcode_b[4] = '\0';
    opcode = HEX(opcode_b); //strtol(opcode, NULL, 2);

    reg_b[0] = ir[4];
    reg_b[1] = ir[5];
    reg_b[2] = '\0';
    reg = HEX(reg_b); //strtol(reg, NULL, 2);

    mode_b[0] = ir[6];
    mode_b[1] = ir[7];
    mode_b[2] = '\0';
    mode = HEX(mode_b); //strtol(mode, NULL, 2);

    free(ir);
    free(opcode_b);
    free(reg_b);
    free(mode_b);

    switch (mode)
    {
    case MR:
        switch (RAR)
        {
        case AX:
            RDR = RAX;
            break;
        case BX:
            RDR = RBX;
            break;
        case CX:
            RDR = RCX;
            break;
        case DX:
            RDR = RDX;
            break;
        }
        break;
    case MI:
        RDR = RAR;
        break;
    case MM:
    case MB:
        break;
    }

    /*********** EXECUTE ***********/
    switch (opcode)
    {
    case HRK:
        switch (reg)
        {
        case AX:
            RAX = RDR;
            break;
        case BX:
            RBX = RDR;
            break;
        case CX:
            RCX = RDR;
            break;
        case DX:
            RDX = RDR;
            break;
        }
        break;
    case TOP:
        break;
    case CRP:
        break;
    case CIK:
        break;
    case BOL:
        break;
    }

}

static void init()
{
    RAX = 0x00;
    RBX = 0x00;
    RCX = 0x00;
    RDX = 0x00;
    RIR = 0x00;
    RAR = 0x00;
    RDR = 0x00;
    RTR = 0x00;

    CODE_SEGMENT_END = RPC-1;
    DATA_SEGMENT_BEGIN = RPC;

    RPC = 0x01;
}

static void run()
{
    while (RPC < CODE_SEGMENT_END)
    {
        printf("\e[1;1H\e[2J");
        MEMDUMP();
        ftdcex();
        REGDUMP();
        getc(stdin);
    }
}

void load_program(char *src_file)
{
    StreamObject *streamObject = open_stream(src_file, "r");
    char buffer[3];
    short int hex;
    char *temp;

    while (fgets(buffer, sizeof(buffer), streamObject->stream))
    {
        if (*buffer != '\n')
        {
            hex = strtol(buffer, NULL, 16);
            temp = BIN(hex);
            strcpy(MEMORY[RPC++], temp);
            free(temp);
        }
    }

    init();
    run();

}
