#ifndef MEM_H
#define MEM_H

#include "memory_pool.h"

class MemPoolSingleton
{
public:
    static MemPool *getInst()
    {
        return &m_memPool;
    }

private:
    static MemPool m_memPool;
    MemPoolSingleton() {}
    ~MemPoolSingleton() {}
};

int Init(const std::vector<Config> &configs);
void *getMem(int type, int size, const char *fileName, const char *funcName, int line);
int RetMem(void *ptr);

#define GetMem(type, size) getMem(type, size, __FILE__, __FUNCTION__, __LINE__)

#endif