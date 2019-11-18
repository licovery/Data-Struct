#include "multi_threads.h"
#include <thread>
#include <mutex>
#include <iostream>

std::mutex mtx;

void print_block(int n, char c)
{
    std::unique_lock<std::mutex> lck(mtx);//lock initialization
    for (int i = 0; i < n; i++)
    {
        std::cout << c;
    }
    std::cout << std::endl;
}// lck析构自动unlock

void multiThreadsTest()
{
    constexpr int n = 50;

    //第一个参数是线程启动函数，后面参数是附带给函数的参数
    std::thread th1(print_block, n, '@');
    std::thread th2(print_block, n, '#');
    //注意参数类型一定要填对，不然编译会报错，或者运行出错
    // std::thread th3(print_block, n, "/");
    // [with _Callable = void (&)(int, char); _Args = {const int&, const char (&)[2]}]'
    

    th1.join();
    th2.join();
}










