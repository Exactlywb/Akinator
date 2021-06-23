//
// Created by 1 on 01.12.2020.
//
#include "akinator.h"

#define streq(str1, str2) !strcmp(str1, str2)

bool ShowMenuReturnStopCicleOrNo (Tree* mainTree) {

    if (mainTree == nullptr) {

        printf ("Null pointer on tree in function %s\n", __FUNCTION__);
        return Yes;

    }

    txSpeak ("\vChoose Akinator's mode\n");

    txSpeak ("\v1. Play\n"
             "2. Get information about element\n"
             "3. Compare two objects\n"
             "4. Show tree\n"
             "5. Exit\n");

    size_t playMode = 1;
    int mode = 0;

    if ( scanf ("%d", &mode) == playMode ) {

        switch (mode) {
            case PlayMode:
                PlayAkinator (mainTree);
                break;
            case PrintInfo:
                PrintInfoFunc (mainTree);
                break;
            case CompareTwo:
                CompareTwoElements (mainTree);
                break;
            case ShowTree:
                ShowTreeFunc (mainTree);
                break;
            case Exit:
                return Yes;
            default:
                printf ("You should enter number in range %d - %d!\n", StartEnum + 1, EndEnum - 1);
                break;

        }

    } else {

        printf ("You should enter number in range %d - %d!\n", StartEnum + 1, EndEnum - 1);

    }

    return No;

}

void PlayAkinator (Tree* mainTree) {

    if (mainTree == nullptr) {

        printf ("Null pointer on tree in function %s!\n", __FUNCTION__);
        return;

    }

    txSpeak ("\vYou should enter \"y\" or \"n\"\n");
    char readAnswer [MAX_ANSWER_SIZE] = "";
    Node* currentNode = mainTree->root;

    while (true) {

        if (currentNode->leftNode == nullptr) break;

        txSpeak ("\v%s?\n", currentNode->value);

        if (ReadYesAndNo (readAnswer)) {

            currentNode = currentNode->leftNode;

        } else {

            currentNode = currentNode->rightNode;

        }

    }

    txSpeak ("\vIs it %s?\n", currentNode->value);

    if (ReadYesAndNo (readAnswer)) {

        txSpeak ("\vHa-ha. Gotcha!\n");

    } else {

        txSpeak ("\vEverybody make mistakes! Do you want to add new object?\n");

        if (ReadYesAndNo (readAnswer)) AddNewElem (mainTree, currentNode);

    }

}

size_t ReadAnswerFunc (char* buffer) {

    fflush (stdin);
    fgets (buffer, MAX_ANSWER_SIZE, stdin);
    buffer[strlen(buffer) - 1] = '\0';

    return strlen (buffer);

}

YesNo ReadYesAndNo (char* answer) {

    while (true) {

        size_t lengthStr = ReadAnswerFunc (answer);

        if (lengthStr != 1 || (answer[0] != 'y' && answer[0] != 'n')) {

            txSpeak ("\vYou should enter only \"y\" or \"n\"\n");

        } else break;

    }

    if (answer[0] == 'y') return Yes;

    return No;

}

void PrintTreeIntoFile (Node* currentNode, FILE* output, const size_t deep) {

    assert (output);

    if (currentNode == nullptr) return;

    for (size_t i = 0; i < deep - 1; i++) fprintf (output, "\t");

    fprintf (output, "[\n");

    for (size_t i = 0; i < deep; i++) fprintf (output, "\t");

    if (currentNode->leftNode != nullptr) {
        printf ("%s\n", currentNode->value);
        fprintf (output, "?%s?\n", currentNode->value);

    } else {

        fprintf (output, "`%s`\n", currentNode->value);

    }

    PrintTreeIntoFile (currentNode->leftNode, output, deep + 1);
    PrintTreeIntoFile (currentNode->rightNode, output, deep + 1);

    for (size_t i = 0; i < deep - 1; i++)
        fprintf (output, "\t");

    fprintf (output, "]\n");

}

void RePrintFile (Tree* mainTree) {

    if (mainTree == nullptr) {

        printf ("null pointer on mainTree in function %s\n", __FUNCTION__);
        return;

    }

    FILE* outputTree = fopen ("../data/firstBase.txt", "w");
    assert (outputTree);

    size_t deepStart = 1;
    PrintTreeIntoFile (mainTree->root, outputTree, deepStart);

    fclose (outputTree);

}

void AddNewElem (Tree* mainTree, Node* currentNode) {

    if (mainTree == nullptr) {

        printf ("null pointer on mainTree in function %s\n", __FUNCTION__);
        return;

    }

    if (currentNode == nullptr) {

        printf ("null pointer on currentNode in function %s\n", __FUNCTION__);
        return;

    }

    txSpeak ("\vEnter the name of new element : ");

    char* getName = (char*)calloc(MAX_ANSWER_SIZE, sizeof(char));
    ReadPhrase (getName, MAX_ANSWER_SIZE);

    txSpeak ("\vWhat is the difference between %s and %s?\n", getName, currentNode->value);

    char* difference = (char*)calloc(MAX_ANSWER_SIZE, sizeof(char));
    ReadPhrase (difference, MAX_ANSWER_SIZE);

    AddNodeSide (mainTree, currentNode, getName, LeftSide);
    AddNodeSide (mainTree, currentNode, currentNode->value, RightSide);

    currentNode->value = difference;

    RePrintFile (mainTree);

}

void ShowTreeFunc (Tree* mainTree) {

    TreeDump (mainTree);
    system ("start treeDump.png");

}

