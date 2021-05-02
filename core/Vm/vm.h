#ifndef __CORE_VM_VM_H__
#define __CORE_VM_VM_H__

// OPCODES
#define HRK   0x00
#define TOP   0x01
#define CRP   0x02
#define CIK   0x03
#define BOL   0x04
#define VE    0x05
#define VEYA  0x06
#define DEG   0x07
#define SS    0x08
#define SSD   0x09
#define SN    0x0A
#define SP    0x0B

// REGISTERS
#define AX 0x0
#define BX 0x1
#define CX 0x2
#define DX 0x3

#define IR 0x4
#define AR 0x5
#define DR 0x6
#define TR 0x7
#define PC 0x8

// ADDRESSING MODE
#define MB 0x0
#define MR 0x1
#define MM 0x2
#define MI 0x3

// MEMORY PROPERTY
#define MEMORY_SIZE 0xF
#define INSTRUCTION_SIZE 0x8

// MEMORY OFFSETS
static unsigned int CODE_SEGMENT_BEGIN = 0x01;
static unsigned int CODE_SEGMENT_END = 0xc8;

static unsigned int DATA_SEGMENT_BEGIN = 0xc9;
static unsigned int DATA_SEGMENT_END = 0xff;


void load_program(char *);

#endif
