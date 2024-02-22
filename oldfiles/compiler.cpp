#include <stdio.h>
#include "spu.hpp"

int main()
{
    SPU p = {};
    SpuCtor (&p);
    FILE* infile = fopen ("asm.txt", "r");

    int x = 0;
    while (x != HLT)
    {
        fscanf (infile, "%d", &x);
        elem_t elem = 0;
        reg y = ax;
        switch (x)
        {

        case push:
            fscanf (infile, "%d", &elem);
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

        case div:
            elem = StackPop (&(p.stk));
            StackPush (&(p.stk), StackPop (&(p.stk)) / elem);
            break;

        case pop:
            fscanf (infile, "%d", &y);


                switch (y)
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

        case rpush:
            fscanf (infile, "%d", &y);


                switch (y)
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

        case out:
            printf ("%d\n", StackPop (&(p.stk)));
            break;

        case in:
            putchar ('>');
            scanf ("%d", &elem);
            StackPush (&(p.stk), elem);
            break;
        }
    }
    fclose (infile);
    SpuDtor (&p);
}

