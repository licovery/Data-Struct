#ifndef BM_H
#define BM_H

#include <vector>
#include <string>

using namespace std;



vector<int> getRight(const string &p);//某个字符在模式串中最后一次出现的位置，若字符不在模式串中，则为-1
int bmSearch(const string &t, const string &p);

#endif