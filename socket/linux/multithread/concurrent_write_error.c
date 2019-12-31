#include "comm.h"

static int count = 0;
#define N 50000

void *AddCount(void *arg)
{
    for (int i = 0; i < N; i++)
    {
        // 这样处理更慢，更容易出现错误
        int val = count;
        printf("%ld %d\n", Pthread_self(), val);
        count = val + 1;
        // count++;
    }
    return NULL;
}

// 通过加互斥锁保证线程安全
static pthread_mutex_t count_metex = PTHREAD_MUTEX_INITIALIZER; //必须初始化
void *AddCountSafe(void *arg)
{
    pthread_mutex_lock(&count_metex);   // 加锁
    AddCount(NULL);
    pthread_mutex_unlock(&count_metex); // 解锁
    return NULL;
}

int main()
{
    Threadfunc *pfunc = AddCountSafe;
    pthread_t tid1, tid2;
    Pthread_create(&tid1, NULL, pfunc, NULL);
    Pthread_create(&tid2, NULL, pfunc, NULL);

    Pthread_join(tid1, NULL);
    Pthread_join(tid2, NULL);

    printf("%d\n", count);
    return 0;
}

/* 输出结果
140114797541120 6121
140114797541120 6122
140114797541120 6123
140114797541120 6124
140114789148416 4205
140114789148416 4206
140114789148416 4207
140114789148416 4208

......
最后结果5000

再次运行结果5921 5926，一系列随机值

即使是转成count++，也会出现问题，因为++不是原子操作

➜  multithread git:(master) ✗ ./build/concurrent_write_error 
7460
➜  multithread git:(master) ✗ ./build/concurrent_write_error 
5000
➜  multithread git:(master) ✗ ./build/concurrent_write_error 
10000

*/