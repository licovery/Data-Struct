#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <functional>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <vector>
#include <iostream>
#include <sstream>

namespace lf_tem
{

using namespace std;

//函数模板
template <typename T>
int mycompare(const T &v1, const T &v2)//可声明内联
{
    if (less<T>()(v1, v2))
    {
        return -1;
    }
    else if (less<T>()(v2, v1))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//非类型模板参数，一般用来处理数组，非类型模板参数的实参必须是常量表达式
template <unsigned N, unsigned M>
int mycompare(const char (&s1)[N], const char (&s2)[M])
{
    return strcmp(s1, s2);
}


typedef const char * ConstCharPointer;
// 特例化
template <>
int mycompare(const ConstCharPointer &p1, const ConstCharPointer &p2);


template <typename T>
class Blob
{
public:
    typedef typename vector<T>::size_type size_type;
    //need 'typename' before 'std::vector<_RealType>::size_type' because 'std::vector<_RealType>' is a dependent scope
    //vector<T>::size_type是一个依赖名，因为不知道T具体是什么，size_type也不知道是类型或者是静态数据或静态函数，需要加typename以说明
    Blob();
    Blob(initializer_list<T> il);
    template <typename It> Blob(It b, It e);//成员模板
    void push_back(const T &t);
    void pop_back();
    size_type size() const;

private:
    shared_ptr<vector<T>> data;
};

template <typename T>
Blob<T>::Blob(): data(make_shared<vector<T>>())
{

}

template <typename T>
Blob<T>::Blob(initializer_list<T> il): data(make_shared<vector<T>>(il))
{

}

template <typename T>
template <typename It>
Blob<T>::Blob(It b, It e):data(make_shared<vector<T>> (b, e))
{

}

template <typename T>
void Blob<T>::push_back(const T &t)
{
    data->push_back(t);
}

template <typename T>
void Blob<T>::pop_back()
{
    data->pop_back();
}

template <typename T>
typename Blob<T>::size_type Blob<T>::size() const
{
    return data->size();
}



//模板与友元

//前置声明
//类最好都加上前置声明，有些编译器对没有前置声明支持不好
class Pal;

template<typename T> 
class Print;

template<typename T> 
class Data;

template <typename T>
class Access;

template<typename T>//模板函数前置声明
bool operator==(const Data<T> &v1, const Data<T> &v2);

template<typename T>
class Data
{
public:
    Data(T v = T()): a(v)
    {
    }
    //非模板友元
    friend class Pal;//Pal类是友元，Pal类可以访问所有类型实例化的Data。如果是写了class,不用前置声明。

    //模板友元
    //同类型模板友元
    friend class Print<T>;//同类型实例化的Print类是友元
    friend bool operator==<T>(const Data<T> &v1, const Data<T> &v2);// 使用同类型Data参数的实例化的模板函数是友元，注意函数名后要加<T>
    //任意类型模板友元
    template<typename X> friend class Access;//Access是一个类模板，这个类模板实例化出的所有类都是友元，注意freind在template后面

    //c++11支持模板参数作为友元
    friend T;
private:
    T a;
};


class Pal
{
public:
    void fun()
    {
        Data<int> p; //任意类型实例的Data都可以访问，因为Pal是Data的友元
        cout << p.a << endl;
    }
private:
};


template <typename T> 
class Print//可以访问同类型实例化的Data的所有数据
{
public:
    void fun()
    {
        Data<T> d;//只有同类型的Data才能访问
        cout << d.a << endl;
    }
};

template<typename T>//模板实现
bool operator==(const Data<T> &v1, const Data<T> &v2)
{
    cout << v1.a << " " << v2.a << endl;
    cout << "==" << endl;
    return v1.a == v2.a;
}

template <typename T>
class Access
{
public:
    void fun()
    {
        Data<int> d;//任意类型都能访问
        cout << d.a << endl;
    }
};


//模板类的静态变量
template <typename T> 
class FooT
{
public:
    static int value()
    {
        return a;
    }
private:
    static int a;
};

//模板类的静态变量在.h定义也不会产生重定义的问题。但是普通类会发生
template <typename T>
int FooT<T>::a = 99;

 
// class Foo
// {
// public:
//     static int value()
//     {
//         return a;
//     }
// private:
//     static int a;
// };

// int Foo::a = 0;

//模板类型别名
typedef FooT<int> FooInt;

template <typename X, typename Y>
class TwoTemplate
{

};

template <typename T>
using SameTemplate = TwoTemplate<T, T>;

//固定某个参数
template <typename T>
using IntTemplate = TwoTemplate<int, T>;

template <typename T>
using TemplateInt = TwoTemplate<T, int>;

//默认模板参数

//函数模板
template <typename T, typename F = less<int>>
int compareWithFunc(const T &v1, const T &v2, F f = F())//最后一个默认参数可以这样理解，less<int> f = less<int> ()，函数的默认参数必须显示地使用=
{
    if (f(v1, v2)) return -1;
    if (f(v2, v1)) return 1;
    return 0;
}

//类模板
template <typename T = int>
class Number
{
public:
    Number(T a = 0): val(a)
    {

    }
private:
    T val;
};

//当无法确定返回值类型，需要使用decltype时，可以使用尾置返回类型。decltype可以推测出引用
template <typename It>
auto getValue(It it) -> decltype(*it)
{
    return *it;
}



template <typename T>
void f1(T)
{
    
}

template <typename T>
void f2(T &)
{
    
}

//引用折叠和右值引用参数
template <typename T>
void f3(T &&)
{
    
}


//参数转发 模板参数类型为右值引用，加上forword
template <typename F, typename T1, typename T2>
void flip(F f, T1 &&t1, T2 &&t2)
{
    f(std::forward<T2>(t2), std::forward<T1>(t1));
}

//模板与重载

//可匹配任意调用
template <typename T>
string debug_rep(const T &t)
{
    return "const T &t";
}

//只匹配指针类型参数，更特化
template <typename T>
string debug_rep(T *p)
{
    return "T *p";
}

//特例化 string debug_rep(const T &t)
template <>
string debug_rep(const int &i);

//特例化 string debug_rep(const T &t)
typedef const char * ConstCharPointer;
template <>
string debug_rep(const ConstCharPointer &p);

//普通非模板函数，最特化
string debug_rep(const string &s);


//变参模板
template <typename T, typename ... Args>
void foo(const T &t, const Args & ... args)
{
    cout << sizeof...(Args) << " ";
    cout << sizeof...(args) << endl;
}

//终止函数
template <typename T>
void print(ostream &out, const T &t)
{
    out << t << endl;
}

//递归处理变参
template <typename T, typename ... Args>
void print(ostream &out, const T &t, const Args & ... args)
{
    out << t << ", ";
    print(out, args...);
}

//转发参数包
//使用右值引用和farword一起可以实现转发，加上...实现变参转发
/*
template <typename ... T>
void fun(Args && ... args)
{
    work(std::forward<Args>(args)...);
}
*/

}


#endif