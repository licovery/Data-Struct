#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int t = atoi(argv[1]);
    printf("hello this process will end after %ds\n", t);
    while (t)
    {
        printf("%d\n", t--);
        sleep(1);
    }
    printf("end\n");
    return 0;
}