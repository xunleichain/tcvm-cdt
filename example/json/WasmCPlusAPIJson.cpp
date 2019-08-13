#define DEBUG

#include <stdio.h>
//#include "reflect.h"
//#include "tlv.h"
#include <string>
#include "types.hpp"
#include "tctpl.hpp"


struct Person {
    std::string name;
    int age;
};

struct Student {
    Person base;
    int score;
};

TC_STRUCT(Person,
    TC_FIELD_NAME(name, "name"),
    TC_FIELD_NAME(age, "age"));
    
TC_STRUCT(Student,
    TC_FIELD_NAME(base, "base"),
    TC_FIELD_NAME(score, "score"));

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
    

class WasmCPlusAPIJson : public TCBaseContract {

public:

public:
    const char* Init();
    const char* fnJsonTestString();
    const char* fnJsonTestAddress();
    const char* fnJsonTestHash();
    const char* fnJsonTestToken();
    const char* fnJsonTestBInt();
    bool fnJsonTestBIntCalc();
    bool fnJsonTestBIntCalcString();
    bool fnJsonTestBIntCalcInt16();
    bool fnJsonTestBIntCalcInt();
    const char* fnJsonTestMarshalAndUnMarshal();
    const char* fnJsonTestMarshalAndUnMarshalNoRoot();
};

TC_ABI(WasmCPlusAPIJson,    (fnJsonTestString)\
                            (fnJsonTestAddress)\
                            (fnJsonTestHash)\
                            (fnJsonTestToken)\
                            (fnJsonTestBInt)\
                            (fnJsonTestBIntCalc)\
                            (fnJsonTestBIntCalcString)\
                            (fnJsonTestBIntCalcInt16)\
                            (fnJsonTestBIntCalcInt)\
                            (fnJsonTestMarshalAndUnMarshal)\
                            (fnJsonTestMarshalAndUnMarshalNoRoot)\
                        )

//=====================================================
const char* WasmCPlusAPIJson::Init()
{
    TC_Prints("WasmCPlusAPIJson Init");
    return "init ok";
}

