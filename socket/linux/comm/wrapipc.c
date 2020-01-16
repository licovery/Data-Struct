#include "comm.h"

// POSIX sem

sem_t *Sem_open(const char *ipcName, int oflag, ...)
{
    sem_t *sem = NULL;
    if (oflag & O_CREAT)
    {
        va_list vl;
        va_start(vl, oflag);

        mode_t mode = va_arg(vl, mode_t);
        unsigned int value = va_arg(vl, unsigned int);
        if ((sem = sem_open(ipcName, oflag, mode, value)) == SEM_FAILED)
        {
            err_sys("sem_open error for %s", ipcName);
        }

        va_end(vl);
    }
    else
    {
        if ((sem = sem_open(ipcName, oflag)) == SEM_FAILED)
        {
            err_sys("sem_open error for %s", ipcName);
        }
    }
    return sem;
}

void Sem_close(sem_t *sem)
{
    if (sem_close(sem) == -1)
    {
        err_sys("sem_close error");
    }
}

void Sem_unlink(const char *ipcName)
{
    if (sem_unlink(ipcName) == -1)
    {
        err_sys("sem_unlink error");
    }
}

void Sem_wait(sem_t *sem)
{
    if (sem_wait(sem) == -1)
    {
        err_sys("sem_wait error");
    }
}

int Sem_trywait(sem_t *sem)
{
    int n;
    if ((n = sem_trywait(sem)) == -1 && errno != EAGAIN)
    {
        err_sys("sem_trywait error");
    }
    return n;
}

void Sem_post(sem_t *sem)
{
    if (sem_post(sem) == -1)
    {
        err_sys("sem_post error");
    }
}

int Sem_getvalue(sem_t *sem)
{
    int value;
    if (sem_getvalue(sem, &value) == -1)
    {
        err_sys("sem_getvalue error");
    }
    return value;
}

void Sem_init(sem_t *sem, int shared, unsigned int value)
{
    if (sem_init(sem, shared, value) == -1)
    {
        err_sys("sem_init error");
    }
}

void Sem_destory(sem_t *sem)
{
    if (sem_destroy(sem) == -1)
    {
        err_sys("sem_destory error");
    }
}