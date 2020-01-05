#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <iostream>
#include <functional>


namespace lf_cla
{

class A
{
    // 一般友元最后前置声明，由于hash和equal都在functional里面声明，所以已经前置声明
    friend class std::hash<A>;
    friend class std::equal_to<A>;
public:
    A() = default;
    A(int a, int b);
    A(const A &rhs);
    void print();
private:
    int x = 1;
    int y = 2;
};

}

//类模板特例化
namespace std
{

using ::lf_cla::A;

template <>
struct hash<A>
{
    size_t operator()(const A &a) const
    {
        return hash<int>()(a.x) ^ hash<int>()(a.y);
    }
};

template <>
struct equal_to<A>
{
    bool operator()(const A &a1, const A &a2) const
    {
        return a1.x == a2.x && a1.y == a2.y;
    }
};

}

namespace lf_cla
{
    using ::std::ostream;
    using ::std::string;
//行为像值的类的
class ValueClass
{
    friend ostream & operator<<(ostream &out, const ValueClass &s);
    friend void swap(ValueClass &v1, ValueClass &v2);
public:
    ValueClass(string s = "");
    ValueClass(const ValueClass &rhs);
    ValueClass & operator=(const ValueClass &rhs);
    ~ValueClass();
private:
    string *ps;
};

ostream & operator<<(ostream &out, const ValueClass &s);
void swap(ValueClass &v1, ValueClass &v2);

//行为像指针的类
class PointerClass
{
    struct ShareString //嵌套类
    {
        ShareString(const string &s = "", size_t u = 0): str(s), use(u) {}
        string str;
        size_t use;//引用计数应该和资源本身紧密结合
    };
    friend ostream & operator<<(ostream &out, const PointerClass &p);
public:
    PointerClass(const string &s = "");
    PointerClass(const PointerClass &rhs);
    PointerClass & operator=(const PointerClass &rhs);
    ~PointerClass();
private:
    ShareString *ps;
};

ostream & operator<<(ostream &out, const PointerClass &p);

//测试sizeof
class SizeA
{
    char a;
    char *p;
    char b;
};//结构体的大小会根据其内部最长基本类型对齐

class SizeB
{
    char c;
    SizeA a;//结构体所处的地址要是内部最长基本类型对齐
    // char a;
    // char *p;
    // char b;
    char c1;
};

}

#endif