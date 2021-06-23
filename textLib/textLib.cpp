//
// Created by 1 on 01.12.2020.
//

#include "textLib.h"

void ReadPhrase (char* str, size_t maxSize) {

    fflush (stdin);
    fgets (str, maxSize, stdin);
    str[strlen(str) - 1] = '\0';

}

char* ReadTitle (FILE* input) {

    char* readResult = (char*)calloc(MAX_TITLE_SIZE, sizeof(char));

    char currentChar = '\0';

    while ( (currentChar = fgetc(input)) != EOF ) {

        if (currentChar == '?') {

            while ( (currentChar = fgetc(input) ) != '?' ) {

                readResult[strlen(readResult)] = currentChar;

            }

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

void TreeParserFromFile (Tree* mainTree, Node* parent, FILE* input) {

    char current_char = '\0';

    while ( (current_char = fgetc(input)) != EOF ) {

        if (current_char == '[') {

            char* write = ReadTitle (input);

            if (parent == nullptr) {

                Node* newRoot = CreateTreeRoot (mainTree, write);
                TreeParserFromFile (mainTree, newRoot, input);
                continue;

            }

            if (parent->leftNode == nullptr) {

                Node* newRoot = AddNodeSide(mainTree, parent, write, 0);
                TreeParserFromFile (mainTree, newRoot, input);
                continue;

            }

            if (parent->rightNode == nullptr) {

                Node* newRoot = AddNodeSide(mainTree, parent, write, 1);
                TreeParserFromFile (mainTree, newRoot, input);
                continue;

            }

        }

        if (current_char == ']') return;

    }

}