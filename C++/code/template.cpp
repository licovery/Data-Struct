#include "template.h"

namespace lf_tem
{


//显式实例化函数模板
template int mycompare(const int &, const int &);

//显式实例化类模板
template class Blob<int>;
template class Blob<double>;
template class Blob<string>;

//模板特例化定义
template <>
string debug_rep(const int &i)
{
    return "specialization const T & -> const int &";
}

//模板特例化定义
template <>
string debug_rep(const ConstCharPointer &p)
{
    return "specialization const T & -> const ConstCharPointer &";
}

//非模板函数定义
string debug_rep(const string &s)
{
    return "const string &s";
}


//模板特例化定义
template <>
int mycompare(const ConstCharPointer &p1, const ConstCharPointer &p2)
{
    return strcmp(p1, p2);
}

}