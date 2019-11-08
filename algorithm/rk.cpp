#include "rk.h"

constexpr int MODULE = 9973;//一个大的素数
constexpr int CHARSET_SIZE = 256;//字符串看成256进制数字

int stringHash(const string &s)
{
    int res = 0;
    for (char c : s)
    {
        res = (res * CHARSET_SIZE + c) % MODULE;
    }
    return res;
}

//主串相邻字符串hash值有递推规律
// x(i+1) = (x(i) - t(i)*R^(M-1))*R + t(i+M)

int rkSearch(const string &t, const string &p)
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
    //R ^ (M - 1) % Q
    int rM_1 = 1;
    for (int i = 0; i < pLen - 1; ++i)
    {
        rM_1 = (rM_1 * CHARSET_SIZE) % MODULE;
    }

    int patHash = stringHash(p);
    int textHash = stringHash(t.substr(0, pLen));

    for (int i = 0; i + pLen - 1 < tLen; ++i)
    {
        if (patHash == textHash && p == t.substr(i, pLen))
        {
            return i;
        }
        //通过递推公式计算下一个待匹配串的hash
        textHash = ((textHash - t[i] * rM_1) * CHARSET_SIZE + t[i + pLen]) % MODULE;
        textHash = (textHash + MODULE) % MODULE;
    }
    return -1;
}