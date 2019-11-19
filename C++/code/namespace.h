#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <iostream>

namespace lf_nam
{
class Foo
{

};

//参数是类类型，可以突破名字空间
void f(Foo);


//参数没有类类型，名字会被外层隐藏
void g(int);
void g();

using std::cout;
using std::endl;



}

#endif