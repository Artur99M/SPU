#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include "stack_protection.h"

typedef int elem_t;

struct stack {
    size_t capacity;
    size_t size;
    elem_t* data;
};

int isFull (stack* p);
int isEmpty (stack* p);
int StackPush (stack* p, const elem_t number);
elem_t StackPop (stack* p, int* ERROR = nullptr);
int StackCtor (stack* p, const size_t capacity = 4);
int StackDtor (stack* p);

#endif
