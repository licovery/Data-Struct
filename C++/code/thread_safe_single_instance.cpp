#include "thread_safe_single_instance.h"
#include <iostream>
#include <vector>
#include <thread>

//初始化静态成员变量
SingleInstance *SingleInstance::m_SingleInstance = NULL;
std::mutex SingleInstance::m;

SingleInstance* SingleInstance::GetInstance()
{
    if (m_SingleInstance == NULL)
    {
        std::unique_lock<std::mutex> lck(m);
        if (m_SingleInstance == NULL)
        {
            m_SingleInstance = new SingleInstance;  // 没有加锁是线程不安全的，当线程并发时会创建多个实例
        }
    }
    return m_SingleInstance;
}

void SingleInstance::deleteInstance()
{
    if (m_SingleInstance)
    {
        delete m_SingleInstance;
        m_SingleInstance = NULL;
    }
}

void SingleInstance::Print()
{
	std::cout << "instance addr:" << this << std::endl;
}

SingleInstance::SingleInstance()
{
    std::cout << "construct" << std::endl;
}

SingleInstance::~SingleInstance()
{
    std::cout << "destory" << std::endl;
}


// 线程函数
void PrintHello(int threadId)
{
    // std::cout << "Hi, 我是线程 ID:[" << threadId << "]" << std::endl;

    //为了让线程同时进入临界判断，更容易触发线程安全问题
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    // 打印实例地址
    SingleInstance::GetInstance()->Print();
}


void threadSafeTest()
{
    constexpr int NUM_THREADS = 5; // 线程个数
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        // std::cout << "创建线程:[" << i << "]" << std::endl;
        threads.push_back(std::thread(PrintHello, i));
    }

    for (auto &th : threads)
    {
        th.join();
    }

    // 手动释放单实例的资源
    SingleInstance::deleteInstance();
}
