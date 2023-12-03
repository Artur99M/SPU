#include "spu.hpp"
#include <stdio.h>


int SpuCtor (SPU* p)
{
    if (p == nullptr) return 1;

    p->rax = 0;
    p->rbx = 0;
    p->rcx = 0;
    p->rdx = 0;

    p->ip = 0;

    if (StackCtor (&(p->stk)) != 0) return 2;

    return 0;
}
int SpuDtor (SPU* p)
{
    if (p == nullptr) return 1;

    p->rax = 0;
    p->rbx = 0;
    p->rcx = 0;
    p->rdx = 0;

    p->ip = 0;

    if (StackDtor (&(p->stk)) != 0) return 2;

    return 0;
}

void SpuDump (SPU p, char* name, char* file, int numline, const char* func)
{
    printf ("\n\n>>> SPU_DUMP() has been started\nSPU \"%s\" from file \"%s\", func \"%s\" on %d line\n    "
    "%s\n{\n>>> rax = %d\n>>> rbx = %d\n>>> rcx = %d\n>>> rdx = %d\n\n>>> STACK\n    %s.stk.capacity = %lu\n"
    "    %s.stk.size = %lu\n   {\n", name, file, func, numline, name, p.rax, p.rbx, p.rcx, p.rdx, name,
    p.stk.capacity, name, p.stk.size);

    for (size_t i = 0; i < p.stk.capacity; i++)
    {
        if (i == p.stk.size)
        {
            printf (">>>    %d    <<<\n", p.stk.data[i]);
        } else
        {
            printf ("       %d\n", p.stk.data[i]);
        }
    }

    printf ("   }\n}\n>>> SPU_DUMP() has been finished\n\n\n");
}

int ProgramCtor (int** program, char* infile_name)
{
    FILE* infile = fopen (infile_name, "r");

    stack s = {};
    StackCtor (&s);
    for (int i = 0; s.data[s.size - 1] != -1; i++)
    {
        elem_t x = 0;
        fscanf (infile, "%d", &x);
        StackPush (&s, x);
    }

    fclose (infile);

    *program = s.data;

    return 0;
}
