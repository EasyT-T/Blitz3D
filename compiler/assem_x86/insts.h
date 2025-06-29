#ifndef INSTS_H
#define INSTS_H

//operand addressing modes
enum
{
    REG = 0x0001, REG8 = 0x0002, REG16 = 0x0004, REG32 = 0x0008,
    IMM = 0x0010, IMM8 = 0x0020, IMM16 = 0x0040, IMM32 = 0x0080,
    MEM = 0x0100, MEM8 = 0x0200, MEM16 = 0x0400, MEM32 = 0x0800,
    R_M = 0x1000, R_M8 = 0x2000, R_M16 = 0x4000, R_M32 = 0x8000,

    AL = 0x10000, AX = 0x20000, EAX = 0x40000,
    CL = 0x80000, CX = 0x100000, ECX = 0x200000,
    ST0 = 0x400000, FPUREG = 0x800000,

    NONE = 0x80000000
};

//flags
enum
{
    O16 = 0x001, O32 = 0x0002, OW_OD = 0x0004,
    PLUSREG = 0x0008, PLUSCC = 0x0010,
    _0 = 0x0020, _1 = 0x0040, _2 = 0x0080, _3 = 0x0100,
    _4 = 0x0200, _5 = 0x0400, _6 = 0x0800, _7 = 0x1000, _R = 0x2000,
    IB = 0x4000, IW = 0x8000, ID = 0x10000, RW_RD = 0x20000
};

//an instruction
struct Inst
{
    const char* name; //0, then same as last.
    int lmode, rmode, flags; //left mode,right mode,flags
    const char* bytes; //the bytes.
};

extern Inst insts[];

#endif