const char* WasmCPlusAPIJson::fnJsonTestString()
{
    bool bResult = true;
    do
    {
        JsonRoot testRoot = TC_JsonNewObject();
        const char* testKey = "fnJsonTestString";
        tc::string testValue = tc::string("fnJsonTestStringValue");
        tc::json::PutString(testRoot, testKey, testValue);
        const char* testKey1 = "fnJsonTestString1";
        tc::string testValue1 = tc::string("fnJsonTestStringValue1");
        tc::json::PutString(testRoot, testKey1, testValue1);

        tc::string testGetValue0;
        int result = tc::json::GetString(testRoot, testKey, testGetValue0);
        Print("fnJsonTestString", "put", testValue, "get0", testGetValue0, "getResult", result);
        if(testValue != testGetValue0) {
            bResult = false;
            break;
        }
        tc::string testGetValue00;
        result = tc::json::GetString(testRoot, testKey1, testGetValue00);
        Print("fnJsonTestString", "put1", testValue1, "get00", testGetValue00, "getResult", result);
        if(testValue1 != testGetValue00) {
            bResult = false;
            break;
        }
        tc::string testGetValue000;
        result = tc::json::GetString(testRoot, testKey1, testGetValue000);
        Print("fnJsonTestString", "put", testValue, "get000", testGetValue000, "getResult", result);
        if(testValue == testGetValue000) {
            bResult = false;
            break;
        }        

        char* testGetValue1;
        result = tc::json::GetCString(testRoot, testKey, testGetValue1);
        Print("fnJsonTestString", "put", testValue, "get1", testGetValue1, "getResult", result);
        if(testValue != testGetValue1) {
        // if(0 != std::strcmp(testValue, testGetValue1)) {
            bResult = false;
            break;
        }
        char* testGetValue11;
        result = tc::json::GetCString(testRoot, testKey1, testGetValue11);
        Print("fnJsonTestString", "put1", testValue, "get11", testGetValue11, "getResult", result);
        if(testValue1 != testGetValue11) {
        // if(0 != std::strcmp(testValue, testGetValue1)) {
            bResult = false;
            break;
        }
        char* testGetValue111;
        result = tc::json::GetCString(testRoot, testKey1, testGetValue111);
        Print("fnJsonTestString", "put", testValue, "get111", testGetValue111, "getResult", result);
        if(testValue == testGetValue111) {
        // if(0 != std::strcmp(testValue, testGetValue1)) {
            bResult = false;
            break;
        }

        const char* testGetValue2;
        result = tc::json::GetCCString(testRoot, testKey, testGetValue2);
        Print("fnJsonTestString", "put", testValue, "get2", testGetValue2, "getResult", result);
        if(testValue != testGetValue2) {
        // if(0 != std::strcmp(testValue, testGetValue2)) {
            bResult = false;
            break;
        }
        const char* testGetValue22;
        result = tc::json::GetCCString(testRoot, testKey1, testGetValue22);
        Print("fnJsonTestString", "put1", testValue, "get22", testGetValue22, "getResult", result);
        if(testValue1 != testGetValue22) {
        // if(0 != std::strcmp(testValue, testGetValue2)) {
            bResult = false;
            break;
        }     
        const char* testGetValue222;
        result = tc::json::GetCCString(testRoot, testKey1, testGetValue222);
        Print("fnJsonTestString", "put", testValue, "get222", testGetValue222, "getResult", result);
        if(testValue == testGetValue222) {
        // if(0 != std::strcmp(testValue, testGetValue2)) {
            bResult = false;
            break;
        }

        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnJsonTestString", " SUCCESS!") : strconcat("fnJsonTestString", " FAILURE!");
    return result;
}


const char* WasmCPlusAPIJson::fnJsonTestAddress()
{
    bool bResult = true;
    do
    {
        JsonRoot testRoot = TC_JsonNewObject();
        const char* testKey = "fnJsonTestAddress";
        TC_Prints("fnJsonTestAddress start");
        tc::Address testValue = tc::Address("0x54fb1c7d0f011dd63b08f85ed7b518ab82028100");
        TC_Prints("test");
        tc::json::PutAddress(testRoot, testKey, testValue);
        Print("PutAddress", "put", testValue);
        tc::Address testGetValue;
        int result = tc::json::GetAddress(testRoot, testKey, testGetValue);
        Print("fnJsonTestAddress1", "put", testValue, "get", testGetValue, "getResult", result, "testGetValue.isHex()", itoa(testGetValue.isHex()), "testGetValue.isZero()", itoa(testGetValue.isZero()) );
        if((0 != strcmp(testValue.toString(), testGetValue.toString())) || (!testGetValue.isHex()) || (testGetValue.isZero())) {
            bResult = false;
            break;
        }
        tc::Address testValue1;
        testValue1.set("0X54fb1c7d0f011dd63b08f85ed7b518ab82028100");
        Print("PutAddress", "set", testValue1.toString());
        tc::json::PutAddress(testRoot, testKey, testValue1);        
        result = tc::json::GetAddress(testRoot, testKey, testGetValue);
        Print("fnJsonTestAddress2", "put", testValue1, "get", testGetValue, "getResult", result, "testGetValue.isHex()", itoa(testGetValue.isHex()), "testGetValue.isZero()", itoa(testGetValue.isZero()) );
        if((0 != strcmp(testValue1.toString(), testGetValue.toString())) || (!testGetValue.isHex()) || (testGetValue.isZero())) {
            bResult = false;
            break;
        }
        testValue.set("0x0000000000000000000000000000000000000000");
        Print("PutAddress", "set", testValue.toString());
        tc::json::PutAddress(testRoot, testKey, testValue);        
        result = tc::json::GetAddress(testRoot, testKey, testGetValue);
        Print("fnJsonTestAddress3", "put", testValue, "get", testGetValue, "getResult", result, "testGetValue.isHex()", itoa(testGetValue.isHex()), "testGetValue.isZero()", itoa(testGetValue.isZero()) );
        if((0 != strcmp(testValue.toString(), testGetValue.toString())) || (!testGetValue.isHex()) || (!testGetValue.isZero())) {
            bResult = false;
            break;
        }
        bResult = true;
        TC_Prints("fnJsonTestAddress end");
    } while (0);
    const char* result =  bResult ? strconcat("fnJsonTestAddress", " SUCCESS!") : strconcat("fnJsonTestAddress", " FAILURE!");
    return result;
}
        

const char* WasmCPlusAPIJson::fnJsonTestHash()
{
    bool bResult = true;
    do
    {
        JsonRoot testRoot = TC_JsonNewObject();
        const char* testKey = "fnJsonTestHash";
        TC_Prints("fnJsonTestHash start");
        tc::Hash testValue = tc::Hash("0x0dec6b34e8318c229fb691b8c0002e6819ae7127f8afeeb8438f7e873a786e04");
        TC_Prints("test");
        tc::json::PutHash(testRoot, testKey, testValue);
        Print("PutHash", "put", testValue);

        tc::Hash testGetValue;
        // tc::Hash testGetValue = tc::Hash("", false);
        int result = tc::json::GetHash(testRoot, testKey, testGetValue);
        Print("fnJsonTestHash", "put", testValue, "get", testGetValue, "getResult", result);
        if(0 != strcmp(testValue.toString(), testGetValue.toString())) {
            bResult = false;
            break;
        }

        testValue.set("0X0dec6b34e8318c229fb691b8c0002e6819ae7127f8afeeb8438f7e873a786e04");
        tc::json::PutHash(testRoot, testKey, testValue);
        Print("PutHash", "set", testValue.toString());
        result = tc::json::GetHash(testRoot, testKey, testGetValue);
        Print("fnJsonTestHash", "put", testValue, "get", testGetValue, "getResult", result);
        if(0 != strcmp(testValue.toString(), testGetValue.toString())) {
            bResult = false;
            //break;
        }

        testValue.set("0x1234abcd");
        tc::json::PutHash(testRoot, testKey, testValue);
        Print("PutHash", "set", testValue.toString());
        result = tc::json::GetHash(testRoot, testKey, testGetValue);
        Print("fnJsonTestHash", "put", testValue, "get", testGetValue, "getResult", result);
        if(0 != strcmp(testValue.toString(), testGetValue.toString())) {
            bResult = false;
            break;
        }

        bResult = true;
        TC_Prints("fnJsonTestHash end");
    } while (0);
    const char* result =  bResult ? strconcat("fnJsonTestHash", " SUCCESS!") : strconcat("fnJsonTestHash", " FAILURE!");
    return result;
}
        
inline void PutToken11(JsonRoot root, const char *key, const tc::Token& val) {
    void *child = TC_JsonNewObject();
    TC_JsonPutAddress(child, "id", (address)val.id().toString());
    TC_JsonPutBigInt(child, "val", (BigInt)val.value().toString());
    TC_JsonPutObject(root, key, child);
}
const char* WasmCPlusAPIJson::fnJsonTestToken()
{
    bool bResult = true;
    do
    {
        JsonRoot testRoot = TC_JsonNewObject();
        const char* testKey = "fnJsonTestToken";
        TC_Prints("fnJsonTestToken start");
        tc::Token testValue;
        TC_Prints("test");
        Print("PutToken", "testValue.id().toString()", testValue.id().toString());
        Print("PutToken", "testValue.value().toString()", testValue.value().toString());
        
        PutToken11(testRoot, testKey, testValue);
        TC_Prints(TC_JsonToString(testRoot));
        TC_Prints("test1");

        tc::json::PutToken(testRoot, testKey, testValue);
        TC_Prints(TC_JsonToString(testRoot));
        TC_Prints("test2");        
        Print("PutToken", "put", testValue);
        Print("PutToken", "testValue.id().toString()", testValue.id().toString());
        Print("PutToken", "testValue.value().toString()", testValue.value().toString());
        tc::App* app = tc::App::getInstance();
        Print("App", "address", app->address());

        tc::Token testGetValue = tc::Token();
        int result = tc::json::GetToken(testRoot, testKey, testGetValue);
        Print("fnJsonTestToken", "put.value", testValue.value());
        tc::TokenID tokenId = testGetValue.id();
        Print("fnJsonTestToken", "get.id", tokenId);
        Print("fnJsonTestToken", "put.value", testValue.value(), "get.value", testGetValue.value(), "get.id", testGetValue.id(), "get.canTransfer", testGetValue.canTransfer(), "get.balance", testGetValue.balance(app->address()), "getResult", result);
        if((0 != strcmp(ZERO_ADDRESS, testGetValue.id().toString())) || (0 != strcmp("0", testGetValue.value().toString())) || (0 != strcmp(testValue.value().toString(), testGetValue.value().toString())) )
        {
            bResult = false;
            break;            
        }


        tc::Token testValue1 = tc::Token("10000000");
        TC_Prints("test");
        tc::json::PutToken(testRoot, testKey, testValue1);
        Print("PutToken", "put.value", testValue1.value());

        result = tc::json::GetToken(testRoot, testKey, testGetValue);
        Print("fnJsonTestToken", "put.value", testValue1.value(), "get.value", testGetValue.value(), "get.id", testGetValue.id(), "get.canTransfer", testGetValue.canTransfer(), "get.balance", testGetValue.balance(app->address()), "getResult", result);
        if((0 != strcmp(testValue1.id().toString(), testGetValue.id().toString())) || (0 != strcmp(testValue1.value().toString(), testGetValue.value().toString())) )
        {
            bResult = false;
            break;            
        }
        

        tc::Token testValue2 = tc::Token("0x54fb1c7d0f011dd63b08f85ed7b518ab82028100", "10000000");
        TC_Prints("test");
        tc::json::PutToken(testRoot, testKey, testValue2);
        Print("PutToken", "put.value", testValue2.value());

        result = tc::json::GetToken(testRoot, testKey, testGetValue);
        Print("fnJsonTestToken", "put.value", testValue2.value(), "get.value", testGetValue.value(), "get.id", testGetValue.id(), "get.canTransfer", testGetValue.canTransfer(), "get.balance", testGetValue.balance(app->address()), "getResult", result);
        if((0 != strcmp(testValue2.id().toString(), testGetValue.id().toString())) || (0 != strcmp(testValue2.value().toString(), testGetValue.value().toString())) )
        {
            bResult = false;
            break;            
        }

        bResult = true;
        TC_Prints("fnJsonTestToken end");
    } while (0);
    const char* result =  bResult ? strconcat("fnJsonTestToken", " SUCCESS!") : strconcat("fnJsonTestToken", " FAILURE!");
    return result;
}
        


const char* WasmCPlusAPIJson::fnJsonTestBInt()
{
    bool bResult = true;
    do
    {
        JsonRoot testRoot = TC_JsonNewObject();
        const char* testKey = "fnJsonTestBInt";
        TC_Prints("fnJsonTestBInt start");
        tc::BInt testValue = tc::BInt("0x12312231231312");
        tc::json::PutBInt(testRoot, testKey, testValue);
        Print("PutBInt", "put", testValue.toString());
        tc::BInt testGetValue = tc::BInt();
        int result = tc::json::GetBInt(testRoot, testKey, testGetValue);
        Print("GetBInt", "get", testGetValue.toString());
        if(!(testValue == testGetValue))
        {
            bResult = false;
            break;            
        }

        tc::BInt testValue1 = tc::BInt(10);
        tc::json::PutBInt(testRoot, testKey, testValue1);
        Print("PutBInt", "put1", testValue1.toString());
        tc::BInt testGetValue1 = tc::BInt();
        result = tc::json::GetBInt(testRoot, testKey, testGetValue1);
        Print("GetBInt", "get1", testGetValue1.toString());
        if(!(testValue1 == testGetValue1))
        {
            bResult = false;
            break;            
        }

        tc::BInt testValue2 = tc::BInt("0x1234567890abcd");
        tc::json::PutBInt(testRoot, testKey, testValue2);
        Print("PutBInt", "put2", testValue2.toString());
        tc::BInt testGetValue2 = tc::BInt();
        result = tc::json::GetBInt(testRoot, testKey, testGetValue2);
        Print("GetBInt", "get2", testGetValue2.toString());
        if(!(testValue2 == testGetValue2))
        {
            bResult = false;
            break;            
        }

        tc::BInt testValue3 = tc::BInt("0x1234567890abcdef0987654321");
        tc::json::PutBInt(testRoot, testKey, testValue3);
        Print("PutBInt", "put3", testValue3.toString());
        tc::BInt testGetValue3 = tc::BInt();
        result = tc::json::GetBInt(testRoot, testKey, testGetValue3);
        Print("GetBInt", "get2", testGetValue3.toString());
        if(!(testValue3 == testGetValue3))
        {
            bResult = false;
            break;            
        }

        bResult = true;
        TC_Prints("fnJsonTestBInt end");
    } while (0);
    const char* result =  bResult ? strconcat("fnJsonTestBInt", " SUCCESS!") : strconcat("fnJsonTestBInt", " FAILURE!");
    return result;
}

bool WasmCPlusAPIJson::fnJsonTestBIntCalc()
{
    Print("fnJsonTestBIntCalc start");
    bool bResult1 = fnJsonTestBIntCalcString();
    Print("fnJsonTestBIntCalc", "fnJsonTestBIntCalcString", bResult1);
    bool bResult2 = fnJsonTestBIntCalcInt16();
    Print("fnJsonTestBIntCalc", "fnJsonTestBIntCalcInt16", bResult2);
    bool bResult3 = fnJsonTestBIntCalcInt();
    Print("fnJsonTestBIntCalc", "fnJsonTestBIntCalcInt", bResult3);
    bool bResult = bResult1 & bResult2 & bResult3;
    Print("fnJsonTestBIntCalc end");
    return bResult;
}

bool WasmCPlusAPIJson::fnJsonTestBIntCalcString()
{
    bool bResult = true;
    do
    {
        JsonRoot testRoot = TC_JsonNewObject();
        const char* testKey = "fnJsonTestBIntCalc";
        TC_Prints("fnJsonTestBIntCalc start");
        tc::BInt testValue = tc::BInt("0x123456789abcdef0000000000000000000000000000000000000");
        TC_Prints("test");
        
        tc::json::PutBInt(testRoot, testKey, (const tc::BInt)testValue);
        TC_Prints("test2");
        Print("PutBInt1", "put", testValue);

        tc::BInt testGetValue = tc::BInt();
        int result = tc::json::GetBInt(testRoot, testKey, testGetValue);
        Print("GetBInt2", "get", testGetValue);
        if(!(testValue == testGetValue))
        {
            bResult = false;
            break;            
        }
        testGetValue = testValue;
        tc::BInt bIntTmp = ++testGetValue;
        Print("GetBInt3", "bIntTmp1", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("1") || (testGetValue - testValue) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = --testGetValue;
        Print("GetBInt4", "bIntTmp2", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("-1") || (testGetValue - testValue) != tc::BInt("-1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue++;
        Print("GetBInt33", "bIntTmp1", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("0") || (testGetValue - testValue) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue--;
        Print("GetBInt44", "bIntTmp2", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("0") || (testGetValue - testValue) != tc::BInt("-1"))
        {
            bResult = false;
            break;    
        } 
        testGetValue = testValue;
        bIntTmp = testGetValue * tc::BInt("2");
        Print("GetBInt5", "bIntTmp3", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp / testValue) != tc::BInt("2"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue / tc::BInt("10");
        Print("GetBInt6", "bIntTmp4", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((testValue / bIntTmp) != tc::BInt("10"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue % tc::BInt("2");
        Print("GetBInt7", "bIntTmp5", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (bIntTmp != tc::BInt("0"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp += tc::BInt("1");
        Print("GetBInt8", "bIntTmp6", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp -= tc::BInt("1");
        Print("GetBInt9", "bIntTmp6", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((testValue - bIntTmp) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        } 
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp *= tc::BInt("2");
        Print("GetBInt10", "bIntTmp8", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp / testValue) != tc::BInt("2"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp /= tc::BInt("10");
        Print("GetBInt11", "bIntTmp9", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((testValue / bIntTmp) != tc::BInt("10"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp %= tc::BInt("2");
        Print("GetBInt12", "bIntTmp10", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (bIntTmp != tc::BInt("0"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp.reset();
        Print("GetBInt13", "bIntTmp11", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (bIntTmp != tc::BInt("0"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        int64 iIntTmp = bIntTmp.toInt64();
        Print("GetBInt14", "iIntTmp", iIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (iIntTmp != 0)
        {
            bResult = false;
            break;    
        }

        bResult = true;
        TC_Prints("fnJsonTestBIntCalc end");
    } while (0);
    const char* result =  bResult ? strconcat("fnJsonTestBIntCalc", " SUCCESS!") : strconcat("fnJsonTestBIntCalc", " FAILURE!");
    return bResult;
}


bool WasmCPlusAPIJson::fnJsonTestBIntCalcInt16()
{
    bool bResult = true;
    do
    {
        JsonRoot testRoot = TC_JsonNewObject();
        const char* testKey = "fnJsonTestBIntCalc";
        TC_Prints("fnJsonTestBIntCalc start");
        tc::BInt testValue = tc::BInt(9223372036854775800);
        TC_Prints("test");
        
        tc::json::PutBInt(testRoot, testKey, testValue);
        TC_Prints("test2");
        Print("PutBInt1", "put", testValue);

        tc::BInt testGetValue = tc::BInt();
        int result = tc::json::GetBInt(testRoot, testKey, testGetValue);
        Print("GetBInt2", "get", testGetValue);
        if(!(testValue == testGetValue))
        {
            bResult = false;
            break;            
        }
        testGetValue = testValue;
        tc::BInt bIntTmp = ++testGetValue;
        Print("GetBInt3", "bIntTmp1", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("1") || (testGetValue - testValue) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = --testGetValue;
        Print("GetBInt4", "bIntTmp2", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("-1") || (testGetValue - testValue) != tc::BInt("-1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue++;
        Print("GetBInt33", "bIntTmp1", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("0") || (testGetValue - testValue) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue--;
        Print("GetBInt44", "bIntTmp2", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("0") || (testGetValue - testValue) != tc::BInt("-1"))
        {
            bResult = false;
            break;    
        } 
        testGetValue = testValue;
        bIntTmp = testGetValue * tc::BInt("2");
        Print("GetBInt5", "bIntTmp3", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp / testValue) != tc::BInt("2"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue / tc::BInt("10");
        Print("GetBInt6", "bIntTmp4", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((testValue / bIntTmp) != tc::BInt("10"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue % tc::BInt("2");
        Print("GetBInt7", "bIntTmp5", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (bIntTmp != tc::BInt("0"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp += tc::BInt("1");
        Print("GetBInt8", "bIntTmp6", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp -= tc::BInt("1");
        Print("GetBInt9", "bIntTmp6", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((testValue - bIntTmp) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        } 
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp *= tc::BInt("2");
        Print("GetBInt10", "bIntTmp8", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp / testValue) != tc::BInt("2"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp /= tc::BInt("10");
        Print("GetBInt11", "bIntTmp9", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((testValue / bIntTmp) != tc::BInt("10"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp %= tc::BInt("2");
        Print("GetBInt12", "bIntTmp10", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (bIntTmp != tc::BInt("0"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp.reset();
        Print("GetBInt13", "bIntTmp11", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (bIntTmp != tc::BInt("0"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp = bIntTmp.toInt64();
        Print("GetBInt14", "bIntTmp11", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (bIntTmp != 9223372036854775800)
        {
            bResult = false;
            break;    
        }

        bResult = true;
        TC_Prints("fnJsonTestBIntCalc end");
    } while (0);
    const char* result =  bResult ? strconcat("fnJsonTestBIntCalc", " SUCCESS!") : strconcat("fnJsonTestBIntCalc", " FAILURE!");
    return bResult;
}


bool WasmCPlusAPIJson::fnJsonTestBIntCalcInt()
{
    bool bResult = true;
    do
    {
        JsonRoot testRoot = TC_JsonNewObject();
        const char* testKey = "fnJsonTestBIntCalc";
        TC_Prints("fnJsonTestBIntCalc start");
        tc::BInt testValue = tc::BInt(1314);
        TC_Prints("test");
        
        tc::json::PutBInt(testRoot, testKey, (const tc::BInt)testValue);
        TC_Prints("test2");
        Print("PutBInt1", "put", testValue);

        tc::BInt testGetValue = tc::BInt();
        int result = tc::json::GetBInt(testRoot, testKey, testGetValue);
        Print("GetBInt2", "get", testGetValue);
        if(!(testValue == testGetValue))
        {
            bResult = false;
            break;            
        }
        testGetValue = testValue;
        tc::BInt bIntTmp = ++testGetValue;
        Print("GetBInt3", "bIntTmp1", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("1") || (testGetValue - testValue) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = --testGetValue;
        Print("GetBInt4", "bIntTmp2", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("-1") || (testGetValue - testValue) != tc::BInt("-1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue++;
        Print("GetBInt33", "bIntTmp1", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("0") || (testGetValue - testValue) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue--;
        Print("GetBInt44", "bIntTmp2", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("0") || (testGetValue - testValue) != tc::BInt("-1"))
        {
            bResult = false;
            break;    
        } 
        testGetValue = testValue;
        bIntTmp = testGetValue * tc::BInt("2");
        Print("GetBInt5", "bIntTmp3", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp / testValue) != tc::BInt("2"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue / tc::BInt("10");
        Print("GetBInt6", "bIntTmp4", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((testValue / bIntTmp) != tc::BInt("10"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue % tc::BInt("2");
        Print("GetBInt7", "bIntTmp5", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (bIntTmp != tc::BInt("0"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp += tc::BInt("1");
        Print("GetBInt8", "bIntTmp6", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp - testValue) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp -= tc::BInt("1");
        Print("GetBInt9", "bIntTmp6", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((testValue - bIntTmp) != tc::BInt("1"))
        {
            bResult = false;
            break;    
        } 
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp *= tc::BInt("2");
        Print("GetBInt10", "bIntTmp8", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((bIntTmp / testValue) != tc::BInt("2"))
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp /= 10;
        Print("GetBInt11", "bIntTmp9", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if ((testValue / bIntTmp) != 10)
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp %= tc::BInt("2");
        Print("GetBInt12", "bIntTmp10", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (bIntTmp != 0)
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        bIntTmp.reset();
        Print("GetBInt13", "bIntTmp11", bIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (bIntTmp != 0)
        {
            bResult = false;
            break;    
        }
        testGetValue = testValue;
        bIntTmp = testGetValue;
        int64 iIntTmp = bIntTmp.toInt64();
        Print("GetBInt14", "iIntTmp", iIntTmp, "put", testValue, "testGetValue", testGetValue);
        if (iIntTmp != 1314)
        {
            bResult = false;
            break;    
        }

        bResult = true;
        TC_Prints("fnJsonTestBIntCalc end");
    } while (0);
    const char* result =  bResult ? strconcat("fnJsonTestBIntCalc", " SUCCESS!") : strconcat("fnJsonTestBIntCalc", " FAILURE!");
    return bResult;
}

const char* WasmCPlusAPIJson::fnJsonTestMarshalAndUnMarshal()
{
    bool bResult = true;
    do
    {
        JsonRoot testRoot = TC_JsonNewObject();
        const char* testKey = "fnJsonTestMarshalAndUnMarshal";

        TC_Prints("fnJsonTestMarshalAndUnMarshal start");
        tc::Address testValue = tc::Address("0x54fb1c7d0f011dd63b08f85ed7b518ab82028100");
        tc::json::Marshal(testRoot, testKey, testValue);
        Print("fnJsonTestMarshalAndUnMarshal1", "Marshal_put", testValue.toString());
        tc::Address testGetValue;
        int result = tc::json::Unmarshal(testRoot, testKey, testGetValue);
        Print("fnJsonTestMarshalAndUnMarshal2", "UnMarshal_Gut", testGetValue.toString());
        if(0 != strcmp(testValue.toString(), testGetValue.toString()) || (!testGetValue.isHex()) || (testGetValue.isZero())) {
            bResult = false;
            break;
        }

        tc::string testValue3 = tc::string("kutenglaoshuhunya");
        const char* testKey3 = "fnJsonTestMarshalAndUnMarshal_3";
        tc::json::Marshal(testRoot, testKey3, testValue3);
        Print("fnJsonTestMarshalAndUnMarshal7", "Marshal_3", testValue3);
        tc::string testGetValue3;
        result = tc::json::Unmarshal(testRoot, testKey3, testGetValue3);
        Print("fnJsonTestMarshalAndUnMarshal8", "UnMarshal_3", testGetValue3);
        if(testValue3 != testGetValue3){
            bResult = false;
            break;
        }

        bResult = true;
        TC_Prints("fnJsonTestMarshalAndUnMarshal end");
    } while (0);
    const char* result =  bResult ? strconcat("fnJsonTestMarshalAndUnMarshal", " SUCCESS!") : strconcat("fnJsonTestMarshalAndUnMarshal", " FAILURE!");
    return result;
}


const char* WasmCPlusAPIJson::fnJsonTestMarshalAndUnMarshalNoRoot() {
    bool bResult = false;
    do
    {
        Student s;
        Person p;
        s.score = 100;
        p.age = 30;
        p.name = std::string("ywh");
        s.base = p;
    
        
        Student tmp;
        Print("fnJsonTestMarshalAndUnMarshalNoRoot_1");
        char* data2 = tc::json::Marshal(s);
        Print("fnJsonTestMarshalAndUnMarshalNoRoot_2", "data2", data2);
        int result = tc::json::Unmarshal(data2, tmp);
        Print("fnJsonTestMarshalAndUnMarshalNoRoot_3", "result", result);
        
        FALSE_PRINT_BREAK("tmp.base.age", tmp.base.age, s.base.age);
        FALSE_PRINT_BREAK("tmp.base.name", tmp.base.name, s.base.name);
        FALSE_PRINT_BREAK("tmp.score", tmp.score, s.score);

        Print("fnJsonTestMarshalAndUnMarshalNoRoot_4");

        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnJsonTestMarshalAndUnMarshalNoRoot", " SUCCESS!") : strconcat("fnJsonTestMarshalAndUnMarshalNoRoot", " FAILURE!");
    return result;
}
