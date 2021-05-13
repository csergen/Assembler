#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "util.h"
#include "vm.h"
#include "color.h"

// (8 bit, MSB sign bit, max = 127, min = -128)

// REGISTERS
static int8_t RAX = 0x00;
static int8_t RBX = 0x00;
static int8_t RCX = 0x00;
static int8_t RDX = 0x00;
static uint8_t RIR = 0x00;
static uint8_t RAR = 0x00;
static uint8_t RDR = 0x00;
static int8_t RTR = 0x00;
static uint8_t RPC = 0x01;

// MEMORY
static char MEMORY[MEMORY_SIZE][INSTRUCTION_SIZE + 1];

// ########################### GENERAL UTILS ###############################

// hex to bin
char *BIN(const int8_t hex)
{
    char *binary = (char *)malloc(9);
    char *hex_buffer = malloc(3);

    sprintf(hex_buffer, "%02X", hex & 0xFF);

    char ch1 = hex_buffer[1];
    char ch2 = hex_buffer[0];

    char *bin1 = malloc(sizeof(char) * 4);
    char *bin2 = malloc(sizeof(char) * 4);

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

// bin to hex
int8_t HEX(const char *binary)
{
    return h(strtol(binary, NULL, 2));
}

// ########################### OPCODES #############################
static int8_t not(int8_t d1)
{
    return h(~d1);
}

static inline int8_t sum(int8_t s1, int8_t s2)
{
    return h(s1 + s2);
}

static inline int8_t mul(int8_t s1, int8_t s2)
{
    return h(s1 * s2);
}

static inline int8_t sub(int8_t s1, int8_t s2)
{
    return h(s1 - s2);
}

static inline int8_t divi(int8_t s1, int8_t s2)
{
    if (s2 == 0)
    {
        printf(BRED "\nError: Division by zero\n"RESET);
        exit(EXIT_FAILURE);
    }

    return h(s1 / s2);
}

// ##################### MEMORY | REGISTER DUMP ######################
static void MEMDUMP()
{
    int i, j = 1;

	printf(RED "\n--------------------------------------------Memory--------\n" RESET);
    printf(WHITE "00\t%s\n" RESET, MEMORY[0]);

    for (i = 1; i <= CODE_SEGMENT_END; i += 2)
    {
        printf("%02x\t", i);
        if (RPC == i)
#ifdef _WIN32
            printf("[ %s %s ]\n", MEMORY[j], MEMORY[j + 1]);
#else
            printf(GRN "%s %s\n" RESET, MEMORY[j], MEMORY[j + 1]);
#endif
        else
            printf("%s %s\n", MEMORY[j], MEMORY[j + 1]);

        j += 2;
    }
}

static void REGDUMP()
{
    char *bin_rax = BIN(RAX);
    char *bin_rbx = BIN(RBX);
    char *bin_rcx = BIN(RCX);
    char *bin_rdx = BIN(RDX);
    char *bin_rir = BIN(RIR);
    char *bin_rar = BIN(RAR);
    char *bin_rdr = BIN(RDR);
    char *bin_rtr = BIN(RTR);
    char *bin_rpc = BIN(RPC);

	printf(BLUE "\n------------------------------------------Registers-------\n" RESET);
    printf("AX\t%s\t%02X\t%d\n" RESET, bin_rax, h(RAX), RAX);
    printf(WHITE "BX\t%s\t%02X\t%d\n" RESET, bin_rbx, h(RBX), RBX);
    printf("CX\t%s\t%02X\t%d\n", bin_rcx, h(RCX), RCX);
    printf(WHITE "DX\t%s\t%02X\t%d\n\n" RESET, bin_rdx, h(RDX), RDX);
    printf("IR\t%s\t%02X\t%d\n", bin_rir, h(RIR), RIR);
    printf(WHITE "AR\t%s\t%02X\t%d\n" RESET, bin_rar, h(RAR), RAR);
    printf("DR\t%s\t%02X\t%d\n", bin_rdr, h(RDR), RDR);
    printf(WHITE "TR\t%s\t%02X\t%d\n\n" RESET, bin_rtr, h(RTR), RTR);
    printf("PC\t%s\t%02X\t%d\n", bin_rpc, h(RPC), RPC);

    free(bin_rax);
    free(bin_rbx);
    free(bin_rcx);
    free(bin_rdx);
    free(bin_rir);
    free(bin_rar);
    free(bin_rdr);
    free(bin_rtr);
    free(bin_rpc);
}

// #################### FETCH & DECODE & EXECUTE ###################
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

    int8_t opcode;
    int8_t reg;
    int8_t mode;

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
    case MB:
        RDR = RAR;
        break;
    case MM:
        RDR = HEX(MEMORY[RAR]);
        break;
    }

    /*********** EXECUTE ***********/
    char *bin_rdr;
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
        switch (reg)
        {
        case AX:
            RAX = sum(RAX, RDR);
            RTR = RAX;
            break;
        case BX:
            RBX = sum(RBX, RDR);
            RTR = RBX;
            break;
        case CX:
            RCX = sum(RCX, RDR);
            RTR = RCX;
            break;
        case DX:
            RDX = sum(RDX, RDR);
            RTR = RDX;
            break;
        }
        break;
    case CRP:
        switch (reg)
        {
        case AX:
            RAX = mul(RAX, RDR);
            RTR = RAX;
            break;
        case BX:
            RBX = mul(RBX, RDR);
            RTR = RBX;
            break;
        case CX:
            RCX = mul(RCX, RDR);
            RTR = RCX;
            break;
        case DX:
            RDX = mul(RDX, RDR);
            RTR = RDX;
            break;
        }
        break;
    case CIK:
        switch (reg)
        {
        case AX:
            RAX = sub(RAX, RDR);
            RTR = RAX;
            break;
        case BX:
            RBX = sub(RBX, RDR);
            RTR = RBX;
            break;
        case CX:
            RCX = sub(RCX, RDR);
            RTR = RCX;
            break;
        case DX:
            RDX = sub(RDX, RDR);
            RTR = RDX;
            break;
        }
        break;
    case BOL:
        switch (reg)
        {
        case AX:
            RAX = divi(RAX, RDR);
            RTR = RAX;
            break;
        case BX:
            RBX = divi(RBX, RDR);
            RTR = RBX;
            break;
        case CX:
            RCX = divi(RCX, RDR);
            RTR = RCX;
            break;
        case DX:
            RDX = divi(RDX, RDR);
            RTR = RDX;
            break;
        }
        break;
    case VE:
        switch (reg)
        {
        case AX:
            RAX = RAX & RDR;
            break;
        case BX:
            RBX = RBX & RDR;
            break;
        case CX:
            RCX = RCX & RDR;
            break;
        case DX:
            RDX = RDX & RDR;
            break;
        }
        break;
    case VEYA:
        switch (reg)
        {
        case AX:
            RAX = RAX | RDR;
            break;
        case BX:
            RBX = RBX | RDR;
            break;
        case CX:
            RCX = RCX | RDR;
            break;
        case DX:
            RDX = RDX | RDR;
            break;
        }
        break;
    case DEG:
        switch (mode)
        {
        case MR:
            switch (RAR)
            {
            case AX:
                RAX = not(RDR);
                break;
            case BX:
                RBX = not(RDR);
                break;
            case CX:
                RCX = not(RDR);
                break;
            case DX:
                RDX = not(RDR);
                break;
            }
            break;
        case MM:
            bin_rdr = BIN(not(RDR));
            strcpy(MEMORY[RAR], bin_rdr);
            free(bin_rdr);
            break;
        case MI:
            RDR = not(RDR);
            break;
        default:
            exit(EXIT_FAILURE);
        }
        break;
    case SS:
        if (RTR == 0)
            RPC = RDR;
        break;
    case SSD:
        if (RTR != 0)
            RPC = RDR;
        break;
    case SN:
        if (RTR < 0)
            RPC = RDR;
        break;
    case SP:
        if (RTR > 0)
            RPC = RDR;
        break;
    }
}

