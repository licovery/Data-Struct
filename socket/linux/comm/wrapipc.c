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

// posix message queue
mqd_t Mq_open(const char *ipcName, int oflag, ...)
{
    mqd_t mqd;

    if (oflag & O_CREAT)
    {
        va_list vl;
        va_start(vl, oflag);
        mode_t mode = va_arg(vl, mode_t);
        struct mq_attr *attr = va_arg(vl, struct mq_attr *);
        va_end(vl);

        if ((mqd = mq_open(ipcName, oflag, mode, attr)) == -1)
        {
            err_sys("mq_open error");
        }
    }
    else
    {
        if ((mqd = mq_open(ipcName, oflag)) == -1)
        {
            err_sys("mq_open error");
        }
    }
    return mqd;
}

void Mq_close(mqd_t mqd)
{
    if (mq_close(mqd) == -1)
    {
        err_sys("mq_close error");
    }
}

void Mq_unlink(const char *ipcName)
{
    if (mq_unlink(ipcName) == -1)
    {
        err_sys("mq_unlink error");
    }
}

void Mq_getattr(mqd_t mqd, struct mq_attr *attr)
{
    if (mq_getattr(mqd, attr) == -1)
    {
        err_sys("mq_getattr error");
    }
}

void Mq_setattr(mqd_t mqd, const struct mq_attr *attr, struct mq_attr *oattr)
{
    if (mq_setattr(mqd, attr, oattr) == -1)
    {
        err_sys("mq_setattr error");
    }
}

void Mq_send(mqd_t mqd, const char *ptr, size_t len, unsigned int prio)
{
    if (mq_send(mqd, ptr, len, prio) == -1)
    {
        err_sys("mq_send error");
    }
}

size_t Mq_receive(mqd_t mqd, char *ptr, size_t len, unsigned int *prio)
{
    ssize_t n;
    if ((n = mq_receive(mqd, ptr, len, prio)) == -1)
    {
        err_sys("mq_receive error");
    }
    return n;
}

void Mq_notify(mqd_t mqd, const struct sigevent *notification)
{
    if (mq_notify(mqd, notification) == -1)
    {
        err_sys("mq_notify error");
    }
}