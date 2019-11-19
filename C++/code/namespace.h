#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <iostream>

namespace lf_nam
{

class Foo;

//参数是类类型，可以突破名字空间
void f(Foo);

//参数没有类类型，名字会被外层隐藏
void g(int);
void g();

void fun();//函数的定义可以在命名空间内，也可以外层空间（例如全局）

    //嵌套命名空间，lf_nam::inner::Foo 不同于 lf_nam::Foo
    namespace inner
    {
        class Foo {};
    }

    inline namespace sameLevel
    {
        // 这里面的名字可以直接被外层所见
        // 不知道有什么作用，感觉就是存在多个版本的代码，定义n个命名空间中，更换版本的时候就把inline放到想用的版本前
    }

}


//命名空间别名
namespace ns = lf_nam;



#endif