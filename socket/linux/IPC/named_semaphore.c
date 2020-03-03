#include "comm.h"

int val = 0;

#define SEM_NAME "/countMutexSem"
#define THREAD_NUM 10
#define MAX_VAL 100000

void *add(void *context)
{
    sem_t *mutexSem = Sem_open(SEM_NAME, 0);
    int loop = 1;
    while (loop)
    {
        Sem_wait(mutexSem);

        if (val != MAX_VAL)
        {
            val++;
            (*(int *)context)++;
        }
        else
        {
            loop = 0;
        }

        Sem_post(mutexSem);
    }

    Sem_close(mutexSem);
    return NULL;
}

int main()
{
    sem_t *mutexSem = Sem_open(SEM_NAME, O_CREAT | O_EXCL, FILE_MODE, 1);

    pthread_t pids[THREAD_NUM];
    int count[THREAD_NUM] = {0};

    for (int i = 0; i < THREAD_NUM; i++)
    {
        Pthread_create(pids + i, NULL, add, count + i);
    }
    int sum = 0;
    for (int i = 0; i < THREAD_NUM; i++)
    {
        Pthread_join(pids[i], NULL);
        sum += count[i];
        printf("thread%d: %d\n", i, count[i]);
    }

    if (sum != val)
    {
        printf("error\n");
    }

    Sem_close(mutexSem);
    Sem_unlink(SEM_NAME);
}