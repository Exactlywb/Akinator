#include "akinator/akinator.h"

int main () {

    txPlaySound("../data/background.wav", SND_ASYNC);

    Tree* mainTree = Construct();

    FILE* new_input = fopen ("../input.txt", "rb");
    Parser (mainTree, mainTree->root, new_input);
    fclose (new_input);

    ShowMenu (mainTree);

    DeleteTree (mainTree, mainTree->root);

    return 0;
}
