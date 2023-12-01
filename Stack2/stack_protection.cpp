#include "stack_protection.h"
#include <stdio.h>
#include "stack.hpp"
#include <stdlib.h>

void StackDump (stack p, char* stackname, char* file, int numline, const char* func)
{
    printf (">>> StackDump() has been started\nStack \"%s\" from file \"%s\", func \"%s\" on %d line\n"
    "%s\n{\n    p.capacity = %lu\n    p.size = %lu\n   {\n", stackname, file, func, numline,
    stackname, p.capacity, p.size);

    for (size_t i = 0; i < p.capacity; i++)
    {
        if (i == p.size)
        {
            printf (">>>    %d    <<<\n", p.data[i]);
        } else
        {
            printf ("       %d\n", p.data[i]);
        }
    }

    printf ("   }\n}\n>>> StackDump() has been finished\n");
}
