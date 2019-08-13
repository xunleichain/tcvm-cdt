
#include "tcapi.h"
#include "string.h"




//===================================================
//定义类型及宏
//---------------------------------------------------
// typedef unsigned int uint; 
// #define NULL (char*)0
// #define FALSE_BREAK   {break;}
// #define BUFFER_SIZE 200
// #define OUTPUT_BUFFER_SIZE 2000
#define PRINT_BUFFER (2000)
//===================================================


//===================================================
char* Init(char* pOutput)
{
   return "init ok";
}
//===================================================
void fnPrint(char* info)
{
    // char* str = (char*)malloc(PRINT_BUFFER);
    // memset((char*)str, 0, PRINT_BUFFER);
    // sprintf(str, "[WASM_API_GAS_TEST_DEBUG] | %s |", info);
    // TC_Prints(str);
    // free((char*)str);
}

//===================================================
//一、C语言函数 这里主要说明wasm虚拟机底层提供的部分C标准库接口：
void fnBottom_exit()
{
    exit(0);
    fnPrint("exit");
}
void fnBottom_abort()
{
    abort();
    fnPrint("abort");
}

void fnBottom_malloc()
{
    char* buffer = (char*)malloc(1024);
    fnPrint(buffer);
    free((char*)buffer);
    fnPrint("malloc");    
}
void fnBottom_realloc()
{
    char* buffer = (char*)malloc(1024);
    char* buffer2 = (char*)realloc(buffer, 1024);
    free((char*)buffer);
    free((char*)buffer2);
    fnPrint("realloc");    
    return;
}
void fnBottom_calloc()
{
    char* pCalloc = (char*)calloc(1024, sizeof(char));
    free((char*)pCalloc);  
    fnPrint("calloc");    
    return;
}

