#include "class.h"
#include <iostream>

namespace lf_cla
{
using ::std::cout;
using ::std::endl;
//A
A::A(int a, int b): x(a), y(b)
{

}

A::A(const A &rhs)
{
    //如果定义了自己的拷贝构造函数，就没有合成的。
    x = rhs.x;
}

void A::print()
{
    cout << "x: " << x << endl;
    cout << "y: " << y << endl;
}


//ValueClass

ValueClass::ValueClass(string s): ps(new string(s))
{
    cout << "construct" << endl;
}

ValueClass::ValueClass(const ValueClass &rhs): ps (new string(*(rhs.ps)))
{
    cout << "copy construct" << endl;
}

ValueClass::~ValueClass()
{
    delete ps;
}

ValueClass & ValueClass::operator=(const ValueClass &rhs)
{
    //为了处理自赋值的情况，第一步要先要把右值拷到一个局部临时变量中
    string *temp = new string(*(rhs.ps));
    //或者直接做自赋值的特殊处理
    // if (this == &rhs)
    // {
    //     return *this;
    // }
    delete ps;
    ps = temp;
    return *this;
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

//PointerClass

PointerClass::PointerClass(const string &s): ps(new ShareString(s, 1))
{

}
PointerClass::PointerClass(const PointerClass &rhs): ps(rhs.ps)
{
    ++(ps->use);
}
PointerClass & PointerClass::operator=(const PointerClass &rhs)
{
    ++(rhs.ps->use);
    --(ps->use);
    if (ps->use == 0)
    {
        delete ps;
    }
    ps = rhs.ps;
    return *this;
}
PointerClass::~PointerClass()
{
    --(ps->use);
    if (ps->use == 0)
    {
        delete ps;
    }
}

ostream & operator<<(ostream &out, const PointerClass &p)
{
    out << "value: " << p.ps->str << " use: " << p.ps->use;
    return out;
}

}