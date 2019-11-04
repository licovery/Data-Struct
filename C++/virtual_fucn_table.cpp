#if 0

#include <cstdio>
#include <iostream>

class Base
{
public:
    Base(int mem1 = 1, int mem2 = 2) : m_iMem1(mem1), m_iMem2(mem2){ ; }

    virtual void vfunc1() { std::cout << "In vfunc1()" << std::endl; }
    virtual void vfunc2() { std::cout << "In vfunc2()" << std::endl; }
    virtual void vfunc3() { std::cout << "In vfunc3()" << std::endl; }

private:
    int m_iMem1;
    int m_iMem2;
};

int main(int argc, char* argv[])
{
    Base b;

    // 对象b的地址
    int *bAddress = (int *)&b;    

    // 对象b的vtptr的值
    int *vtptr = (int *)*(bAddress + 0);
    printf("vtptr: 0x%08x\n", vtptr);

    // 对象b的第一个虚函数的地址
    int *pFunc1 = (int *)*(vtptr + 0);
    int *pFunc2 = (int *)*(vtptr + 1);
    int *pFunc3 = (int *)*(vtptr + 2);
    printf("\t vfunc1addr: 0x%08x \n" 
           "\t vfunc2addr: 0x%08x \n" 
           "\t vfunc3addr: 0x%08x \n",
           pFunc1, 
           pFunc2, 
           pFunc3);

    // 对象b的两个成员变量的值（用这种方式可轻松突破private不能访问的限制）
    int mem1 = (int)*(bAddress + 1);
    int mem2 = (int)*(bAddress + 2);
    printf("m_iMem1: %d \nm_iMem2: %d \n\n",mem1, mem2);

    // 调用虚函数
    // (FUNC(pFunc1))();
    // (FUNC(pFunc2))();
    // (FUNC(pFunc3))();
    return 0;
}

#endif