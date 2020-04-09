#ifndef OPERATOR_OVERLOAD_H
#define OPERATOR_OVERLOAD_H

#include <functional>
#include <iostream>

namespace lf_ope
{

//member access operator overload

struct Value
{
    int foo, bar;
};

struct B
{
    B() { a = new Value; }
    Value *a;
    Value *operator->() const { return a; }
    Value &operator*() const { return *a; }
};

struct C
{
    B b;
    B operator->() const { return b; }
    Value &operator*() const { return *this->operator->(); }
};

struct D
{
    C c;
    C operator->() const { return c; }
    Value &operator*() const { return *this->operator->(); }
};

// 前置声明
class Book;
// 操作函数
bool operator<(const Book &b1, const Book &b2);

class Book
{
public:
    Book(const std::string &n, double p) : name(n), price(p) {}
    friend bool operator<(const Book &b1, const Book &b2);

private:
    std::string name;
    double price;
};

//()运算符重载

int add(int, int);

struct divide
{
    int operator()(int denominator, int divisor)
    {
        return denominator / divisor;
    }
};

// 类型运算符重载
// 一般不要定义过多的类型转换，因为这些类型转换是隐式的，可能会产生奇怪的逻辑错误
class SmallInt
{
public:
    SmallInt(int i)
    {
        if (i < 0 || i > 255)
        {
            value = 0;
        }
        else
        {
            value = i;
        }
        std::cout << "construct" << std::endl;
    }

    //隐式转换
    operator int() const
    {
        return value;
    }

    //显式转换，需要使用类型强制转换，但是有例外，在if, while, for, &&, || 等场景下会隐式地执行
    explicit operator bool() const
    {
        return value != 0;
    }

private:
    int value = 0;
};

class SmallInt2
{
public:
    SmallInt2(int i)
    {
    }
};

// void fun(int i);
void fun(SmallInt i);
void fun(SmallInt2 i);

} // namespace lf_ope

#endif