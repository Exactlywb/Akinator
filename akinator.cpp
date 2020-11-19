//
// Created by 1 on 18.11.2020.
//

#include "akinator.h"

Node* PlayMode (Node* currentNode) {

    assert (currentNode);

    txSpeak ("\v%s\n", currentNode->value);

    if (currentNode->left == nullptr) return currentNode;

    char readAnswer[AnswerMaxLength] = {};
    scanf ("%s", readAnswer);

    if ( !strcmp(readAnswer, "yes") ) {

        return PlayMode (currentNode->left);

    } else if ( !strcmp(readAnswer, "no") ){

        return PlayMode (currentNode->right);

    } else {

        txSpeak("\vSyntax error! You had to enter \"yes\" or \"no\"!\n");
        exit   (WRONG_ANSWER);

    }

}

void TreatmentAnswer (Node* answerNode, Tree* mainTree, bool mode) {

    assert (answerNode && mainTree);

    txSpeak("\vIs this answer correct?\n");

    char readAnswer[AnswerMaxLength] = {};
    scanf ("%s", readAnswer);

    if (!strcmp(readAnswer, "yes")) {

        txSpeak("\vHa-ha. Gotcha!");

    } else if ( !strcmp(readAnswer, "no") ){

        if (!mode) {

            txSpeak("\vEverybody make mistakes!\n");
            return;

        }

        ElementTree newElementName = (char*)calloc(elementMaxSize, sizeof(char));
        if (!newElementName) {

            printf ("There is a bad with memory in %s func!\n", __FUNCTION__);
            exit (MEMORY_ERR);

        }

        txSpeak("\vEnter the real answer : \n");
        fflush (stdin);
        gets (newElementName);

        txSpeak ("\vWhat is different between %s and %s?\n", newElementName, answerNode->value);

        ElementTree newQuestion = (char*)calloc(elementMaxSize + 1, sizeof(char));
        if (!newQuestion) {

            printf ("There is a bad with memory in %s func!\n", __FUNCTION__);
            exit (MEMORY_ERR);

        }

        fflush (stdin);
        gets (newQuestion);
        newQuestion[strlen(newQuestion)] = '?';

        char* oldAnswer = answerNode->value;
        answerNode->value = newQuestion;

        AddLeft(mainTree, answerNode, newElementName);
        AddRight(mainTree, answerNode, oldAnswer);

        FILE* outputTree = fopen ("../input.txt", "w");
        assert (outputTree);

        PrintTreeIntoFile    (mainTree->root, outputTree, 1);

        fclose (outputTree);

    } else {

        txSpeak ("\vSyntax error! You have to enter \"yes\" or \"no\"!\n");
        exit (WRONG_ANSWER);

    }

}

Way GetWay (Tree* mainTree, ElementTree value) {

    assert (mainTree);

    Node* foundedNode = FindNode (mainTree->root, value);

    if (foundedNode == nullptr) {

        txSpeak ("\vThere is none element named like that!\n");
        exit (NO_ELEM);

    }

    size_t maxPathLength = mainTree->size;

    ElementTree* path = (ElementTree*)calloc(maxPathLength, sizeof(ElementTree));
    if (!path) {

        printf ("There is a bad with memory in %s func!\n", __FUNCTION__);
        exit (MEMORY_ERR);

    }

    size_t indexPath = 0;

    for (indexPath = 0; foundedNode->parent != nullptr;) {

        if (foundedNode == foundedNode->parent->left) {

            path[indexPath] = foundedNode->parent->value;
            path[indexPath][strlen(path[indexPath]) - 1] = ',';
            indexPath++;

        }

        foundedNode = foundedNode->parent;

    }

    path[indexPath - 1][strlen(path[indexPath - 1]) - 1] = '.';

    Way result = {indexPath, path};

    return result;

}

