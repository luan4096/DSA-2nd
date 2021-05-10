#include "BusSystem.h"

int main()
{
    // TESTCASE INPUT
    // === Example
    BusSystem* bs = new BusSystem();
	cout << bs->query("SQ 500") << endl;
	cout << bs->query("INS 50 50D1-23342 1234 5678") << endl;
	cout << bs->query("INS 50 50D1-23343 0 4000 5000") << endl;
	cout << bs->query("INS 50 50D1-23342 1 1200 1222") << endl;
	cout << bs->query("CS 50 1210") << endl;
    // === END: Example
    // END: TESTCASE INPUT

    return 0;
}