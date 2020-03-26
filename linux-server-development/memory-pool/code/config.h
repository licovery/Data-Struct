#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <algorithm>

enum MemType
{
    INST,
    BUFF,
    IO,
    DRIVE,
    SERVICE,
    N_TYPE
};

struct Config
{
    int type;
    int size;
    int total;

    static void combineSameItem(std::vector<Config> &configs)
    {
        std::sort(configs.begin(), configs.end());
        for (int i = 0; i + 1 < configs.size(); i++)
        {
            if (configs[i].type == configs[i + 1].type &&
                configs[i].size == configs[i + 1].size)
            {
                configs[i].total += configs[i + 1].total;
                configs.erase(configs.begin() + i + 1);
            }
        }
    }

    static bool isConfigValid(const std::vector<Config> &configTable)
    {
        if (configTable.size() == 0)
        {
            return false;
        }

        for (int i = 0; i < configTable.size(); i++)
        {
            if (!isConfigValid(configTable[i]))
            {
                return false;
            }
        }

        return true;
    }

    static bool isConfigValid(const Config &config)
    {
        if (0 <= config.type &&
            config.type < static_cast<int>(N_TYPE) &&
            0 <= config.size &&
            0 <= config.total)
        {
            return true;
        }
        return false;
    }
};

inline bool operator<(const Config &c1, const Config &c2)
{
    if (c1.type < c2.type)
    {
        return true;
    }
    else if (c1.type > c2.type)
    {
        return false;
    }
    else
    {
        return c1.size < c2.size;
    }
}

inline bool operator==(const Config &c1, const Config &c2)
{
    return c1.type == c2.type &&
           c1.size == c2.size &&
           c1.total == c2.total;
}

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

#endif