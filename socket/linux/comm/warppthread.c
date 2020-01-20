#include "comm.h"

pthread_t Pthread_create(pthread_t *tid, pthread_attr_t *arrt, Threadfunc *func, void *arg)
{
    int n = pthread_create(tid, arrt, func, arg);
    if (n != 0)
    {
        errno = n;
        err_sys("pthread_create error");
    }
    return *tid;
}

void Pthread_join(pthread_t tid, void **retval)
{
    int n = pthread_join(tid, retval);
    if (n != 0)
    {
        errno = n;
        err_sys("pthread_join error");
    }
}

pthread_t Pthread_self()
{
    return pthread_self();
}

void Pthread_detach(pthread_t tid)
{
    int n = pthread_detach(tid);
    if (n != 0)
    {
        errno = n;
        err_sys("pthread_death error");
    }
}

// retval不能指向调用者的局部变量
void Pthread_exit(void *retval)
{
    pthread_exit(retval);
}

// mutex lock

void Pthread_mutex_init(pthread_mutex_t *mptr, pthread_mutexattr_t *attr)
{
    int n;
    if ((n = pthread_mutex_init(mptr, attr)) != 0)
    {
        errno = n;
        err_sys("pthread_mutex_init error");
    }
}

void Pthread_mutex_destory(pthread_mutex_t *mutex)
{
    int n = pthread_mutex_destroy(mutex);
    if (n != 0)
    {
        errno = n;
        err_sys("pthread_mutex_destory error");
    }
}

void Pthread_mutex_lock(pthread_mutex_t *mutex)
{
    int n = pthread_mutex_lock(mutex);
    if (n != 0)
    {
        errno = n;
        err_sys("pthread_mutex_lock error");
    }
}

int Pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    int n = pthread_mutex_trylock(mutex);
    if (n != 0 && n != EBUSY)
    {
        errno = n;
        err_sys("pthread_mutex_trylock error");
    }
    return n;
}

void Pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    int n = pthread_mutex_unlock(mutex);
    if (n != 0)
    {
        errno = n;
        err_sys("pthread_mutex_unlock error");
    }
}

// POSIX cond
void Pthread_condattr_init(pthread_condattr_t *attr)
{
    int n;

    if ((n = pthread_condattr_init(attr)) == 0)
        return;
    errno = n;
    err_sys("pthread_condattr_init error");
}

void Pthread_condattr_destroy(pthread_condattr_t *attr)
{
    int n;

    if ((n = pthread_condattr_destroy(attr)) == 0)
        return;
    errno = n;
    err_sys("pthread_condattr_destroy error");
}

void Pthread_cond_signal(pthread_cond_t *cptr)
{
    int n;

    if ((n = pthread_cond_signal(cptr)) == 0)
        return;
    errno = n;
    err_sys("pthread_cond_signal error");
}

void Pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr)
{
    int n;

    if ((n = pthread_cond_wait(cptr, mptr)) == 0)
        return;
    errno = n;
    err_sys("pthread_cond_wait error");
}

void Pthread_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr, const struct timespec *tsptr)
{
    int n;

    if ((n = pthread_cond_timedwait(cptr, mptr, tsptr)) == 0)
        return;
    errno = n;
    err_sys("pthread_cond_timedwait error");
}
