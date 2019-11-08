#include <string>
#include <vector>
#include <iostream>
#include "kmp.h"
#include "bm.h"
#include "rk.h"

using namespace std;

void kmpTest()
{
    string text = "abcaacbabbacab";
	string pattern = "abcbab";
	cout << kmpSearch(text, pattern) << endl;
}

void bmTest()
{
    string text = "abcaacbabbacab";
	string pattern = "abcbab";
	cout << bmSearch(text, pattern) << endl;
}

void rkTest()
{
    string text = "abcaacbabbacab";
	string pattern = "abcbab";
	cout << rkSearch(text, pattern) << endl;
}