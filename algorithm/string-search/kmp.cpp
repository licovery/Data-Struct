#include "kmp.h"

vector<int> getNext(const string &p)
{
	vector<int> next(p.size(), 0);
	next[0] = -1;
	int i = 0, j = -1;

	while (i < p.size())
	{
		if (j == -1 || p[i] == p[j])
		{
			++i;
			++j;
            if (i < p.size())
            {
                next[i] = j;
            }
		}	
		else
		{
			j = next[j];
		}
	}
	return next;
}

int kmpSearch(const string &t, const string &p) 
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

	int i = 0; 
	int j = 0;//j表示已经匹配字符个数，恰好p[j]可以表示下一个待匹配的字符，非常巧妙

	vector<int> next = getNext(p);

	while (i < tLen && j < pLen)
	{
		if (j == -1 || t[i] == p[j]) //j = -1证明就是p和t的首次匹配就失败了，那么i++，j应该变成0; J=-1,J++刚好实现了这个效果
		{
			i++;
       		j++;
		}
	 	else 
	 	{
       		j = next[j];
    	}
    }

    //结束循环，如果是因为j == p.size()退出循环的，那么已经匹配成功，否则匹配失败
    if (j == pLen)
    {
       return i - j;
    }
    else
    {
       return -1;
    }
}


