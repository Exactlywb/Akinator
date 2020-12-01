//
// Created by 1 on 18.11.2020.
//

#ifndef AKINATOR_TREE_H
#define AKINATOR_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <assert.h>
#include <cstring>
#include <ctype.h>
#include <math.h>
#include <sys/stat.h>

static const size_t elementMaxSize = 40;
typedef char* ElementTree;
static ElementTree poison = nullptr;

struct Node {

    ElementTree value = poison;

    Node* left  = nullptr;
    Node* right = nullptr;

    Node* parent  = nullptr;

};

struct Tree {

    Node*  root = nullptr;
    size_t size = 0;

};

enum whichSide {

    leftSide,
    rightSide

};

enum ErrTree {

    NO_ERR_TREE,
    NULL_POINTER_ON_TREE,
    BAD_SIZE_TREE

};

enum TypeErr {

    ROOT_EXIST,
    NONE_TREE,
    NULL_PARENT,
    LEFT_NODE_EXIST,
    RIGHT_NODE_EXIST,
    INCORRECT_INPUT_TREE

};

static const size_t ErrCount = 10;

static const char* ErrMessages[ErrCount][elementMaxSize] = {

        "Root already exists in this tree!",
        "Tree's pointer is NULL!",
        "Parent's pointer is NULL!",
        "Left node of parent node already exists!",
        "Right node of parent node already exists!",
        "Incorrect input! There couldn't be more than 2 branches!"

};

Node*   CreateTreeRoot        (Tree* mainTree, ElementTree value);
Node*   AddLeft               (Tree* mainTree, Node* parent, ElementTree value);
Node*   AddRight              (Tree* mainTree, Node* parent, ElementTree value);
Node*   AddSide               (Tree* mainTree, Node* parent, ElementTree value, bool side);
void    DeleteTree            (Tree* mainTree, Node* treeRoot);
void    PrintTreeIntoConsole  (Node* currentNode);
void    PrintTreeIntoFile     (Node* currentNode, FILE* output, size_t deep);
void    PrintNodeIntoGraphviz (Node* currentNode, FILE* output);
void    TreeDump              (Tree* mainTree);
void    CreateTreeFromFile    (Tree* mainTree, Node* parent, FILE* input);
Node*   FindNode              (Node* startSearch, ElementTree value);
void    PrintTree             (Tree* mainTree);
ErrTree VerificateTree        (Tree* mainTree);

#endif //AKINATOR_TREE_H
