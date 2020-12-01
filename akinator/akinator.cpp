//
// Created by 1 on 18.11.2020.
//

#include "akinator.h"

#define MEM_ERR_CHECK(pointer) \
            if (!pointer) {\
                printf ("There is a bad with memory in %s func!\n", __FUNCTION__);\
                return MEMORY_ERR;                    \
            }

#define STR_EQ(str1, str2) !strcmp(str1, str2)

Node* PlayMode (Node* currentNode) {

    assert (currentNode);

    txSpeak ("\v%s\n", currentNode->value);

    if (currentNode->left == nullptr) return currentNode;

    char readAnswer[AnswerMaxLength] = "";
    scanf ("%s", readAnswer);

    if ( STR_EQ(readAnswer, "yes") ) {

        return PlayMode (currentNode->left);

    } else if ( STR_EQ(readAnswer, "no") ){

        return PlayMode (currentNode->right);

    } else {

        txSpeak("\vSyntax error! You had to enter \"yes\" or \"no\"!\n");
        return currentNode;

    }

}

AkinatorErrors TreatmentAnswer (Node* answerNode, Tree* mainTree, bool shouldSave) {

    assert (answerNode);
    assert (mainTree);

    txSpeak("\vIs this answer correct?\n");

    char readAnswer[AnswerMaxLength] = "";
    scanf ("%s", readAnswer);

    if (!strcmp(readAnswer, "yes")) { //stay define

        txSpeak("\vHa-ha. Gotcha!\n");

    } else if ( !strcmp(readAnswer, "no") ){

        if (!shouldSave) {

            txSpeak("\vEverybody make mistakes!\n");
            return NO_ERR;

        }

        ElementTree newElementName = (char*)calloc(elementMaxSize, sizeof(char));
        MEM_ERR_CHECK(newElementName)

        txSpeak("\vEnter the real answer : \n");
        ReadPhrase(&newElementName, elementMaxSize);

        txSpeak ("\vWhat is different between %s and %s?\n", newElementName, answerNode->value);
        ElementTree newQuestion = (char*)calloc(elementMaxSize + 1, sizeof(char));
        MEM_ERR_CHECK(newQuestion)

        ReadPhrase(&newQuestion, elementMaxSize);
        newQuestion[strlen(newQuestion)] = '?';

        char* oldAnswer = answerNode->value;
        answerNode->value = newQuestion;

        AddSide (mainTree, answerNode, newElementName, leftSide);
        AddSide (mainTree, answerNode, oldAnswer, rightSide);

        FILE* outputTree = fopen ("../input.txt", "w");
        assert (outputTree);

        size_t deepStart = 1;
        PrintTreeIntoFile    (mainTree->root, outputTree, deepStart);

        fclose (outputTree);

    } else {

        txSpeak ("\vSyntax error! You have to enter \"yes\" or \"no\"!\n");
        return WRONG_ANSWER;

    }

}

AkinatorErrors GetWay (Tree* mainTree, const ElementTree value, Way* currentWay) {

    assert (mainTree);

    Node* foundedNode = FindNode (mainTree->root, value);

    if (foundedNode == nullptr) {

        txSpeak ("\vThere is none element named like that!\n");
        currentWay->path = nullptr;
        currentWay->pathLength = 0;
        return NO_NAME;

    }

    size_t maxPathLength = mainTree->size;

    ElementTree* path = (ElementTree*)calloc(maxPathLength, sizeof(ElementTree));
    MEM_ERR_CHECK(path)

    size_t indexPath = 0;

    for (indexPath = 0; foundedNode->parent != nullptr;) {

        if (foundedNode == foundedNode->parent->left) {

            path[indexPath] = foundedNode->parent->value;
            path[indexPath][strlen(path[indexPath]) - 1] = ',';
            indexPath++;

        }

        foundedNode = foundedNode->parent;

    }

    if (indexPath == 0) {

        indexPath++;
        char* doNothingLabel = "nothing:)";
        path[0] = doNothingLabel;
        currentWay->path = path;
        currentWay->pathLength = indexPath;
        return NO_ERR;

    }

    path[indexPath - 1][strlen(path[indexPath - 1]) - 1] = '.';

    currentWay->pathLength = indexPath;
    currentWay->path = path;

    return NO_ERR;

}

