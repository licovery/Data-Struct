#include "template_lib.h"

class Application : public Library
{
protected:
    virtual bool step2() override
    {
        //... 子类重写实现
        return true;
    }

    virtual void step4() override
    {
        //... 子类重写实现
    }
};

int main()
{
    Library *app = new Application;
    app->run();
    delete app;
}
