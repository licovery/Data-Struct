#include "comm.h"

#define CHILD_PROCESS_NUM 2

int main(int argc, char *argv[])
{
    int pid = 0;
    for (int i = 0; i < CHILD_PROCESS_NUM; i++)
    {
        pid = Fork();
        if (pid == 0)//子进程
        {
            printf("child process start\n");
            //一个进程一旦调用exec系列函数，它本身就相当于死亡了，
            //系统会把代码段替换成新的程序的代码，废弃原有的数据段和堆栈段，并为新程序分配新的数据段和堆栈段。
            execl("call_process", "call_process", "10", NULL);//参数就是main函数参数，注意有arg0
            //第一个参数路径指的是程序运行的路径，即不是源文件路径也不是二进文件的路径是shell执行时候的路径
            //返回即出错了
            perror("exec error");
            exit(errno);
        }//子进程作用范围结束了
        else
        {
            printf("create child process %d\n", pid);
        }
        
    }

    //这里是父进程，等待子进程结束，并回收资源
    printf("I am main process, waiting for child process terminated\n");
    time_t start = time(NULL);
    for (int i = 0; i < CHILD_PROCESS_NUM; i++)
    {
        pid = Wait(NULL);
        printf("I am main process, child process %d run %lds and terminated\n", pid, time(NULL) - start);
    }
    
    return 0;
}