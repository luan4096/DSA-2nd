#include "BKUTree.h"

int main()
{
    // TESTCASE INPUT
    // === Example
    int set[10] = { 44, 25, 16, 30, 12, 50, 34, 2, 9, 31 };

    BKUTree<int, int>* tree = new BKUTree<int, int>();
    for (int i = 0; i < 10; i++)
        tree->add(set[i], set[i] * set[i]);
    tree->remove(set[2]);
    tree->printTree();
    // === END: Example
    // END: TESTCASE INPUT
    return 0;
}