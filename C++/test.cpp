#include "test.h"
#include <vector>
#include "operator_overloading.h"
#include "class_test.h"
#include "strvec.h"
#include <map>
#include <algorithm>
#include <functional>
#include "oop.h"
#include "template.h"
#include <typeinfo>

using namespace std;

void synthesizeCopyConstructTest()
{
    A a;
    A b(100, 199);
    A c(b);
    a.print();
    b.print();
    c.print();
}

void deleteTest()
{
    auto p = new char;
    auto q = new char[1];
    delete p;
    delete[] q;
    auto p1 = new char[1];
    auto q2 = new char[10];
    delete p1;//不报错，但是会产生未定义的行为
    delete q2;//不报错，但是会产生未定义的行为
}

void valueClassTest()
{
    ValueClass a("hello");
    cout << a << endl;
    a = a;
    cout << a << endl;
    ValueClass b(a);
    cout << b << endl;
    ValueClass c("world");
    cout << c << endl;
    b = c;
    cout << b << endl;
    swap(b, c);
    cout << b << endl << c << endl;
}

void pointerClassTest()
{
    PointerClass a("hello");
    cout << a << endl;
    a = a;
    cout << a << endl;
    PointerClass b(a);
    cout << b << endl;
    PointerClass c("world");
    cout << c << endl;
    b = c;
    cout << b << endl;
}


// void NewA()
// {
//     A *p;
//     void *mem = operator new(sizeof(A));
//     p = static_cast<A *>(mem);
//     p->A::A(1, 2);
// }

void sizeofTest()
{
    cout << sizeof(SizeA) << endl;
    cout << sizeof(SizeB) << endl;
}

void virtualFunTest()
{
    // Document d("t", "c");
    // d.openFile();
    MyDoc d1("t1", "c1");
    d1.openFile();
}

void inheritanceCompositionCtorDtorTest()
{
    Derived d;
}


void vectorPushBackTest()//容器里面的数据和参数没有什么关系
{
    string a = "hello";
    vector<string> v;
    v.push_back(a);
    cout << v[0] << endl;
    a[0] = 'w';
    cout << v[0] << endl;
    v[0][0] = 'a';
    cout << a << endl;
}

void strVecTest()
{
    StrVec v;
    v.push_back("1");
    cout << v << endl;
    v.push_back("2");
    cout << v << endl;
    v.push_back("3");
    cout << v << endl;
    v.push_back("4");
    cout << v << endl;
    StrVec v2(v);
    cout << v << endl << v2 << endl;
    v2.push_back("100");
    cout << v2 << endl;
    v = {"1", "2", "3", "6"};
    const StrVec cv = v;
    v[0] = "";
    cv[0];
    v++;
    ++v;
}

void strVecMoveTest()
{
    vector<StrVec> v;
    v.push_back(StrVec ());

    StrVec a = std::move(retStrVec());
    a = retStrVec();
    cout << a << endl;
}

void memberAccessOperatorOverloadTest()
{
    D d;
    d->foo = 999;
    cout << (*d).foo << endl;
    (*d).foo == 10000;
    cout  << d->foo << endl;
}

void stlFunctionObjectTest()
{
    less<Book> f;
    Book b1("li", 10.0), b2("hello", 19.0);
    cout << f(b1, b2) << endl;
}



class Stu
{
public:
    Stu(string n = "", string i = "", int a = 0): name(n), id(i), age(a)
    {
    }
    string name;
    string id;
    int age;
    friend ostream & operator<<(ostream &out, const Stu &s);
};

ostream & operator<<(ostream &out, const Stu &s)
{
    out << s.name << " " << s.id << " " << s.age;
    return out;
}

bool lessStu(const Stu &s1, const Stu &s2)
{
    return s1.id < s2.id;
}

void customizeClassInMap()
{
    map<Stu, int, decltype(lessStu) *>  scoreTable(lessStu);
    Stu a("li", "00", 22);
    Stu b("wang", "01", 23);
    Stu c("chen", "99", 21);
    scoreTable[b] = 2;
    scoreTable[c] = 3;
    scoreTable[a] = 1;
    for (auto p : scoreTable)
    {
        cout << p.first << " " << p.second << endl;
    }
    //pair要使用{}来赋值或者初始化
    pair<string, int> p = {"hello", 10};

}

