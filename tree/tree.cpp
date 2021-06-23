//
// Created by 1 on 01.12.2020.
//

#include "tree.h"

#define TREE_ASSERT if ( TreeVerificator (mainTree) != NoTreeError ) { \
                                                                       \
                        printf ("TREE_ASSERT on line %d\n", __LINE__);  \
                                                                       \
                    }

Node* CreateNode (ElementTree value, Node* parent, Node* left, Node* right) {

    Node* newNode = (Node*)calloc (1, sizeof(Node));

    newNode->value = value;

    newNode->parentNode = parent;

    newNode->leftNode  = left;
    newNode->rightNode = right;

    return newNode;

}
Node* CreateTreeRoot (Tree* mainTree, ElementTree value) {

    TREE_ASSERT

    if (mainTree->root != nullptr) {

        printf ("Error %d in function %s : root in tree is already exist!\n", RootNodeAlreadyExist, __FUNCTION__);
        return nullptr;

    }

    Node* newRoot = CreateNode (value, nullptr, nullptr, nullptr);
    mainTree->root = newRoot;

    mainTree->size++;

    TREE_ASSERT

    return newRoot;

}

Node* FindNode (Tree* mainTree, Node* currentNode, ElementTree value) {

    TREE_ASSERT
    if (currentNode == nullptr) return nullptr;

    if ( !strcmp(currentNode->value, value) ) return currentNode;

    Node* foundNode = FindNode (mainTree, currentNode->leftNode, value);

    if (foundNode == nullptr) foundNode = FindNode (mainTree, currentNode->rightNode, value);

    TREE_ASSERT

    return foundNode;

}

Node* AddNodeSide (Tree* mainTree, Node* parent, ElementTree value, int WhichSide) {

    TREE_ASSERT

    if (parent == nullptr) {

        printf ("Error %d in function %s : nullpointer on parent!\n", NullPointerOnParent, __FUNCTION__);
        return nullptr;

    }

    Node* pointerToReturn = nullptr;

    if (WhichSide == LeftSide) {

        if (parent->leftNode != nullptr) {

            printf ("Error %d in function %s : left node already exists!\n", LeftNodeAlreadyExist, __FUNCTION__);
            return pointerToReturn;

        }

        pointerToReturn = CreateNode (value, parent, nullptr, nullptr);

        parent->leftNode = pointerToReturn;

    } else if (WhichSide == RightSide) {

        if (parent->rightNode != nullptr) {

            printf ("Error %d in function %s : right node already exists!\n", RightNodeAlreadyExist, __FUNCTION__);
            return pointerToReturn;

        }

        pointerToReturn = CreateNode (value, parent, nullptr, nullptr);

        parent->rightNode = pointerToReturn;

    } else {

        printf ("Error %d : unexpected side type in function %s!\n", UnexpectedSideToAdd, __FUNCTION__);
        return nullptr;

    }

    mainTree->size++;

    TREE_ASSERT

    return pointerToReturn;

}

void DeleteSubTree (Tree* mainTree, Node* currentNode) {

    if (currentNode == nullptr) return;

    DeleteSubTree (mainTree, currentNode->leftNode);
    DeleteSubTree (mainTree, currentNode->rightNode);

    currentNode->value = poison;
    currentNode->leftNode  = currentNode->rightNode = currentNode->parentNode = nullptr;

    mainTree->size--;

    free (currentNode);

}

void PrintNodesIntoGraphviz (Node* currentNode, FILE* output) {

    assert (output);
    if (currentNode == nullptr) return;

    static size_t index = 0;

    size_t currentIndex = index;

    if (currentNode->leftNode != nullptr) {

        fprintf (output, "\"box%zu\" [shape = \"record\", label = \"{%s?}\"]\n", index, currentNode->value);

    } else {

        fprintf (output, "\"box%zu\" [shape = \"record\", label = \"{%s}\"]\n", index, currentNode->value);

    }

    index++;

    if (currentNode->leftNode != nullptr) {

        size_t nextIndex = index;
        PrintNodesIntoGraphviz (currentNode->leftNode, output);
        fprintf (output, "\"box%zu\" -> \"box%zu\"\n", currentIndex, nextIndex);

    }

    if (currentNode->rightNode != nullptr) {

        size_t nextIndex = index;
        PrintNodesIntoGraphviz (currentNode->rightNode, output);
        fprintf (output, "\"box%zu\" -> \"box%zu\"\n", currentIndex, nextIndex);

    }

}

void  TreeDump (Tree* mainTree) {

    TREE_ASSERT

    FILE* graph = fopen ("treeDump.dot", "w");
    assert (graph);

    fprintf (graph, "digraph List {\n"
                    "node [shape=\"circle\"]\n");

    PrintNodesIntoGraphviz (mainTree->root, graph);

    fprintf (graph, "}\n");

    fclose (graph);

    system ("dot -Tpng treeDump.dot -o treeDump.png");

    TREE_ASSERT

}

size_t  GetSubTreeSize (Node* currentNode) {

    if (currentNode == nullptr)
        return 0;

    return GetSubTreeSize (currentNode->leftNode) + GetSubTreeSize(currentNode->rightNode) + 1;

}

TreeErr TreeVerificator (Tree* mainTree) {

    if (mainTree == nullptr) {

        printf ("Null pointer on tree! Error %d\n", NullPointerOnTree);
        currentTreeErr = NullPointerOnTree;
        return NullPointerOnTree;

    }

    size_t realTreeSize = GetSubTreeSize (mainTree->root);
    if (realTreeSize != mainTree->size) {

        printf ("Wrong size of tree! Error %d : In struct tree size is %d but real size is %d\n", BadTreeSize, mainTree->size, realTreeSize);
        currentTreeErr = BadTreeSize;
        return BadTreeSize;

    }

    return NoTreeError;

}