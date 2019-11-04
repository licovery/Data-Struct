#ifndef CLASS_TEST_H
#define CLASS_TEST_H

#include <string>
#include <iostream>
#include "template.h"
using namespace std;

class A
{
public:
    A() = default;
    A(int a, int b);
    A(const A &rhs);
    void print();
private:
    int x = 1;
    int y = 2;
};

//行为像值的类的
class ValueClass
{
public:
    ValueClass(string s = ""): ps(new string(s))
    {
        cout << "construct" << endl;
    }

    ValueClass(const ValueClass &rhs): ps (new string(*(rhs.ps)))
    {
        cout << "copy construct" << endl;
    }

    ~ValueClass()
    {
        delete ps;
    }

    ValueClass & operator=(const ValueClass &rhs)
    {
        //为了处理自赋值的情况，第一步要先要把右值拷到一个局部临时变量中
        string *temp = new string(*(rhs.ps));
        //或者直接做自赋值的特殊处理
        // if (this == &rhs)
        // {
        //     return *this;
        // }
        delete ps;
        ps = temp;
        return *this;
    }
    friend ostream & operator<<(ostream &out, const ValueClass &s);
    friend void swap(ValueClass &v1, ValueClass &v2);
private:
    string *ps;
};

ostream & operator<<(ostream &out, const ValueClass &s);
void swap(ValueClass &v1, ValueClass &v2);

struct ShareString
{
    ShareString(const string &s = "", size_t u = 0): str(s), use(u)
    {}
    string str;
    size_t use;//引用计数应该和资源本身紧密结合
};

//行为像指针的类
class PointerClass
{
public:
    PointerClass(const string &s = ""): ps(new ShareString(s, 1))
    {

    }
    PointerClass(const PointerClass &rhs): ps(rhs.ps)
    {
        ++(ps->use);
    }
    PointerClass & operator=(const PointerClass &rhs)
    {
        ++(rhs.ps->use);
        --(ps->use);
        if (ps->use == 0)
        {
            delete ps;
        }
        ps = rhs.ps;
        return *this;
    }
    ~PointerClass()
    {
        --(ps->use);
        if (ps->use == 0)
        {
            delete ps;
        }
    }
    friend ostream & operator<<(ostream &out, const PointerClass &p);
private:
    ShareString *ps;
};

ostream & operator<<(ostream &out, const PointerClass &p);

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

class Document
{
public:
    Document(const string &t = "", const string &c = ""): 
    title(new string(t)), content(new string(c))
    {
    }
    Document(const Document &rhs): 
    title(new string(*(rhs.title))), content(new string(*(rhs.content)))
    {
    }
    Document & operator=(const Document &rhs)
    {
        auto tempT = new string(*(rhs.title));
        auto tempC = new string(*(rhs.content));
        delete title;
        delete content;
        title = tempT;
        content = tempC;
        return *this;
    }
    virtual ~Document()
    {
        delete title;
        delete content;
    }
    virtual void print() const //要被子类ovrewrite
    {
    }
    string * getTitle() const
    {
        return title;
    }
    string * getContent() const
    {
        return content;
    }
    void openFile()
    {
        print();
    }
private:
    string *title;
    string *content;
};

class MyDoc: public Document
{
public:
    MyDoc(const string &t = "", const string &c = ""):Document(t, c)
    {
    }
    void print() const
    {
        cout << *getTitle() << endl;
        cout << *getContent() << endl;
    }
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

#endif