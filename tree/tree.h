//
// Created by 1 on 01.12.2020.
//

#ifndef MYAKINATOR_TREE_H
#define MYAKINATOR_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <cstring>

enum TreeErr {

    NoTreeError,
    NullPointerOnTree,
    NullPointerOnParent,
    BadTreeSize,
    RightNodeAlreadyExist,
    LeftNodeAlreadyExist,
    RootNodeAlreadyExist,
    UnexpectedSideToAdd

};

enum AddNodeSide {

    LeftSide,
    RightSide

};


typedef char* ElementTree;
static ElementTree poison = nullptr;

static TreeErr currentTreeErr = NoTreeError;

struct Node {

    Node* parentNode = nullptr;

    Node* leftNode  = nullptr;
    Node* rightNode = nullptr;

    ElementTree value;

};

struct Tree {

    Node* root = nullptr;

    size_t size = 0;

};

Node*   CreateNode              (ElementTree value, Node* parent, Node* left, Node* right);
Node*   CreateTreeRoot          (Tree* mainTree, ElementTree value);
Node*   AddNodeSide             (Tree* mainTree, Node* parent, ElementTree value, int WhichSide);
void    DeleteSubTree           (Tree* mainTree, Node* currentNode);
void    TreeDump                (Tree* mainTree);
size_t  GetSubTreeSize          (Node* currentNode);
TreeErr TreeVerificator         (Tree* mainTree);
void    PrintNodesIntoGraphviz  (Node* currentNode, FILE* output);
Node*   FindNode                (Tree* mainTree, Node* currentNode, char* valueToFind);

#endif //MYAKINATOR_TREE_H
