#include "comm.h"

pthread_t Pthread_create(pthread_t *tid, pthread_attr_t *arrt, Threadfunc *func, void *arg)
{
    errno = pthread_create(tid, arrt, func, arg);
    if (errno != 0)
    {
        err_sys("pthread_create error");
    }
    return *tid;
}

void Pthread_join(pthread_t tid, void **retval)
{
    errno = pthread_join(tid, retval);
    if (errno != 0)
    {
        err_sys("pthread_join error");
    }
}

pthread_t Pthread_self()
{
    return pthread_self();
}

void Pthread_detach(pthread_t tid)
{
    errno = pthread_detach(tid);
    if (errno != 0)
    {
        err_sys("pthread_death error");
    }
}

// retval不能指向调用者的局部变量
void Pthread_exit(void *retval)
{
    pthread_exit(retval);
}