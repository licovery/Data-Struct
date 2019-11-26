#include <exception> // exception bad_exception
#include <stdexcept> //runtime_error  logic_error
#include <new> //bad_alloc
#include <typeinfo> //bad_cast bad_typeid
#include <iostream>
#include "exception.h"

using std::cerr;
using std::cout;
using std::endl;

namespace lf_exc
{

void throwTest(int index)
{
    switch(index)
    {
        case 0:
        {
            throw std::exception();
        }
        case 1:
        {
            throw std::bad_alloc();
        }
        case 2:
        {
            throw std::bad_cast();
        }
        case 3:
        {
            throw std::bad_typeid();
        }
        case 4:
        {
            throw std::bad_exception();
        }
        //有些异常类型只能默认初始化，其余异常类型都不能默认初始化，用字符串初始化
        case 5:
        {
            //子类还包括其他异常
            throw std::logic_error("logic_error");
        }
        case 6:
        {
            //子类还包括其他异常
            throw std::runtime_error("runtime_error");
        }
        case 7:
        {
            throw int(10);
        }
    }
}

void exceptionTest(int index)
{
    try
    {
       throwTest(index);
    }
    //catch写要按照一定的顺序,throw会挑选第一个匹配的catch，基类也可以匹配派生类
    catch(const std::bad_alloc &b_a)
    {
        cerr << b_a.what() << endl;
    }
    catch(const std::bad_cast &b_c)
    {
        cerr << b_c.what() << endl;
    }
    catch(const std::bad_typeid &b_t)
    {
        cerr << b_t.what() << endl;
    }
    catch(const std::bad_exception &b_e)
    {
        cerr << b_e.what() << endl;
    }
    catch(const std::logic_error &l_e)
    {
        cerr << l_e.what() << endl;
    }
    catch(const std::runtime_error &r_e)
    {
        cerr << r_e.what() << endl;
    }
    //最通用的基类要写在最后面
    catch(const std::exception &e)
    {
        cerr << e.what() << endl;
    }
    catch(...)
    {
        cerr << "unknow exception!" << endl;
        //重新抛出，把捕获到的异常重新抛出，异常类型不变。
        //空throw只能出现在catch作用域内，（catch调用的函数的内部也可以，一定被catch包围着）
        throw;
    }
}

//函数try语句块

//写在这个位置可以处理初始化过程和函数体
Foo::Foo(int i) try: a(i)
{

}
catch(const std::exception &e)
{
    cerr << e.what() << endl;
}

//try写在这里可以处理析构过程和函数体
Foo::~Foo() try
{

}
catch(const std::exception &e)
{
    cerr << e.what() << endl;
}


}