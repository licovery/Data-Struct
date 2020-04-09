#ifndef VIRTUAL_FUNC_TABLE_H
#define VIRTUAL_FUNC_TABLE_H

// #define endl "\n" //vscode调试无法处理endl

#include <iostream>

namespace vtbl
{

using std::cout;
using std::endl;

typedef void (*Func)();
typedef void *PointerValue; //此类型和指针类型一样大

// 单继承
class Base
{
public:
    Base(int i1 = 1, int i2 = 2) : a(i1), b(i2) {}

    virtual void vfunc1() { cout << "Base::vfunc1" << endl; }
    virtual void vfunc2() { cout << "Base::vfunc2" << endl; }
    void func3(){};

private:
    int a;
    int b;
};

class Deriverd : public Base
{
public:
    Deriverd() : Base(3, 4) {}
    virtual void vfunc1() override { cout << "Deriverd::vfunc1" << endl; }
};

template <typename T>
void singleInheritobjModelTest()
{
    T obj;
    //打印对象占内存大小
    cout << "obj size: " << sizeof(obj) << endl;

    // 对象obj的地址
    PointerValue *bAddress = (PointerValue *)&obj;

    // 对象obj的vtptr的值
    PointerValue *vtptr = (PointerValue *)(*bAddress);
    cout << "vtptr: " << vtptr << endl;

    // 对象obj的两个成员变量的值（用这种方式可轻松突破private不能访问的限制）
    int *data = (int *)(bAddress + 1);
    int mem1 = *data;
    int mem2 = *(data + 1);
    cout << "int a: " << mem1 << endl
         << "int b: " << mem2 << endl;

    // 对象obj的虚函数的地址
    Func pFunc1 = (Func)vtptr[0];
    Func pFunc2 = (Func)vtptr[1];
    Func pFunc3 = (Func)vtptr[2];

    cout << "pvfunc1: " << (PointerValue)pFunc1 << endl;
    cout << "pvfunc2: " << (PointerValue)pFunc2 << endl;
    cout << "pvfunc3: " << pFunc3 << endl; //由于只有两个虚函数，虚函数表的第三项应该是空

    pFunc1();
    pFunc2();
}

// 多继承
/*  多继承的情况下，理论情况下，派生类会有多个虚函数表指针，分别指向一个虚函数表。
    覆盖了父类的虚函数，那就在对应的虚函数表上覆盖函数指针。
    对于派生类新增的虚函数，填入第一个（派生列表上第一个具有虚函数的类）虚函数表的末尾。
    
    但实际情况不一定会这样，编译器底层可能会做一些优化，一下代码在clang（https://godbolt.org/）中编译
*/

class B1
{
public:
    virtual void fooB1() { cout << "B1::fooB1" << endl; }
    virtual void barB1() { cout << "B1::barB1" << endl; }
};

/*
vtable for B1:
        .quad   0
        .quad   typeinfo for B1
        .quad   B1::fooB1()
        .quad   B1::barB1()
*/

class B2
{
public:
    virtual void fooB2() { cout << "B2::fooB2" << endl; }
    virtual void barB2() { cout << "B2::barB2" << endl; }
};

/*
vtable for B2:
        .quad   0
        .quad   typeinfo for B2
        .quad   B2::fooB2()
        .quad   B2::barB2()
*/

class D1 : public B1, public B2
{
    virtual void fooD1() { cout << "D1::fooD1" << endl; }
    virtual void barD1() { cout << "D1::barD1" << endl; }

    virtual void fooB1() override { cout << "D1::fooB1" << endl; }
    virtual void barB2() override { cout << "D1::barB2" << endl; }
};

/*
vtable for D1:
        .quad   0
        .quad   typeinfo for D1   第一个虚函数表(B1)
        .quad   D1::fooB1()
        .quad   B1::barB1()
        .quad   D1::fooD1() [complete object destructor]
        .quad   D1::barD1() [complete object destructor]
        .quad   D1::barB2()   <------------------------------
        .quad   -8                                          |
        .quad   typeinfo for D1   第二个虚函数表(B2)         |
        .quad   B2::fooB2()                                 | 
        .quad   non-virtual thunk to D1::barB2()  <----------   

    可以看到，编译器似乎把D1的函数都聚集在第一个虚函数表了（我猜是一种优化，方便找D1的虚函数）
    barB2是D1 override B2的序函数，应该在第二个虚函数表，
    但是只是在第二个表上建立一个链接，真正的实体还是在第一个表上。
    从效果上来说，和理论是不冲突的，但是实际的实现，编译器还是会优化
*/

void doubleInheritobjModelTest()
{
    D1 obj;
    cout << "obj size: " << sizeof(obj) << endl;
    PointerValue *vtptr1 = (PointerValue *)&obj;

    cout << "vtptr1: " << *vtptr1 << endl;
    Func *vtable1 = (Func *)*vtptr1;

    Func vtb1f0 = (Func)vtable1[0];
    Func vtb1f1 = (Func)vtable1[1];
    Func vtb1f2 = (Func)vtable1[2];
    Func vtb1f3 = (Func)vtable1[3];
    // Func vtb1f4 = (Func)vtable1[4];
    // Func vtb1f5 = (Func)vtable1[5];

    cout << (PointerValue)vtb1f0 << endl;
    cout << (PointerValue)vtb1f1 << endl;
    cout << (PointerValue)vtb1f2 << endl;
    cout << (PointerValue)vtb1f3 << endl;
    // cout << (PointerValue)vtb1f4 << endl;
    // cout << (PointerValue)vtb1f5 << endl;

    vtb1f0();
    vtb1f1();
    vtb1f2();
    vtb1f3();
    // vtb1f4();

    PointerValue *vtptr2 = vtptr1 + 1;
    cout << "vtptr2: " << *(vtptr2) << endl;
    Func *vtable2 = (Func *)*vtptr2;

    Func vtb2f0 = (Func)vtable2[0];
    Func vtb2f1 = (Func)vtable2[1];
    // Func vtb2f2 = (Func)vtable2[2];  null

    cout << (PointerValue)vtb2f0 << endl;
    cout << (PointerValue)vtb2f1 << endl;
    // cout << (PointerValue)vtb2f2 << endl;

    vtb2f0();
    vtb2f1();
}

} // namespace vtbl

#endif