#define DEBUG
#include "tcapi.h"
#include <stdio.h>
#include "types.hpp"
#include "tctpl.hpp"
#include<string.h>

// typedef char* string; 
typedef uint64 uint256;
typedef unsigned char uint8;
#define LINK_ADDRESS ZERO_ADDRESS
#define PRINT_BUFFER (2000)
/// WasmAddStorage implement IERC20
class WasmAddStorage : public TCBaseContract  {

public:
    WasmAddStorage();
    ~WasmAddStorage();
    // 定义token的名称
    const char* TOKEN_NAME = "WasmAddStorage add c++ template";
    // token使用的小数点后几位
    uint8 decimals = 18;
    // 兑换价格，1个token可以额换多少本token
    uint256 _price = 5;
    const char* STORAGE_NAME = "STORAGE_NAME";
    const char* RESULT_FAILURE = "failure!";
    const char* RESULT_SUCCESS = "success!";

private:
    void _fnPrint(const char* str1, const char* str2, const char* str3);
    char* fnOutputResult(char* value, bool success);

public:
    char *Init();
    char*  fnGetName();
    char*  fnGetSelfAddress();
    bool fnSetInfo(char*  testInfo);
    char*  fnGetInfo();
    //---------------------------------------
    void fnStorageSetString(const char* key, const char* value);
    char*  fnStorageGetString(const char* key);
    void fnStorageSetBytes(const char* key, const char* value, uint32_t size);
    char* fnStorageGetBytes(const char* key);
    void fnStoragePureSetString(const char* key, uint32_t size, const char *val);
    char* fnStoragePureGetString(const char* key, uint32_t size);
    void fnStoragePureSetBytes(const char* key, uint32_t size1, const char* val, uint32_t size2);
    char* fnStoragePureGetBytes(const char* key, uint32_t size1);
    //---------------------------------------
}; 
TC_ABI(WasmAddStorage,  (fnGetName)\
                (fnGetSelfAddress)\
                (fnSetInfo)\
                (fnGetInfo)\
                (fnStorageSetString)\
                (fnStorageGetString)\
                (fnStorageSetBytes)\
                (fnStorageGetBytes)\                
                (fnStoragePureSetString)\
                (fnStoragePureGetString)\
                (fnStoragePureSetBytes)\
                (fnStoragePureGetBytes))


WasmAddStorage::WasmAddStorage()
{
    TC_Prints("WasmAddStorage|structure|initual");
}

WasmAddStorage::~WasmAddStorage()
{

}

//初始化
char* WasmAddStorage::Init() {
    //BigInt totalSupply = TC_BigIntMul((BigInt)TC_ONELINK, (BigInt)"10000");
    //BigInt totalSupply = TC_BigIntMul((BigInt)TC_ONELINK, "10000");
    BigInt totalSupply = TC_TOKEN(10000);
    TC_Issue(totalSupply);
    Transfer(LINK_ADDRESS, TC_GetSelfAddress(), TC_GetSelfAddress(), atoi64(totalSupply));
    return EMPTY_CSTRING;
}

//内部函数
void WasmAddStorage::_fnPrint(const char* str1, const char* str2, const char* str3)
{
    // char* result = strconcat("[WasmAddStorage_TOKEN_TEST_DEBUG] | ", strconcat(str1, strconcat( "|", strconcat(str2, strconcat("|", str3)))));
    // TC_Prints(result);
    Print("[WasmAddStorage_TOKEN_TEST_DEBUG]", "str1", str1, "str2", str2, "str3", str3);
}

char* WasmAddStorage::fnOutputResult(char* value, bool success)
{
    JsonRoot pResult = TC_JsonNewObject();
    TC_JsonPutString(pResult, "type", "char* ");
    TC_JsonPutString(pResult, "value", value);
    TC_JsonPutInt(pResult, "success", success);
    return TC_JsonToString(pResult);
}

