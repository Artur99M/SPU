#ifndef STACK_PROTECTION_H
#define STACK_PROTECTION_H

#define STACK_DUMP(p) StackDump(p, #p, __FILE__, __LINE__, __func__)

#include "stack.hpp"

void StackDump (struct stack p, char* stackname, char* file, int numline, const char* func);

#endif
