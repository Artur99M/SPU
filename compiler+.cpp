#include <stdio.h>
#include "spu.hpp"


int main()
{
    SPU p;
    SpuCtor (&p);

    int* program;
    ProgramCtor (&program, "asm.txt");

    for (; program[p.ip] != cmd_HLT; p.ip++)
    {
        elem_t elem = 0, elem1 = 0, elem2 = 0;
        int ERROR1 = 0, ERROR2 = 0;
        switch (program[p.ip])
        {

        case cmd_push:
            elem = program[++p.ip];
            StackPush (&(p.stk), elem);
            break;

        case cmd_add:
            StackPush (&(p.stk), StackPop (&(p.stk)) + StackPop (&(p.stk)));
            break;

        case cmd_sub:
            elem = StackPop (&(p.stk));
            StackPush (&(p.stk), StackPop (&(p.stk)) - elem);
            break;

        case cmd_mul:
            StackPush (&(p.stk), StackPop (&(p.stk)) * StackPop (&(p.stk)));
            break;

        case cmd_div:
            elem = StackPop (&(p.stk));
            StackPush (&(p.stk), StackPop (&(p.stk)) / elem);
            break;

        case cmd_in:
            putchar ('>');
            scanf ("%d", &elem);
            StackPush (&(p.stk), elem);
            break;

        case cmd_out:
            printf ("%d\n", StackPop (&(p.stk)));
            break;

        case cmd_rpush:
            elem = program[++p.ip];

                switch (elem)
                {
                    case ax:
                        StackPush (&(p.stk), p.rax);
                        break;
                    case bx:
                        StackPush (&(p.stk), p.rbx);
                        break;
                    case cx:
                        StackPush (&(p.stk), p.rcx);
                        break;
                    case dx:
                        StackPush (&(p.stk), p.rdx);
                        break;
                }

            break;

        case cmd_pop:
            elem = program[++p.ip];
            switch (elem)
            {
                case ax:
                    p.rax = StackPop (&(p.stk));
                    break;
                case bx:
                    p.rbx = StackPop (&(p.stk));
                    break;
                case cx:
                    p.rcx = StackPop (&(p.stk));
                    break;
                case dx:
                    p.rdx = StackPop (&(p.stk));
                    break;
            }
            break;

        case cmd_jmp:
            p.ip = program[p.ip + 1] - 1;
            break;

        case cmd_jb:
            elem2 = StackPop (&(p.stk), &ERROR2);
            elem1 = StackPop (&(p.stk), &ERROR1);

            if (ERROR1 != 0 && ERROR2 != 0)
            {
                printf ("ERROR in command jb\nip = %lu\n", p.ip);
                SPU_DUMP (p);

            } else
            {
                if (elem1 < elem2)
                {
                    p.ip = program[p.ip + 1] - 1;
                } else
                {
                    p.ip++;
            }
            break;

        case cmd_je:
            elem2 = StackPop (&(p.stk), &ERROR2);
            elem1 = StackPop (&(p.stk), &ERROR1);

            if (ERROR1 != 0 && ERROR2 != 0)
            {
                printf ("ERROR in command je\nip = %lu\n", p.ip);
                SPU_DUMP (p);

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
            elem2 = StackPop (&(p.stk), &ERROR2);
            elem1 = StackPop (&(p.stk), &ERROR1);

            if (ERROR1 != 0 && ERROR2 != 0)
            {
                printf ("ERROR in command jbe\nip = %lu\n", p.ip);
                SPU_DUMP (p);

            } else
            {
                if (elem1 <= elem2)
                {
                    p.ip = program[p.ip + 1] - 1;
                } else
                {
                    p.ip++;
            }
            break;

        case cmd_ja:
            elem2 = StackPop (&(p.stk), &ERROR2);
            elem1 = StackPop (&(p.stk), &ERROR1);

            if (ERROR1 != 0 && ERROR2 != 0)
            {
                printf ("ERROR in command ja\nip = %lu\n", p.ip);
                SPU_DUMP (p);

            } else
            {
                if (elem1 > elem2)
                {
                    p.ip = program[p.ip + 1] - 1;
                } else
                {
                    p.ip++;
            }
            break;

        case cmd_jae:
            elem2 = StackPop (&(p.stk), &ERROR2);
            elem1 = StackPop (&(p.stk), &ERROR1);

            if (ERROR1 != 0 && ERROR2 != 0)
            {
                printf ("ERROR in command jae\nip = %lu\n", p.ip);
                SPU_DUMP (p);

            } else
            {
                if (elem1 >= elem2)
                {
                    p.ip = program[p.ip + 1] - 1;
                } else
                {
                    p.ip++;
            }
            break;

        case cmd_jne:
            elem2 = StackPop (&(p.stk), &ERROR2);
            elem1 = StackPop (&(p.stk), &ERROR1);

            if (ERROR1 != 0 && ERROR2 != 0)
            {
                printf ("ERROR in command jne\nip = %lu\n", p.ip);
                SPU_DUMP (p);

            } else
            {
                if (elem1 != elem2)
                {
                    p.ip = program[p.ip + 1] - 1;
                } else
                {
                    p.ip++;
            }
            break;

        case cmd_HLT:
            break;

        default:
            puts ("ERROR");
            SPU_DUMP (p);
            break;

        }
    }
    SpuDtor (&p);
}

