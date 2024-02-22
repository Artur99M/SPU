#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include "stack_protection.h"

const int changesize = 2;

typedef int elem_t;

struct stack {
    canary_t canary_left;
    size_t capacity;
    size_t size;
    elem_t* data;
    canary_t canary_right;
};

int isFull (stack* p);
int isEmpty (stack* p);
STACK_ERROR StackPush (stack* p, const elem_t* number);
STACK_ERROR StackPop (stack* p, elem_t* pointer_for_elem);
STACK_ERROR StackPop (stack* p);
STACK_ERROR StackCtor (stack* p);
STACK_ERROR StackCtor (stack* p, size_t capacity);
STACK_ERROR StackDtor (stack* p);

#endif
