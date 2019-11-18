#include "test.h"
#include "io.h"
#include "multi_threads.h"

class Test
{

};

void simpleTest()
{
    // const char * const p = nullptr;//这里才是真正的常量的指针
    // cout <<sizeof(Test) << endl;//空的类由于有特殊的实现，size是1
    // cout << __cplusplus << endl;
}


void test()
{
    // synthesizeCopyConstructTest();
    // deleteTest();
    // valueClassTest();
    // pointerClassTest();
    // sizeofTest();
    // virtualFunTest();
    // inheritanceCompositionCtorDtorTest();
    // vectorPushBackTest();
    // strVecTest();
    // strVecMoveTest();
    chineseStringTest();
    cinTest();
    getlineTest();
    // memberAccessOperatorOverloadTest();
    // stlFunctionObjectTest();
    // customizeClassInMap();
    // bindFunctionObjectTest();
    // typeConversionOperatorOverloadTest();
    // oopStaticTest();
    // oopVirtualFunTest();
    // scopeVirtualFunTest();
    
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
}