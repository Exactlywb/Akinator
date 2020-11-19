//
// Created by 1 on 18.11.2020.
//

#ifndef AKINATOR_MINONEGIN_H
#define AKINATOR_MINONEGIN_H

#include <stdlib.h>
#include <stdio.h>
#include "../tree/tree.h"

char* ReadTitle (FILE* input);
void  Parser    (Tree* mainTree, Node* parent, FILE* input);

#endif //AKINATOR_MINONEGIN_H