// ######################### MANAGER ###############################
static void init()
{
    RAX = h(0x00);
    RBX = h(0x00);
    RCX = h(0x00);
    RDX = h(0x00);
    RIR = h(0x00);
    RAR = h(0x00);
    RDR = h(0x00);
    RTR = h(0x00);

    CODE_SEGMENT_END = RPC - 1;

    RPC = h(0x01);
}

static void run()
{
    while (RPC < CODE_SEGMENT_END)
    {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
        MEMDUMP();
        ftdcex();
        REGDUMP();
        getc(stdin);
    }
}

void load_program(char *executable)
{
    StreamObject *streamObject = open_stream(executable, "r");
    if (streamObject == NULL)
    {
        printf(BRED"\nError: executable file is null.\n"RESET);
        exit(EXIT_FAILURE);
    }

    char buffer[3];
    int8_t hex;
    char *temp;

    int data_end_flag = 0;

    while (fgets(buffer, sizeof(buffer), streamObject->stream))
    {
        if (*buffer == '$')
            data_end_flag = 1;

        if (*buffer != '\n' && *buffer != '$')
        {
            if (data_end_flag == 0)
            {
                uint8_t address = strtol(buffer, NULL, 16);
                fgets(buffer, sizeof(buffer), streamObject->stream);
                hex = h(strtol(buffer, NULL, 16));
                temp = BIN(hex);
                strcpy(MEMORY[address], temp);
            }
            else
            {
                hex = h(strtol(buffer, NULL, 16));
                temp = BIN(hex);
                strcpy(MEMORY[RPC++], temp);
            }
            free(temp);
        }
    }

    init();
    run();
}
