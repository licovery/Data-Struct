#include "test.h"
#include "io_lib.h"
#include "multi_threads.h"
#include "thread_safe_single_instance.h"

#include <iostream>
using std::cout;
using std::endl;

class Test
{
    
};

void simpleTest()
{
    unsigned int a = 5;
    int b = -1;
    //if (a < b) 为真，注意！！因为signed和unsigned比较，signed要先转换为unsigned

    // const char * const p = nullptr;//这里才是真正的常量的指针
    // cout <<sizeof(Test) << endl;//空的类由于有特殊的实现，size是1
    // cout << __cplusplus << endl;
}


void test()
{
    // synthesizeCopyConstructTest();
    // valueClassTest();
    // pointerClassTest();
    // sizeofTest();
    // virtualFunTest();
    // inheritanceCompositionCtorDtorTest();
    // vectorPushBackTest();
    // strVecTest();
    strVecMoveTest();
    // chineseStringTest();
    // cinTest();
    // getlineTest();
    // memberAccessOperatorOverloadTest();
    // stlFunctionObjectTest();
    // customizeClassInMap();
    // bindFunctionObjectTest();
    // typeConversionOperatorOverloadTest();
    // oopStaticTest();
    // oopVirtualFunTest();
    // scopeVirtualFunTest();
    // threadSafeTest();
    // funTemplateTest();
    // classTemplateTest();
    // templateFriendTest();
    // templateStaticTest();
    // templateParaTest();
    // memberTemplateTest();
    // typeTransformationTest();
    // templateOverloadTest();
    // variadicTemplateTest();
    // templateSpecializationTest();
    // vptrVtblTest();
    // simpleTest();
    // newDeleteTest();
    // classNewDeleteTest();
    // placementNewTest();
    // tryCatchTest();
}