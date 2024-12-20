#include "spu.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

static const canary_t SPU_canary = time(NULL) * sin(time(NULL));


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

    StackDtor (&(p->stk));
    StackDtor (&(p->stk_ip));

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



SPU_ERROR IsAliveSPUCanary (SPU* p)
{
    if (p == nullptr) return SPU_NULLPTR;

    int ERROR = 0;
    if (p->canary_left != SPU_canary) ERROR += 256;
    if (p->canary_right != SPU_canary) ERROR += 512;

    return (SPU_ERROR) ERROR;
}

SPU_ERROR ProgramCtor (int16_t** program, char* infile_name)
{
    FILE* infile = fopen (infile_name, "r");

    struct stat buff;
    stat(infile_name, &buff);
    size_t program_size = (size_t) buff.st_size;

    if ((*program = (int16_t*) calloc (program_size, sizeof(int16_t))) == nullptr)
        return PROGRAM_CALLOC_ERROR;
    for (size_t i = 0; fscanf (infile, "%hd", *program + i) == 1; i++);

    return SPU_NO_ERROR;
}