void bindFunctionObjectTest()
{
    vector<int> vec;
    count_if(vec.begin(), vec.end(), bind(greater<int> (), placeholders::_1, 1024));
    find_if(vec.begin(), vec.end(), bind(not_equal_to<int> (), placeholders::_1, 1024));
    transform(vec.begin(), vec.end(), vec.begin(), bind(multiplies<int> (), placeholders::_1, 2));
}

void callableObjTest()
{
    auto mod = [] (int i, int j) {return i % j;};
    function<int(int , int)> f1 = add;
    decltype(f1) f2 = mod;
    decltype(f1) f3 = divide ();//这里的括号代表一个匿名对象
    //由于function的函数不是explicit，所以这里可以=,
    //等价于function<int(int , int)> f1(add)
}

void typeConversionOperatorOverloadTest()
{
    SmallInt obj = 100;                 //构造函数定义的隐式转换
    int i = obj;                        //运算符重载的隐式转换
    bool b = static_cast<bool>(obj);    //运算符重载的显式转换
    // fun(10); //二义性调用
}

void oopStaticTest()
{
    //无论派生出多少个派生类，对于每个静态成员只有唯一的示例
    Qoute q("123-456", 23.5);
    cout << q.get() << endl;
    q.set(100);
    cout << q.get() << endl;
    Bulk_qoute bq("123-456", 23.5, 10, 0.8);
    cout << bq.get() << endl;
    bq.set(500);
    cout << q.get() << endl;
}

void oopVirtualFunTest()
{
    Qoute q;
    Bulk_qoute bq;
    Qoute *p = &q;
    p->debug();
    p = &bq;
    p->debug();
}

void scopeVirtualFunTest()
{
    B1 bobj;
    D1 d1obj;
    D2 d2obj;

    bobj.fcn();
    // d1obj.fcn(); 因为定义与父类虚函数同名的其他函数，由于作用域不同，不能成为重载，其实是隐藏了父类同名函数。
    d2obj.fcn();

    B1 *bp1 = &bobj, *bp2 = &d1obj, *bp3 = &d2obj;
    bp1->fcn();
    bp2->fcn();//这里没有被隐藏，因为调用的静态类型是B1类，里面确实有fcn这个函数。
    bp3->fcn();

    //总结，静态类型决定了哪些成员是可见的。
}

extern template int compare(const int &, const int &);
extern template class Blob<int>;
extern template class Blob<double>;
extern template class Blob<string>;

void funTemplateTest()
{
    cout << compare(1, 2) << endl;
    char a[5] = "123";
    char b[10] = "123887";
    cout << compare(a, b) << endl;
}

void classTemplateTest()
{
    Blob<string> v;
    v.push_back("hello");
    v.push_back("world");
    v.pop_back();
    Blob<string> v1 = {"1", "2", "3"};
    cout << v1.size() << endl;
    v1 = v;
    v.push_back("li");
    cout << v1.size() << endl;
}

void templateFriendTest()
{
    Data<int> d;
    Data<int> d2;
    d == d2;

    Print<string> p;
    p.fun();

    Access<double> a;
    a.fun();

    Pal pa;
    pa.fun();
}

void templateStaticTest()
{
    FooT<int> ft;
    cout << ft.value() << endl;
}

bool com(int a, int b)
{
    return true;
}

void templateParaTest()
{
    TwoTemplate<int, double> a;
    SameTemplate<int> b;
    IntTemplate<double> c;
    TemplateInt<double> d;

    compareWithFunc(1, 2);
    Number<> i(10);
    Number<double> f(19.99);
}

void memberTemplateTest()
{
    int ia[] = {0, 1, 2};
    vector<double> vi = {0.9, 1.4};
    vector<const char *> w = {"hello", "world"};
    Blob<int> a(begin(ia), end(ia));
    Blob<double> b(vi.begin(), vi.end());
    Blob<string> c(w.begin(), w.end());
}

void simpleTest()
{
    
}