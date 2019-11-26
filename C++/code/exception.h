#ifndef EXCEPTION_H
#define EXCEPTION_H

namespace lf_exc
{

void throwTest(int index);
void exceptionTest(int index);

class Foo
{
public:
    Foo(int i = 0);
    ~Foo();
private:
    int a;
};

} // lf_exc



#endif