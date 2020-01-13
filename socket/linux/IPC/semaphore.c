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


#include <semaphore.h>
