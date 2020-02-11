#include "timer.h"
#include <vector>
#include <iostream>

Timer::Timer(int r, int s, CallBackFunc *func, void *d):rotation(r), slot(s), callbackFunc(func), data(d)
{

}

TimerManager::TimerManager()
{
    curslot = 0;
}

TimerManager::~TimerManager()
{
    for (int i = 0; i < N; i++)
    {
        for (auto it = slots[i].begin(); it != slots[i].end(); it++)
        {
            if (*it != nullptr)
            {
                delete *it;
            }
        }
    }
}

Timer * TimerManager::setTimer(int timeout, CallBackFunc *func, void *args)
{
    if (timeout < 0) // 设定了无意义的超时值
    {
        return NULL;
    }
    int tick = timeout / TI;
    int rotation = tick / N;
    int slot = (curslot + tick) % N;
    Timer *timer = new Timer(rotation, slot, func, args);
    slots[slot].push_back(timer);
    return timer;
}

void TimerManager::removeTimer(Timer *timer)
{
    if (timer == nullptr)
    {
        return;
    }
    slots[timer->slot].remove(timer);
    delete timer;
}

void TimerManager::tick()
{
    std::vector<Timer *> deleteTimers;

    for (auto it = slots[curslot].begin(); it != slots[curslot].end(); it++) 
    {
        Timer *curTimer = *it;
        
        if (curTimer->rotation == 0) // 已经超时
        {
            deleteTimers.push_back(curTimer); // 加入待删除列表
            if (curTimer->callbackFunc) // 如果已经设置超时处理函数
            {
                curTimer->callbackFunc(curTimer->data); // 调用超时处理函数
            }
        }
        else
        {
            curTimer->rotation--;
        }
    }

    for (auto timer : deleteTimers)
    {
        removeTimer(timer);
    }

    // 先处理超时，再拨动slot指针
    // 如果先curslot先增加，再处理超时，处理tick为 0, N, 2N的定时器就会延后一轮
    curslot = (curslot + 1) % N;
}


void TimerManager::debug()
{
    std::cout << "curSlot: " << curslot << std::endl;
    for (int i = 0; i < N; i++)
    {
        if (slots[i].size() == 0)
        {
            continue;
        }
        std::cout << "slot: " << i << " ";
        for (auto it = slots[i].begin(); it != slots[i].end(); it++)
        {
            Timer *timer = *it;
            std::cout << "(" << timer->rotation << ", " << timer->slot << ") "; 
        }
        std::cout << std::endl;
    }
}

