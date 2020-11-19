//
// Created by 1 on 18.11.2020.
//

#include "minOnegin.h"

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

                Node* newRoot = CreateTreeRoot (mainTree, write);
                Parser (mainTree, newRoot, input);
                continue;

            }

            if (parent->left == nullptr) {

                Node* newRoot = AddLeft (mainTree, parent, write);
                Parser (mainTree, newRoot, input);
                continue;

            }

            if (parent->right == nullptr) {

                Node* newRoot = AddRight (mainTree, parent, write);
                Parser (mainTree, newRoot, input);
                continue;

            }

        }

        if (current_char == ']') return;

    }

}
