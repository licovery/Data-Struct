#include <string>
#include <vector>
#include <iostream>
#include "kmp.h"
#include "bm.h"
#include "rk.h"
#include "timer.h"

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

void timerTest()
{
    TimerManager timerManager;
    timerManager.setTimer(0); //无效
    timerManager.setTimer(1);
    timerManager.setTimer(30);
    timerManager.setTimer(59);
    timerManager.setTimer(60);
    timerManager.setTimer(61);
    timerManager.setTimer(65);
    timerManager.setTimer(120);
    timerManager.debug();
    timerManager.tick();
    timerManager.debug();
    for(int i = 0; i < 59; i++)
    {
        timerManager.tick();
    }
    timerManager.debug();
}