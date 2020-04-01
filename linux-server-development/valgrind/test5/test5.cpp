#include <iostream>

int main()
{
    // 申请和释放不匹配
    int *p = new int[10];
    // 应该使用delete []
    delete p;

    return 0;
}