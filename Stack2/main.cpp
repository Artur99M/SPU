#include <stdio.h>
#include "stack.h"
#include "stack_protection.h"

int main ()
{
    stack stk;
    STACK_ERROR err;
    if ((err = StackCtor (&stk, 8)) != STACK_NO_ERROR)
    {
        printf ("%d\n", (int) err);
        STACK_DUMP (&stk);
    }
    elem_t p = 8;
    for (; p < 20; p++)
    {
        puts ("bark");
        if ((err = StackPush (&stk, &p)) != STACK_NO_ERROR)
        {
            printf ("8 %d\n", (int) err);
            STACK_DUMP (&stk);
        }
    }
    STACK_DUMP (&stk);
    for (int i = 0; i < 7; i++)
    {
        if ((err = StackPop (&stk)) != STACK_NO_ERROR)
        {
            printf ("%d\n", (int) err);
            STACK_DUMP (&stk);
        }
    }
    STACK_DUMP (&stk);
    if ((err = StackDtor (&stk)) != STACK_NO_ERROR)
    {
        printf ("%d\n", (int) err);
        STACK_DUMP (&stk);
    }
}

