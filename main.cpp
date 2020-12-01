#include "akinator/akinator.h"

int main () {

    //txPlaySound("../data/background.wav", SND_ASYNC);

    Tree mainTree = {};

    while (true) {

        if (ShowMenu(&mainTree)) break;

    }

    DeleteTree (&mainTree, mainTree.root);

    return 0;
}
