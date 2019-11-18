#include "operator_overloading.h"
#include <functional>

using namespace std;
using namespace lf_ope;

void callableObjTest()
{
    auto mod = [] (int i, int j) {return i % j;};
    function<int(int , int)> f1 = add;
    decltype(f1) f2 = mod;
    decltype(f1) f3 = divide ();//这里的括号代表一个匿名对象
    //由于function的函数不是explicit，所以这里可以=,
    //等价于function<int(int , int)> f1(add)
}

void stlFunctionObjectTest()
{
    less<Book> f;
    Book b1("li", 10.0), b2("hello", 19.0);
    cout << f(b1, b2) << endl;
}

void typeConversionOperatorOverloadTest()
{
    SmallInt obj = 100;                 //构造函数定义的隐式转换
    int i = obj;                        //运算符重载的隐式转换
    bool b = static_cast<bool>(obj);    //运算符重载的显式转换
    // fun(10); //二义性调用
}

void memberAccessOperatorOverloadTest()
{
    D d;
    d->foo = 999;
    cout << (*d).foo << endl;
    (*d).foo == 10000;
    cout  << d->foo << endl;
}