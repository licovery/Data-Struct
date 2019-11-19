#include "namespace.h"

namespace lf_nam
{

void f(Foo)
{

}

void g(int)
{

}

void g()
{

}

}


void f()//与类类型函数lf_nam::f(Foo)形成重载
{

}

void g()//不重载也没有重复定义，在全局作用域直接隐藏lf_nam::g这个名字，隐藏的包含所有的重载函数
{

}


void argumentTypeNameSearchTest()
{
    lf_nam::Foo obj;
    //命名空间名字查找的特殊规则，当给函数传递一个类类型的实参（注意是实参）时，除了在常规作用域查找外，还会查找实参所属的命名空间。对引用和指针也是如此
    f(obj); //lf_nam::f(Foo)
    f(); // ::f()
    g(); //全局作用域 ::g()
    // g(4); 在全局作用域已经被隐藏
}
