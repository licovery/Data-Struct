#include "template.h"
#include "class.h"
#include <unordered_map>

using namespace std;
using namespace lf_tem;
using lf_cla::A;

extern template int lf_tem::mycompare(const int &, const int &);
extern template class lf_tem::Blob<int>;
extern template class lf_tem::Blob<double>;
extern template class lf_tem::Blob<string>;

void funTemplateTest()
{
    cout << mycompare(1, 2) << endl;
    char a[5] = "123";
    char b[10] = "123887";
    cout << mycompare(a, b) << endl;
    auto p1 = "hhh";
    auto p2 = "www";
    cout << mycompare(p1, p2) << endl;
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

void argumentTemplateTest()
{
    mycompare<int>(1, 2);
    int i;
    const int ci = 5;

    //模板参数形式为T
    f1(i);
    f1(ci);//顶层const忽略
    f1(5);
    //以上三种T均为int

    //模板参数形式为T &
    f2(i);//T为int &
    f2(ci);//T为const int &
    // f2(5);不能把右值绑定到左值引用上

    //模板参数形式为T &&
    //如果模板参数是右值引用形式（T &&），那么左值i会的类型T会被推导到int &,int & &&被折叠成int &
    f3(i);//T为int &  T &&->int & &&->int &
    f3(ci);//T为const int &  T &&->const int & &&->const int &
    f3(5);//T为int T &&->int &&
}



void templateOverloadTest()
{
    const char *p = "hello world";
    string s = "hello";
    const string *sp = &s;
    char c;
    int i;
    cout << debug_rep(c) << endl;
    cout << debug_rep(i) << endl;
    //并不会调用特例化版本，因为重载的候选函数是基模板函数和非模板函数，只有已经选定基模板函数的情况下，才考虑特例化函数。
    cout << debug_rep(p) << endl;
    cout << debug_rep(s) << endl;
    cout << debug_rep(sp) << endl;
    cout << debug_rep("hello") << endl;
    
}

void variadicTemplateTest()
{
    int i = 0;
    double d = 1.1;
    string s = "hello";
    foo(i);
    foo(i, d, s);
    foo(1, 2, 3, 4);
    print(cout, 1, 2.0, "hello", 'c');

    //使用 Args && ... args  和 std::forward<Args>(args)... 可以实现参数包的转发
    //make_shared就是这样做的,还有allocator的construct函数
}

void templateSpecializationTest()
{
    unordered_map<A, int> m;
    A a1(10, 20), a2(20, 30);
    m[a1] = 20;
    m[a2] = 50;
}