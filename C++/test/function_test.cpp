#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include "function.h"

using namespace std;
using namespace lf_fun;

void functionOverloadTest()
{
    // int i = 10;
    // myf(i);二义性调用，一般有了传值和传引用一般只定义一个。不然容易发生二义性调用
}

void functionNameSearch()
{
    void li();//内部作用域名字隐藏了外部作用域的名字。名字查找先于类型检查，只要找到了名字就停止查找外层作用域名字
    // li(5);编译失败
}



void bindFunctionObjectTest()
{
    vector<int> vec;
    count_if(vec.begin(), vec.end(), bind(greater<int> (), placeholders::_1, 1024));
    find_if(vec.begin(), vec.end(), bind(not_equal_to<int> (), placeholders::_1, 1024));
    transform(vec.begin(), vec.end(), vec.begin(), bind(multiplies<int> (), placeholders::_1, 2));
}