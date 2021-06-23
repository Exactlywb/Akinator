

#include "akinator/akinator.h"

int main () {

    Tree mainTree = {};

    FILE* inputFile = fopen ("../data/firstBase.txt", "r");
    assert (inputFile);

    TreeParserFromFile (&mainTree, mainTree.root, inputFile);

    fclose (inputFile);

    while (true) {

        if ( ShowMenuReturnStopCicleOrNo (&mainTree) ) break;

    }

    DeleteSubTree (&mainTree, mainTree.root);

    return 0;

}