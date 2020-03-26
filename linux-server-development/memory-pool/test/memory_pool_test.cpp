#include "memory_pool_test.h"
#include "mem.h"

TEST_F(ConfigTest, valid_config)
{
    std::vector<Config> testConf = {{0, 128, 400},
                                    {1, 1, 1}};

    EXPECT_TRUE(Config::isConfigValid(testConf));
}

TEST_F(ConfigTest, invalid_type_config)
{
    std::vector<Config> testConf = {{-1, 1, 1}};
    EXPECT_FALSE(Config::isConfigValid(testConf));
}

TEST_F(ConfigTest, invalid_size_config)
{
    std::vector<Config> testConf = {{1, -1, 1}};
    EXPECT_FALSE(Config::isConfigValid(testConf));
}

TEST_F(ConfigTest, invalid_total_config)
{
    std::vector<Config> testConf = {{1, 1, -1}};
    EXPECT_FALSE(Config::isConfigValid(testConf));
}

TEST_F(ConfigTest, same_item_config)
{
    std::vector<Config> testConf = {{2, 1, 5},
                                    {1, 1, 10},
                                    {1, 2, 15},
                                    {1, 1, 20}};

    Config::combineSameItem(testConf);
    std::vector<Config> conf = {{1, 1, 30},
                                {1, 2, 15},
                                {2, 1, 5}};
    EXPECT_EQ(conf, testConf);
}

TEST_F(MemPoolTest, count_memory_single)
{
    countMemTest({{1, 128, 1000}});
}

TEST_F(MemPoolTest, count_memory_muli)
{
    countMemTest({{1, 128, 1000},
                  {1, 256, 200}});
}

TEST_F(MemPoolTest, count_memory_muli2)
{
    countMemTest({{1, 128, 1000},
                  {1, 256, 200},
                  {2, 32, 500},
                  {2, 128, 100},
                  {3, 128, 200}});
}

TEST_F(MemPoolTest, init)
{
    initTest({{1, 128, 1000}});
}

TEST_F(MemPoolTest, init1)
{
    initTest({{1, 128, 1000},
              {1, 256, 200}});
}

TEST_F(MemPoolTest, init2)
{
    initTest({{1, 128, 1000},
              {1, 256, 200},
              {2, 32, 500},
              {2, 128, 100},
              {3, 128, 200}});
}

TEST_F(MemPoolTest, get_ret_protect_field)
{
    initPool({{1, 1, 1}});
    void *ptr = get(1, 1);
    *(int *)ptr = 0; //写了四个字节
    EXPECT_EQ(FAIL, ret(ptr));
}

TEST_F(MemPoolTest, get_ret_1)
{
    initPool({{1, 1, 1}});
    const int N = 100;
    void *ptr = nullptr;
    //重复申请释放
    for (int i = 0; i < N; i++)
    {
        ptr = get(1, 1);
        ASSERT_NE(nullptr, ptr);
        *(char *)ptr = 'a';
        int status = ret(ptr);
        ASSERT_EQ(SUCC, status);
    }

    ptr = get(1, 2);
    ASSERT_EQ(nullptr, ptr);
    ptr = get(2, 1);
    ASSERT_EQ(nullptr, ptr);
    EXPECT_TRUE(isAllMemBlockFree());
}

TEST_F(MemPoolTest, get_ret_2)
{
    verfiyGetRet({{0, 1, 100}});
}

TEST_F(MemPoolTest, get_ret_3)
{
    std::vector<void *> vPtr;

    initPool({{0, 1, 100},
              {0, 2, 100},
              {0, 4, 100}});

    void *ptr = nullptr;
    for (int i = 0; i < 300; i++)
    {
        ptr = get(0, 1);
        ASSERT_NE(nullptr, ptr);
        vPtr.push_back(ptr);
    }

    EXPECT_FALSE(isAllMemBlockFree());
    EXPECT_EQ(nullptr, get(0, 1));

    for (int i = 0; i < vPtr.size(); i++)
    {
        ASSERT_EQ(SUCC, ret(vPtr[i]));
    }
    EXPECT_TRUE(isAllMemBlockFree());
}

TEST_F(MemPoolTest, get_ret_4)
{
    verfiyGetRet({{0, 1, 100},
                  {0, 2, 100},
                  {0, 4, 100}});
}

TEST_F(MemPoolTest, get_ret_5)
{
    verfiyGetRet({{0, 1, 100},
                  {0, 2, 100},
                  {0, 4, 100},
                  {1, 128, 1000},
                  {2, 1024, 200}});
}

TEST_F(MemPoolTest, get_ret_api)
{

    Init({{0, 4, 1}});
    void *ptr = GetMem(0, 4);
    ASSERT_NE(nullptr, ptr);
    ASSERT_EQ(SUCC, RetMem(ptr));
}
