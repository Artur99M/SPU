#include <assert.h>
#include <stdio.h>
#include "stack.h"
#include "stack_protection.h"

const size_t start_capacity = 8;


int isFull (stack* p){
    assert (p != nullptr);
    return p->size == p->capacity;
}

int isEmpty (stack* p) {
    assert (p != nullptr);
    return p->size == 0;
}

STACK_ERROR StackPush (stack* p, const elem_t* number)
{
    STACK_ERROR answer = STACK_NO_ERROR;
    if ((answer = StackVerifi(p)) != STACK_NO_ERROR)
        return answer;

    if ((answer = IsAliveCanary (p)) != STACK_NO_ERROR) return answer;


    if (isFull (p))
    {
        if ((answer = DataReCtorPush (p)) != STACK_NO_ERROR) return answer;
    }

    p->data[p->size] = *number;
    p->size++;

    return STACK_NO_ERROR;
}

STACK_ERROR StackPop (stack* p, elem_t* pointer_for_elem)
{
    STACK_ERROR answer = STACK_NO_ERROR;
    if ((answer = StackVerifi(p)) != STACK_NO_ERROR)
        return answer;

    if ((answer = IsAliveCanary (p)) != STACK_NO_ERROR)
        return answer;

    if (isEmpty (p))
        return STACK_EMPTY;

    p->size--;
    if (pointer_for_elem != nullptr) *pointer_for_elem = p->data[p->size];
    p->data[p->size] = NULL;


    if (p->size < p->capacity / changesize && p->capacity > start_capacity)
        answer = DataReCtorPop (p);

    return answer;
}

STACK_ERROR StackPop (stack* p)
{
    STACK_ERROR answer = STACK_NO_ERROR;
    if ((answer = StackVerifi(p)) != STACK_NO_ERROR)
        return answer;

    if ((answer = IsAliveCanary (p)) != STACK_NO_ERROR)
        return answer;

    if (isEmpty (p))
        return STACK_EMPTY;

    p->size--;
    p->data[p->size] = NULL;


    if (p->size < p->capacity / changesize && p->capacity > start_capacity)
        answer = DataReCtorPop (p);

    return answer;
}

STACK_ERROR StackCtor (stack* p, size_t capacity)// оно отказывается смотреть на значение по умолчанию
{                                                                // поэтому со значением по умолчанию во всех функциях беда
    STACK_ERROR answer = STACK_NO_ERROR;
    if (p == nullptr)
        return STACK_NULLPTR;
    if (capacity == 0) capacity = start_capacity;

    p->capacity = capacity;
    p->size = 0;
    if ((answer = CanaryCtor (p)) != 0) return answer;
    if ((answer = DataCtor (p)) != 0) return answer;

    return STACK_NO_ERROR;
}

STACK_ERROR StackCtor (stack* p)// оно отказывается смотреть на значение по умолчанию
{                              // поэтому со значением по умолчанию во всех функциях беда
    STACK_ERROR answer = STACK_NO_ERROR;
    if (p == nullptr)
        return STACK_NULLPTR;

    p->capacity = start_capacity;
    p->size = 0;
    if ((answer = CanaryCtor (p)) != 0) return answer;
    if ((answer = DataCtor (p)) != 0) return answer;

    return STACK_NO_ERROR;
}

STACK_ERROR StackDtor (stack* p)
{
    if (p == nullptr)
        return STACK_NULLPTR;

    free ((canary_t*)p->data - 1);
    p->capacity = 0;
    p->size = 0;
    p->data = nullptr;
    return STACK_NO_ERROR;
}


// elem_t StackPop (stack* p)
// {
//     if (p == nullptr)
//         return 0;
//     if (IsAliveCanary (p) != 0)
//         return 0;
//
//     elem_t value = 0;
//
//     if (isEmpty (p))
//         return 0;
//
//     p->size--;
//     value = p->data[p->size];
//     p->data[p->size] = NULL;
//
//
//     if (p->size < p->capacity / changesize && p->capacity > start_capacity)
//     {
//         DataReCtorPop (p);
//     }
//
//     return value;
// }

// int StackCtor (stack* p)
// {
//     if (p == nullptr)
//         return 1;
//
//     p->capacity = start_capacity;
//     p->size = 0;
//     if (CanaryCtor (p) != 0) return 2;
//     if (DataCtor (p) != 0) return 4;
//
//     return 0;
// }
