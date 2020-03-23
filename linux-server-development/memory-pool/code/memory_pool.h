#include <sys/queue.h>

#include <vector>

#include "config.h"

constexpr int MAX_STR_LEN = 64;
constexpr int RESERVE_BYTE = 8;
constexpr char PROTECT_FIELD[RESERVE_BYTE] = {0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};
constexpr int MEM_OFFSET = RESERVE_BYTE + sizeof(void *);
constexpr int MEM_EXTRA_SIZE = 2 * RESERVE_BYTE + sizeof(void *);
constexpr int SUCC = 0;
constexpr int FAIL = -1;

//作为全局变量共享
struct MemoryAddr
{
    void *controlStart;
    // int controlSize;
    void *controlCur;

    void *dataStart;
    // int dataSize;
    void *dataCur;
};

//前置声明
struct MemControlNode;
struct MemBlockManager;

//内存块头部
struct MemBlockHead
{
    MemBlockHead(MemControlNode *ctrlNode);

    char m_reserveHead[RESERVE_BYTE]; //保护字段
    MemControlNode *ctrlNode;         //索引到控制节点
};

//内存块尾部
struct MemBlockTail
{
    MemBlockTail();

    char m_reserveTail[RESERVE_BYTE]; //保护字段
};

/*
内存布局说明
MemBlockHead:
    0x5A (8bytes)
    void *
Payload:
    ...
    ...
    ...
MemBlockTail:
    0x5A (8bytes)
*/

//一个真正的内存块，包含头部，payload，尾部
struct MemBlock
{
    static void *construct(int size, MemControlNode *ctrlNode);
    static int countMemory(int size);
};

//调试信息
struct DebugInfo
{
    DebugInfo();

    char m_file[MAX_STR_LEN]; //文件名
    char m_func[MAX_STR_LEN]; //函数名
    int m_line;               //行号
};

//内存块控制节点
struct MemControlNode
{
    MemControlNode(MemBlockManager *manager);
    static MemControlNode *construct(MemBlockManager *manager);
    static int countMemory();

    bool m_used;                //空闲使用标志
    DebugInfo m_debuginfo;      //调试信息
    void *m_ptr;                //数据内存块的地址
    MemBlockManager *m_manager; //归属内存块管理器

    TAILQ_ENTRY(MemControlNode)
    entry;
};

//内存管理器，管理特定类型，特定大小的所有内存块
TAILQ_HEAD(ControlList, MemControlNode);
struct MemBlockManager
{
    MemBlockManager(int type, int size, int total);
    static MemBlockManager *construct(int type, int size, int total);
    static void countMemory(int size, int total, int &cSize, int &dSize);

    int m_type;                  //类型
    int m_size;                  //内存块大小
    int m_total;                 //总数量
    int m_free;                  //空闲内存块数量
    MemControlNode *m_ctrlNodes; //控制节点数组
    ControlList m_freeList;      //空闲内存块队列
    ControlList m_usedList;      //占用内存卡队列
    TAILQ_ENTRY(MemBlockManager) // 同类型不同大小的内存管理器连接起来
    entry;
};

//内存池
TAILQ_HEAD(BlockMangerList, MemBlockManager);
struct MemPool
{
    MemPool(const std::vector<Config> &configs);
    void init();
    void destory();
    ~MemPool();

    std::vector<Config> m_configs;                         //用户配置的内存池
    bool m_valid;                                          //配置是否合法
    void *m_buffer;                                        //真正向系统申请的内存
    int m_bufferSize;                                      //buffer大小
    int m_ctrlSize;                                        //控制区域大小
    int m_dataSize;                                        //数据区域大小
    BlockMangerList m_typeIndex[static_cast<int>(N_TYPE)]; //所有类型对应的内存管理器队列

private:
    void countMemory(const std::vector<Config> &configs);
};

void *getMem(int type, int size);
int retMem(int type, void *ptr);
