#define DEBUG

#include <stdio.h>
#include "types.hpp"
#include "tctpl.hpp"
#include<string.h>


#define FALSE_PRINT_BREAK(name, A,B)\
Print("比较结果", "name", name, "A", A, "B", B);\
if (!(A == B))\
{\
    break;\
}
#define FALSE_NOPRINT_BREAK(name, A,B)\
if (!(A == B))\
{\
    break;\
}
#define FALSE_PRINT_STRCMP_BREAK(name, A,B)\
Print("比较结果", "name", name, "A", A, "B", B);\
if (strcmp(A, B) != 0)\
{\
    break;\
}

//======================================================
struct testValue {
    int value1;
    tc::BInt value2;
    int value3;
};
TC_STRUCT(testValue,
    TC_FIELD_NAME(value1, "value1"),
    TC_FIELD_NAME(value2, "value2"),
    TC_FIELD_NAME(value3, "value3"));
//======================================================
class WasmCPlusAPICallSubCont : public TCBaseContract {
public:
public:
    virtual ~WasmCPlusAPICallSubCont() {}
    const char* Init(){TC_Prints("ClassTest1 Init");return "init ok";};
    const char* fnTest1() {TC_Prints("fnTest1 Init");return "fnTest1 ok";};
    const char* fnTest11(const char* data) {Print("fnTest11", "data", data);return "fnTest11 ok";};
    const char* fnTest111(testValue data) {Print("fnTest111", "data.value1", data.value1, "data.value2", data.value2, "data.value3", data.value3);return "fnTest111 ok";};
    const char* fnTest2() {TC_Prints("fnTest2 Init");return "fnTest2 ok";};
    const char* fnTest22(const char* data) {Print("fnTest22", "data", data);return "fnTest22 ok";};
    const char* fnTest222(testValue data) {Print("fnTest222", "data.value1", data.value1, "data.value2", data.value2, "data.value3", data.value3);return "fnTest222 ok";};    
};

TC_ABI(WasmCPlusAPICallSubCont,     (fnTest1)\
                                    (fnTest11)\
                                    (fnTest111)\
                                    (fnTest2)\
                                    (fnTest22)\
                                    (fnTest222)\
                                    )



