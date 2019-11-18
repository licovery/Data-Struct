#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

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


void typeTransformationTest()
{
    int a = 10;
    int &r = a;
    remove_reference<decltype(r)>::type i = 20;//如果r是一个模板类型的变量，那么需要在前面加typename
}