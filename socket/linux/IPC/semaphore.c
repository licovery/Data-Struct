/* POSIX信号量

三种操作
/create
wait (p操作)
post (v操作)


有名信号量(named)            无名信号量(unnamed)
    sem_open()                 sem_init()
              \               /         
                sem_wait()
                sem_trywait()
                sem_post()
                sem_getvalue()
               /               \
    sem_close()                 sem_destory()
    sem_unlink()

*/

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#include "comm.h"

#define BUF_SIZE 10
#define MAX_THREAD 100

// 全局变量均初始化为0

int nitem, nproducer, nconsumer;

struct
{
    int buf[BUF_SIZE]; // 共享缓冲区
    int nput;          // 生产者放入的产品个数
    int nputVal;       // 生产者放入产品的值
    int nget;          // 消费者取出的产品个数
    int ngetVal;       // 消费者取出产品的值
    sem_t mutex;       // 互斥信号量
    sem_t nempty;      // 生产者可放入产品个数
    sem_t nproduct;    // 消费者可取出产品个数
} sharedBuf;

void *ProducerProc(void *arg);
void *ConsumerProc(void *arg);

int main(int argc, char **argv)
{
    if (argc != 4)
        err_quit("usage: %s <#items> <#producers> <#consumers>", argv[0]);
    nitem = atoi(argv[1]);
    nproducer = min(atoi(argv[2]), MAX_THREAD);
    nconsumer = min(atoi(argv[3]), MAX_THREAD);

    int producerCount[MAX_THREAD] = {0}; // 记录每个生产者线程生产产品个数
    int consumerCount[MAX_THREAD] = {0}; // 记录每个消费者线程消费产品个数

    pthread_t producerTid[MAX_THREAD];
    pthread_t consumerTid[MAX_THREAD];

    // 初始化无名信号量
    Sem_init(&sharedBuf.mutex, 0, 1);
    Sem_init(&sharedBuf.nempty, 0, BUF_SIZE);
    Sem_init(&sharedBuf.nproduct, 0, 0);

    // 创建线程
    for (int i = 0; i < nproducer; i++)
    {
        Pthread_create(&producerTid[i], NULL, ProducerProc, &producerCount[i]);
    }
    for (int i = 0; i < nconsumer; i++)
    {
        Pthread_create(&consumerTid[i], NULL, ConsumerProc, &consumerCount[i]);
    }
    // wait 子进程
    for (int i = 0; i < nproducer; i++)
    {
        Pthread_join(producerTid[i], NULL);
        printf("producer count[%d] = %d\n", i, producerCount[i]);
    }
    for (int i = 0; i < nconsumer; i++)
    {
        Pthread_join(consumerTid[i], NULL);
        printf("consumer count[%d] = %d\n", i, consumerCount[i]);
    }
    // 销毁无名信号量
    Sem_destory(&sharedBuf.mutex);
    Sem_destory(&sharedBuf.nempty);
    Sem_destory(&sharedBuf.nproduct);

    return 0;
}

// p操作可能会阻塞，所以多个p操作的顺序不当会死锁，而多个v操作应该对程序逻辑没有太大的影响

void *ProducerProc(void *arg)
{
    while (1)
    {
        Sem_wait(&sharedBuf.nempty);
        Sem_wait(&sharedBuf.mutex); // 等互斥信号量要放到后面，否则死锁

        if (sharedBuf.nput >= nitem)
        {
            // 归还信号量，与while后面的两个sem_wait对应
            Sem_post(&sharedBuf.mutex);
            Sem_post(&sharedBuf.nempty);
            // 因为消费者总会阻塞在sem_wait(nproduct)
            // 生产者必须发送一个“假产品”，让消费者可以运行
            // 然后消费者运行到if判断，发现要取的产品已经取完，结束循环
            Sem_post(&sharedBuf.nproduct);  
            break;
        }

        sharedBuf.buf[sharedBuf.nput % BUF_SIZE] = sharedBuf.nputVal;
        sharedBuf.nput++;
        sharedBuf.nputVal++;

        Sem_post(&sharedBuf.mutex);
        Sem_post(&sharedBuf.nproduct);

        (*((int *)arg))++;
    }
    return NULL;
}

void *ConsumerProc(void *arg)
{
    while (1) // 不能在这里使用sharedBuf的变量，一定要在互斥区中使用
    {
        Sem_wait(&sharedBuf.nproduct);
        Sem_wait(&sharedBuf.mutex); // 等互斥信号量要放到后面，否则死锁

        if (sharedBuf.nget >= nitem) // 判断是否要结束
        {
            // 线程应该结束了，归还信号量，与while循环中刚开始的sem_wait对应
            Sem_post(&sharedBuf.mutex);
            Sem_post(&sharedBuf.nproduct);
            break;
        }

        // 消费者消费过程就是检查生产者放入的数值是否正确
        int i = sharedBuf.nget % BUF_SIZE;
        if (sharedBuf.buf[i] != sharedBuf.ngetVal)
        {
            printf("buf error, buf[%d] = %d", i, sharedBuf.buf[i]);
        }
        sharedBuf.nget++;
        sharedBuf.ngetVal++;
        // 消费结束，解开互斥，通知生产者
        Sem_post(&sharedBuf.mutex);
        Sem_post(&sharedBuf.nempty);

        (*((int *)arg))++;
    }

    return NULL;
}
