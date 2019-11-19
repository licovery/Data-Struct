#ifndef THREAD_SAFE_SINGLE_INSTANCE_H
#define THREAD_SAFE_SINGLE_INSTANCE_H

#include <mutex>    // std::mutex

class SingleInstance
{
public:
    // 获取单例对象
    static SingleInstance *GetInstance();

    // 释放单例，进程退出时调用
    static void deleteInstance();
	
	// 打印单例地址
    void Print();

private:
	// 将其构造和析构成为私有的, 禁止外部构造和析构
    SingleInstance();
    ~SingleInstance();

private:
    // 唯一单例对象指针
    static SingleInstance *m_SingleInstance;
    static std::mutex m; //互斥锁用
};

void threadSafeTest();

#endif