void fnBottom_memset_zero()
{
    char* buffer = (char*)malloc(1024);
    memset(buffer, 0, 1024);
    free((char*)buffer);      
    fnPrint("memset");        
    return;
}
void fnBottom_memset_10()
{
    char* buffer = (char*)malloc(1024);
    memset(buffer, 0, 1024);
    memset(buffer, 10, 1023);
    free((char*)buffer);
    fnPrint("memset_10");          
    return;
}
void fnBottom_memcpy()
{
    char* buffer = (char*)malloc(1024);
    memset(buffer, 0, 1024);    
    char* buffer2 = (char*)malloc(1024);
    memcpy(buffer2, buffer, 1024);
    free((char*)buffer);  
    free((char*)buffer2);          
    fnPrint("memcpy");       
    return;
}
void fnBottom_memcmp()
{
    char* buffer = (char*)malloc(1024);
    memset(buffer, 0, 1024);    
    char* buffer2 = (char*)malloc(1024);
    memset(buffer, 0, 1024);
    memset(buffer2, 10, 1023);
    memcmp(buffer, buffer2, 1023);
    free((char*)buffer);  
    free((char*)buffer2);      
    fnPrint("memcmp");      
    return;
}
void fnBottom_memmove()
{
    char* buffer = (char*)malloc(1024);
    memset(buffer, 0, 1024);    
    char* buffer2 = (char*)malloc(1024);
    memset(buffer2, 0, 1024);
    memset(buffer2, 10, 1023);   
    memmove(buffer, buffer2, 1023);
    free((char*)buffer);  
    free((char*)buffer2);      
    fnPrint("memmove");       
    return;
}
void fnBottom_strconcat()
{
    char* buffer = (char*)malloc(1024);
    memset(buffer, 0, 1024);    
    memset(buffer, 0x31, 1023);   
    char* buffer2 = (char*)malloc(1024);
    memset(buffer2, 0, 1024);
    memset(buffer2, 0x32, 1023);   
    char* buffer3 = (char*)strconcat(buffer, buffer2);
    free((char*)buffer);  
    free((char*)buffer2);      
    fnPrint(buffer3);
    fnPrint("strconcat");      
}
void fnBottom_itoa()
{
    itoa(32767);
    fnPrint("fnBottom_itoa");     
}
void fnBottom_i64toa()
{
    i64toa(2147483647, 16);
    fnPrint("fnBottom_i64toa");   
}
void fnBottom_printsl()
{
    prints_l("hello wasm!", 11);
    fnPrint("fnBottom_printsl");   
}
void fnBottomAPI(char *action, char *args) {
    ACTION_FUNC("fnBottom_exit", fnBottom_exit);
    ACTION_FUNC("fnBottom_abort", fnBottom_abort);
    ACTION_FUNC("fnBottom_malloc", fnBottom_malloc);
    ACTION_FUNC("fnBottom_calloc", fnBottom_calloc);
    ACTION_FUNC("fnBottom_realloc", fnBottom_realloc);
    ACTION_FUNC("fnBottom_memset_zero", fnBottom_memset_zero);
    ACTION_FUNC("fnBottom_memset_10", fnBottom_memset_10);
    ACTION_FUNC("fnBottom_memcpy", fnBottom_memcpy);
    ACTION_FUNC("fnBottom_memcmp", fnBottom_memcmp);
    ACTION_FUNC("fnBottom_memmove", fnBottom_memmove);
    ACTION_FUNC("fnBottom_strconcat", fnBottom_strconcat);
    ACTION_FUNC("fnBottom_itoa", fnBottom_itoa);
    ACTION_FUNC("fnBottom_i64toa", fnBottom_i64toa);
    ACTION_FUNC("fnBottom_printsl", fnBottom_printsl);
}
//===================================================
//二、Json编解码接口为方便编解码json对象，暂时由wasm虚拟机提供json相关的接口，后期建议使用第三方json库来完成此功能。
char* jsonData = "{\"int\":32767, \"int64\":2147483647, \"string\":\"zhangzitao\", \"address\":\"0x7753940535fac4e5dc176b826c40254be3286bbb\", \"bigint\":\"0x904488673e571beb504b3b0dfd6b033c052b699db1bba72ea134573f6463f4f9\",\"object\":{\"key\":\"value\"}}";
void fnJson_TC_JsonParse()
{
    TC_JsonParse(jsonData);
    fnPrint("fnJson_TC_JsonParse");   
}
void fnJson_TC_JsonGetInt()
{
    char* result = TC_JsonParse(jsonData);
    TC_JsonGetInt(result, "int");
    fnPrint("fnJson_TC_JsonGetInt");   
}
void fnJson_TC_JsonGetInt64()
{
    char* result = TC_JsonParse(jsonData);
    TC_JsonGetInt64(result, "int64");
    fnPrint("fnJson_TC_JsonGetInt64");   
}
void fnJson_TC_JsonGetString()
{
    char* result = TC_JsonParse(jsonData);
    TC_JsonGetString(result, "string");
    fnPrint("fnJson_TC_JsonGetString");  
}
void fnJson_TC_JsonGetAddress()
{
    char* result = TC_JsonParse(jsonData);
    TC_JsonGetAddress(result, "address");
    fnPrint("fnJson_TC_JsonGetAddress");  
}
void fnJson_TC_JsonGetBigInt()
{
    char* result = TC_JsonParse(jsonData);
    TC_JsonGetBigInt(result, "bigint");
    fnPrint("fnJson_TC_JsonGetBigInt");  
}
// void fnJson_TC_JsonGetFloat()
// {
//     char* result = TC_JsonParse(jsonData);
//     float data = TC_JsonGetFloat(result, "float");
//     return;
// }
// void fnJson_TC_JsonGetDouble()
// {
//     char* result = TC_JsonParse(jsonData);
//     double data = TC_JsonGetDouble(result, "double");
//     return;
// }
void fnJson_TC_JsonGetObject()
{
    char* result = TC_JsonParse(jsonData);
    TC_JsonGetObject(result, "object");
    fnPrint("fnJson_TC_JsonGetObject");  
}
void fnJson_TC_JsonNewObject()
{
    TC_JsonNewObject();
    fnPrint("fnJson_TC_JsonNewObject");  
}
void fnJson_TC_JsonPutInt()
{
    char* object = TC_JsonNewObject();
    TC_JsonPutInt(object, "int", 32767);
    fnPrint("fnJson_TC_JsonPutInt"); 
}
void fnJson_TC_JsonPutInt64()
{
    char* object = TC_JsonNewObject();
    TC_JsonPutInt64(object, "int64", 2147483647);
    fnPrint("fnJson_TC_JsonPutInt64"); 
}
void fnJson_TC_JsonPutString()
{
    char* object = TC_JsonNewObject();
    TC_JsonPutString(object, "string", "zhangzitao");
    fnPrint("fnJson_TC_JsonPutString"); 
}
// void fnJson_TC_JsonPutFloat()
// {
//     char* object = TC_JsonNewObject();
//     TC_JsonPutFloat(object, "float", 12345);
//     return;
// }
// void fnJson_TC_JsonPutDouble()
// {
//     char* object = TC_JsonNewObject();
//     TC_JsonPutDouble(object, "double", 123456789.123456789);
//     return;
// }
char* object = "{\"key\":\"value\"}";
void fnJson_TC_JsonPutObject()
{
    char* object = TC_JsonNewObject();
    TC_JsonPutObject(object, "object", object);
    fnPrint("fnJson_TC_JsonPutObject"); 
}
void fnJson_TC_JsonToString()
{
    char* object = TC_JsonNewObject();
    TC_JsonPutObject(object, "object", object);
    TC_JsonToString(object);
    fnPrint("fnJson_TC_JsonToString"); 
}
void fnJsonAPI(char *action, char *args) {
    ACTION_FUNC("fnJson_TC_JsonParse", fnJson_TC_JsonParse);
    ACTION_FUNC("fnJson_TC_JsonGetInt", fnJson_TC_JsonGetInt);
    ACTION_FUNC("fnJson_TC_JsonGetInt64", fnJson_TC_JsonGetInt64);
    ACTION_FUNC("fnJson_TC_JsonGetString", fnJson_TC_JsonGetString);
    ACTION_FUNC("fnJson_TC_JsonGetAddress", fnJson_TC_JsonGetAddress);
    ACTION_FUNC("fnJson_TC_JsonGetBigInt", fnJson_TC_JsonGetBigInt);
    // ACTION_FUNC("fnJson_TC_JsonGetFloat", fnJson_TC_JsonGetFloat);
    // ACTION_FUNC("fnJson_TC_JsonGetDouble", fnJson_TC_JsonGetDouble);
    ACTION_FUNC("fnJson_TC_JsonGetObject", fnJson_TC_JsonGetObject);
    ACTION_FUNC("fnJson_TC_JsonNewObject", fnJson_TC_JsonNewObject);
    ACTION_FUNC("fnJson_TC_JsonPutInt", fnJson_TC_JsonPutInt);
    ACTION_FUNC("fnJson_TC_JsonPutInt64", fnJson_TC_JsonPutInt64);
    ACTION_FUNC("fnJson_TC_JsonPutString", fnJson_TC_JsonPutString);
    // ACTION_FUNC("fnJson_TC_JsonPutFloat", fnJson_TC_JsonPutFloat);
    // ACTION_FUNC("fnJson_TC_JsonPutDouble", fnJson_TC_JsonPutDouble);
    ACTION_FUNC("fnJson_TC_JsonPutObject", fnJson_TC_JsonPutObject);
    ACTION_FUNC("fnJson_TC_JsonToString", fnJson_TC_JsonToString);
}
//===================================================
//三、BigInt操作接口：标准的C库不提供BigInt操作，考虑到区块链底层与余额有关的操作，都是以BigInt作为操作数，而第三方的C库与go本身的BigInt操作会存在不兼容的问题，因此统一由wasm虚拟机提供此类接口，以c字符串形式表示；
BigInt a = "333333333333333555555555555555555555555555555555555556333333333333333555555555555555555555555555555555555556";
BigInt b = "111111111111111111111111111111111222222222222222222223111111111111111111111111111111111222222222222222222223";
void fnBigInt_TC_BigIntAdd()
{
    TC_BigIntAdd(a, b);
    fnPrint("fnBigInt_TC_BigIntAdd");     
}
void fnBigInt_TC_BigIntSub()
{
    TC_BigIntSub(a, b);
    fnPrint("fnBigInt_TC_BigIntSub");         
}
void fnBigInt_TC_BigIntMul()
{
    TC_BigIntMul(a, b);
    fnPrint("fnBigInt_TC_BigIntMul");        
}
void fnBigInt_TC_BigIntDiv()
{
    TC_BigIntDiv(a, b);
    fnPrint("fnBigInt_TC_BigIntDiv");       
}
void fnBigInt_TC_BigIntMod()
{
    TC_BigIntMod(a, b);
    fnPrint("fnBigInt_TC_BigIntMod");         
}
void fnBigInt_TC_BigIntCmp()
{
    TC_BigIntCmp(a, b);
    fnPrint("fnBigInt_TC_BigIntCmp");      
}
void fnBigInt_TC_BigIntToInt64()
{
    TC_BigIntToInt64(a);
    fnPrint("fnBigInt_TC_BigIntToInt64");          
}
void fnBigIntAPI(char *action, char *args) {
    ACTION_FUNC("fnBigInt_TC_BigIntAdd", fnBigInt_TC_BigIntAdd);
    ACTION_FUNC("fnBigInt_TC_BigIntSub", fnBigInt_TC_BigIntSub);
    ACTION_FUNC("fnBigInt_TC_BigIntMul", fnBigInt_TC_BigIntMul);
    ACTION_FUNC("fnBigInt_TC_BigIntDiv", fnBigInt_TC_BigIntDiv);
    ACTION_FUNC("fnBigInt_TC_BigIntMod", fnBigInt_TC_BigIntMod);
    ACTION_FUNC("fnBigInt_TC_BigIntCmp", fnBigInt_TC_BigIntCmp);
    ACTION_FUNC("fnBigInt_TC_BigIntToInt64", fnBigInt_TC_BigIntToInt64);
}
//===================================================
//四、区块链相关接口：为方便wasm合约可以方便的访问区块链相关信息，wasm虚拟机对外提供了Storage、Message、Event、Contract相关的接口。
//1.Storage相关接口在wasm合约中，需要合约显式的调用Storage接口，实现对数据的持久化存储与读取。
char* storageKey = "testKey";
void fnChain_TC_StorageSet()
{
    TC_StorageSet(storageKey, "zhangzitao");
    fnPrint("fnChain_TC_StorageSet");      
}
void fnChain_TC_StorageGet()
{
    TC_StorageGet(storageKey);
    fnPrint("fnChain_TC_StorageGet");          
}
void fnChain_TC_StorageDel()
{
    TC_StorageDel(storageKey);
    fnPrint("fnChain_TC_StorageDel");    
}
//===================================================
address systemAddr = "0x54fb1c7d0f011dd63b08f85ed7b518ab82028100";
//2.转账接口TC_Transfer
void fnChain_TC_Transfer()
{
    TC_Transfer(systemAddr, "1000");
    fnPrint("fnChain_TC_Transfer");    
}
//===================================================
//3. Message相关接口：
void fnChain_TC_GetMsgData()
{
    TC_GetMsgData();
    fnPrint("fnChain_TC_GetMsgData");        
}
void fnChain_TC_GetMsgGas()
{
    TC_GetMsgGas();
    fnPrint("fnChain_TC_GetMsgGas");       
}
void fnChain_TC_GetMsgSender()
{
    TC_GetMsgSender();
    fnPrint("fnChain_TC_GetMsgSender");         
}
void fnChain_TC_GetMsgSign()
{
    TC_GetMsgSign();
    fnPrint("fnChain_TC_GetMsgSign");    
}
void fnChain_TC_GetMsgValue()
{
    TC_GetMsgValue();
    fnPrint("fnChain_TC_GetMsgValue");        
}
void fnChain_TC_GetNumber()
{
    TC_GetNumber();
    fnPrint("fnChain_TC_GetNumber");       
}
void fnChain_TC_GetGasLimit()
{
    TC_GetGasLimit();
    fnPrint("fnChain_TC_GetGasLimit");           
}
void fnChain_TC_GetCoinbase()
{
    TC_GetCoinbase();
    fnPrint("fnChain_TC_GetCoinbase");        
}
void fnChain_TC_BlockHash()
{
    TC_BlockHash(100);
    fnPrint("fnChain_TC_BlockHash");        
}
void fnChain_TC_GetTxGasPrice()
{
    fnPrint("fnChain_TC_GetTxGasPrice1");
    TC_GetTxGasPrice();
    fnPrint("fnChain_TC_GetTxGasPrice2");            
}
void fnChain_TC_GetTxOrigin()
{
    TC_GetTxOrigin();
    fnPrint("fnChain_TC_GetTxOrigin");       
}
void fnChain_TC_GasLeft()
{
    TC_GasLeft();
    fnPrint("fnChain_TC_GasLeft");           
}
void fnChain_TC_GetBalance()
{
    TC_GetBalance(systemAddr);
    fnPrint("fnChain_TC_GetBalance");             
}
void fnChain_TC_GetSelfAddress()
{
    TC_GetSelfAddress();
    fnPrint("fnChain_TC_GetSelfAddress");      
}
void fnChain_TC_GetMsgTokenValue()
{
    TC_GetMsgTokenValue();
    fnPrint("fnChain_TC_GetMsgTokenValue");      
}
//===================================================
//4. Event相关接口
void fnChain_TC_Notify()
{
    TC_Notify("Transfer(address,address,uint256)", "{\"from\":\"0x54fb1c7d0f011dd63b08f85ed7b518ab82028100\",\"to\":\"0x0000000000000000000000000000000000000000\",\"value\":10000}");
    fnPrint("fnChain_TC_Notify");
}
//===================================================
//5.Contract相关接口
void fnChain_TC_SelfDestruct()
{
    TC_SelfDestruct(systemAddr);
    fnPrint("fnChain_TC_SelfDestruct");    
}
//===================================================
//6.相关辅助接口
void fnChain_TC_Assert()
{
    TC_Assert(true);
    fnPrint("fnChain_TC_Assert");    
}
void fnChain_TC_Require()
{
    TC_Require(true);
    fnPrint("TC_Require");       
    fnPrint("fnChain_TC_Require");    
}
void fnChain_TC_RequireWithMsg()
{
    TC_RequireWithMsg(true, "test success!");
    fnPrint("TC_RequireWithMsg");   
    fnPrint("fnChain_TC_RequireWithMsg"); 
}
void fnChain_TC_Assert_false()
{
    TC_Assert(false);
    fnPrint("fnChain_TC_Assert_false");    
}
void fnChain_TC_Require_false()
{
    TC_Require(false);
    fnPrint("TC_Require");       
    fnPrint("fnChain_TC_Require_false");    
}
void fnChain_TC_RequireWithMsg_false()
{
    TC_RequireWithMsg(false, "test fail!");
    fnPrint("TC_RequireWithMsg");   
    fnPrint("fnChain_TC_RequireWithMsg_false"); 
}
void fnChain_TC_Revert()
{
    TC_Revert();
    fnPrint("TC_Revert");    
    fnPrint("fnChain_TC_Revert"); 
}
void fnChain_TC_RevertWithMsg()
{
    TC_RevertWithMsg("revert test fail!");
    fnPrint("TC_RevertWithMsg");
    fnPrint("fnChain_TC_RevertWithMsg"); 
}
void fnChain_TC_IsHexAddress()
{
    TC_IsHexAddress(systemAddr);
    fnPrint("fnChain_TC_IsHexAddress");    
}
void fnChain_TC_Payable()
{
    TC_Payable(false);
    fnPrint("fnChain_TC_Payable");
    return;
}
void fnChain_TC_Prints()
{
    TC_Prints("test print info");
    return;
}
void fnChain_TC_Now()
{
    TC_Now();
    fnPrint("fnChain_TC_Now");
}
void fnChain_TC_CheckSign()
{
    TC_CheckSign("0xd63b8dc359e5a8426de739e78cb796c6fe3aed07", "0xbfe945fac5528eb8ca316e865d728eaa330cb02fc31740aa75d12633e6e891f3", "0x36541dc4411865cb2c8cfa4db769c8b3a3d40a43b82629061d70a9c0b20e4a735cb9b0f2df64b26cf4feb8c7b1fe11de19967bc11e634f69f1399be2c0a1298500");
    fnPrint("fnChain_TC_CheckSign");
}
void fnChainAPI(char *action, char *args) {
    ACTION_FUNC("fnChain_TC_StorageSet", fnChain_TC_StorageSet);
    ACTION_FUNC("fnChain_TC_StorageGet", fnChain_TC_StorageGet);
    ACTION_FUNC("fnChain_TC_StorageDel", fnChain_TC_StorageDel);
    ACTION_FUNC("fnChain_TC_Transfer", fnChain_TC_Transfer);

    ACTION_FUNC("fnChain_TC_GetMsgData", fnChain_TC_GetMsgData);
    ACTION_FUNC("fnChain_TC_GetMsgGas", fnChain_TC_GetMsgGas);
    ACTION_FUNC("fnChain_TC_GetMsgSender", fnChain_TC_GetMsgSender);
    ACTION_FUNC("fnChain_TC_GetMsgSign", fnChain_TC_GetMsgSign);
    ACTION_FUNC("fnChain_TC_GetMsgValue", fnChain_TC_GetMsgValue);
    ACTION_FUNC("fnChain_TC_GetNumber", fnChain_TC_GetNumber);
    ACTION_FUNC("fnChain_TC_GetGasLimit", fnChain_TC_GetGasLimit);
    ACTION_FUNC("fnChain_TC_GetCoinbase", fnChain_TC_GetCoinbase);
    ACTION_FUNC("fnChain_TC_BlockHash", fnChain_TC_BlockHash);
    ACTION_FUNC("fnChain_TC_GetTxGasPrice", fnChain_TC_GetTxGasPrice);
    ACTION_FUNC("fnChain_TC_GetTxOrigin", fnChain_TC_GetTxOrigin);
    ACTION_FUNC("fnChain_TC_GasLeft", fnChain_TC_GasLeft);
    ACTION_FUNC("fnChain_TC_GetBalance", fnChain_TC_GetBalance);
    ACTION_FUNC("fnChain_TC_GetSelfAddress", fnChain_TC_GetSelfAddress);
    ACTION_FUNC("fnChain_TC_GetMsgTokenValue", fnChain_TC_GetMsgTokenValue);

    ACTION_FUNC("fnChain_TC_Notify", fnChain_TC_Notify);

    ACTION_FUNC("fnChain_TC_SelfDestruct", fnChain_TC_SelfDestruct);

    ACTION_FUNC("fnChain_TC_Assert", fnChain_TC_Assert);
    ACTION_FUNC("fnChain_TC_Require", fnChain_TC_Require);
    ACTION_FUNC("fnChain_TC_RequireWithMsg", fnChain_TC_RequireWithMsg);
    ACTION_FUNC("fnChain_TC_Revert", fnChain_TC_Revert);
    ACTION_FUNC("fnChain_TC_RevertWithMsg", fnChain_TC_RevertWithMsg);
    ACTION_FUNC("fnChain_TC_IsHexAddress", fnChain_TC_IsHexAddress);
    ACTION_FUNC("fnChain_TC_Payable", fnChain_TC_Payable);
    ACTION_FUNC("fnChain_TC_Prints", fnChain_TC_Prints);
    ACTION_FUNC("fnChain_TC_Now", fnChain_TC_Now);
    ACTION_FUNC("fnChain_TC_CheckSign", fnChain_TC_CheckSign);
}
//===================================================
//五、签名算法接口：与EVM虚拟机提供的签名算法兼容，wasm虚拟机也提供对应的接口。
void fnCalcHash_TC_Ecrecover()
{
    TC_Ecrecover("0xbfe945fac5528eb8ca316e865d728eaa330cb02fc31740aa75d12633e6e891f3", "0xec8d", "0x36541dc4411865cb2c8cfa4db769c8b3a3d40a43b82629061d70a9c0b20e4a73", "0x5cb9b0f2df64b26cf4feb8c7b1fe11de19967bc11e634f69f1399be2c0a12985");
    fnPrint("fnCalcHash_TC_Ecrecover");    
}
void fnCalcHash_TC_Ripemd160()
{
    TC_Ripemd160("zhangzitao hash test!");
    fnPrint("fnCalcHash_TC_Ecrecover");
}
void fnCalcHash_TC_Sha256()
{
    TC_Sha256("zhangzitao hash test!");
    fnPrint("fnCalcHash_TC_Sha256");
}
void fnCalcHash_TC_Keccak256()
{
    TC_Keccak256("zhangzitao hash test!");
    fnPrint("fnCalcHash_TC_Keccak256");
}
void fnCalcHashAPI(char *action, char *args) {
    ACTION_FUNC("fnCalcHash_TC_Ecrecover", fnCalcHash_TC_Ecrecover);
    ACTION_FUNC("fnCalcHash_TC_Ripemd160", fnCalcHash_TC_Ripemd160);
    ACTION_FUNC("fnCalcHash_TC_Sha256", fnCalcHash_TC_Sha256);
    ACTION_FUNC("fnCalcHash_TC_Keccak256", fnCalcHash_TC_Keccak256);
}
//===================================================
const char* testKey="contractTestKey";
//===================================================
//六、tokenV2相关接口
void fnCalcHash_TC_Issus()
{
    TC_Issue("1000000");
    fnPrint("fnCalcHash_TC_Issus");    
}
void fnCalcHash_TC_TransferToken()
{
    TC_TransferToken(systemAddr, ZERO_ADDRESS, "1000");
    fnPrint("fnCalcHash_TC_TransferToken");
}
void fnCalcHash_TC_TokenBalance()
{
    TC_TokenBalance(systemAddr, ZERO_ADDRESS);
    fnPrint("fnCalcHash_TC_TokenBalance");
}
void fnCalcHash_TC_TokenAddress()
{
    TC_TokenAddress();
    fnPrint("fnCalcHash_TC_TokenAddress");
}