void CompareMode (Tree* mainTree) {

    assert (mainTree);

    char* firstName  = (char*)calloc(elementMaxSize, sizeof(char));
    if (!firstName) {

        printf ("There is a bad with memory in %s func!\n", __FUNCTION__);
        exit   (MEMORY_ERR);

    }
    char* secondName = (char*)calloc(elementMaxSize, sizeof(char));
    if (!secondName) {

        printf ("There is a bad with memory in %s func!\n", __FUNCTION__);
        exit   (MEMORY_ERR);

    }

    txSpeak ("\vEnter two objects you want to compare : \n");

    fflush (stdin);
    gets (firstName);

    fflush (stdin);
    gets (secondName);

    Way firstPath  = GetWay (mainTree, firstName);
    Way secondPath = GetWay (mainTree, secondName);

    txSpeak ("\vSame properties : ");

    while ( !strcmp(firstPath.path[firstPath.pathLength - 1], secondPath.path[secondPath.pathLength - 1]) )  {

        txSpeak ("\v%s", firstPath.path[--firstPath.pathLength]);
        secondPath.pathLength--;

        if (firstPath.pathLength * secondPath.pathLength == 0) break;

    }

    printf ("\n");

    txSpeak ("\vUnique properties.\n");
    txSpeak ("\v%s : ", firstName);

    for (size_t pathIndex = 0; firstPath.path[pathIndex] != nullptr && pathIndex < firstPath.pathLength; pathIndex++) {

        if (pathIndex + 1 == firstPath.pathLength) {

            firstPath.path[pathIndex][strlen(firstPath.path[pathIndex]) - 1] = '.';

        }

        txSpeak("\v%s", firstPath.path[pathIndex]);

    }

    printf ("\n%s : ", secondName);
    for (size_t pathIndex = 0; secondPath.path[pathIndex] != nullptr && pathIndex < secondPath.pathLength; pathIndex++) {

        if (pathIndex + 1 == secondPath.pathLength) {

            secondPath.path[pathIndex][strlen(secondPath.path[pathIndex]) - 1] = '.';

        }
        txSpeak("\v%s", secondPath.path[pathIndex]);

    }

    printf("\n");

    free (firstName);
    free (secondName);

}

void PrintPerson (Tree* mainTree) {

    assert (mainTree);

    txSpeak ("\vWhich person is your object to find?\n");

    fflush (stdin);
    char* getName = (char*)calloc(elementMaxSize, sizeof(char));
    if (!getName) {

        printf ("There is a bad with memory in %s func!\n", __FUNCTION__);
        exit   (MEMORY_ERR);

    }
    gets (getName);

    Way savePath = GetWay (mainTree, getName);

    for (size_t pathIndex = 0; savePath.path[pathIndex] != nullptr; pathIndex++)
        txSpeak ("\v%s", savePath.path[pathIndex]);

    printf ("\n");

    free (getName);

}

void ShowMenu (Tree* mainTree) {

    char chooseMode = '\0';
    txSpeak("\vChoose Akinator's mode\n"
           "0. Play\n"
           "1. Print tree\n"
           "2. Compare two\n"
           "3. Print info\n"
           "4. Play without saving\n");

    scanf("%c", &chooseMode);

    if (!isdigit(chooseMode)) {

        txSpeak("\vYou should enter the number!\n");
        exit (WRONG_ANSWER);

    }

    if ((chooseMode - '0') > 4) {

        txSpeak("\vYou should enter number in range 0 - 4\n");
        exit (WRONG_ANSWER);

    }

    int checkMode = chooseMode - '0';

    switch (checkMode) {

        case PLAY:
            txSpeak ("\vYou have to enter only \"yes\" or \"no\"!\n");
            TreatmentAnswer(PlayMode(mainTree->root), mainTree, 1);
            break;
        case DUMP:
            PrintTree(mainTree);
            break;
        case COMPARE:
            CompareMode(mainTree);
            break;
        case PRINT:
            PrintPerson(mainTree);
            break;
        case PLAY_NO_SAVE:
            txSpeak ("\vYou have to enter only \"yes\" or \"no\"!\n");
            TreatmentAnswer(PlayMode(mainTree->root), mainTree, 0);
            break;
        default:
            txSpeak ("\vUnexpected error!");
            exit(UNEXPECTED_ERR);

    }

}