AkinErr PrintPath (Stack* thou, Node* currentNode) {

    if (thou == nullptr) {

        printf ("Null pointer on stack in function %s!\n", __FUNCTION__);
        return NullPointer;

    }

    if (currentNode == nullptr) {

        printf ("Null pointer on current node in function%s!\n", __FUNCTION__);
        return NullPointer;

    }

    char* prevName = StackTop (thou);
    StackPop (thou);
    char* currName = StackTop (thou);
    StackPop (thou);

    while (thou->size > 0) {

        if ( streq(currentNode->rightNode->value, currName) ) {

            currentNode = currentNode->rightNode;
            txSpeak ("\vnot ");

        } else
            currentNode = currentNode->leftNode;

        txSpeak ("\v%s, ", prevName);
        prevName = currName;
        currName = StackTop (thou);
        StackPop (thou);

    }

    if ( streq(currentNode->rightNode->value, currName) )
        txSpeak ("\vnot ");

    txSpeak ("\v%s. \n", prevName);

    return NoErrorsAkinator;

}

AkinErr BuildPath (Stack* thou, Node* currentNode, ElementTree value) {//можно без рекурсии через родителей. Лучше переписать.

    if (thou == nullptr) {

        printf ("Null pointer on thou in function %s\n", __FUNCTION__);
        return NullPointer;

    }

    if (currentNode == nullptr) {

        printf ("Null pointer on currentNode in function %s\n", __FUNCTION__);
        return NullPointer;

    }

    if ( streq(currentNode->value, value) ) {

        StackPush (thou, currentNode->value);
        return NoErrorsAkinator;

    }

    if (currentNode->rightNode && (BuildPath (thou, currentNode->rightNode, value) == NoErrorsAkinator) ) {

        StackPush (thou, currentNode->value);
        return NoErrorsAkinator;

    }

    if (currentNode->leftNode && (BuildPath (thou, currentNode->leftNode, value) == NoErrorsAkinator) ) {

        StackPush (thou, currentNode->value);
        return NoErrorsAkinator;

    }

    return IncorrectValue;

}

void PrintInfoFunc (Tree* mainTree) {

    if (mainTree == nullptr) {

        printf ("Null pointer on tree in function %s\n", __FUNCTION__);
        return;

    }

    char readAnswer [MAX_ANSWER_SIZE] = "";
    txSpeak ("\vEnter name of object you want to find:");
    ReadPhrase (readAnswer, MAX_ANSWER_SIZE);

    Stack mainStack = {};
    StackConstruct (&mainStack, mainTree->size);

    BuildPath (&mainStack, mainTree->root, readAnswer);

    PrintPath (&mainStack, mainTree->root);

    DeleteStack (&mainStack);

}

void PrintTwoPath (char* firstName, char* secondName, Stack* firstPath, Stack* secondPath, Node* currentNode) {

    if (firstName == nullptr) {

        printf ("Null pointer on firstName in function %s\n", __FUNCTION__);
        return;

    }

    if (secondName == nullptr) {

        printf ("Null pointer on secondName in function %s\n", __FUNCTION__);
        return;

    }

    if (firstPath == nullptr) {

        printf ("Null pointer on firstPath in function %s\n", __FUNCTION__);
        return;

    }

    if (secondPath == nullptr) {

        printf ("Null pointer on secondPath in function %s\n", __FUNCTION__);
        return;

    }

    if (currentNode == nullptr) {

        printf ("Null pointer on currentNode in function %s\n", __FUNCTION__);
        return;

    }

    txSpeak ("\v%s and %s are both ", firstName, secondName);

    char* firstNameCurr = StackTop (firstPath);
    StackPop (firstPath);
    char* firstNameNext = StackTop (firstPath);
    StackPop (firstPath);

    char* secondNameCurr = StackTop (secondPath);
    StackPop (secondPath);
    char* secondNameNext = StackTop (secondPath);
    StackPop (secondPath);

    while ( streq(firstNameNext, secondNameNext) ) {

        if ( streq(currentNode->rightNode->value, firstNameNext) ) {

            txSpeak ("\vnot ");
            currentNode = currentNode->rightNode;

        } else currentNode = currentNode->leftNode;

        txSpeak ("\v%s", firstNameCurr);

        firstNameCurr = firstNameNext;
        secondNameCurr = secondNameNext;

        firstNameNext = StackTop (firstPath);
        StackPop (firstPath);

        secondNameNext = StackTop (secondPath);
        StackPop (secondPath);

        if ( streq(firstNameNext, secondNameNext) ) printf (", ");

    }

    printf (".\n");

    txSpeak ("\vBut %s also is ", firstName);
    StackPush (firstPath, firstNameNext);
    StackPush (firstPath, firstNameCurr);
    PrintPath (firstPath, currentNode);

    txSpeak ("\vBut %s also is ", secondName);
    StackPush (secondPath, secondNameNext);
    StackPush (secondPath, secondNameCurr);
    PrintPath (secondPath, currentNode);

    printf ("\n");

}

void CompareTwoElements (Tree* mainTree) {

    if (mainTree == nullptr) {

        printf ("Null pointer on mainTree in function %s\n", __FUNCTION__);
        return;

    }

    char firstName [MAX_ANSWER_SIZE]  = "";
    char secondName [MAX_ANSWER_SIZE] = "";

    txSpeak ("\vYou have to enter two objects you want to compare!\n");

    ReadPhrase (firstName, MAX_ANSWER_SIZE);
    ReadPhrase (secondName, MAX_ANSWER_SIZE);

    Stack firstStack = {};
    StackConstruct (&firstStack, mainTree->size);

    Stack secondStack = {};
    StackConstruct (&secondStack, mainTree->size);

    BuildPath (&firstStack, mainTree->root, firstName);
    BuildPath (&secondStack, mainTree->root, secondName);

    PrintTwoPath (firstName, secondName, &firstStack, &secondStack, mainTree->root);

    DeleteStack (&firstStack);
    DeleteStack (&secondStack);

}
