#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <assert.h>
#include <cstring>
#include <ctype.h>
#include <math.h>
#include <sys/stat.h>

const size_t elementMaxSize = 40;
typedef char* ElementTree;
ElementTree poison = "\0";

#define MAX(a, b) a >= b ? a : b

struct Node {

    ElementTree value = poison;

    Node* left  = nullptr;
    Node* right = nullptr;

    Node* prev  = nullptr;

};

struct Tree {

    Node*  root = nullptr;
    size_t size = 0;

};

namespace treeSpace {

    //!TODO подумай, мб есть смысл убрать такое количество дефайнов и глобальный массив. Хз.

    enum TypeErr {

#define ERR_TMPL(name, message) name,

#include "ERR_TEMP.txt"

#undef ERR_TMPL

    };

    const char* ErrMessages[10][elementMaxSize] = {

#define ERR_TMPL(name, message) message,

#include "ERR_TEMP.txt"

#undef ERR_TMPL

    };

#define HANDLE_ERR(name)    printf ("%s\n", ErrMessages[name]); \
                                exit (name);

    Tree* Construct () {

        Tree* tempPointer = (Tree*)calloc(1, sizeof(Tree));

        return tempPointer;

    }

    Node* CreateTreeRoot (Tree* mainTree, ElementTree value) {

        if (mainTree->root != nullptr) {

            HANDLE_ERR(ROOT_EXIST)

        }

        if (mainTree == nullptr) {

            HANDLE_ERR(NONE_TREE)

        }

        Node* tempRoot = (Node*)calloc(1, sizeof(Node));

        tempRoot->value = value;

        mainTree->root = tempRoot;
        mainTree->size++;

        return tempRoot;

    }

    Node* AddLeft (Tree* mainTree, Node* parent, ElementTree value) {

        if (mainTree == nullptr) {

            HANDLE_ERR(NONE_TREE)
        }

        if (parent   == nullptr) {

            HANDLE_ERR(NULL_PARENT)

        }
        if (parent->left != nullptr) {

            HANDLE_ERR(LEFT_NODE_EXIST)

        }

        Node* tempNode = (Node*)calloc(1, sizeof(Node));
        tempNode->prev = parent;
        tempNode->value = value;

        parent->left = tempNode;

        mainTree->size++;

        return tempNode;

    }

    Node* AddRight (Tree* mainTree, Node* parent, ElementTree value) {

        if (mainTree == nullptr) {

            HANDLE_ERR(NONE_TREE)
        }

        if (parent == nullptr) {

            HANDLE_ERR(NULL_PARENT)

        }
        if (parent->right != nullptr) {

            HANDLE_ERR(RIGHT_NODE_EXIST)

        }

        Node* tempNode = (Node*)calloc(1, sizeof(Node));

        tempNode->prev  = parent;
        tempNode->value = value;

        parent->right = tempNode;

        mainTree->size++;

        return tempNode;

    }

    void DeleteTree (Tree* mainTree, Node* treeRoot) {

        if (treeRoot != nullptr) {

            DeleteTree (mainTree, treeRoot->left);
            DeleteTree (mainTree, treeRoot->right);

            treeRoot->value = poison;
            treeRoot->left  = treeRoot->right = treeRoot->prev = nullptr;

            mainTree->size--;

            free (treeRoot);

        }

    }

    void PrintTreeIntoConsole (Node* currentNode) {

        if (!currentNode) return;

        printf ("[%s", currentNode->value);

        PrintTreeIntoConsole (currentNode->left);
        PrintTreeIntoConsole (currentNode->right);

        printf ("]");

    }

    void PrintTreeIntoFile (Node* currentNode, FILE* output, size_t deep) {

        if (!currentNode) return;

        for (size_t i = 0; i < deep - 1; i++) fprintf (output, "\t");

        fprintf (output, "[\n");

        for (size_t i = 0; i < deep; i++) fprintf (output, "\t");

        if (currentNode->value[strlen(currentNode->value) - 1] == '?') {

            fprintf (output, "?%s\n", currentNode->value);

        } else {

            fprintf (output, "`%s`\n", currentNode->value);

        }

        PrintTreeIntoFile (currentNode->left, output, deep + 1);
        PrintTreeIntoFile (currentNode->right, output, deep + 1);

        for (size_t i = 0; i < deep - 1; i++) fprintf (output, "\t");
        fprintf (output, "]\n");

    }

    void PrintNodeIntoGraphviz (Node* currentNode, FILE* output) {

        if (currentNode == nullptr) return;

        static size_t index = 0;

        size_t currentIndex = index;

        fprintf (output, "\"box%zu\" [shape = \"record\", label = \"{%s}\"]\n", index, currentNode->value);
        index++;

        if (currentNode->left != nullptr) {

            size_t nextIndex = index;
            PrintNodeIntoGraphviz (currentNode->left, output);
            fprintf (output, "\"box%zu\" -> \"box%zu\"\n", currentIndex, nextIndex);

        }

        if (currentNode->right != nullptr) {

            size_t nextIndex = index;
            PrintNodeIntoGraphviz (currentNode->right, output);
            fprintf (output, "\"box%zu\" -> \"box%zu\"\n", currentIndex, nextIndex);

        }


    }

