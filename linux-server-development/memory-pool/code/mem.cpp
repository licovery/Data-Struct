#include "mem.h"

//MemPoolSingleton
MemPool MemPoolSingleton::m_memPool;

// API

int Init(const std::vector<Config> &configs)
{
    return MemPoolSingleton::getInst()->init(configs);
}

void *getMem(int type, int size, const char *fileName, const char *funcName, int line)
{
    return MemPoolSingleton::getInst()->getMem(type, size, fileName, funcName, line);
}

int RetMem(void *ptr)
{
    return MemPoolSingleton::getInst()->retMem(ptr);
}
