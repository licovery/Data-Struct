#include <iostream>

int main()
{
    int *p = new int;
    // 使用未初始化内存
    if (*p == 0)
    {
        std::cout << "yes" << std::endl;
    }
    delete p;

    return 0;
}