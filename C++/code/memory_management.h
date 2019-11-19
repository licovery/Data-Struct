#ifndef MEMORY_MANAGEMEMT_H
#define MEMORY_MANAGEMEMT_H

#include <iostream>

// new 和 delete 都是表达式，不可改变，表达式里面调用了operator new，可以重载函数改变表达式的最终行为

//重载全局 ::operator new delete
//不可以声明在某个命名空间内
//其他任意文件使用new delete的都会调用到这里来，影响非常大
#if 0
void * operator new(size_t size);
void * operator new[](size_t size);

void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;
#endif

namespace lf_mem
{

class Foo
{
public:
    Foo();
    Foo(int v);
    ~Foo();
    //隐式static，编译器自动
    void * operator new(size_t size);
    void * operator new[](size_t size);
    void operator delete(void *ptr);
    void operator delete[](void *ptr);

    //placement new
    // 其他形式的new和delete
    void * operator new(size_t size, int i, int j);
    // 这个delete一般不会被调用到，只有在对应的new(自定义参数一致)出来的对象，在构造函数中抛出异常的时候才会被调用
    // 也可以不定义，那么就是不处理那个对应构造对象的异常
    void operator delete(void *ptr, int i, int j);
private:
    int i;
};

}


#endif