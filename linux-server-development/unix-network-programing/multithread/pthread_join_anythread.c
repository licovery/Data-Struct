#include "comm.h"

// 使用互斥所和条件变量共同实现通知主进程某个子进程已经结束

#define THREAD_NUM 5

#define START 0
#define END 1
#define JOINED 2

static int done = 0; // 已经完成的进程数量
// 必须初始化
static pthread_mutex_t done_mutex = PTHREAD_MUTEX_INITIALIZER; // 互斥量
static pthread_cond_t done_cond = PTHREAD_COND_INITIALIZER;    // 条件信号，必须和互斥量一起使用

struct ThreadInfo
{
    pthread_t tid;
    int status;
    int second; //运行时间
};

void *Fun(void *arg)
{
    struct ThreadInfo *pThreadInfo = (struct ThreadInfo *)arg;

    sleep(pThreadInfo->second);

    Pthread_mutex_lock(&done_mutex);
    done++;                          // done++通知主线程已经有进程结束
    pThreadInfo->status = END;       // 作为线程结束的标志，以便找到结束的线程
    Pthread_cond_signal(&done_cond); // 发送条件变量信号唤醒主进程,如果发信号的时候，主进程没有wati，那么相当于信号消失
    Pthread_mutex_unlock(&done_mutex);
    return NULL;
}

int main()
{
    struct ThreadInfo threadInfo[THREAD_NUM];

    threadInfo[4].second = 1;
    threadInfo[3].second = 2;
    threadInfo[1].second = 3;
    threadInfo[2].second = 4;
    threadInfo[0].second = 5;

    for (int i = 0; i < THREAD_NUM; i++)
    {
        threadInfo[i].status = START;
        Pthread_create(&threadInfo[i].tid, NULL, Fun, &threadInfo[i]);
    }

    int runing = THREAD_NUM;
    while (runing > 0)
    {
        Pthread_mutex_lock(&done_mutex); // 加锁，防止while判断结束后，done由别的线程改变导致主进程不知道有进程完成。
        while (done == 0)                // 不能去掉这个判断，因为可能信号在wait之前已经发过了，直接wait会永远阻塞
        {
            // 等待有线程完成并发送信号唤醒
            Pthread_cond_wait(&done_cond, &done_mutex); // 调用时解锁，阻塞。收到信号后返回，加锁
            // man: pthread_cond_wait() shall be called with mutex locked by the calling thread or undefined behavior results.
            // 这个wait一般都是和一个变量判断条件一起使用，这个变量代表是否有发送过信号。例如此处的done
        } 

        for (int i = 0; i < THREAD_NUM; i++)
        {
            if (threadInfo[i].status == END)
            {
                Pthread_join(threadInfo[i].tid, NULL); // 这个函数只能等待指定tid线程。但是那个线程先完成是不确定的。
                printf("end thread index:%d\n", i);
                threadInfo[i].status = JOINED;
                runing--;
                done--; // 还需要锁
            }
        }
        Pthread_mutex_unlock(&done_mutex); // 必须在done--之后
    }

    return 0;
}
