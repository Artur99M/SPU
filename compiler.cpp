#include <stdio.h>
#include "spu.h"
#include <stdlib.h>
#include <stdint.h>

#define command_mask(x) 0x001F&x
#define memory_mask(x) (x >> 13)&0x0007
#define RAM 4
#define REGISTER 2
#define NUMBER 1
#define RAM_SIZE 10737418240 // = 10 * 2^30

int main()
{
    SPU p;
    SpuCtor (&p);

    FILE* logfile = fopen ("compiler.log", "w");
    SPU_ERROR err = SPU_NO_ERROR;
    STACK_ERROR err_stk = STACK_NO_ERROR;

    int16_t* program;
    if ((err = ProgramCtor (&program, "asm.txt")) != SPU_NO_ERROR)
    {
        fprintf (logfile, "ProgramCtor ERROR = %d\n", err);
        abort();
    }

    int16_t* ram = (int16_t*) calloc (1, RAM_SIZE);
    if (ram == nullptr)
    {
        fputs ("RAM HAS SIZE 0\n", logfile);
        abort();
    }

    elem_t elem = 0, elem1 = 0, elem2 = 0;
    reg regist = ax;
    for (; program[p.ip] != cmd_HLT; p.ip++)
    {
        if (IsAliveSPUCanary (&p) != SPU_NO_ERROR)
        {
            fputs ("Canary died!\n", logfile);
            SPU_DUMP (p, logfile);
        }
        switch (command_mask (program[p.ip]))
        {

            case cmd_push:
                elem = program[p.ip + 1];
                if ((err_stk = StackPush (&(p.stk), &elem)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "PUSH ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                if (memory_mask(program[p.ip]) == REGISTER)
                    switch (elem)
                    {
                        case ax:
                            StackPush (&(p.stk), &(p.rax));
                            break;
                        case bx:
                            StackPush (&(p.stk), &(p.rbx));
                            break;
                        case cx:
                            StackPush (&(p.stk), &(p.rcx));
                            break;
                        case dx:
                            StackPush (&(p.stk), &(p.rdx));
                            break;
                    }
                else if (memory_mask(program[p.ip]) == NUMBER)
                    StackPush (&(p.stk), &elem);

                //else if (memory_mask (program[p.ip]) == RAM)

                else
                {
                    fprintf (logfile, "PUSH ERROR\n");
                    SPU_DUMP (p, logfile);
                    abort();
                }
                p.ip++;
                break;

            case cmd_add:
                if ((err_stk = StackPop (&(p.stk), &elem1)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "ADD POP ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                if ((err_stk = StackPop (&(p.stk), &elem2)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "ADD POP ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                elem = elem1 + elem2;
                if ((err_stk = StackPush (&(p.stk), &elem)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "ADD PUSH ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                break;

            case cmd_sub:
                if ((err_stk = StackPop (&(p.stk), &elem1)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "SUB POP ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                if ((err_stk = StackPop (&(p.stk), &elem2)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "SUB POP ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                elem = elem2 - elem1;
                if ((err_stk = StackPush (&(p.stk), &elem)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "SUB PUSH ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                break;

            case cmd_mul:
                if ((err_stk = StackPop (&(p.stk), &elem1)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "MUL POP ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                if ((err_stk = StackPop (&(p.stk), &elem2)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "MUL POP ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                elem = elem1 * elem2;
                if ((err_stk = StackPush (&(p.stk), &elem)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "MUL PUSH ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                break;

            case cmd_div:
                if ((err_stk = StackPop (&(p.stk), &elem1)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "DIV POP ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                if ((err_stk = StackPop (&(p.stk), &elem2)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "DIV POP ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                elem = elem2 / elem1;
                if ((err_stk = StackPush (&(p.stk), &elem)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "DIV PUSH ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                break;

            case cmd_in:
                putchar ('>');
                while (scanf ("%d", &elem) == 0)
                {
                    printf ("\n>");
                    puts ("It has to be a number!");
                }
                if ((err_stk = StackPush (&(p.stk), &elem)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "IN PUSH ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                break;

            case cmd_out:
                if ((err_stk = StackPop (&(p.stk), &elem)) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "OUT POP ERROR = %d\n", err_stk);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                printf ("%d\n", elem);
                break;

            case cmd_pop:
                if (memory_mask(program[p.ip]) == REGISTER)
                    switch (program[p.ip + 1])
                    {
                        case ax:
                            StackPop (&(p.stk), &(p.rax));
                            break;
                        case bx:
                            StackPop (&(p.stk), &(p.rbx));
                            break;
                        case cx:
                            StackPop (&(p.stk), &(p.rcx));
                            break;
                        case dx:
                            StackPop (&(p.stk), &(p.rdx));
                            break;
                    }
                //else if (memory_mask (program[p.ip]) == RAM)
                else
                {
                    fputs ("POP ERROR\n", logfile);
                    SPU_DUMP (p, logfile);
                    abort();
                }

                p.ip++;
                break;

            case cmd_jmp:
                p.ip = program[p.ip + 1] - 1;
                break;

            case cmd_jb:
                if (StackPop (&(p.stk), &elem2) != STACK_NO_ERROR || StackPop (&(p.stk), &elem1) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "ERROR in command jb\nip = %lu\n", p.ip);
                    SPU_DUMP (p, logfile);
                    abort();

                } else
                {
                    if (elem1 < elem2)
                    {
                        p.ip = program[p.ip + 1] - 1;
                    } else
                    {
                        p.ip++;
                    }
                }
                break;

            case cmd_je:
                if (StackPop (&(p.stk), &elem2) != STACK_NO_ERROR || StackPop (&(p.stk), &elem1) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "ERROR in command jb\nip = %lu\n", p.ip);
                    SPU_DUMP (p, logfile);
                    abort();

                } else
                {
                    if (elem1 == elem2)
                    {
                        p.ip = program[p.ip + 1] - 1;
                    } else
                    {
                        p.ip++;
                    }
                }
                break;

            case cmd_jbe:
                if (StackPop (&(p.stk), &elem2) != STACK_NO_ERROR || StackPop (&(p.stk), &elem1) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "ERROR in command jb\nip = %lu\n", p.ip);
                    SPU_DUMP (p, logfile);
                    abort();

                } else
                {
                    if (elem1 <= elem2)
                    {
                        p.ip = program[p.ip + 1] - 1;
                    } else
                    {
                        p.ip++;
                    }
                }
                break;

            case cmd_ja:
                if (StackPop (&(p.stk), &elem2) != STACK_NO_ERROR || StackPop (&(p.stk), &elem1) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "ERROR in command jb\nip = %lu\n", p.ip);
                    SPU_DUMP (p, logfile);
                    abort();

                } else
                {
                    if (elem1 > elem2)
                    {
                        p.ip = program[p.ip + 1] - 1;
                    } else
                    {
                        p.ip++;
                    }
                }
                break;

            case cmd_jae:
                if (StackPop (&(p.stk), &elem2) != STACK_NO_ERROR || StackPop (&(p.stk), &elem1) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "ERROR in command jb\nip = %lu\n", p.ip);
                    SPU_DUMP (p, logfile);
                    abort();

                } else
                {
                    if (elem1 >= elem2)
                    {
                        p.ip = program[p.ip + 1] - 1;
                    } else
                    {
                        p.ip++;
                    }
                }
                break;

            case cmd_jne:
                if (StackPop (&(p.stk), &elem2) != STACK_NO_ERROR || StackPop (&(p.stk), &elem1) != STACK_NO_ERROR)
                {
                    fprintf (logfile, "ERROR in command jb\nip = %lu\n", p.ip);
                    SPU_DUMP (p, logfile);
                    abort();

                } else
                {
                    if (elem1 != elem2)
                    {
                        p.ip = program[p.ip + 1] - 1;
                    } else
                    {
                        p.ip++;
                    }
                }
                break;

            case cmd_HLT:
                break;

            case cmd_call:
                elem = p.ip + 1;
                if (StackPush (&(p.stk_ip), &(elem)) != STACK_NO_ERROR)
                {
                    fputs ("CALL ERROR", logfile);
                    SPU_DUMP (p,logfile);
                    abort();
                }
                p.ip = program[p.ip + 1] - 1;
                break;

            case cmd_ret:
                int16_t newip = 0;
                if (StackPop (&(p.stk_ip), &newip) != STACK_NO_ERROR)
                {
                    fputs ("RET ERROR", logfile);
                    SPU_DUMP (p, logfile);
                    abort();
                }
                p.ip = newip - 1;
                break;


        }
    }
    fclose (logfile);
    SpuDtor (&p);
}

