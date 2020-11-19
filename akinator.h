//
// Created by 1 on 18.11.2020.
//

#ifndef AKINATOR_AKINATOR_H
#define AKINATOR_AKINATOR_H

#include "../tree/tree.h"
#include "../minOnegin/minOnegin.h"

#define TX_USE_SPEAK
#include "D:\\TX\\TXLib.h"

enum AkinatorModeGame {

    #define DEF_MODE(name) name,
        #include "../data/Akinator_Play_Modes.txt"
    #undef DEF_MODE

};

enum AkinatorErrors {

#define AKIN_ERR(name, num) name = (num),
#include "../data/AKIN_ERR.txt"
#undef AKIN_ERR

};

struct Way {

    size_t pathLength = 0;
    ElementTree* path = nullptr;

};

static const size_t AnswerMaxLength = 3;

Node* PlayMode          (Node* currentNode);
void  TreatmentAnswer   (Node* answerNode, Tree* mainTree);
Way   GetWay            (Tree* mainTree, ElementTree value);
void  CompareMode       (Tree* mainTree);
void  PrintPerson       (Tree* mainTree);
void  ShowMenu          (Tree* mainTree);

#endif //AKINATOR_AKINATOR_H
