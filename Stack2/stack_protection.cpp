#include "stack_protection.h"
#include <stdio.h>
#include "stack.h"
#include <stdlib.h>
#include <math.h>

static const canary_t canary = time (NULL) * sin (time (NULL));

void StackDump (const stack* p, char* stackname, char* file, int numline, const char* func)
{
    if ((StackVerifi((stack*)p)) != STACK_NO_ERROR)
    {
        printf ("StackDump is empty\n");
        abort();
    }
    printf (">>> StackDump() has been started\nStack \"%s\" from file \"%s\", func \"%s\" on %d line\n"
    "%s\n{\n    p.capacity = %lu\n    p.size = %lu\n   {\n", stackname, file, func, numline,
    stackname, p->capacity, p->size);

    for (size_t i = 0; i < p->capacity; i++)
    {
        if (i == p->size)
        {
            printf (">>>    %d    <<<\n", p->data[i]);
        } else
        {
            printf ("       %d\n", p->data[i]);
        }
    }

    printf ("   }\n}\n>>> StackDump() has been finished\n");
}

STACK_ERROR CanaryCtor (struct stack* p)
{
    if (p == nullptr)
        return STACK_NULLPTR;

    p->canary_left = canary;
    p->canary_right = canary;

    return STACK_NO_ERROR;
}

STACK_ERROR IsAliveCanary (struct stack* p)
{
    int alive_canary = 0;
    STACK_ERROR answer = STACK_NO_ERROR;
    if ((answer = StackVerifi(p)) != STACK_NO_ERROR)
        return answer;

    if (p->canary_left != canary) alive_canary += 2;
    if (p->canary_right != canary) alive_canary += 4;
    if (*((canary_t*)p->data - 1) != canary) alive_canary += 8;
    if (*((canary_t*)(p->data + p->capacity)) != canary) alive_canary += 16;

    return (STACK_ERROR) alive_canary;
}
STACK_ERROR DataCtor (struct stack* p)
{
    if (p == nullptr) return STACK_NULLPTR;


    if ((p->data = (elem_t*) calloc (1, p->capacity * sizeof (elem_t) + 2 * sizeof (canary_t))) == nullptr)
        return STACK_CALLOC_ERROR;

    canary_t* canary_left = (canary_t*) p->data;
    *canary_left = canary;
    p->data = (elem_t*) ((canary_t*)p->data + 1);

    canary_t* canary_right = (canary_t*) (p->data + p->capacity);
    *canary_right = canary;

    return STACK_NO_ERROR;
}

STACK_ERROR DataReCtorPush (struct stack* p)
{
    STACK_ERROR answer = STACK_NO_ERROR;
    if ((answer = StackVerifi(p)) != STACK_NO_ERROR)
        return answer;

    p->capacity *= changesize;

    elem_t* data = nullptr;
    if ((data = (elem_t*) realloc ((void*)((canary_t*)p->data - 1), p->capacity * sizeof (elem_t) + 2 * sizeof (canary_t))) == nullptr)
        return STACK_REALLOC_ERROR;


    p->data = (elem_t*)((canary_t*)data + 1);
    canary_t* canary_right = (canary_t*) (p->data + p->capacity);
    *canary_right = canary;


    return STACK_NO_ERROR;
}

STACK_ERROR DataReCtorPop (struct stack* p)
{
    STACK_ERROR answer = STACK_NO_ERROR;
    if ((answer = StackVerifi(p)) != STACK_NO_ERROR)
        return answer;

    p->capacity /= changesize;

    elem_t* data = nullptr;
    if ((data = (elem_t*) realloc ((void*)((canary_t*)p->data - 1), p->capacity * sizeof (elem_t) + 2 * sizeof (canary_t))) == nullptr)
        return STACK_REALLOC_ERROR;


    p->data = (elem_t*)((canary_t*)data + 1);
    canary_t* canary_right = (canary_t*) (p->data + p->capacity);
    *canary_right = canary;

    return STACK_NO_ERROR;
}

STACK_ERROR StackVerifi (struct stack* p)
{
    if (p == nullptr)
        return STACK_NULLPTR;

    if (p->data == nullptr)
        return STACK_DATA_NULLPTR;

    return STACK_NO_ERROR;
}
