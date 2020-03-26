#include "memory_pool.h"
#include <iostream>
#include <cstring>
#include <cassert>

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

bool MemBlock::isValid(void *ptr)
{
    MemBlockHead *head = reinterpret_cast<MemBlockHead *>(static_cast<char *>(ptr) - MEM_OFFSET);
    // 检查头部保护字段
    if (memcmp(PROTECT_FIELD, head->m_reserveHead, RESERVE_BYTE) != 0)
    {
        std::cerr << "head protect field destory !" << std::endl;
        return false;
    }

    // 检查对应控制节点
    MemControlNode *ctrlNode = reinterpret_cast<MemControlNode *>(head->ctrlNode);
    if (ctrlNode == nullptr || ctrlNode->mem() != ptr || !ctrlNode->isUsed())
    {
        std::cerr << "error ctrlNode for memory block !" << std::endl;
        return false;
    }

    // 检查尾部保护字段
    MemBlockTail *tail = reinterpret_cast<MemBlockTail *>(static_cast<char *>(ptr) + ctrlNode->blockSize());
    if (memcmp(PROTECT_FIELD, tail->m_reserveTail, RESERVE_BYTE) != 0)
    {
        std::cerr << "tail protect field destory !" << std::endl;
        return false;
    }
    return true;
}

MemControlNode *MemBlock::getCtrlNode(void *ptr)
{
    MemBlockHead *head = reinterpret_cast<MemBlockHead *>(static_cast<char *>(ptr) - MEM_OFFSET);
    return head->ctrlNode;
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

void *MemControlNode::data() const
{
    return m_ptr;
}

void *MemControlNode::mem() const
{
    return static_cast<char *>(m_ptr) + MEM_OFFSET;
}

bool MemControlNode::isUsed() const
{
    return m_used;
}

int MemControlNode::blockSize() const
{
    MemBlockManager *manager = static_cast<MemBlockManager *>(m_manager);
    assert(manager != nullptr);
    return manager->blockSize();
}

void MemControlNode::setUsed()
{
    m_used = true;
}

void MemControlNode::setFree()
{
    m_used = false;
}

void MemControlNode::setDebug(const char *fileName, const char *funcName, int line)
{
    strncpy(m_debuginfo.m_file, fileName, MAX_STR_LEN);
    strncpy(m_debuginfo.m_func, funcName, MAX_STR_LEN);
    m_debuginfo.m_line = line;
}

int MemControlNode::ret()
{
    MemBlockManager *manager = static_cast<MemBlockManager *>(m_manager);
    assert(manager != nullptr);
    manager->retMem(this);
    return SUCC;
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
    cSize = sizeof(MemBlockManager) + MemControlNode::countMemory() * total;
    dSize = MemBlock::countMemory(size) * total;
}

int MemBlockManager::blockSize() const
{
    return m_size;
}

int MemBlockManager::freeBlock() const
{
    return m_free;
}

void *MemBlockManager::getMem(const char *fileName, const char *funcName, int line)
{
    MemControlNode *ctrlNode = nullptr;

    if (m_free <= 0 && TAILQ_EMPTY(&m_freeList))
    {
        return nullptr;
    }

    ctrlNode = TAILQ_LAST(&m_freeList, ControlList);
    TAILQ_REMOVE(&m_freeList, ctrlNode, entry);
    m_free--;
    ctrlNode->setUsed();
    ctrlNode->setDebug(fileName, funcName, line);
    return ctrlNode->mem();
}

int MemBlockManager::retMem(MemControlNode *ctrlNode)
{
    TAILQ_INSERT_TAIL(&m_freeList, ctrlNode, entry);
    ctrlNode->setFree();
    m_free++;
    return SUCC;
}

// ==========MemPool==========

MemPool::MemPool()
{
    m_bufferSize = m_ctrlSize = m_dataSize = 0;
    m_buffer = nullptr;
    m_init = false;

    for (int i = 0; i < static_cast<int>(N_TYPE); i++)
    {
        TAILQ_INIT(&m_typeIndex[i]);
    }
}

// 委托构造
MemPool::MemPool(const std::vector<Config> &configs) : MemPool()
{
    init(configs);
}

int MemPool::init(const std::vector<Config> &configs)
{

    if (m_init) //已经初始化过了
    {
        std::cerr << "memory pool init before!" << std::endl;
        return FAIL;
    }

    m_configs = configs;
    if (!Config::isConfigValid(m_configs))
    {
        std::cerr << "config is invalid!" << std::endl;
        m_init = false;
        return FAIL;
    }

    Config::combineSameItem(m_configs);
    countMemory(m_configs);

    m_bufferSize = (m_ctrlSize + m_dataSize);
    m_buffer = malloc(m_bufferSize);
    if (m_buffer == nullptr)
    {
        std::cerr << "malloc memory error!" << std::endl;
        m_init = false;
        return FAIL;
    }

    memAddr.controlStart = memAddr.controlCur = m_buffer;
    memAddr.dataStart = memAddr.dataCur = static_cast<char *>(m_buffer) + m_ctrlSize;

    for (int i = 0; i < m_configs.size(); i++)
    {
        auto manager = MemBlockManager::construct(m_configs[i].type, m_configs[i].size, m_configs[i].total);
        TAILQ_INSERT_TAIL(&m_typeIndex[m_configs[i].type], manager, entry);
    }

    m_init = true;
    return SUCC;
}

void MemPool::destory()
{
    if (m_buffer)
    {
        free(m_buffer);
        m_buffer = nullptr;
    }
    m_init = false;
}

MemPool::~MemPool()
{
    if (m_buffer)
    {
        destory();
    }
}

void *MemPool::getMem(int type, int size, const char *fileName, const char *funcName, int line)
{
    if (type < 0 || type >= static_cast<int>(N_TYPE) || size <= 0)
    {
        return nullptr;
    }
    void *ptr = nullptr;
    auto managerList = m_typeIndex[type];
    MemBlockManager *manager = nullptr;
    TAILQ_FOREACH(manager, &managerList, entry)
    {
        if (manager->blockSize() >= size && manager->freeBlock() > 0)
        {
            ptr = manager->getMem(fileName, funcName, line);
            break;
        }
    }
    return ptr;
}

int MemPool::retMem(void *ptr)
{
    if (ptr == nullptr)
    {
        return FAIL;
    }

    if (!MemBlock::isValid(ptr))
    {
        return FAIL;
    }

    auto ctrlNode = MemBlock::getCtrlNode(ptr);
    return ctrlNode->ret();
}

void MemPool::countMemory(const std::vector<Config> &configs)
{
    m_ctrlSize = m_dataSize = 0;
    int c = 0;
    int d = 0;
    for (int i = 0; i < configs.size(); i++)
    {
        MemBlockManager::countMemory(configs[i].size, configs[i].total, c, d);
        m_ctrlSize += c;
        m_dataSize += d;
    }
}
