//
// Created by 1 on 01.12.2020.
//

#ifndef MYAKINATOR_TEXTLIB_H
#define MYAKINATOR_TEXTLIB_H

#include "../tree/tree.h"
#include <cstring>

static size_t MAX_TITLE_SIZE = 100;

void TreeParserFromFile (Tree* mainTree, Node* parent, FILE* input);
void ReadPhrase         (char* str, size_t maxSize);

#endif //MYAKINATOR_TEXTLIB_H
