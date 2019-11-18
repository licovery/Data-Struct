#ifndef FUNCTION_H
#define FUNCTION_H

namespace lf_fun
{
//出现二义性调用
void myf(int);
void myf(const int &);

//作用域屏蔽名字查找
void li(int);
}

#endif