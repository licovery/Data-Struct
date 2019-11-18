#include "oop.h"
#include "virtual_func_table.h"

using namespace std;
using namespace lf_oop;
using namespace vtbl;


void virtualFunTest()
{
    // Document d("t", "c");
    // d.openFile();
    MyDoc d1("t1", "c1");
    d1.openFile();
}

void vptrVtblTest()
{
    cout << "===Base===" << endl;
    vtbl::objModelTest<vtbl::Base>();
    
    cout  << endl;

    cout << "===Derived===" << endl;
    vtbl::objModelTest<vtbl::Deriverd>();
}

void inheritanceCompositionCtorDtorTest()
{
    Derived d;
}

void oopStaticTest()
{
    //无论派生出多少个派生类，对于每个静态成员只有唯一的示例
    Qoute q("123-456", 23.5);
    cout << q.get() << endl;
    q.set(100);
    cout << q.get() << endl;
    Bulk_qoute bq("123-456", 23.5, 10, 0.8);
    cout << bq.get() << endl;
    bq.set(500);
    cout << q.get() << endl;
}

void oopVirtualFunTest()
{
    Qoute q;
    Bulk_qoute bq;
    Qoute *p = &q;
    p->debug();
    p = &bq;
    p->debug();
}

void scopeVirtualFunTest()
{
    B1 bobj;
    D1 d1obj;
    D2 d2obj;

    bobj.fcn();
    // d1obj.fcn(); 因为定义与父类虚函数同名的其他函数，由于作用域不同，不能成为重载，其实是隐藏了父类同名函数。
    d2obj.fcn();

    B1 *bp1 = &bobj, *bp2 = &d1obj, *bp3 = &d2obj;
    bp1->fcn();
    bp2->fcn();//这里没有被隐藏，因为调用的静态类型是B1类，里面确实有fcn这个函数。
    bp3->fcn();

    //总结，静态类型决定了哪些成员是可见的。
}