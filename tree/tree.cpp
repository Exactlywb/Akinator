//
// Created by 1 on 18.11.2020.
//

#include "tree.h"

#define ASSERT_OK assert (VerificateTree (mainTree) == NO_ERR_TREE);

#define HANDLE_ERR(name)    printf ("%s\n", ErrMessages[name]); \
                            exit (name);

Node* CreateTreeRoot (Tree* mainTree, const ElementTree value) {

    ASSERT_OK

    if (mainTree == nullptr) {

        HANDLE_ERR(NONE_TREE)

    }

    if (mainTree->root != nullptr) {

        HANDLE_ERR(ROOT_EXIST)

    }

    mainTree->root = (Node*)calloc(1, sizeof(Node));
    mainTree->root->value = value;
    mainTree->size++;

    ASSERT_OK

    return mainTree->root;

}

//side 0 - means left, side 1 - means right

Node* AddSide (Tree* mainTree, Node* parent, const ElementTree value, bool side) {

    ASSERT_OK

    if (mainTree == nullptr) {

        HANDLE_ERR(NONE_TREE)

    }

    if (parent == nullptr) {

        HANDLE_ERR(NULL_PARENT)

    }

    Node* tempNode = (Node*)calloc(1, sizeof(Node));

    if (side) {

        if (parent->right != nullptr) {

            free (tempNode);
            HANDLE_ERR(RIGHT_NODE_EXIST)

        }

        parent->right = tempNode;

    } else {

        if (parent->left != nullptr) {

            free (tempNode);
            HANDLE_ERR(LEFT_NODE_EXIST)

        }

        parent->left = tempNode;

    }

    tempNode->parent = parent;
    tempNode->value = value;

    mainTree->size++;

    ASSERT_OK

    return tempNode;

}

/*Node* AddLeft (Tree* mainTree, Node* parent, const ElementTree value) {

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
    tempNode->parent = parent;
    tempNode->value = value;

    parent->left = tempNode;

    mainTree->size++;

    return tempNode;

}

Node* AddRight (Tree* mainTree, Node* parent, const ElementTree value) {

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

    tempNode->parent  = parent;
    tempNode->value = value;

    parent->right = tempNode;

    mainTree->size++;

    return tempNode;

}*/

void DeleteTree (Tree* mainTree, Node* treeRoot) {

    assert (mainTree);

    if (treeRoot != nullptr) {

        DeleteTree (mainTree, treeRoot->left);
        DeleteTree (mainTree, treeRoot->right);

        treeRoot->value = poison;
        treeRoot->left  = treeRoot->right = treeRoot->parent = nullptr;

        mainTree->size--;

        free (treeRoot);

    }

}

void PrintTreeIntoConsole (Node* rootNode) {

    if (rootNode == nullptr) return;

    printf ("[%s", rootNode->value);

    PrintTreeIntoConsole (rootNode->left);
    PrintTreeIntoConsole (rootNode->right);

    printf ("]");

}

void PrintTreeIntoFile (Node* currentNode, FILE* output, const size_t deep) {

    assert (output);

    if (currentNode == nullptr) return;

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

    for (size_t i = 0; i < deep - 1; i++)
        fprintf (output, "\t");

    fprintf (output, "]\n");

}

void PrintNodeIntoGraphviz (Node* currentNode, FILE* output) {

    assert (output);
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

    assert (mainTree);

    FILE* graph = fopen("tree.dot", "w");
    assert (graph);

    fprintf (graph, "digraph List {\n"
                    "node [shape=\"circle\"]\n");

    PrintNodeIntoGraphviz (mainTree->root, graph);
    fprintf (graph, "}\n");

    fclose (graph);

    system ("dot -Tpng tree.dot -o tree.png");

}

void CreateTreeFromFile (Tree* mainTree, Node* parent, FILE* input) {//покажи Искану

    assert (mainTree);
    assert (input);

    char current_char = '\0';
    ElementTree value = poison;

    while ( (current_char = fgetc(input)) != EOF ) {

        if (current_char == '[') {

            fscanf (input, "%s]", &value);

            if (parent == nullptr) {

                Node* newRoot = CreateTreeRoot (mainTree, value);
                CreateTreeFromFile(mainTree, newRoot, input);
                continue;

            }

            if (parent->left == nullptr) {

                Node* newRoot = AddSide (mainTree, parent, value, 0);
                CreateTreeFromFile(mainTree, newRoot, input);
                continue;

            }

            if (parent->right == nullptr) {

                Node* newRoot = AddSide (mainTree, parent, value, 1);
                CreateTreeFromFile(mainTree, newRoot, input);
                continue;

            }

            HANDLE_ERR(INCORRECT_INPUT_TREE)

        }

        if (current_char == ']') return;

    }

    ASSERT_OK

}

Node* FindNode (Node* startSearch, const ElementTree value) {

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

void PrintTree (Tree* mainTree) {

    ASSERT_OK

    TreeDump (mainTree);
    system ("start tree.png");

    ASSERT_OK

}

size_t Tree_BFS (Tree* mainTree, Node* currentNode) {

    assert (mainTree);

    if (currentNode == nullptr) return 0;

    return Tree_BFS (mainTree, currentNode->left) + Tree_BFS (mainTree, currentNode->right) + 1;

}

ErrTree VerificateTree (Tree* mainTree) {

    if (mainTree == nullptr) return NULL_POINTER_ON_TREE;

    size_t currentRealSize = Tree_BFS (mainTree, mainTree->root);

    if (mainTree->size != currentRealSize) return BAD_SIZE_TREE;

    return NO_ERR_TREE;

}

#undef ASSERT_OK
#undef HANDLE_ERR
