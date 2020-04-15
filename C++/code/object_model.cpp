#include "object_model.h"

namespace vtbl
{

void singleInheritobjModelTest()
{
    Deriverd obj;
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
    Func pFunc4 = (Func)vtptr[3];

    cout << "pvfunc1: " << (PointerValue)pFunc1 << endl;
    cout << "pvfunc2: " << (PointerValue)pFunc2 << endl;
    cout << "pvfunc3: " << (PointerValue)pFunc3 << endl;
    cout << "pvfunc4: " << (PointerValue)pFunc4 << endl; // nullptr

    pFunc1();
    pFunc2();
    pFunc3();
}

void doubleInheritobjModelTest()
{
    cout << "B1 obj size: " << sizeof(B1) << endl;
    cout << "B2 obj size: " << sizeof(B2) << endl;
    cout << "D1 obj size: " << sizeof(D1) << endl;

    D1 obj;
    D1 *pD1 = &obj;
    B1 *pB1 = &obj;
    B2 *pB2 = &obj; // pB2的值会改变

    cout << "B1 address " << (PointerValue)pB1 << endl;
    cout << "B2 address " << (PointerValue)pB2 << endl;
    cout << "D1 address " << (PointerValue)pD1 << endl;

    cout << "=====D1 memory object model=====" << endl;
    // B1
    PointerValue *vtptr1 = (PointerValue *)&obj;
    cout << vtptr1 << ": vtptr1=" << *vtptr1 << endl;
    int *p = (int *)(vtptr1 + 1);
    cout << p << ": a=" << *p << endl;
    // B2
    PointerValue *vtptr2 = (PointerValue *)((B1 *)&obj + 1);
    cout << vtptr2 << ": vtptr2=" << *vtptr2 << endl;
    p = (int *)(vtptr2 + 1);
    cout << p << ": b=" << *p << endl;
    // D1
    p++;
    cout << p << ": c=" << *p << endl;

    // vtable1
    cout << "=====vtable from B1=====" << endl;
    Func *vtable1 = (Func *)*vtptr1;

    Func vtb1f0 = (Func)vtable1[0];
    Func vtb1f1 = (Func)vtable1[1];
    Func vtb1f2 = (Func)vtable1[2];
    Func vtb1f3 = (Func)vtable1[3];

    cout << (PointerValue)vtb1f0 << ": ";
    vtb1f0();
    cout << (PointerValue)vtb1f1 << ": ";
    vtb1f1();
    cout << (PointerValue)vtb1f2 << ": ";
    vtb1f2();
    cout << (PointerValue)vtb1f3 << ": ";
    vtb1f3();

    // vtable2
    cout << "=====vtable from B2=====" << endl;
    Func *vtable2 = (Func *)*vtptr2;

    Func vtb2f0 = (Func)vtable2[0];
    Func vtb2f1 = (Func)vtable2[1];

    cout << (PointerValue)vtb2f0 << ": ";
    vtb2f0();
    cout << (PointerValue)vtb2f1 << ": ";
    vtb2f1();
}

void vitrualInheritObjModelTest()
{
    VB b;
    VD1 d1;
    VD3 d3;
    cout << "VB size: " << sizeof(VB) << endl;
    cout << "VD1 size: " << sizeof(VD1) << endl;
    cout << "VD3 size: " << sizeof(VD3) << endl;

    // VB
    cout << "=====object model VB=====" << endl;
    PointerValue *vptr = (PointerValue *)&b;
    Func *vtable = (Func *)*vptr;
    cout << (PointerValue)vptr << ": " << *vptr << endl;
    cout << (PointerValue)(vptr + 1) << ": " << *(int *)(vptr + 1) << endl;
    cout << "*****vtable of VB*****" << endl;
    for (int i = 0; i < 2; i++)
    {
        vtable[i]();
    }
    cout << endl;

    // VD1
    cout << "=====object model VD1=====" << endl;
    vptr = (PointerValue *)&d1;
    vtable = (Func *)*vptr;

    cout << (PointerValue)vptr << ": " << *vptr << endl;
    cout << (PointerValue)(vptr + 1) << ": " << *(int *)(vptr + 1) << endl;
    cout << "*****vtable of VD1(new)*****" << endl;
    for (int i = 0; i < 3; i++)
    {
        vtable[i]();
    }

    vptr = (PointerValue *)(VB *)&d1;
    vtable = (Func *)*vptr;
    cout << (PointerValue)vptr << ": " << *vptr << endl;
    cout << (PointerValue)(vptr + 1) << ": " << *(int *)(vptr + 1) << endl;
    cout << "*****vtable of VB(virtual Base)*****" << endl;

    cout << "virtual thunk to VD1::fooVB()" << endl; //vtable[0]() 会出错
    vtable[1]();
    cout << endl;

    // VD3
    cout << "=====object model VD3=====" << endl;
    vptr = (PointerValue *)&d3;
    vtable = (Func *)*vptr;

    cout << (PointerValue)vptr << ": " << *vptr << endl;
    cout << (PointerValue)(vptr + 1) << ": " << *(int *)(vptr + 1) << endl;
    cout << "*****vtable of VD1*****" << endl;
    for (int i = 0; i < 5; i++)
    {
        vtable[i]();
    }

    vptr = (PointerValue *)(VD2 *)&d3;
    vtable = (Func *)*vptr;

    cout << (PointerValue)vptr << ": " << *vptr << endl;
    cout << (PointerValue)(vptr + 1) << ": " << *(int *)(vptr + 1) << endl;
    cout << "*****vtable of VD2*****" << endl;
    for (int i = 0; i < 3; i++)
    {
        vtable[i]();
    }

    int *p = (int *)(vptr + 1) + 1;
    cout << (PointerValue)p << ": " << *p << endl;

    vptr = (PointerValue *)(VB *)&d1;
    vtable = (Func *)*vptr;
    cout << (PointerValue)vptr << ": " << *vptr << endl;
    cout << (PointerValue)(vptr + 1) << ": " << *(int *)(vptr + 1) << endl;
    cout << "*****vtable of VB(virtual Base)*****" << endl;

    cout << "virtual thunk to VD1::fooVB()" << endl; //vtable[0]() 会出错
    cout << "virtual thunk to VD2::barVB()" << endl; //vtable[1]() 会出错

    cout << endl;
}

} // namespace vtbl