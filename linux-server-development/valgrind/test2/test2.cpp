
int main()
{
    // C++，只申请不释放，内存泄漏
    int *p = new int[10];
    return 0;
}