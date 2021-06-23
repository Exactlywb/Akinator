//
// Created by 1 on 02.12.2020.
//

#ifndef MYAKINATOR_STACK_H
#define MYAKINATOR_STACK_H

#include <stdlib.h>
#include <stdio.h>

typedef char* StackElement;
static StackElement stackPoison = nullptr;

struct Stack {

    StackElement* data = nullptr;

    size_t capacity = 0; //max size
    size_t size     = 0;

};

enum StackErr {

    NoErrorStack,
    NullPointerOnStack,
    BadSize,
    PickError

};
//update
StackErr     StackConstruct (Stack* thou, size_t capacity);
StackErr     StackPush      (Stack* thou, StackElement value);
StackErr     StackPop       (Stack* thou);
StackErr     DeleteStack    (Stack* thou);
StackElement StackTop       (Stack* thou);

#endif //MYAKINATOR_STACK_H
