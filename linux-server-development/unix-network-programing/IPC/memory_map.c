#include "comm.h"

#define FILENAME "/tmp/memorymap"

void fileMemoryMap()
{
    // 打开一个文件
    int fd = Open(FILENAME, O_RDWR | O_CREAT | O_TRUNC, FILE_MODE);
    char buf[MAX_BUF_SIZE] = {0};
    Write(fd, buf, 100); // 文件的大小是100字节
    // mmap映射到内存
    // 建立映射时的size可以大于文件的大小，但是访问的时候需要保证不超过文件的真实大小
    void *ptr = Mmap(NULL, MAX_BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    Close(fd);

    snprintf(buf, MAX_BUF_SIZE, "hello world");
    strcpy(ptr, buf);

    // 强制刷新同步映射内存和文件
    Msync(ptr, MAX_BUF_SIZE, MS_SYNC);
    // 移除映射
    Munmap(ptr, MAX_BUF_SIZE);
}

// 匿名文件映射flag加上MAP_ANONYMOUS,fd=-1，适合有亲缘关系的进程间IPC
void anonymousMemoryMap()
{
    void *ptr = Mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    *(int *)ptr = 512;

    Munmap(ptr, sizeof(int));
}

int main()
{
    fileMemoryMap();
    anonymousMemoryMap();
    return 0;
}