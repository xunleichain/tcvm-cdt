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

class WasmCPlusAPICallSubCont {
    
    TC_CONTRACT(WasmCPlusAPICallSubCont)

public:
    virtual ~WasmCPlusAPICallSubCont() {}
    const char* Init(){return "init ok";}
    const char* fnTest1() {return "fnTest1 ok";}
    const char* fnTest11(const char* data) {return "fnTest11 ok";}
    const char* fnTest111(testValue data) {return "fnTest111 ok";}
    const char* fnTest2() {return "fnTest2 ok";}
    const char* fnTest22(const char* data) {return "fnTest22 ok";}
    const char* fnTest222(testValue data) {return "fnTest222 ok";}
};

class WasmCPlusAPICall : public TCBaseContract {
public:
    WasmCPlusAPICall() {}
    virtual ~WasmCPlusAPICall() {}
    const char* Init();
    template<typename T>
    const char* fnOutputResult(T t, bool success);
    const char* fnTestCallTest1(tc::Address contractAddr);
    const char* fnTestCallTest11(tc::Address contractAddr, const char* data);
    const char* fnTestCallTest111(tc::Address contractAddr, testValue data);    
    const char* fnTestCallTest2(tc::Address contractAddr);
    const char* fnTestCallTest22(tc::Address contractAddr, const char* data);
    const char* fnTestCallTest222(tc::Address contractAddr, testValue data);
};
TC_ABI(WasmCPlusAPICall,    (fnTestCallTest1)\
                            (fnTestCallTest11)\
                            (fnTestCallTest111)\
                            (fnTestCallTest2)\
                            (fnTestCallTest22)\
                            (fnTestCallTest222)\
                            )

//=====================================================
const char* WasmCPlusAPICall::Init() {
    TC_Prints("WasmCPlusAPICall Init");
    return "init ok";
}

template<typename T>
const char* WasmCPlusAPICall::fnOutputResult(T t, bool success)
{
    JsonRoot jsonRoot = TC_JsonNewObject();
    tc::json::Marshal(jsonRoot, "type", "string");
    tc::json::Marshal(jsonRoot, "value", t);
    tc::json::Marshal(jsonRoot, "success", success);
    return TC_JsonToString(jsonRoot);
}


const char* WasmCPlusAPICall::fnTestCallTest1(tc::Address contractAddr) {
    bool bResult = false;
    const char* result = nullptr;    
    do
    {
        TC_Prints("fnTestCallTest1 start");
        WasmCPlusAPICallSubCont contractTest1(contractAddr);
        result = CALL(contractTest1, fnTest1);
        TC_Prints(result);
        TC_Prints("fnTestCallTest1 end");
        bResult = true;
    } while (0);
    return fnOutputResult(result, bResult);
}
const char* WasmCPlusAPICall::fnTestCallTest11(tc::Address contractAddr, const char* data) {
    bool bResult = false;
    const char* result = nullptr;
    do
    {
        TC_Prints("fnTestCallTest11 start");
        WasmCPlusAPICallSubCont contractTest11(contractAddr);
        result = CALL(contractTest11, fnTest11, data);
        TC_Prints(result);
        TC_Prints("fnTestCallTest11 end");
        bResult = true;
    } while (0);
    return fnOutputResult(result, bResult);
}

const char* WasmCPlusAPICall::fnTestCallTest111(tc::Address contractAddr, testValue data) {
    bool bResult = false;
    const char* result = nullptr;
    do
    {
        TC_Prints("fnTestCallTest111 start");
        Print("fnTestCallTest111 start10");
        Print("fnTestCallTest111 start1", "contractAddr", contractAddr, "data1", data.value1, "data2", data.value2, "data3", data.value3);
        Print("fnTestCallTest111 start11");
        WasmCPlusAPICallSubCont contractTest111(contractAddr);
        Print("fnTestCallTest111 start2");
        result = CALL(contractTest111, fnTest111, data);
        Print("fnTestCallTest111 start3");
        TC_Prints(result);
        TC_Prints("fnTestCallTest111 end");
        bResult = true;
    } while (0);
    return fnOutputResult(result, bResult);
}


const char* WasmCPlusAPICall::fnTestCallTest2(tc::Address contractAddr) {
    bool bResult = false;
    const char* result = nullptr;    
    do
    {
        TC_Prints("fnTestCallTest2 start");
        WasmCPlusAPICallSubCont contractTest2(contractAddr);
        result = DELEGATE_CALL(contractTest2, fnTest2);
        TC_Prints(result);
        TC_Prints("fnTestCallTest2 end");
        bResult = true;
    } while (0);
    return fnOutputResult(result, bResult);
}
const char* WasmCPlusAPICall::fnTestCallTest22(tc::Address contractAddr, const char* data) {
    bool bResult = false;
    const char* result = nullptr;
    do
    {
        TC_Prints("fnTestCallTest22 start");
        WasmCPlusAPICallSubCont contractTest22(contractAddr);
        result = DELEGATE_CALL(contractTest22, fnTest22, data);
        TC_Prints(result);
        TC_Prints("fnTestCallTest22 end");
        bResult = true;
    } while (0);
    return fnOutputResult(result, bResult);
}


const char* WasmCPlusAPICall::fnTestCallTest222(tc::Address contractAddr, testValue data) {
    bool bResult = false;
    const char* result = nullptr;
    do
    {
        TC_Prints("fnTestCallTest222 start");
        WasmCPlusAPICallSubCont contractTest222(contractAddr);
        result = DELEGATE_CALL(contractTest222, fnTest222, data);
        TC_Prints(result);
        TC_Prints("fnTestCallTest222 end");
        bResult = true;
    } while (0);
    return fnOutputResult(result, bResult);
}
