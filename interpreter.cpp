#include <stdio.h>
#include <string.h>
#include "spu.hpp"


int main()
{
    stack p;
    StackCtor (&p);
    FILE* in = fopen ("proga.txt", "r");
    forever
    {
        char s[12] = "";
        fscanf (in, "%s", s);
        if (strcmp (s, "HLT") == 0)
        {
            break;
        } else if (strcmp (s, "add") == 0)
        {
            StackPush (&p, StackPop (&p) + StackPop (&p));
        } else if (strcmp (s, "sub") == 0)
        {
            elem_t x = StackPop (&p);
            StackPush (&p, StackPop (&p) - x);
        } else if (strcmp (s, "mul") == 0)
        {
            StackPush (&p, StackPop (&p) * StackPop (&p));
        } else if (strcmp (s, "div") == 0)
        {
            elem_t elem = StackPop (&p);
            StackPush (&p, StackPop (&p) / elem);
        } else if (strcmp (s, "out") == 0)
        {
            printf ("%d\n", StackPop (&p));
        }
        else if (strcmp (s, "push") == 0)
        {
            elem_t x = 0;
            fscanf (in, "%d", &x);
            StackPush (&p, x);
        } else
        {
            printf ("ERROR\n");
            break;
        }
    }

    fclose (in);
    StackDtor (&p);

    return 0;
}
