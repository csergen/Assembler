#ifndef __INIT_VM_H__
#define __INIT_VM_H__

/*
# OPCODES
-----------------
hrk	0000	0
top	0001	1
crp	0010	2
cik	0011	3
bol	0100	4
ve	0101	5
veya	0110	6
deg	0111	7
ss	1000	8
ssd	1001	9
sn	1010	A
sp	1011	B
-----------------


# REGISTERS
-----------------
ax	00	0
bx	01	1
cx	10	2
dx	11	3
-----------------


# ADDRESSING MODE
-------------------------
branch		00	0
register	01	1
memory		10	2
immediate	11	3
-------------------------


# INSTRUCTION TABLE
---------------------------------------------------
=======================HRK=========================
hrk reg(ax), reg(?)		0000 00 01	01
hrk reg(bx), reg(?)		0000 01 01	05
hrk reg(cx), reg(?)		0000 10 01	09
hrk reg(dx), reg(?)		0000 11 01	0D

hrk reg(ax), memory		0000 00 10	02
hrk reg(bx), memory		0000 01 10	06
hrk reg(cx), memory		0000 10 10	0A
hrk reg(dx), memory		0000 11 10	0E

hrk reg(ax), immediate		0000 00 11	03
hrk reg(bx), immediate		0000 01	11	07
hrk reg(cx), immediate		0000 10 11	0B
hrk reg(dx), immediate		0000 11	11	0F

=======================TOP=========================
top reg(ax), reg(?)		0001 00 01	11
top reg(bx), reg(?)		0001 01 01	15
top reg(cx), reg(?)		0001 10 01	19
top reg(dx), reg(?)		0001 11 01	1D

top reg(ax), memory		0001 00 10	12
top reg(bx), memory		0001 01 10	16
top reg(cx), memory		0001 10 10	1A
top reg(dx), memory		0001 11 10	1E

top reg(ax), immediate		0001 00 11	13
top reg(bx), immediate		0001 01	11	17
top reg(cx), immediate		0001 10 11	1B
top reg(dx), immediate		0001 11	11	1F

=======================CRP=========================
crp reg(ax), reg(?)		0010 00 01	21
crp reg(bx), reg(?)		0010 01 01	25
crp reg(cx), reg(?)		0010 10 01	29
crp reg(dx), reg(?)		0010 11 01	2D

crp reg(ax), memory		0010 00 10	22
crp reg(bx), memory		0010 01 10	26
crp reg(cx), memory		0010 10 10	2A
crp reg(dx), memory		0010 11 10	2E

crp reg(ax), immediate		0010 00 11	23
crp reg(bx), immediate		0010 01	11	27
crp reg(cx), immediate		0010 10 11	2B
crp reg(dx), immediate		0010 11	11	2F

=======================CIK=========================
cik reg(ax), reg(?)		0011 00 01	31
cik reg(bx), reg(?)		0011 01 01	35
cik reg(cx), reg(?)		0011 10 01	39
cik reg(dx), reg(?)		0011 11 01	3D

cik reg(ax), memory		0011 00 10	32
cik reg(bx), memory		0011 01 10	36
cik reg(cx), memory		0011 10 10	3A
cik reg(dx), memory		0011 11 10	3E

cik reg(ax), immediate		0011 00 11	33
cik reg(bx), immediate		0011 01	11	37
cik reg(cx), immediate		0011 10 11	3B
cik reg(dx), immediate		0011 11	11	3F

=======================BOL=========================
bol reg(ax), reg(?)		0011 00 01	41
bol reg(bx), reg(?)		0011 01 01	45
bol reg(cx), reg(?)		0011 10 01	49
bol reg(dx), reg(?)		0011 11 01	4D

bol reg(ax), memory		0011 00 10	42
bol reg(bx), memory		0011 01 10	46
bol reg(cx), memory		0011 10 10	4A
bol reg(dx), memory		0011 11 10	4E

bol reg(ax), immediate		0011 00 11	43
bol reg(bx), immediate		0011 01	11	47
bol reg(cx), immediate		0011 10 11	4B
bol reg(dx), immediate		0011 11	11	4F

========================VE=========================
ve reg(ax), reg(?)		0101 00 01	51
ve reg(bx), reg(?)		0101 01 01	55
ve reg(cx), reg(?)		0101 10 01	59
ve reg(dx), reg(?)		0101 11 01	5D

ve reg(ax), memory		0101 00 10	52
ve reg(bx), memory		0101 01 10	56
ve reg(cx), memory		0101 10 10	5A
ve reg(dx), memory		0101 11 10	5E

ve reg(ax), immediate		0101 00 11	53
ve reg(bx), immediate		0101 01	11	57
ve reg(cx), immediate		0101 10 11	5B
ve reg(dx), immediate		0101 11	11	5F

=======================VEYA========================
veya reg(ax), reg(?)		0110 00 01	61
veya reg(bx), reg(?)		0110 01 01	65
veya reg(cx), reg(?)		0110 10 01	69
veya reg(dx), reg(?)		0110 11 01	6D

veya reg(ax), memory		0110 00 10	62
veya reg(bx), memory		0110 01 10	66
veya reg(cx), memory		0110 10 10	6A
veya reg(dx), memory		0110 11 10	6E

veya reg(ax), immediate		0110 00 11	63
veya reg(bx), immediate		0110 01	11	67
veya reg(cx), immediate		0110 10 11	6B
veya reg(dx), immediate		0110 11	11	6F

========================DEG========================
deg reg(?)		0111 00 01	71
deg memory		0111 00 10	72
deg immediate	0111 00 11	73

========================SS=========================
ss branch			1000 00 00	80

========================SSD========================
ssd branch			1001 00 00	90

========================SN=========================
sn branch			1010 00 00	A0

========================SP=========================
sp branch			1011 00 00	B0
---------------------------------------------------
*/

