#ifndef SPU_H
#define SPU_H

#include "Stack2/stack.h"
#include "Stack2/stack_protection.h"
#define SPU_DUMP(p, file) SpuDump (p, #p, __FILE__, __LINE__, __func__, file)

enum spu {
    cmd_HLT  = 0,
    cmd_add  = 1,
    cmd_sub  = 2,
    cmd_mul  = 3,
    cmd_div  = 4,
    cmd_in   = 5,
    cmd_out  = 6,
    cmd_push = 7,
    cmd_pop  = 8,
    cmd_jmp  = 9,
    cmd_jb   = 10, // <
    cmd_jbe  = 11, // ≤
    cmd_ja   = 12, // >
    cmd_jae  = 13, // ≥
    cmd_je   = 14, // ==
    cmd_jne  = 15, // ≠
    cmd_call = 16,
    cmd_ret  = 17,
    dir_org  = 18
};

enum reg {ax = 0, bx = 1, cx = 2, dx = 3};

enum SPU_ERROR
{
    SPU_NO_ERROR          = 0,
    SPU_NULLPTR           = 1,
    STACK_CTOR_ERROR      = 2,
    STACK_IP_CTOR_ERROR   = 4,
    STACK_IP_DTOR_ERROR   = 8,
    SPU_INFILE_NULLPTR    = 16,
    SPU_OUTFILE_NULLPTR   = 32,
    SPU_FOPEN_ERROR       = 64,
    STACKPUSH_ERROR       = 128,
    LEFT_CANARY_DIED      = 256,
    RIGHT_CANARY_DIED     = 512,
    TWO_TWO_CANARIES_DIED = 768,
    PROGRAM_CALLOC_ERROR  = 1024,
    SPU_NO_COMMAND_ERROR  = 2048,
    SPU_MARKER            = 4096,
    SPU_NO_MARKER_ERROR   = 8192

};

struct commands
{
    char str[9];
    spu cmd;
};

struct regist
{
    char str[3];
    reg value;
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
SPU_ERROR ProgramCtor (int16_t** program, char* infile_name);
SPU_ERROR IsAliveSPUCanary (SPU* p);

const commands cmds[] = {
    {"HLT" , cmd_HLT},
    {"add" , cmd_add},
    {"sub" , cmd_sub},
    {"mul" , cmd_mul},
    {"div" , cmd_div},
    {"in"  , cmd_in},
    {"out" , cmd_out},
    {"push", cmd_push},
    {"pop" , cmd_pop},
    {"jmp" , cmd_jmp},
    {"jb"  , cmd_jb},
    {"jbe" , cmd_ja},
    {"ja"  , cmd_ja},
    {"jae" , cmd_jae},
    {"je"  , cmd_je},
    {"jne" , cmd_jne},
    {"call", cmd_call},
    {"ret" , cmd_ret},
    {"org" , dir_org}
};
const regist registers[] = {
    {"ax", ax},
    {"bx", bx},
    {"cx", cx},
    {"dx", dx}
};
#endif
