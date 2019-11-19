#include "namespace.h"

namespace lf_nam
{

class Foo {};

void f(Foo) {}

void g(int) {}

void g() {}

// void fun() {} //在命名空间内部的实现

} // lf_nam


// 未命名命名空间，不能跨越文件，其他文件不可见，用于替换C中static的声明
namespace
{
    int p = 0;
}


//从这里开始时全局作用域
void lf_nam::fun() {} //在外层的实现

//与类类型函数lf_nam::f(Foo)形成重载
void f() {}

//不重载也没有重复定义，在全局作用域直接隐藏lf_nam::g这个名字，隐藏的包含所有的重载函数
void g() {}


void argumentTypeNameSearchTest()
{
    lf_nam::Foo obj;
    // ADL特性 (Argument-Dependent name Lookup)
    //命名空间名字查找的特殊规则，当给函数传递一个类类型的实参（注意是实参）时，除了在常规作用域查找外，还会查找实参所属的命名空间。对引用和指针也是如此
    f(obj); //lf_nam::f(Foo)
    f(); // ::f()
    g(); //全局作用域 ::g()
    // g(4); 在全局作用域已经被隐藏
}

void useNameInNamespaceTest()
{
    // using声明的名字的作用域与using语句的作用域一致，也就是说在函数内部引入Foo这个名字，是一个局部的声明
    using lf_nam::Foo;//using声明，可能会重复声明
    Foo a;
    // using指示的作用域不是局部，会将lf_nam这个名字空间的名字注入到最近的非局部作用域
    // 将会影响到函数外面，一般不要用
    // using namespace lf_nam;

    p = 0;//未命名命名空间中的变量
}



namespace ns1
{
    int i = 10;
    int j = 10;
}

int i = 0;


void usingNamespacexxxErrorTest()
{
    using namespace ns1;
    // i = 5;二义性，因为ns1::i已经被注入到全局作用域和全局部变量冲突。但不访问就不会出错
    // 仍然可以通过::限定访问
    ns1::i = 5;
    ::i = 5;
    //只有在同一个作用域的相同名字才会冲突，内层作用域的同名会覆盖
    int j = 0;
}

//类的继承与命名空间

namespace father
{
    int baseNamespaceVar = 0;
    class Base
    {

    };

    void display(const Base &) {}
}

//继承并不能继承命名空间，Derived还是在全局作用域的
class Derived : public father::Base
{
    // int i = baseNamespaceVar;
};



void findFunctionInFatherNamespace()
{
    Derived d;
    display(d);//通过实参查找法则，会从实参，然后实参的基类一直往上找名字空间。
}
