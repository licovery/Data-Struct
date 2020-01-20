#include "comm.h"

int count;

void signal_handler(int m)
{
    count++;
    printf("%d\n", count);
}

int main()
{
    Signal(SIGALRM, signal_handler);
    struct itimerval itv;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &itv, NULL);

    while (1)
    {
        sleep(1);
    }

    return 0;
}