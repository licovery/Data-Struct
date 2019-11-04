#include "class_test.h"
#include <iostream>

using namespace std;

A::A(int a, int b): x(a), y(b)
{

}

A::A(const A &rhs)
{
    //如果定义了自己的拷贝构造函数，合成的就完全不会工作了。
    x = rhs.x;
}

void A::print()
{
    cout << "x: " << x << endl;
    cout << "y: " << y << endl;
}

ostream & operator<<(ostream &out, const ValueClass &s)
{
    out << s.ps << ": " << *(s.ps);
    return out;
}

void swap(ValueClass &v1, ValueClass &v2)
{
    auto temp = v1.ps;
    v1.ps = v2.ps;
    v2.ps = temp;
    cout << "ValueClass Swap" << endl;
}


ostream & operator<<(ostream &out, const PointerClass &p)
{
    out << "value: " << p.ps->str << " use: " << p.ps->use;
    return out;
}

