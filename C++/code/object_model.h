#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H

#include <iostream>

namespace vtbl
{

using std::cout;
using std::endl;

typedef void (*Func)();
typedef void *PointerValue; //此类型和指针类型一样大

// 单继承
class Base //函数虚函数的类，都会有一个虚函数指针，指向虚函数表
{
public:
    virtual void vfunc1() { cout << "Base::vfunc1" << endl; }
    virtual void vfunc2() { cout << "Base::vfunc2" << endl; }
    void func3(){};

private:
    int a = 1;
};

class Deriverd : public Base
{
public:
    virtual void vfunc1() override { cout << "Deriverd::vfunc1" << endl; }
    virtual void vfunc3() { cout << "Deriverd::vfunc3" << endl; };

private:
    int b = 2;
};

void singleInheritobjModelTest();

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

private:
    int b1 = 1;
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

private:
    int b2 = 2;
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
public:
    virtual void fooD1() { cout << "D1::fooD1" << endl; }
    virtual void barD1() { cout << "D1::barD1" << endl; }

    virtual void fooB1() override { cout << "D1::fooB1" << endl; }
    virtual void barB2() override { cout << "D1::barB2" << endl; }

private:
    int b3 = 3;
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
        .quad   -16                                          |
        .quad   typeinfo for D1   第二个虚函数表(B2)         |
        .quad   B2::fooB2()                                 | 
        .quad   non-virtual thunk to D1::barB2()  <----------   

    可以看到，编译器似乎把D1的函数都聚集在第一个虚函数表了（我猜是一种优化，方便找D1的虚函数）
    barB2是D1 override B2的序函数，应该在第二个虚函数表，
    但是只是在第二个表上建立一个链接，真正的实体还是在第一个表上。
    从效果上来说，和理论是不冲突的，但是实际的实现，编译器还是会优化
*/

void doubleInheritobjModelTest();

// 虚拟继承，虚基类都在派生类的最靠后的位置
class VB
{
public:
    virtual void fooVB() { cout << "VB::fooVB" << endl; }
    virtual void barVB() { cout << "VB::barVB" << endl; }

private:
    int vb = 1;
};

/* 
VB的结构和普通类一样
void *vptr      (8)
int vb          (4) 
padding         (4)
---------------------
sizeof(VB)      (16)

vtable for VB:
        .quad   0
        .quad   typeinfo for VB
        .quad   VB::fooVB()
        .quad   VB::barVB()
*/

/*
虚拟继承
虚继承的子类，编译器为其生成一个虚函数指针（vptr）以及一张虚函数表（即使没有虚函数），不与父类共享
在g++中 虚函数与虚基类地址偏移值共享一个虚表（vtable）
虚表中会有一个虚基类地址偏移值 ，说明虚基类相对于虚指针的偏移值
虚表中第一个虚函数条目之前（内存地址自高向低方向）
依次填入了typeinfo（用于RTTI）、虚指针到整个对象开始处的偏移值、虚基类地址偏移值

然后是派生类的数据成员
最后的内存部分是虚基类成员
*/
class VD1 : public virtual VB
{
public:
    virtual void fooVD1() { cout << "VD1::fooD1" << endl; }
    virtual void barVD1() { cout << "VD1::barD1" << endl; }

    virtual void fooVB() override { cout << "VD1::fooVB" << endl; }

private:
    int vd1 = 2;
};

/*
void *vptr_VD1      (8)
int vd1             (4)
padding             (4)
void *vptr_VB       (8)
int vb              (4)
padding             (4)
-------------------------
sizeof(VD1)         (32)

vtable for VD1:
        .quad   16  //虚基类偏移
        .quad   0   //vptr的偏移
        .quad   typeinfo for VD1
        .quad   VD1::fooVD1() [complete object destructor]   (vptr_VD1)
        .quad   VD1::barVD1() [complete object destructor]
        .quad   VD1::fooVB() <----------------------|
        .quad   0                                   |
        .quad   -16                                 |
        .quad   -16                                 |
        .quad   typeinfo for VD1                    |        (vptr_VB)
        .quad   virtual thunk to VD1::fooVB() <-----|
        .quad   VB::barVB()
*/

class VD2 : public virtual VB
{
public:
    virtual void fooVD2() { cout << "VD2::fooD2" << endl; }
    virtual void barVD2() { cout << "VD2::barD2" << endl; }

    virtual void barVB() override { cout << "VD2::barVB" << endl; }

private:
    int vd2 = 3;
};

// 虚拟继承的菱形继承
class VD3 : public VD1, public VD2
{
    virtual void fooVD3() { cout << "VD3::fooD3" << endl; }
    virtual void barVD3() { cout << "VD3::barD3" << endl; }

private:
    int vd3 = 4;
};

/*

void *vptrVD1       (8)
int vd1             (4)
padding             (4)
-----------------------
void *vptrVD2       (8)
int vd2             (4)
-----------------------
int vd3             (4)
-----------------------
void *vptrVB        (8)
int vb              (4)
padding             (4)
-----------------------
sizeof(VD3)         (48)

vtable for VD3:
        .quad   32 //虚基类偏移
        .quad   0  // vptr的偏移
        // 从VD1继承过来的
        .quad   typeinfo for VD3                (vptrVD1)
        .quad   VD1::fooVD1() [complete object destructor]
        .quad   VD1::barVD1() [complete object destructor]
        .quad   VD1::fooVB()
        // 新增的虚函数
        .quad   VD3::fooVD3()
        .quad   VD3::barVD3()
        .quad   16
        .quad   -16
        // 从VD2继承过来的
        .quad   typeinfo for VD3                (vptrVD2)
        .quad   VD2::fooVD2() [base object destructor]
        .quad   VD2::barVD2() [base object destructor]
        .quad   VD2::barVB()
        .quad   -16
        .quad   -32
        .quad   -32
        //虚基类
        .quad   typeinfo for VD3                (vptrVD)
        .quad   virtual thunk to VD1::fooVB()
        .quad   virtual thunk to VD2::barVB()
*/

void vitrualInheritObjModelTest();

} // namespace vtbl

#endif