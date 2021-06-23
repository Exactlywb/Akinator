//
// Created by 1 on 01.12.2020.
//

#ifndef MYAKINATOR_AKINATOR_H
#define MYAKINATOR_AKINATOR_H

#define TX_USE_SPEAK

#include "../tree/tree.h"
#include "../textLib/textLib.h"
#include "../stack/stack.h"
#include "C:\\Users\\1\\Desktop\\TX\\TX\\TXLib.h"

enum AkinatorsMode {

    StartEnum,
    PlayMode,
    PrintInfo,
    CompareTwo,
    ShowTree,
    Exit,
    EndEnum

};

enum YesNo {

    No,
    Yes

};

enum AkinErr {

    NoErrorsAkinator,
    NullPointer,
    IncorrectValue

};

const size_t MAX_ANSWER_SIZE = 100;

bool    ShowMenuReturnStopCicleOrNo (Tree* mainTree);
size_t  ReadAnswerFunc              (char* buffer);
YesNo   ReadYesAndNo                (char* answer);
void    PrintTreeIntoFile           (Node* currentNode, FILE* output, const size_t deep);
void    RePrintFile                 (Tree* mainTree);
void    AddNewElem                  (Tree* mainTree, Node* currentNode);
AkinErr PrintPath                   (Stack* thou, Node* currentNode);
AkinErr BuildPath                   (Stack* thou, Node* currentNode, ElementTree value);

void PrintInfoFunc      (Tree* mainTree);
void PlayAkinator       (Tree* mainTree);
void ShowTreeFunc       (Tree* mainTree);
void PrintTwoPath       (char* firstName, char* secondName, Stack* firstPath, Stack* secondPath, Node* currentNode);
void CompareTwoElements (Tree* mainTree);

#endif //MYAKINATOR_AKINATOR_H
