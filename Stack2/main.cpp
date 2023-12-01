#include <stdio.h>
#include "stack.h"
#include "stack_protection.h"

int main ()
{
    stack p;
    StackCtor (&p, 5);

    for (int i = 0; i < 100; i++)
        printf ("%d %d\n", i, StackPush (&p, i));
    putchar ('\n');
    int q = 0;
    for (int i = 0; i < 60; i++)
        printf ("%d %d\n", StackPop (&p, &q), q);


    STACK_DUMP (p);
    StackDtor (&p);
}

