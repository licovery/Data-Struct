#include "comm.h"

#define SHARED_MEMORY_NAME "/sharedmemory"
#define SHARED_MEMORY_SIZE 4096
#define MAX_LOOP 1024

void CreateSharedMemoryAndSemphore()
{
    int fd = Shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT, FILE_MODE);
    Ftruncate(fd, SHARED_MEMORY_SIZE);

    void *ptr = Mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    Close(fd);

    sem_t *sharedSem = (sem_t *)ptr;
    Sem_init(sharedSem, 1, 1);

    Munmap(ptr, SHARED_MEMORY_SIZE);
}

void DestorySharedMemoryAndSemphore()
{
    int fd = Shm_open(SHARED_MEMORY_NAME, O_RDWR, 0);
    void *ptr = Mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    Close(fd);
    Sem_destory((sem_t *)ptr);
    Munmap(ptr, SHARED_MEMORY_SIZE);
    Shm_unlink(SHARED_MEMORY_NAME);
}

void WriteSharedMemory()
{
    int fd = Shm_open(SHARED_MEMORY_NAME, O_RDWR, 0);
    struct stat info = {0};
    Fstat(fd, &info);
    printf("memory size:%ld\n", info.st_size);
    void *ptr = Mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    Close(fd);

    unsigned char *curByte = (unsigned char *)ptr;
    sem_t *shareSem = (sem_t *)curByte;
    curByte += sizeof(sem_t);

    Sem_wait(shareSem);
    for (int i = 0; i < MAX_LOOP; i++)
    {
        *curByte++ = i % 256;
    }
    printf("write spend 5s\n");
    sleep(5);
    Sem_post(shareSem);

    Munmap(ptr, SHARED_MEMORY_SIZE);
}

void ReadSharedMemory()
{
    int fd = Shm_open(SHARED_MEMORY_NAME, O_RDWR, 0);
    void *ptr = Mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    struct stat info = {0};
    Fstat(fd, &info);
    printf("memory size:%ld\n", info.st_size);
    Close(fd);

    unsigned char *curByte = (unsigned char *)ptr;
    sem_t *shareSem = (sem_t *)curByte;
    curByte += sizeof(sem_t);

    Sem_wait(shareSem);
    for (int i = 0; i < MAX_LOOP; i++)
    {
        if (*curByte++ != i % 256)
        {
            Sem_post(shareSem);
            err_quit("verify error");
        }
    }
    printf("read spend 3s\n");
    sleep(3);
    Sem_post(shareSem);

    Munmap(ptr, SHARED_MEMORY_SIZE);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("%s c/d/w/r\n", argv[0]);
        return 0;
    }

    if (argv[1][0] == 'c')
    {
        CreateSharedMemoryAndSemphore();
    }
    else if (argv[1][0] == 'd')
    {
        DestorySharedMemoryAndSemphore();
    }
    else if (argv[1][0] == 'w')
    {
        WriteSharedMemory();
    }
    else if (argv[1][0] == 'r')
    {
        ReadSharedMemory();
    }
    return 0;
}