#include "operator_overloading.h"


bool operator<(const Book &b1, const Book &b2)
{
    return b1.price < b2.price;
}

int add(int a, int b)
{
    return a + b;
}

// void fun(int i)
// {
//     std::cout << "para int" << std::endl;
// }

void fun(SmallInt i)
{
    std::cout << "para SmallInt" << std::endl;
}

void fun(SmallInt2 i)
{
    std::cout << "para SmallInt2" << std::endl;
}