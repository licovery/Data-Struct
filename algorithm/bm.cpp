#include "bm.h"

constexpr int CHARSET_SIZE = 256;//字符集大小

vector<int> getRight(const string &p)
{
    vector<int> right(CHARSET_SIZE, -1);
    for (int i = 0; i < p.size(); ++i)
    {
        right[p[i]] = i;
    }
    return right;
}

int bmSearch(const string &t, const string &p)
{
    int tLen = t.size();
    int pLen = p.size();
    if (pLen == 0)//任何串都包括空串
    {
        return 0;
    }
    if (tLen == 0)//主串为空，模式串不空
    {
        return -1;
    }

    vector<int> right = getRight(p);
    int shift = 0;

    for (int i  = 0; i + pLen - 1 < tLen; i += shift)
    {
        int j = pLen - 1;
        for (; j >= 0; j--)
        {
            if (t[i + j] != p[j])//匹配失败，模式串要右移多少位继续比较
            {
                shift = j - right[t[i + j]];
                if (shift < 1)
                {
                    shift = 1;
                }
                break;
            }
        }
        if (j < 0)
        {
            return i;
        }
    }
    return -1;
}