    void TreeDump (Tree* mainTree) {

        FILE* graph = fopen("tree.dot", "w");

        fprintf (graph, "digraph List {\n"
                        "node [shape=\"box\"]\n");

        PrintNodeIntoGraphviz (mainTree->root, graph);
        fprintf (graph, "}\n");

        fclose (graph);

        system ("dot -Tpng tree.dot -o tree.png");

    }

    void RecreateTree (Tree* mainTree, Node* parent, FILE* input) {

        char current_char = '\0';

        ElementTree value = poison;

        while ( (current_char = fgetc(input)) != EOF ) {

            if (current_char == '[') {

                fscanf (input, "%s]", &value);

                if (parent == nullptr) {

                    Node* newRoot = CreateTreeRoot (mainTree, value);
                    RecreateTree (mainTree, newRoot, input);
                    continue;

                }

                if (parent->left == nullptr) {

                    Node* newRoot = AddLeft (mainTree, parent, value);
                    RecreateTree (mainTree, newRoot, input);
                    continue;

                }

                if (parent->right == nullptr) {

                    Node* newRoot = AddRight (mainTree, parent, value);
                    RecreateTree (mainTree, newRoot, input);
                    continue;

                }

                HANDLE_ERR(INCORRECT_INPUT_TREE)

            }

            if (current_char == ']') return;

        }

    }

    Node* FindNode (Node* startSearch, ElementTree value) {

        if ( !strcmp(startSearch->value, value) ) {

            return startSearch;

        } else {

            if ( startSearch->left != nullptr ) {

                Node* leftSearch = FindNode (startSearch->left, value);

                if (leftSearch == nullptr) {

                    return FindNode (startSearch->right, value);

                }

                return leftSearch;

            }

            return nullptr;

        }

    }

#undef HANDLE_ERR

}

char* ReadTitle (FILE* input) {

    char* readResult = (char*)calloc(elementMaxSize, sizeof(char));

    char currentChar = '\0';

    while ( (currentChar = fgetc(input)) != EOF ) {

        if (currentChar == '?') {

            while ( (currentChar = fgetc(input) ) != '?' ) {

                readResult[strlen(readResult)] = currentChar;

            }

            readResult[strlen(readResult)] = '?';

            return readResult;

        }

        if (currentChar == '`') {

            while ( (currentChar = fgetc(input) ) != '`' ) {

                readResult[strlen(readResult)] = currentChar;

            }

            return readResult;

        }

    }

    return readResult;

}

void Parser (Tree* mainTree, Node* parent, FILE* input) {

    char current_char = '\0';

    while ( (current_char = fgetc(input)) != EOF ) {

        if (current_char == '[') {

            char* write = ReadTitle (input);

            if (parent == nullptr) {

                Node* newRoot = treeSpace::CreateTreeRoot (mainTree, write);
                Parser (mainTree, newRoot, input);
                continue;

            }

            if (parent->left == nullptr) {

                Node* newRoot = treeSpace::AddLeft (mainTree, parent, write);
                Parser (mainTree, newRoot, input);
                continue;

            }

            if (parent->right == nullptr) {

                Node* newRoot = treeSpace::AddRight (mainTree, parent, write);
                Parser (mainTree, newRoot, input);
                continue;

            }

        }

        if (current_char == ']') return;

    }

}

namespace akinator {

    struct Way {

        size_t pathLength = 0;
        ElementTree* path = nullptr;

    };

    const size_t AnswerMaxLength = 3;

    Node* PlayMode (Node* currentNode) {

        printf ("%s\n", currentNode->value);

        if (currentNode->left == nullptr) return currentNode;

        char* readAnswer = (char*)calloc(AnswerMaxLength, sizeof(char));

        scanf ("%s", readAnswer);

        if ( !strcmp(readAnswer, "yes") ) {

            free (readAnswer);
            return PlayMode (currentNode->left);

        } else if ( !strcmp(readAnswer, "no") ){

            free (readAnswer);
            return PlayMode (currentNode->right);

        } else {

            free (readAnswer);
            printf ("Syntax error! You have to enter \"yes\" or \"no\"!\n");
            return currentNode;

        }

    }

