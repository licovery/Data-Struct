#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "io.h"

using namespace std;

void chineseStringTest()
{
    string s;
    cin >> s;
    cout << "size: " << s.size() << " content: ";
    for (char c : s)
    {
        cout << c;
    }
    cout << endl;
}

void cinTest()
{
    int a, c; 
    double b;
    cin >> a >> c >> b;
    
    if (cin.good())
    {
        cout << "goodbit" << endl;
    }
    if (cin.bad())
    {
        cout << "badbit" << endl;
    }
    if (cin.fail())
    {
        cout << "failbit" << endl;
    }
    if (cin.rdstate() & istream::eofbit)
    {
        cout << "eofbit" << endl;
    }
    

    if (cin)
    {
        cout << "success" << endl;
    }
    else
    {
        cout << "fail" << endl;
    }
    cout << a << " " << c << " " << b << endl;
}

void getlineTest()
{
    string s1, s2;
    getline(cin, s1);//把一行的内容全部读入，中间空格忽略。
    cout << s1 << endl;
    cin >> s2;//cin遇到空格会停止 
    cout << s2 << endl;
}