char*  WasmAddStorage::fnGetName(){
    _fnPrint("fnGetName", (char*)"step", (char*)"1");
    TC_Payable(false);
    _fnPrint("fnGetName", (char*)"step", (char*)"2");
    return (char* )TOKEN_NAME;
}

char*  WasmAddStorage::fnGetSelfAddress(){
    char*  addressInfo = TC_GetSelfAddress();
    return addressInfo;
}

bool WasmAddStorage::fnSetInfo(char*  testInfo){
    _fnPrint("fnSetInfo", (char*)"step", (char*)"1");
    bool bResult = false;        
    TC_Payable(false);
    TC_StorageSetString(STORAGE_NAME, testInfo);
    bResult = true;
    _fnPrint("fnSetInfo", (char*)"step", (char*)"2");
    return bResult;
}
char*  WasmAddStorage::fnGetInfo(){
    _fnPrint("fnGetInfo", (char*)"step", (char*)"1");
    TC_Payable(false);
    char*  testInfo = TC_StorageGetString(STORAGE_NAME);
    _fnPrint("fnGetInfo", (char*)"step", (char*)"2");
    return testInfo;
}
//--------------------------------------
void WasmAddStorage::fnStorageSetString(const char* key, const char* value){
    TC_Payable(false);
    TC_StorageSetString(key, value);
    _fnPrint("fnStorageSetString", (char*)"key", (char*)value);
}
char*  WasmAddStorage::fnStorageGetString(const char* key){
    TC_Payable(false);
    char* testInfo = TC_StorageGetString(key);
    _fnPrint("TC_StorageGetString", "key", (char*)testInfo);
    return testInfo;
}

void WasmAddStorage::fnStorageSetBytes(const char* key, const char* value, uint32_t size){
    Print("[fnStorageSetBytes]", "key", key, "value", value, "size", itoa(size));
    TC_Payable(false);
    TC_StorageSetBytes(key, (const uint8_t*)value, size);
    _fnPrint("TC_StorageSetBytes", (char*)"key", (char*)"ok");
}


char* WasmAddStorage::fnStorageGetBytes(const char* key){
    Print("[fnStorageGetBytes]", "key", key);
    TC_Payable(false);
    uint8_t* testInfo = TC_StorageGetBytes(key);
    _fnPrint("TC_StorageGetBytes", (char*)"key", (char*)"ok");
    return (char*)testInfo;
}

void WasmAddStorage::fnStoragePureSetString(const char* key, uint32_t size, const char *val){
    Print("[fnStoragePureSetString]", "key", key, "size", itoa(size), "val", val);
    TC_Payable(false);
    TC_StoragePureSetString((const uint8_t*)key, size, val);
    _fnPrint("TC_StoragePureSetString", (char*)"key", (char*)"ok");
}
char* WasmAddStorage::fnStoragePureGetString(const char* key, uint32_t size){
    Print("[fnStoragePureGetString]", "key", key, "size", itoa(size));
    TC_Payable(false);
    char* testInfo = TC_StoragePureGetString((const uint8_t*)key, size);
    _fnPrint("TC_StoragePureGetString", (char*)"key", (char*)"ok");
    return testInfo;
}

void WasmAddStorage::fnStoragePureSetBytes(const char* key, uint32_t size1, const char* val, uint32_t size2){
    Print("[fnStoragePureSetBytes]", "key", key, "size1", itoa(size1), "val", val, "size2", itoa(size2));
    TC_Payable(false);
    TC_StoragePureSetBytes((const uint8_t*)key, size1, (const uint8_t*)val, size2);
    _fnPrint("TC_StoragePureSetBytes", (char*)"key", (char*)"ok");
}
char* WasmAddStorage::fnStoragePureGetBytes(const char* key, uint32_t size1){
    Print("[fnStoragePureGetBytes]", "key", key, "size1", itoa(size1));
    TC_Payable(false);
    uint8_t* testInfo = TC_StoragePureGetBytes((const uint8_t*)key, size1);
    _fnPrint("TC_StoragePureGetBytes", (char*)"key", (char*)"ok");
    return (char*)testInfo;
}
//--------------------------------------
