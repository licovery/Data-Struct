#include <memory>
#include "class.h"

using namespace std;

void allocatorTest()
{
    allocator<int> alloc;
    auto p = alloc.allocate(1);
    alloc.construct(p);
}

void deleteTest()
{
    auto p = new char;
    auto q = new char[1];
    delete p;
    delete[] q;
    auto p1 = new char[1];
    auto q2 = new char[10];
    delete p1;//不报错，但是会产生未定义的行为
    delete q2;//不报错，但是会产生未定义的行为
}