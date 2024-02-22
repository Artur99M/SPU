#ifndef SPU_H
#define SPU_H

#define forever for(;;)
#include "Stack2/stack.h"
#include "Stack2/stack_protection.h"
#define SPU_DUMP(p, file) SpuDump (p, #p, __FILE__, __LINE__, __func__, file)

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
    cmd_jb = 34, // <
    cmd_jbe = 35, // ≤
    cmd_ja = 36, // >
    cmd_jae = 37, // ≥
    cmd_je = 38, // ==
    cmd_jne = 39, // ≠
    cmd_call = 40,
    cmd_ret = 41,
    org = 100
};

enum reg {ax = 0, bx = 1, cx = 2, dx = 3};

enum SPU_ERROR
{
    SPU_NO_ERROR = 0,
    SPU_NULLPTR = 1,
    STACK_CTOR_ERROR = 2,
    STACK_IP_CTOR_ERROR = 4,
    STACK_DTOR_ERROR = 8,
    STACK_IP_DTOR_ERROR = 16,
    SPU_FCLOSE_ERROR = 32,
    SPU_FOPEN_ERROR = 64,
    STACKPUSH_ERROR = 128,
    LEFT_CANARY_DIED = 256,
    RIGHT_CANARY_DIED = 512,
    TWO_TWO_CANARIES_DIED = 768,
    PROGRAM_CALLOC_ERROR = 1024

};

struct SPU
{
    canary_t canary_left;
    stack stk;
    elem_t rax;
    elem_t rbx;
    elem_t rcx;
    elem_t rdx;
    size_t ip;
    stack stk_ip;
    canary_t canary_right;
};

void SpuDump (SPU p, char* name, char* file, int numline, const char* func, FILE* outfile);
SPU_ERROR SpuCtor (SPU* p);
SPU_ERROR SpuDtor (SPU* p);
SPU_ERROR ProgramCtor (int** program, char* infile_name);
SPU_ERROR IsAliveSPUCanary (SPU* p);

namespace spaceSPU
{
    extern const char* commands[];
    extern const spu cmds [];
    extern const char* registers[];
}
#endif
