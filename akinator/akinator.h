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

    PLAY,
    DUMP,
    COMPARE,
    PRINT,
    PLAY_NO_SAVE,
    EXIT

};

enum SaveMode {

    noSave,
    shouldSave

};

enum AkinatorErrors {

    NO_ERR = 1,
    WRONG_ANSWER,
    MEMORY_ERR,
    NO_NAME,
    UNEXPECTED_ERR,
    NO_ELEM

};

enum PlayContinue {

    PlayContinue,
    PlayEnd

};

struct Way {

    size_t pathLength = 0;
    ElementTree* path = nullptr;

};

static const size_t AnswerMaxLength = 3;

Node*          PlayMode          (Node* currentNode);
void           TreatmentAnswer   (Node* answerNode, Tree* mainTree);
AkinatorErrors GetWay            (Tree* mainTree, ElementTree value, Way* currentWay);
AkinatorErrors CompareMode       (Tree* mainTree);
AkinatorErrors PrintPerson       (Tree* mainTree);
bool           ShowMenu          (Tree* mainTree);

#endif //AKINATOR_AKINATOR_H
