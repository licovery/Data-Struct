class Library
{
public:
    // 稳定的整体流程
    void run()
    {
        step1();
        if (step2()) //支持变化 ==> 虚函数的多态调用
        {
            step3();
        }
        for (int i = 0; i < 4; i++)
        {
            step4(); //支持变化 ==> 虚函数的多态调用
        }
        step5();
    }

    virtual ~Library() {}

protected:
    // 稳定的子步骤
    void step1() {}
    void step3() {}
    void step5() {}

    // 变化的子步骤
    virtual bool step2() = 0;
    virtual void step4() = 0;
};