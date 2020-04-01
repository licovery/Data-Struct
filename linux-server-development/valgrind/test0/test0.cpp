#include <cstdlib>

//正常示例
int main()
{
    char *p = (char *)malloc(10 * sizeof(char));
    free(p);

    p = new char[10];
    delete[] p;

    p = new char;
    delete p;

    return 0;
}