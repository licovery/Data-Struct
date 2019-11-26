#include "exception.h"
#include "test.h"
#include <iostream>


using namespace std;
using namespace lf_exc;


void tryCatchTest()
{
    for (int i = 0; i <= 7; i++)
    {
        exceptionTest(i);
    }
    cout << "complete !" << endl;
}

