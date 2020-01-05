#ifndef TIMER_H
#define TIMER_H

#include <list>

typedef void (CallBackFunc) (void *args);

class TimerManager;

struct Timer
{
    friend class TimerManager;
public:
    Timer(int r, int s, CallBackFunc *func, void *d);
private:
    int rotation; // 轮数
    int slot;     // 槽位(0 ~ N-1)
    CallBackFunc *callbackFunc; // 回调函数
    void *data;   // 回调函数参数或其他
}; 


class TimerManager
{
public:
    TimerManager();
    Timer * setTimer(int timeout, CallBackFunc *func = nullptr, void *args = nullptr);
    void removeTimer(Timer *timer);
    void tick();
    void debug();
private:
    // static const可以不需要类外定义
    static constexpr int N = 60;
    static constexpr int TI = 1; //一个tick的时间，单位为秒，可根据实际情况修改

    int curslot;
    std::list<Timer *> slots[N];
};



#endif