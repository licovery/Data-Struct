#ifndef OOP_H
#define OOP_H

#include<string>
#include <iostream>

namespace lf_oop
{
    
using std::cout;
using std::endl;
using std::string;

struct Base
{
    Base()
    {
        cout << "Base Construct" << endl;
    }

    virtual ~Base()
    {
        cout << "Base Destruct" << endl;
    }
};


struct Component
{
    Component()
    {
        cout << "Component Construct" << endl;
    }

    ~Component()
    {
        cout << "Component Destruct" << endl;
    }
};

struct Derived: public Base //先构造Base，再构造Component，再构造自身。析构顺序相反
{
    Derived()
    {
        cout << "Derived Construct" << endl;
    }
    ~Derived()
    {
        cout << "Derived DeStruct" << endl;
    }
private:
    Component c;
};

class Qoute
{
public:
    Qoute() = default;
    Qoute(const string &book, double p);
    virtual ~Qoute() = default;

    string isbn() const;
    virtual double net_price(size_t n) const;
    virtual void debug() const;

    static void set(size_t n);
    static size_t get();
    
protected:
    double price = 0.0;
    static size_t totalBookNum;
private:
    string bookNo = "";
};

class Disc_qoute: public Qoute //抽象基类
{
public:
    Disc_qoute() = default;
    Disc_qoute(const string &book, double p, size_t n, double d);
    double net_price(size_t n) const = 0; //纯虚函数
protected:
    size_t minNum = 0;
    double discount = 0.0;
};

class Bulk_qoute: public Disc_qoute
{
public:
    Bulk_qoute() = default;
    Bulk_qoute(const string &book, double p, size_t n, double d);
    // 可以直接继承父类的构造函数，
    // using Disc_qoute::Disc_qoute;
    // 继承过来的构造函数不算是用户自定义的，所以编译器还会自动合成默认构造函数

    double net_price(size_t n) const override;//override让编译器去发现如果没有覆盖基类的虚函数
    void debug() const override final; //final不让虚函数继续被子类覆写
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
    void print() const override
    {
        cout << *getTitle() << endl;
        cout << *getContent() << endl;
    }
};

class B1
{
public:
    virtual void fcn()
    {
        cout << "B1 fcn() virtural" << endl;
    }
};

class D1: public B1
{
public:
    void fcn(int)
    {
        cout << "D1 fcn(int)" << endl;
    }
    virtual void f2()
    {
        cout << "D1 f2() virtural" << endl;
    }
};

class D2: public D1
{
public:
    void fcn()
    {
        cout << "D2 fcn()" << endl;
    }
    void fcn(int)
    {
        cout << "D2 fcn(int)" << endl;
    }
    void f2()
    {
        cout << "D2 f2()";
    }
};


//派生类的构造，拷贝，移动，析构
class Base1
{
public:
    Base1() = default;
    Base1(const Base1 &) = default;
    Base1(Base1 &&) = default;
    Base1 & operator=(const Base1 &) = default;
    Base1 & operator=(Base1 &&) = default;
    virtual ~Base1() = default;//虚函数会增加对象的大小，会让对象多出一个指针
};

class Derived1: public Base1
{
public:
    Derived1(): Base1()
    {
        //列表初始化先调基类的构造函数
        // ...
    }
    Derived1(const Derived1 &rhs): Base1(rhs)
    {
        //列表初始化先调基类的拷贝构造函数
        // ...
    }
    Derived1(Derived1 &&lhs): Base1(std::move(lhs))
    {
        //列表初始化先调基类的移动构造函数
        // ...
    }
    Derived1 & operator=(const Derived1 &rhs)
    {
        //先为基类部分赋值
        Base1::operator=(rhs);
        // ...
        return *this;
    }
    Derived1 & operator=(Derived1 &&lhs)//注意这里lch是左值，因为lch是一个变量
    {
        //先为基类部分赋值
        Base1::operator=(std::move(lhs));
        // ...
        return *this;
    }
    ~Derived1()
    {
        //Base::~Base()被自动调用，无须显式调用
    }
};

} // namespace lf_oop

#endif