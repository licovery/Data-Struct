#include <memory>
#include "class.h"
#include "memory_management.h"

using namespace std;
using namespace lf_mem;

void allocatorTest()
{
    allocator<int> alloc;
    auto p = alloc.allocate(1);
    alloc.construct(p);
}

void newDeleteTest()
{
    auto p = new char;
    auto q = new char[5];
    delete p;
    delete[] q;
}

void classNewDeleteTest()
{
    //根据从Foo及其父类的命名空间开始找operator new
    auto p = new Foo;
    delete p;

    p = new Foo[5];//会额外分配空间指示数组长度
    delete[] p;

    p = ::new Foo;//强制使用全局的
}

void placementNewTest()
{
    auto p = new(1, 2) Foo(5);//这里构造会抛出异常
}
