#include <stdio.h>
#include "spu.hpp"


int main()
{
    SPU p;
    SpuCtor (&p);

    int* program;
    ProgramCtor (&program, "asm.txt");

    for (int i = 0; program[i] != HLT; i++)
    {
        int x = program[i];
        elem_t elem = 0;
        switch (x)
        {

        case push:
            elem = program[++i];
            StackPush (&(p.stk), elem);
            break;

        case add:
            StackPush (&(p.stk), StackPop (&(p.stk)) + StackPop (&(p.stk)));
            break;

        case sub:
            elem = StackPop (&(p.stk));
            StackPush (&(p.stk), StackPop (&(p.stk)) - elem);
            break;

        case mul:
            StackPush (&(p.stk), StackPop (&(p.stk)) * StackPop (&(p.stk)));
            break;

        case divi:
            elem = StackPop (&(p.stk));
            StackPush (&(p.stk), StackPop (&(p.stk)) / elem);
            break;

        case in:
            putchar ('>');
            scanf ("%d", &elem);
            StackPush (&(p.stk), elem);
            break;

        case out:
            printf ("%d\n", StackPop (&(p.stk)));
            break;

        case rpush:
            elem = program[++i];

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
        case pop:
            elem = program[++i];
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

        default:
            puts ("ERROR");
            SPU_DUMP (p);
            break;

        }
    }
    SpuDtor (&p);
}

