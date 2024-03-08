#include "spu.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define command_mask(x) 0x001F&x
#define memory_mask(x) (x >> 13)&0x0007
#define RAM 4
#define REGISTER 2
#define NUMBER 1

int main()
{
    FILE* logfile = fopen ("compiler.log", "w");
    SPU p;
    if (SpuCtor (&p) != SPU_NO_ERROR)
    {
        fputs ("SPU CTOR ERROR\n", logfile);
        abort();
    }

    SPU_ERROR ERROR_SPU = SPU_NO_ERROR;
    STACK_ERROR ERROR_STACK = STACK_NO_ERROR;

    int16_t* program = nullptr;
    if ((ERROR_SPU = ProgramCtor (&program, "asm.txt")) != SPU_NO_ERROR)
    {
        fprintf (logfile, "ProgramCtor ERROR = %d\n", ERROR_SPU);
        abort();
    }

    elem_t elem = 0, elem1 = 0, elem2 = 0;

    for (; program[p.ip] != cmd_HLT; p.ip++)
    {
        if (IsAliveSPUCanary (&p) != SPU_NO_ERROR)
        {
            fputs ("Canary died!\n", logfile);
            SPU_DUMP (p, logfile);
        }
        printf ("%d ", command_mask (program[p.ip]));
        switch (command_mask (program[p.ip]))
        {
            case cmd_push:
                printf ("%d\n", memory_mask (program[p.ip]));
                p.ip++;
                break;
        }
    }
}
