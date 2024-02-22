#include "spu.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>

static const canary_t SPU_canary = time(NULL) * sin(time(NULL));

namespace spaceSPU
{
    const char* commands[] = {"HLT", "add", "sub", "mul", "div", "in", "out", "push", "pop", "jmp", "jb", "jbe", "ja", "jae", "je", "jne", "call", "ret", "org"};
    const spu cmds [] = {cmd_HLT, cmd_add, cmd_sub, cmd_mul, cmd_div, cmd_in, cmd_out, cmd_push, cmd_pop, cmd_jmp, cmd_jb, cmd_jbe, cmd_ja, cmd_jae, cmd_je, cmd_jne, cmd_call, cmd_ret, org};
    const char* registers[] = {"ax", "bx", "cx", "dx"};
}

SPU_ERROR SpuCtor (SPU* p)
{
    if (p == nullptr) return SPU_NULLPTR;

    p->rax = 0;
    p->rbx = 0;
    p->rcx = 0;
    p->rdx = 0;

    p->ip = 0;

    p->canary_right = SPU_canary;
    p->canary_left  = SPU_canary;

    if (StackCtor (&(p->stk)) != STACK_NO_ERROR) return STACK_CTOR_ERROR;
    if (StackCtor (&(p->stk_ip)) != STACK_NO_ERROR) return STACK_IP_CTOR_ERROR;


    return SPU_NO_ERROR;
}
SPU_ERROR SpuDtor (SPU* p)
{
    if (p == nullptr) return SPU_NULLPTR;

    p->rax = 0;
    p->rbx = 0;
    p->rcx = 0;
    p->rdx = 0;

    p->ip = 0;

    if (StackDtor (&(p->stk)) != STACK_NO_ERROR) return STACK_DTOR_ERROR;
    if (StackDtor (&(p->stk_ip)) != STACK_NO_ERROR) return STACK_IP_DTOR_ERROR;

    return SPU_NO_ERROR;
}

void SpuDump (const SPU p, char* name, char* file, int numline, const char* func, FILE* outfile)
{
    SPU_ERROR ERROR = SPU_NO_ERROR;
    if ((ERROR = IsAliveSPUCanary ((SPU*) &p)) != SPU_NO_ERROR) return;

    fprintf (outfile, "\n\n>>> SPU_DUMP() has been started\nSPU \"%s\" from file \"%s\", func \"%s\" on %d line\n    "
    "%s\n{\n>>> rax = %d\n>>> rbx = %d\n>>> rcx = %d\n>>> rdx = %d\n\n>>> STACK\n    %s.stk.capacity = %lu\n"
    "    %s.stk.size = %lu\n   {\n", name, file, func, numline, name, p.rax, p.rbx, p.rcx, p.rdx, name,
    p.stk.capacity, name, p.stk.size);

    for (size_t i = 0; i < p.stk.capacity; i++)
    {
        if (i == p.stk.size)
        {
            fprintf (outfile, ">>>    %d    <<<\n", p.stk.data[i]);
        } else
        {
            fprintf (outfile, "       %d\n", p.stk.data[i]);
        }
    }
    fprintf (outfile, ">>> STACK_IP\n    %s.stk_ip.capacity = %lu\n    %s.stk_ip.size = %lu\n   {\n",
    name, p.stk_ip.capacity, name, p.stk_ip.size);

    for (size_t i = 0; i < p.stk_ip.capacity; i++)
    {
        if (i == p.stk_ip.size)
        {
            fprintf (outfile, ">>>    %d    <<<\n", p.stk_ip.data[i]);
        } else
        {
            fprintf (outfile, "       %d\n", p.stk_ip.data[i]);
        }
    }

    fprintf (outfile, "   }\n}\n>>> SPU_DUMP() has been finished\n\n\n");
}

// SPU_ERROR ProgramCtor (int** program, char* infile_name)
// {
//     FILE* infile = fopen (infile_name, "r");
//
//     struct stat buff;
//     stat (infile_name, &buff);
//     size_t program_size = buff.st_size;
//
//     if (infile == nullptr) return SPU_FOPEN_ERROR;
//     stack s = {};
//     if (StackCtor (&s) != STACK_NO_ERROR) return STACK_CTOR_ERROR;
//     for (int i = 0; i <= program_size; i++)
//     {
//         elem_t x = 0;
//         fscanf (infile, "%d", &x);
//         if (StackPush (&s, &x) != STACK_NO_ERROR) return STACKPUSH_ERROR;
//     }
//
//     if (fclose (infile) != 0) return SPU_FCLOSE_ERROR;
//
//     *program = s.data;
//
//     return SPU_NO_ERROR;
// }

SPU_ERROR IsAliveSPUCanary (SPU* p)
{
    if (p == nullptr) return SPU_NULLPTR;

    int ERROR = 0;
    if (p->canary_left != SPU_canary) ERROR += 256;
    if (p->canary_right != SPU_canary) ERROR += 512;

    return (SPU_ERROR) ERROR;
}

SPU_ERROR ProgramCtor (int** program, char* infile_name)
{
    FILE* infile = fopen (infile_name, "r");

    struct stat buff;
    stat(infile_name, &buff);
    size_t program_size = (size_t) buff.st_size;

    if ((*program = (int*) calloc (program_size, sizeof(int))) == nullptr)
        return PROGRAM_CALLOC_ERROR;
    for (size_t i = 0; fscanf (infile, "%d", *program + i) == 1; i++);

    return SPU_NO_ERROR;
}
