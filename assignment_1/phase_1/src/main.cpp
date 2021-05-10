#include "FragmentLinkedList.h"

using namespace std;

int main()
{
    // TESTCASE INPUT
    // === Example
    FragmentLinkedList<int> fList(4);
    for(int i = 0; i < 20 ; i++)
        fList.add(i, i * i);

    for(FragmentLinkedList<int>::Iterator it = fList.begin(); it != fList.end(); it++)
        cout << *it << " ";
    cout << endl;
    // === END: Example
    // END: TESTCASE INPUT

    return 0;
}