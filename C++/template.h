#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <functional>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <vector>
#include <iostream>
using namespace std;

//函数模板
template <typename T>
inline int compare(const T &v1, const T &v2)//可声明内联
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
int compare(const char (&s1)[N], const char (&s2)[M])
{
    return strcmp(s1, s2);
}


template <typename T>
class Blob
{
public:
    typedef typename vector<T>::size_type size_type;
    //need 'typename' before 'std::vector<_RealType>::size_type' because 'std::vector<_RealType>' is a dependent scope
    //vector<T>::size_type是一个依赖名，因为不知道T具体是什么，size_type也不知道是类型或者是静态数据或静态函数，需要加typename以说明
    Blob();
    Blob(initializer_list<T> il);
    template <typename It> Blob(It b, It e);//模板成员
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

template<typename T> 
class Print;//前置声明

template<typename T> 
class Data;//前置声明

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

template<typename T>//模板实现
bool operator==(const Data<T> &v1, const Data<T> &v2)
{
    cout << v1.a << " " << v2.a << endl;
    cout << "==" << endl;
}

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

#endif