void fnCalcHash_TC_CallContract()
{
    char* addressTest = TC_StorageGet(testKey);
    TC_CallContract(addressTest, "fnChain_TC_GetSelfAddress", "");
    fnPrint("fnCalcHash_TC_CallContract");
}
void fnCalcHash_TC_DelegateCallContract()
{
    char* addressTest = TC_StorageGet(testKey);
    TC_DelegateCallContract(addressTest, "fnChain_TC_GetSelfAddress", "");
    fnPrint("fnCalcHash_TC_DelegateCallContract");
}

void fnCalcHash_TC_StorageSet(char* info)
{
    TC_StorageSet(testKey, info);
    fnPrint("fnCalcHash_TC_StorageSet");      
}

void fnBottom_malloc1024()
{
    char* buffer = (char*)malloc(1024);
    fnPrint(buffer);
    fnPrint("malloc1024");    
}
void fnBottom_malloc1024for()
{
    char* buffer = (char*)malloc(1024);
    fnPrint("malloc1024for");    
    for (int i = 0; i < 1023; i ++)
    {
        *(buffer + i) = 10;
    }
}
void fnBottom_log0()
{
    TC_Log0("0x0000a");
    fnPrint("fnBottom_log0");    
}
void fnBottom_log1()
{
    TC_Log1("0x0000a", "0x000001");
    fnPrint("fnBottom_log1");    
}
void fnBottom_log2()
{
    TC_Log2("0x0000a", "0x000001", "0x000002");
    fnPrint("fnBottom_log2");    
}
void fnBottom_log3()
{
    TC_Log3("0x0000a", "0x000001", "0x000002", "0x000003");
    fnPrint("fnBottom_log3");    
}
void fnBottom_log4()
{
    TC_Log4("0x0000a", "0x000001", "0x000002", "0x000003", "0x000004");
    fnPrint("fnBottom_log4");    
}
void fnTokenV2API(char *action, char *args) {
    ACTION_FUNC("fnCalcHash_TC_Issus", fnCalcHash_TC_Issus);
    ACTION_FUNC("fnCalcHash_TC_TransferToken", fnCalcHash_TC_TransferToken);
    ACTION_FUNC("fnCalcHash_TC_TokenBalance", fnCalcHash_TC_TokenBalance);
    ACTION_FUNC("fnCalcHash_TC_TokenAddress", fnCalcHash_TC_TokenAddress);
    ACTION_FUNC("fnCalcHash_TC_CallContract", fnCalcHash_TC_CallContract);
    ACTION_FUNC("fnCalcHash_TC_DelegateCallContract", fnCalcHash_TC_DelegateCallContract);
    ACTION_FUNC("fnBottom_malloc1024", fnBottom_malloc1024);
    ACTION_FUNC("fnBottom_malloc1024for", fnBottom_malloc1024for);
    ACTION_FUNC("fnBottom_log0", fnBottom_log0);
    ACTION_FUNC("fnBottom_log1", fnBottom_log1);
    ACTION_FUNC("fnBottom_log2", fnBottom_log2);
    ACTION_FUNC("fnBottom_log3", fnBottom_log3);
    ACTION_FUNC("fnBottom_log4", fnBottom_log4);
    ACTION_CODE("fnCalcHash_TC_StorageSet", {     
        void *jsonroot = TC_JsonParse(args);
        char* info = TC_JsonGetAddress(jsonroot, "info");
        return fnCalcHash_TC_StorageSet(info);
    });
}
//===================================================
// void fnCalcHash_API_SetStorage(char* value)
// {
//     TC_StorageSet(testKey, value);
//     fnPrint("fnCalcHash_API_SetStorage");
// }
// void fnCalcHash_API_GetStorage(char* value)
// {
//     char* value = TC_StorageGet(testKey);
//     fnPrint("fnCalcHash_API_GetStorage");
//     return value;
// }
//===================================================
//主函数入口
char *thunderchain_main(char *action, char *args) {
    fnPrint("thunderchain_main");
    fnBottomAPI(action, args);
    fnPrint("fnBottomAPI");
    fnJsonAPI(action, args);
    fnPrint("fnJsonAPI");    
    fnBigIntAPI(action, args);
    fnPrint("fnBigIntAPI");    
    fnChainAPI(action, args);
    fnPrint("fnChainAPI");    
    fnCalcHashAPI(action, args);
    fnPrint("fnCalcHashAPI");        
    fnTokenV2API(action, args);
    fnPrint("fnTokenV2API");  
    return "{\"type\": \"string\", \"value\": \"test ok\", \"succeed\": 1}";
}
//===================================================
