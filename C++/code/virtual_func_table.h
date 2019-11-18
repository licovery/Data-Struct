#ifndef VIRTUAL_FUNC_TABLE_H
#define VIRTUAL_FUNC_TABLE_H

// #define endl "\n" //vscode调试无法处理endl

#include <iostream>
using namespace std;

namespace vtbl
{

class Base
{
public:
    Base(int i1 = 1, int i2 = 2) : a(i1), b(i2) {}

    virtual void vfunc1() {}
    virtual void vfunc2() {}
    void func3()  {};

private:
    int a;
    int b;
};

class Deriverd : public Base
{
public:
    Deriverd(): Base(3, 4) {}
    virtual void vfunc1() override {}
};

template<typename T>
void objModelTest()
{
    typedef void * PointerValue;//此类型和指针类型一样大
    T obj;
    //打印对象占内存大小
    cout << "obj size: " << sizeof(obj) << endl;

    // 对象obj的地址
    PointerValue *bAddress = (PointerValue*)&obj;    

    // 对象obj的vtptr的值
    PointerValue *vtptr = (PointerValue *)(*bAddress);
    cout << "vtptr: " << vtptr << endl;

    // 对象obj的两个成员变量的值（用这种方式可轻松突破private不能访问的限制）
    int *data = (int *)(bAddress + 1);
    int mem1 = *data;
    int mem2 = *(data + 1);
    cout << "int a: " << mem1 << endl <<  "int b: " << mem2 << endl;


    // 对象obj的第一个虚函数的地址
    PointerValue *pFunc1 = (PointerValue *)*(vtptr + 0);
    PointerValue *pFunc2 = (PointerValue *)*(vtptr + 1);
    PointerValue *pFunc3 = (PointerValue *)*(vtptr + 2);
    
    cout << "pvfunc1: " << pFunc1 << endl;
    cout << "pvfunc2: " << pFunc2 << endl;
    cout << "pvfunc3: " << pFunc3 << endl;//由于只有两个虚函数，虚函数表的第三项应该是空
}

}

#endif