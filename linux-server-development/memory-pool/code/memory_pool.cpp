#include "memory_pool.h"
#include <iostream>

// 共享全局变量
MemoryAddr memAddr;

/* to do

字节对齐
单例

*/

// ==========MemBlock==========

MemBlockHead::MemBlockHead(MemControlNode *ctrlNode) : ctrlNode(ctrlNode)
{
    //memcpy
    for (int i = 0; i < RESERVE_BYTE; i++)
    {
        m_reserveHead[i] = 0x5A;
    }
}

MemBlockTail::MemBlockTail()
{
    for (int i = 0; i < RESERVE_BYTE; i++)
    {
        m_reserveTail[i] = 0x5A;
    }
}

void *MemBlock::construct(int size, MemControlNode *ctrlNode)
{
    MemBlockHead *pHead = new (memAddr.dataCur) MemBlockHead(ctrlNode);
    memAddr.dataCur = pHead + 1;

    memAddr.dataCur = static_cast<char *>(memAddr.dataCur) + size;

    MemBlockTail *pTail = new (memAddr.dataCur) MemBlockTail();
    memAddr.dataCur = pTail + 1;

    return pHead;
}

int MemBlock::countMemory(int size)
{
    return sizeof(MemBlockHead) + size + sizeof(MemBlockTail);
}

// ==========MemControlNode==========

MemControlNode::MemControlNode(MemBlockManager *manager) : m_manager(manager)
{
    m_used = false;
    m_ptr = nullptr;
}

MemControlNode *MemControlNode::construct(MemBlockManager *manager)
{
    MemControlNode *pCtrlNode = new (memAddr.controlCur) MemControlNode(manager);
    memAddr.controlCur = pCtrlNode + 1;
    return pCtrlNode;
}

int MemControlNode::countMemory()
{
    return sizeof(MemControlNode);
}

DebugInfo::DebugInfo()
{
    for (int i = 0; i < MAX_STR_LEN; i++)
    {
        m_file[i] = 0;
        m_func[i] = 0;
    }
    m_line = 0;
}

// ==========MemBlockManager==========

MemBlockManager::MemBlockManager(int type, int size, int total) : m_type(type), m_size(size), m_total(total)
{
    m_free = total;
    m_ctrlNodes = nullptr;

    TAILQ_INIT(&m_freeList);
    TAILQ_INIT(&m_usedList);
}

MemBlockManager *MemBlockManager::construct(int type, int size, int total)
{
    MemBlockManager *pBlockManger = new (memAddr.controlCur) MemBlockManager(type, size, total);
    memAddr.controlCur = pBlockManger + 1;
    pBlockManger->m_ctrlNodes = static_cast<MemControlNode *>(memAddr.controlCur);

    for (int i = 0; i < total; i++)
    {
        auto pCtrlNode = MemControlNode::construct(pBlockManger);
        pCtrlNode->m_ptr = MemBlock::construct(size, pCtrlNode);
        TAILQ_INSERT_TAIL(&pBlockManger->m_freeList, pCtrlNode, entry);
    }

    return pBlockManger;
}

void MemBlockManager::countMemory(int size, int total, int &cSize, int &dSize)
{
    cSize += sizeof(MemBlockManager) + MemControlNode::countMemory() * total;
    dSize += MemBlock::countMemory(size) * total;
}

// ==========MemPool==========

MemPool::MemPool(const std::vector<Config> &configs) : m_configs(configs)
{
    m_bufferSize = m_ctrlSize = m_dataSize = 0;
    m_buffer = nullptr;
    m_valid = false;

    for (int i = 0; i < static_cast<int>(N_TYPE); i++)
    {
        TAILQ_INIT(&m_typeIndex[i]);
    }

    if (Config::isConfigValid(m_configs))
    {
        m_valid = true;
        Config::combineSameItem(m_configs);
        countMemory(m_configs);
    }
}

void MemPool::init()
{
    if (!m_valid)
    {
        std::cerr << "config is invalid!" << std::endl;
        return;
    }

    m_bufferSize = (m_ctrlSize + m_dataSize) * 2; //先开大一点内存
    m_buffer = malloc(m_bufferSize);

    memAddr.controlStart = memAddr.controlCur = m_buffer;
    memAddr.dataStart = memAddr.dataCur = static_cast<char *>(m_buffer) + 2 * m_ctrlSize;

    for (int i = 0; i < m_configs.size(); i++)
    {
        auto manager = MemBlockManager::construct(m_configs[i].type, m_configs[i].size, m_configs[i].total);
        TAILQ_INSERT_TAIL(&m_typeIndex[m_configs[i].type], manager, entry);
    }
}

void MemPool::destory()
{
    if (m_buffer)
    {
        free(m_buffer);
        m_buffer = nullptr;
    }
}

MemPool::~MemPool()
{
    if (m_buffer)
    {
        destory();
    }
}

void MemPool::countMemory(const std::vector<Config> &configs)
{
    m_ctrlSize = m_dataSize = 0;
    for (int i = 0; i < configs.size(); i++)
    {
        MemBlockManager::countMemory(configs[i].size, configs[i].total, m_ctrlSize, m_dataSize);
    }
}
