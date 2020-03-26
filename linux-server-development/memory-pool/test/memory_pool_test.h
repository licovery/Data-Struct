#ifndef MEMORY_POOL_TEST_H
#define MEMORY_POOL_TEST_H

#include <gtest/gtest.h>
#include "memory_pool.h"

class ConfigTest : public testing::Test
{
public:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

class MemPoolTest : public testing::Test
{
public:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    void countMemTest(const std::vector<Config> &configs)
    {
        MemPool memPool(configs);
        EXPECT_TRUE(memPool.m_init);

        int ctrlSize = 0;
        int dataSize = 0;

        for (int i = 0; i < configs.size(); i++)
        {
            ctrlSize += sizeof(MemBlockManager) + configs[i].total * sizeof(MemControlNode);
            dataSize += configs[i].total * (configs[i].size + MEM_EXTRA_SIZE);
        }

        EXPECT_EQ(ctrlSize, memPool.m_ctrlSize);
        EXPECT_EQ(dataSize, memPool.m_dataSize);
    }

    void initTest(const std::vector<Config> &configs)
    {
        MemPool memPool(configs);
        int lastType = -1;
        MemBlockManager *manager = nullptr;
        for (int i = 0; i < memPool.m_configs.size(); i++)
        {
            EXPECT_FALSE(TAILQ_EMPTY(&memPool.m_typeIndex[memPool.m_configs[i].type]));

            if (lastType == memPool.m_configs[i].type)
            {
                manager = TAILQ_NEXT(manager, entry);
            }
            else
            {
                manager = TAILQ_FIRST(&memPool.m_typeIndex[memPool.m_configs[i].type]);
                lastType = memPool.m_configs[i].type;
            }

            verifyMemBlockManager(memPool.m_configs[i].type,
                                  memPool.m_configs[i].size,
                                  memPool.m_configs[i].total,
                                  manager);
        }
        memPool.destory();
    }

    void verifyMemBlockManager(int type, int size, int total, MemBlockManager *manager)
    {
        EXPECT_EQ(type, manager->m_type);
        EXPECT_EQ(size, manager->m_size);
        EXPECT_EQ(total, manager->m_total);
        EXPECT_EQ(total, manager->m_free);
        EXPECT_TRUE(TAILQ_EMPTY(&manager->m_usedList));
        EXPECT_FALSE(TAILQ_EMPTY(&manager->m_freeList));
        int n = 0;
        MemControlNode *ctrlNode = nullptr;
        TAILQ_FOREACH(ctrlNode, &manager->m_freeList, entry)
        {
            //MemControlNode
            EXPECT_EQ(false, ctrlNode->m_used);
            EXPECT_EQ(manager, ctrlNode->m_manager);
            //MemBlock
            MemBlockHead *memBlockHead = static_cast<MemBlockHead *>(ctrlNode->m_ptr);
            EXPECT_EQ(ctrlNode, memBlockHead->ctrlNode);
            EXPECT_EQ(0, memcmp(memBlockHead->m_reserveHead, PROTECT_FIELD, RESERVE_BYTE));
            MemBlockTail *memBlockTail = reinterpret_cast<MemBlockTail *>(static_cast<char *>(ctrlNode->m_ptr) + sizeof(MemBlockHead) + size);
            EXPECT_EQ(0, memcmp(memBlockTail->m_reserveTail, PROTECT_FIELD, RESERVE_BYTE));
            n++;
        }
        EXPECT_EQ(total, n);
    }

    // 全部申请，然后全部释放，检查是否全部归还成功
    void verfiyGetRet(const std::vector<Config> &configs)
    {
        std::vector<void *> vPtr;
        initPool(configs);
        void *ptr = nullptr;

        for (Config config : configs)
        {
            for (int i = 0; i < config.total; i++)
            {
                ptr = get(config.type, config.size);
                ASSERT_NE(nullptr, ptr);
                vPtr.push_back(ptr);
            }
        }

        for (Config config : configs)
        {
            EXPECT_FALSE(isAllMemBlockFree());
            EXPECT_EQ(nullptr, get(config.type, config.size));
        }

        for (int i = 0; i < vPtr.size(); i++)
        {
            ASSERT_EQ(SUCC, ret(vPtr[i]));
        }
        EXPECT_TRUE(isAllMemBlockFree());

        // 重复一遍上面的流程，再执行一遍
        vPtr.clear();
        for (Config config : configs)
        {
            for (int i = 0; i < config.total; i++)
            {
                ptr = get(config.type, config.size);
                ASSERT_NE(nullptr, ptr);
                vPtr.push_back(ptr);
            }
        }

        for (Config config : configs)
        {
            EXPECT_FALSE(isAllMemBlockFree());
            EXPECT_EQ(nullptr, get(config.type, config.size));
        }

        for (int i = 0; i < vPtr.size(); i++)
        {
            ASSERT_EQ(SUCC, ret(vPtr[i]));
        }
        EXPECT_TRUE(isAllMemBlockFree());
    }

    void initPool(const std::vector<Config> &configs)
    {
        memPool.init(configs);
    }

    void *get(int type, int size)
    {
        return memPool.getMem(type, size);
    }

    int ret(void *ptr)
    {
        return memPool.retMem(ptr);
    }

    bool isAllMemBlockFree()
    {
        for (int i = 0; i < ARRAY_LEN(memPool.m_typeIndex); i++)
        {
            MemBlockManager *manager = nullptr;
            TAILQ_FOREACH(manager, &memPool.m_typeIndex[i], entry)
            {
                if (manager->m_free != manager->m_total)
                {
                    return false;
                }
            }
        }
        return true;
    }

private:
    MemPool memPool;
};

#endif