#ifndef SPU_H
#define SPU_H

#define forever for(;;)
#include "Stack2/stack.hpp"
#include "Stack2/stack_protection.h"
#define SPU_DUMP(p) SpuDump (p, #p, __FILE__, __LINE__, __func__)

enum spu {
    HLT = -1,
    push = 1,
    add = 2,
    sub = 3,
    mul = 4,
    divi = 5,
    out = 6,
    in = 7,
    rpush = 17,
    pop = 24,
    jmp = 33,
    jb = 34, rjb = 50, // <
    jbe = 35, rjbe = 51, // ≤
    ja = 36, rja = 52, // >
    jba = 37, rjba = 53, // ≥
    je = 38, rje = 54, // ==
    jne = 39, rjne = 55, // ≠
};

enum reg {ax = 1, bx = 2, cx = 3, dx = 4};

struct SPU
{
    stack stk;
    elem_t rax;
    elem_t rbx;
    elem_t rcx;
    elem_t rdx;
};

void SpuDump (SPU p, char* name, char* file, int numline, const char* func);
int SpuCtor (SPU* p);
int SpuDtor (SPU* p);
int ProgramCtor (int** program, char* infile_name);

#endif
