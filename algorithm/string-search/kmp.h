#ifndef KMP_H
#define KMP_H

#include <string>
#include <vector>

using namespace std;

// t for text, p for pattern

vector<int> getNext(const string &p);
int kmpSearch(const string &t, const string &p);

#endif