// OPCODES
#define HRK	0x00
#define TOP	0x01
#define CRP	0x02
#define CIK	0x03
#define BOL	0x04
#define VE	0x05
#define VEYA	0x06
#define DEG	0x07
#define SS	0x08
#define SSD	0x09
#define SN	0x0A
#define SP	0x0B

// REGISTERS
#define AX	0x0
#define BX	0x1
#define CX	0x2
#define DX	0x3

// ADDRESSING MODE
#define MB	0x0
#define MR	0x1
#define MM	0x2
#define MI	0x3

#define INSTRUCTION_OFFSET 12

static const short int 
reserved_instruction_table[INSTRUCTION_OFFSET][INSTRUCTION_OFFSET] = {
  {				// HRK
   0x01, 0x05, 0x09, 0x0D,
   0x02, 0x06, 0x0A, 0x0E,
   0x03, 0x07, 0x0B, 0x0F,
   },
  {				// TOP
   0x11, 0x15, 0x19, 0x1D,
   0x12, 0x16, 0x1A, 0x1E,
   0x13, 0x17, 0x1B, 0x1F,
   },
  {				// CRP
   0x21, 0x25, 0x29, 0x2D,
   0x22, 0x26, 0x2A, 0x2E,
   0x23, 0x27, 0x2B, 0x2F,
   },
  {				// CIK
   0x31, 0x35, 0x39, 0x3D,
   0x32, 0x36, 0x3A, 0x3E,
   0x33, 0x37, 0x3B, 0x3F,
   },
  {				// BOL
   0x41, 0x45, 0x49, 0x4D,
   0x42, 0x46, 0x4A, 0x4E,
   0x43, 0x47, 0x4B, 0x4F,
   },
  {				// VE
   0x51, 0x55, 0x59, 0x5D,
   0x52, 0x56, 0x5A, 0x5E,
   0x53, 0x57, 0x5B, 0x5F,
   },
  {				// VEYA
   0x61, 0x65, 0x69, 0x6D,
   0x62, 0x66, 0x6A, 0x6E,
   0x63, 0x67, 0x6B, 0x6F,
   },
  {				// DEG
   0x71, 0x72, 0x73, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   },
  {				// SS
   0x80, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   },
  {				// SSD
   0x90, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   },
  {				// SN
   0xA0, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   },
  {				// SP
   0xB0, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00,
   },
};


#endif
