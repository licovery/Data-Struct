#include "comm.h"
#include <assert.h>

/*
    pthread_key_create(key, deletor)    // 为每个线程创建一个key, 线程结束会自动调用deletor
    pthread_key_delete(key)             // 删除key
    pthread_once(func)                  // 保证调用线程只在第一次调用执行func
    pthread_setspecific(key, value)     // 设置本线程的的数据
    pthread_getspecific(key)            // 根据key获取对应线程数据
*/ 

// 有非栈区数据（static或者全局）的函数一般是线程不安全的，可以把非栈区数据设置为线程特定数据，保证线程安全。
// 线程特定数据这种技术一般用来改造非线程安全函数为线程安全函数


// 统计某个线程执行的次数，没有特别意义，只是为了说明。
// 首先这个函数肯定线程不安全，因为不同线程调用，加的是同一个i,而且多个同时++可能导致错误;
int ThreadCount()
{
    static int i = 0;
    return ++i;
}

// 使用thread spec data改造为线程安全版本
// 问题的核心是使用了一个static变量

void MyFree(void *p)
{
    if (p != NULL)
    {
        free(p);
    }
    printf("free %p\n", p);
}

static pthread_key_t k;
static pthread_once_t once = PTHREAD_ONCE_INIT;

void CreateKeyOnce()
{
    int n = pthread_key_create(&k, MyFree);
    if (n != 0)
    {
        errno = n;
        err_sys("pthread_key_create");
    }
}


int ThreadCountSafe()
{
    int n;
    // 在这个进程的多个线程中，保证只调用一次CreateKeyOnce
    n = pthread_once(&once, CreateKeyOnce);
    if (n != 0)
    {
        errno = n;
        err_sys("pthread_once");
    }

    int *value = NULL;
    // 只能操作自己的线程特定数据，无法访问别的线程的，这也是线程特定数据的本义；
    if (pthread_getspecific(k) == NULL)
    {
        value = (int *)malloc(sizeof(int));
        *value = 0;
        // 存储指针
        n = pthread_setspecific(k, value);
        if (n != 0)
        {
            errno = n;
            err_sys("pthread_setspecific");
        }
    }
    else
    {
        value = pthread_getspecific(k);
    }
    
    assert(value != NULL);
    return ++(*value);
}


// 业务部分


#define THREAD_NUM 10

void * ThreadProc(void *arg)
{
    int count = 0;
    for (int i = 0; i < 7; i++)
    {
        count = ThreadCountSafe();// 调用的是线程安全版本
    }
    printf("tid:%ld, count:%d\n", Pthread_self(), count);
    return NULL;
}

int main()
{
    pthread_t tid[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++)
    {
        Pthread_create(&tid[i], NULL, ThreadProc, NULL);
    }

    for (int i = 0; i < THREAD_NUM; i++)
    {
        Pthread_join(tid[i], NULL);
    }
    return 0;
}