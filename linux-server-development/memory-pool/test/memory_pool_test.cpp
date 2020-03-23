#include "memory_pool_test.h"

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