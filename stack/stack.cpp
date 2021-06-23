//
// Created by 1 on 02.12.2020.
//

#include "stack.h"

StackErr StackConstruct (Stack* thou, size_t capacity) {

    if (thou == nullptr) {

        printf ("Null pointer on stack in function %s\n", __FUNCTION__);
        return NullPointerOnStack;

    }

    thou->size = 0;
    thou->capacity = capacity;

    thou->data = (StackElement*)calloc (capacity, sizeof(StackElement));

    return NoErrorStack;

}
StackErr StackPush (Stack* thou, StackElement value) {

    if (thou == nullptr) {

        printf ("Null pointer on stack in function %s\n", __FUNCTION__);
        return NullPointerOnStack;

    }

    thou->data[thou->size++] = value;

    return NoErrorStack;

}

StackErr StackPop (Stack* thou) {

    if (thou == nullptr) {

        printf ("Null pointer on stack in function %s\n", __FUNCTION__);
        return NullPointerOnStack;

    }

    thou->data[thou->size--] = stackPoison;

    return NoErrorStack;

}

StackErr DeleteStack (Stack* thou) {

    if (thou == nullptr) {

        printf ("Null pointer on stack in function %s\n", __FUNCTION__);
        return NullPointerOnStack;

    }

    free (thou->data);
    thou->size = thou->capacity = 0;

    return NoErrorStack;

}

StackElement StackTop (Stack* thou) {

    if (thou == nullptr) {

        printf ("Null pointer on stack in function %s\n", __FUNCTION__);
        return stackPoison;

    }

    return thou->data[thou->size - 1];

}

