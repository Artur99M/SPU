#include <assert.h>
#include <stdio.h>
#include "stack.hpp"
#include "stack_protection.h"

static const int changesize = 2;

int isFull (stack* p){
    assert (p != nullptr);
    return p->size == p->capacity;
}

int isEmpty (stack* p) {
    assert(p != nullptr);
    return p->size == 0;
}

int StackPush (stack* p, const elem_t number)
{

    if (p == nullptr)
        return 1;
    if (p->data == nullptr)
        return 2;


    if (isFull (p))
    {
        p->capacity *= changesize;
        elem_t* data = (elem_t*) calloc (p->capacity * changesize, sizeof (elem_t));

        if (data == nullptr)
            return 8;

        for (size_t i = 0; i < p->capacity; i++)
            data[i] = p->data[i];

        p->capacity *= changesize;
        free (p->data);
        p->data = data;
    }

    p->data[p->size] = number;
    p->size++;

    return 0;
}

elem_t StackPop (stack* p, int* ERROR)
{
    if (ERROR != nullptr) *ERROR = 0;
    if (p == nullptr)
    {
        *ERROR += 1;
        return 0;
    }
    /*if (IsAliveCanary (p, sizeof (stack)) != 0)
    {
        *ERROR += 2;
        return 0;
    }*/

    elem_t answer = 0;

    if (isEmpty (p))
    {
        if (ERROR != nullptr) *ERROR += 4;
        return 0;
    }

    p->size--;
    answer = p->data[p->size];
    p->data[p->size] = NULL;


    if (p->size < p->capacity / changesize && p->capacity > 4)
    {
        p->capacity /= changesize;
        elem_t* data = nullptr;

        if ((data = (elem_t*) calloc (p->capacity, sizeof(elem_t))) == nullptr) {
            if (ERROR != nullptr) *ERROR += 8;
            return 0;
        }

        for (size_t i = 0; i < p->size; i++)
            data[i] = p->data[i];
        free(p->data);
        p->data = data;
}

    /*if (IsAliveCanary (p, sizeof (stack)) != 0)
    {
        *ERROR += 16;
        return 0;
    }*/

    return answer;
}

int StackCtor (stack* p, const size_t capacity)
{
    if (p == nullptr)
        return 1;

    p->capacity = capacity;
    p->size = 0;
    if ((p->data = (elem_t*) calloc (capacity, sizeof(elem_t))) == nullptr) return 2;

    return 0;
}

int StackDtor (stack* p)
{
    if (p == nullptr)
        return 1;

    free (p->data);
    p->capacity = 0;
    p->size = 0;
    p->data = nullptr;
    return 0;
}