AkinatorErrors CompareMode (Tree* mainTree) {

    assert (mainTree);

    char* firstName  = (char*)calloc(elementMaxSize, sizeof(char));
    MEM_ERR_CHECK(firstName)

    char* secondName = (char*)calloc(elementMaxSize, sizeof(char));
    MEM_ERR_CHECK(secondName)

    txSpeak ("\vEnter two objects you want to compare : \n");

    ReadPhrase (&firstName, elementMaxSize);
    ReadPhrase (&secondName, elementMaxSize);

    Way firstPath  = {};
    Way secondPath = {};

    AkinatorErrors firstPathErr = GetWay (mainTree, firstName, &firstPath);
    if ( firstPathErr != NO_ERR) return firstPathErr;

    AkinatorErrors secondPathErr = GetWay (mainTree, secondName, &secondPath);
    if ( secondPathErr != NO_ERR ) return secondPathErr;

    txSpeak ("\vSame properties : ");

    if ( STR_EQ(firstPath.path[firstPath.pathLength - 1], secondPath.path[secondPath.pathLength - 1]) ) {

        while ( firstPath.pathLength > 0 && secondPath.pathLength > 0 && STR_EQ(firstPath.path[firstPath.pathLength - 1], secondPath.path[secondPath.pathLength - 1]) )  {

            txSpeak ("\v%s", firstPath.path[--firstPath.pathLength]);
            secondPath.pathLength--;

            if (firstPath.pathLength * secondPath.pathLength == 0) break;

        }

    } else {

        txSpeak ("\vnothing:)");

    }

    printf ("\n");

    txSpeak ("\vUnique properties.\n");
    txSpeak ("\v%s : ", firstName);

    if (firstPath.pathLength > 0) {

        for (size_t pathIndex = 0; firstPath.path[pathIndex] != nullptr && pathIndex < firstPath.pathLength; pathIndex++) {

            txSpeak("\v%s", firstPath.path[pathIndex]);

        }

    } else txSpeak ("\vnothing:)");

    printf ("\n%s : ", secondName);

    if (secondPath.pathLength > 0) {

        for (size_t pathIndex = 0; secondPath.path[pathIndex] != nullptr && pathIndex < secondPath.pathLength; pathIndex++) {

            txSpeak("\v%s", secondPath.path[pathIndex]);

        }

    } else txSpeak ("\vnothing:)");

    printf("\n");

    free (firstName);
    free (secondName);

    return NO_ERR;

}

AkinatorErrors PrintPerson (Tree* mainTree) {

    assert (mainTree);

    txSpeak ("\vWhich person is your object to find?\n");

    fflush (stdin);
    char* getName = (char*)calloc(elementMaxSize, sizeof(char));
    MEM_ERR_CHECK(getName)
    ReadPhrase (&getName, elementMaxSize);

    Way savePath = {};

    AkinatorErrors savePathErr = GetWay (mainTree, getName, &savePath);
    if (savePathErr != NO_ERR) return savePathErr;

    for (size_t pathIndex = 0; savePath.path[pathIndex] != nullptr; pathIndex++)
        txSpeak ("\v%s", savePath.path[pathIndex]);

    printf ("\n");

    free (getName);

}

bool ShowMenu (Tree* mainTree) {

    FILE* new_input = fopen ("../input.txt", "rb");
    Parser (mainTree, mainTree->root, new_input);
    fclose (new_input);

    char chooseMode = '\0';

    txSpeak ("\vChoose Akinator's mode\n");

    txSpeak ("\v0. Play\n"
             "1. Show tree\n"
             "2. Compare two objects\n"
             "3. Print information about object\n"
             "4. Play without saving\n"
             "5. Exit\n");

    fflush (stdin);
    scanf("%c", &chooseMode);

    if (!isdigit(chooseMode)) {

        txSpeak("\vYou should enter the number!\n");
        return PlayContinue;

    }

    if ((chooseMode - '0') > 5) {

        txSpeak("\vYou should enter number in range 0 - 5\n");
        return PlayContinue;

    }

    int checkMode = chooseMode - '0';

    switch (checkMode) {

        case PLAY:
            txSpeak ("\vYou have to enter only \"yes\" or \"no\"!\n");
            TreatmentAnswer(PlayMode(mainTree->root), mainTree, shouldSave);
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
            TreatmentAnswer(PlayMode(mainTree->root), mainTree, noSave);
            break;
        case EXIT:
            return PlayEnd;
        default:
            txSpeak ("\vUnexpected error!");
            return PlayEnd;

    }

    return PlayContinue;

}

#undef MEM_ERR_CHECK
#undef STR_EQ
