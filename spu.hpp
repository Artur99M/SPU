#ifndef SPU_H
#define SPU_H

#define forever for(;;)
#include "Stack2/stack.hpp"
#include "Stack2/stack_protection.h"
#define SPU_DUMP(p) SpuDump (p, #p, __FILE__, __LINE__, __func__)

enum spu {
    cmd_HLT = -1,
    cmd_push = 1,
    cmd_add = 2,
    cmd_sub = 3,
    cmd_mul = 4,
    cmd_div = 5,
    cmd_out = 6,
    cmd_in = 7,
    cmd_rpush = 17,
    cmd_pop = 24,
    cmd_jmp = 33,
    cmd_jb = 34, cmd_rjb = 50, // <
    cmd_jbe = 35, cmd_rjbe = 51, // ≤
    cmd_ja = 36, cmd_rja = 52, // >
    cmd_jba = 37, cmd_rjba = 53, // ≥
    cmd_je = 38, cmd_rje = 54, // ==
    cmd_jne = 39, cmd_rjne = 55, // ≠
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
