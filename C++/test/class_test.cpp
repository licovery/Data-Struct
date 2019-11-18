#include "class.h"

using namespace lf_cla;
using namespace std;

void valueClassTest()
{
    ValueClass a("hello");
    cout << a << endl;
    a = a;
    cout << a << endl;
    ValueClass b(a);
    cout << b << endl;
    ValueClass c("world");
    cout << c << endl;
    b = c;
    cout << b << endl;
    swap(b, c);
    cout << b << endl << c << endl;
}

void pointerClassTest()
{
    PointerClass a("hello");
    cout << a << endl;
    a = a;
    cout << a << endl;
    PointerClass b(a);
    cout << b << endl;
    PointerClass c("world");
    cout << c << endl;
    b = c;
    cout << b << endl;
}


// void NewA()
// {
//     A *p = new A;
//     //分为以下三步
//     void *mem = operator new(sizeof(A));
//     p = static_cast<A *>(mem);
//     p->A::A(1, 2);
// }

void sizeofTest()
{
    cout << sizeof(SizeA) << endl;
    cout << sizeof(SizeB) << endl;
}

void synthesizeCopyConstructTest()
{
    //如果定义了自己的拷贝构造函数，合成的就完全不会工作了。
    A a;
    A b(100, 199);
    A c(b);
    a.print();
    b.print();
    c.print();
}