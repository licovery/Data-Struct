#include "comm.h"

//读写锁常用API
// pthread_rwlock_rdlock()

struct
{
    pthread_rwlock_t rwlock;
    int status;
} rwShared = {PTHREAD_RWLOCK_INITIALIZER};

void *Reader(void *context)
{
    int loop = 1;
    while (loop)
    {
        pthread_rwlock_rdlock(&rwShared.rwlock);
        //read
        if (rwShared.status)
        {
            printf("%d:OK\n", *(int *)context);
            loop = 0;
        }
        else
        {
            printf("%d:NOT OK\n", *(int *)context);
        }
        pthread_rwlock_unlock(&rwShared.rwlock);
        sleep(1); //在互斥锁中sleep纯粹是浪费资源
    }
    return NULL;
}

void *Writer(void *context)
{
    // 如果读锁一直申请，可能会导致写锁饿死
    pthread_rwlock_wrlock(&rwShared.rwlock);
    // write
    rwShared.status = 1;
    printf("write OK\n");
    pthread_rwlock_unlock(&rwShared.rwlock);
    return NULL;
}

#define READER_NUM 10
#define WRITER_NUM 1

int main()
{

    pthread_t rdTids[READER_NUM];
    int index[READER_NUM];
    pthread_t wrTid;

    for (int i = 0; i < READER_NUM; i++)
    {
        index[i] = i;
        Pthread_create(rdTids + i, NULL, Reader, index + i);
    }

    Pthread_create(&wrTid, NULL, Writer, NULL);

    for (int i = 0; i < READER_NUM; i++)
    {
        Pthread_join(rdTids[i], NULL);
    }

    Pthread_join(wrTid, NULL);

    return 0;
}
