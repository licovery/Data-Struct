#include "memory_management.h"
#include <cstdlib>
#include <stdexcept>

using std::cout;
using std::endl;
using std::runtime_error;

#if 0
void * operator new(size_t size)
{
    cout << "lf global new" << endl;
    return malloc(size);
}

void * operator new[](size_t size)
{
    cout << "lf global new[]" << endl;
    return malloc(size);
}


void operator delete(void *ptr) noexcept
{
    cout << "lf global delete" << endl;
    free(ptr);
}

void operator delete[](void *ptr) noexcept
{
    cout << "lf global delete[]" << endl;
    free(ptr);
}
#endif


namespace lf_mem
{

Foo::Foo()
{
    cout << "Foo ctor! this: " << this << " size: " << sizeof(Foo) << endl;
}

Foo::Foo(int v): i(v)
{
    //构造出现异常
    throw runtime_error("Foo ctor error!");
}

Foo::~Foo()
{
    cout << "Foo dtor! this: " << this << endl;
}

void * Foo::operator new(size_t size)
{
    cout << "Foo new, size: " << size << endl;
    return malloc(size);
}

void * Foo::operator new[](size_t size)//会在sizeof(Foo) * N的基础上加上8字节（64位系统）
{
    cout << "Foo new[], size: " << size << endl;
    return malloc(size);
}

void Foo::operator delete(void *ptr)
{
    cout << "Foo delete, ptr: " << ptr << endl;
    free(ptr);
}

void Foo::operator delete[](void *ptr)
{
    cout << "Foo delete[], ptr: " << ptr << endl;
    free(ptr);
}

void * Foo::operator new(size_t size, int i, int j)
{
    cout << "Foo new(size_t, int, int), size: " << size << endl;
    return malloc(size);
}

void Foo::operator delete(void *ptr, int i, int j)
{
    cout << "Foo delete(void *, int, int), ptr: " << ptr << endl;
    free(ptr);
}

}