    void AskIsAnswerTrue (Node* answerNode, Tree* mainTree) {

        printf ("Is this answer correct?\n");

        char* readAnswer = (char*)calloc(AnswerMaxLength, sizeof(char));
        scanf ("%s", readAnswer);

        if (!strcmp(readAnswer, "yes")) {

            free (readAnswer);
            printf ("Ha-ha. Gotcha!\n");

        } else if ( !strcmp(readAnswer, "no") ){

            free (readAnswer);
            ElementTree newElementName = (char*)calloc(elementMaxSize, sizeof(char));
            printf ("Enter the real answer : \n");

            fflush (stdin);
            gets (newElementName);

            printf ("So what is different between %s and %s ?\n", answerNode->value, newElementName);

            fflush (stdin);
            ElementTree newQuestion = (char*)calloc(elementMaxSize, sizeof(char));
            gets (newQuestion);
            newQuestion[strlen(newQuestion)] = '?';

            char* oldAnswer = answerNode->value;
            answerNode->value = newQuestion;

            treeSpace::AddLeft(mainTree, answerNode, newElementName);
            treeSpace::AddRight(mainTree, answerNode, oldAnswer);

            FILE* outputTreeTest = fopen ("../input.txt", "w");

            treeSpace::PrintTreeIntoConsole (mainTree->root);
            treeSpace::PrintTreeIntoFile    (mainTree->root, outputTreeTest, 1);

            fclose (outputTreeTest);

        } else {

            printf ("Syntax error! You have to enter \"yes\" or \"no\"!\n");
            exit (1);

        }

    }

    void PrintTree (Tree* mainTree) {

        treeSpace::TreeDump (mainTree);
        system ("start tree.png");

    }

    akinator::Way GetWay (Tree* mainTree, ElementTree value) {

        Node* foundedNode = treeSpace::FindNode (mainTree->root, value);

        if (foundedNode == nullptr) {

            printf ("There is none person named like that!\n");
            exit (1);

        }

        size_t maxPathLength = 10;

        ElementTree* path = (ElementTree*)calloc(maxPathLength, sizeof(ElementTree));

        size_t indexPath = 0;

        while (foundedNode->prev != nullptr) {

            if (foundedNode == foundedNode->prev->left) {

                path[indexPath++]   = foundedNode->prev->value;
                path[indexPath - 1][strlen(path[indexPath - 1]) - 1] = ';';

            }

            foundedNode = foundedNode->prev;

        }

        Way result = {indexPath, path};

        return result;

    }

    void CompareMode (Tree* mainTree) {

        char* firstName  = (char*)calloc(elementMaxSize, sizeof(char));
        char* secondName = (char*)calloc(elementMaxSize, sizeof(char));

        printf ("Enter two objects you want to compare : \n");

        fflush (stdin);
        gets (firstName);

        fflush (stdin);
        gets (secondName);

        Way firstPath  = GetWay (mainTree, firstName);
        Way secondPath = GetWay (mainTree, secondName);

        printf ("Same properties : ");

        while ( !strcmp(firstPath.path[--firstPath.pathLength], secondPath.path[--secondPath.pathLength]) ) {

            printf ("%[^?], ", firstPath.path[firstPath.pathLength]);

            if (firstPath.pathLength * secondPath.pathLength == 0) break;

        }

        printf ("\nUnique properties : ");

        printf ("\n%s : ", firstName);
        while ( firstPath.pathLength != 0 ) printf ("%s, ", firstPath.path[--firstPath.pathLength]);

        printf ("\n%s : ", secondName);
        while ( secondPath.pathLength != 0 ) printf ("%s, ", secondPath.path[--firstPath.pathLength]);

        free (firstName);
        free (secondName);

    }

    void PrintPerson (Tree* mainTree) {

        printf ("Which person is your object to find?\n");

        fflush (stdin);
        char* getName = (char*)calloc(elementMaxSize, sizeof(char));
        gets (getName);

        Way path = GetWay (mainTree, getName);

        size_t pathIndex = 0;

        while (path.path[pathIndex] != nullptr) {

            printf ("%s", path.path[pathIndex++]);

        }

        printf ("\n");

        free (getName);

    }

}

void ShowMenu (Tree* mainTree) {

    char chooseMode = '\0';
    printf ("Choose Akinator's mode\n"
            "1. Play\n"
            "2. Print tree\n"
            "3. Compare two\n"
            "4. Print info\n"
            "5. Quit\n");

    scanf ("%c", &chooseMode);//string better

    if ( !isdigit (chooseMode) ) printf ("You should enter the number!\n");

    if ( (chooseMode - '0') > 5 ) printf ("You should enter number in range 1 - 5\n");

    int checkMode = chooseMode - '0';

    switch (checkMode) {

        case 1:
            akinator::AskIsAnswerTrue( akinator::PlayMode (mainTree->root), mainTree );
            break;
        case 2:
            akinator::PrintTree(mainTree);
            break;
        case 3:
            akinator::CompareMode(mainTree);
            break;
        case 4:
            akinator::PrintPerson(mainTree);
            break;
        case 5:
            return;
        default:
            printf ("Unexpected error!\n");
            exit (1);

    }

}

int main () {

    using namespace treeSpace;

    Tree* mainTree = Construct();

    FILE* new_input = fopen ("../input.txt", "rb");
    Parser (mainTree, mainTree->root, new_input);
    fclose (new_input);

    ShowMenu (mainTree);

    DeleteTree (mainTree, mainTree->root);

    return 0;
}
