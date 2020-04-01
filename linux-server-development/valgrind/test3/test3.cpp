
int main()
{
    int *p = new int[10];
    // 越界访问
    p[10] = 0;
    delete[] p;
    return 0;
}