#include <cstdlib>

int main()
{
    // C，malloc只申请不释放，内存泄漏
    char *p = (char *)malloc(10 * sizeof(char));
    return 0;
}