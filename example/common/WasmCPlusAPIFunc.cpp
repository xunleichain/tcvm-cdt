#define DEBUG

#include <stdio.h>
//#include "reflect.h"
//#include "tlv.h"
#include "types.hpp"
#include "tctpl.hpp"
#include<string.h>

tc::BInt ONELINK("1000000000000000000");
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

class WasmCPlusAPIFunc : public TCBaseContract {
public:
    WasmCPlusAPIFunc() {}
    virtual ~WasmCPlusAPIFunc() {}
    const char* Init();
    const char* fnGetGloble();
    void fnGetTest(tc::BInt test) {}
    bool fnCheckHash(const Pubkey &pk, const char *data, const Signature &sig);
    tc::Address fnRecoverHash(const tc::Hash &hash, const tc::BInt &v, const tc::BInt &r, const tc::BInt &s);
    const char* fnTestCalcHash(const char* data);
    const char* fnTestCalcStringHash(const string &data);
    const char* fnTestNotify(const char* eventId, const char* data);

    const char* fnTestLog0(const char *data);
    const char* fnTestLog1(const char *data, const char* topic1);
    const char* fnTestLog2(const char *data, const char* topic1, const char* topic2);
    const char* fnTestLog3(const char *data, const char* topic1, const char* topic2, const char* topic3);
    const char* fnTestLog4(const char *data, const char* topic1, const char* topic2, const char* topic3, const char* topic4);

    bool fnTestStorageSet();
    bool fnTestStorageGet();
    const char* fnTestStorage();

    const char* fnTestRequireAndRevert(int iType, bool condition, const char* msg);
    tc::Hash fnTestBlockNum(const int64 height);
    const char* fnTestIssus(const tc::BInt amount);
    const char* fnTestTypeCheck();
};

TC_ABI(WasmCPlusAPIFunc,    (fnGetGloble)\
                            (fnGetTest)\
                            (fnCheckHash)\
                            (fnRecoverHash)\
                            (fnTestCalcHash)\
                            (fnTestCalcStringHash)\
                            (fnTestNotify)\
                            (fnTestLog0)\
                            (fnTestLog1)\
                            (fnTestLog2)\
                            (fnTestLog3)\
                            (fnTestLog4)\
                            (fnTestStorageSet)\
                            (fnTestStorageGet)\
                            (fnTestStorage)\
                            (fnTestRequireAndRevert)\
                            (fnTestIssus)\
                            (fnTestBlockNum)\
                            (fnTestTypeCheck)\
                            )

//=====================================================
const char* WasmCPlusAPIFunc::Init() {
    // m_app = tc::App::getInstance();
    TC_Prints("WasmCPlusAPIFunc Init");
    return "init ok";
}

const char* WasmCPlusAPIFunc::fnGetGloble() {
    bool bResult = false;
    char* result = NULL;
    do
    {
        TC_Prints("fnGetGloble start");
        tc::App *m_app = tc::App::getInstance();
        JsonRoot jsonRoot = TC_JsonNewObject();
        
        tc::Address contractAddr = m_app->address();
        Print("fnGetGloble", "contractAddr", contractAddr.toString());
        tc::json::PutAddress(jsonRoot, "address", contractAddr);

        tc::Address coinbaseAddr = m_app->coinbase();
        Print("fnGetGloble", "coinbaseAddr", coinbaseAddr.toString());
        tc::json::PutAddress(jsonRoot, "coinbase", coinbaseAddr);

        int64_t i64GasLimit = m_app->gasLimit();
        Print("fnGetGloble", "i64GasLimit", i64toa(i64GasLimit, 16));
        tc::json::PutEnum(jsonRoot, "gasLimit", i64GasLimit);

        uint64_t u64Height = m_app->height();
        Print("fnGetGloble", "u64Height", i64toa(u64Height, 10));
        tc::json::PutEnum(jsonRoot, "height", u64Height);

        uint64_t u64Gas = m_app->gas();
        Print("fnGetGloble", "u64Gas", i64toa(u64Gas, 16));
        tc::json::PutEnum(jsonRoot, "gas", u64Gas);

        tc::Address senderAddr = m_app->sender();
        Print("fnGetGloble", "senderAddr", senderAddr.toString());
        tc::json::PutAddress(jsonRoot, "sender", senderAddr);

        tc::BInt bValue = m_app->value();
        Print("fnGetGloble", "bValue", bValue);
        tc::json::PutBInt(jsonRoot, "value", bValue);

        tc::BInt bTokenValue = m_app->tokenValue();
        Print("fnGetGloble", "bTokenValue", bTokenValue);
        tc::json::PutBInt(jsonRoot, "tokenValue", bTokenValue);

        uint64_t u64GasLeft = m_app->gasLeft();
        Print("fnGetGloble", "u64GasLeft", i64toa(u64GasLeft, 16));
        tc::json::PutEnum(jsonRoot, "gasLeft", u64GasLeft);

        uint64_t u64Now = m_app->now();
        Print("fnGetGloble", "u64Now", i64toa(u64Now, 16));
        tc::json::PutEnum(jsonRoot, "now", u64Now);

        uint64_t u64GasPrice = m_app->gasPrice();
        Print("fnGetGloble", "u64GasPrice", i64toa(u64GasPrice, 16));
        tc::json::PutEnum(jsonRoot, "gasPrice", u64GasPrice);

        tc::Address txOriginAddr = m_app->txOrigin();
        Print("fnGetGloble", "txOriginAddr", txOriginAddr.toString());
        tc::json::PutAddress(jsonRoot, "txOrigin", txOriginAddr);

        tc::Address tokenAddr = m_app->tokenAddress();
        Print("fnGetGloble", "tokenAddr", tokenAddr.toString());
        tc::json::PutAddress(jsonRoot, "tokenAddress", tokenAddr);

        TC_Prints(TC_JsonToString(jsonRoot));
        result = TC_JsonToString(jsonRoot);
        TC_Prints("fnGetGloble end");
        bResult = true;

    } while (0);

    const char* result1 =  bResult ? strconcat("fnGetGloble", " SUCCESS!") : strconcat("fnGetGloble", " FAILURE!");
    TC_Prints(result1);
    return result;
}

bool WasmCPlusAPIFunc::fnCheckHash(const Pubkey &pk, const char *data, const Signature &sig)
{
    bool bResult = false;
    do
    {
        TC_Prints("fnCheckHash start");
        bResult = tc::Sign::check(pk, data, sig);
        TC_Prints("fnCheckHash end");
    } while (0);
    const char* result =  bResult ? strconcat("fnCheckHash", " SUCCESS!") : strconcat("fnCheckHash", " FAILURE!");
    TC_Prints(result);
    return bResult;
}
tc::Address WasmCPlusAPIFunc::fnRecoverHash(const tc::Hash &hash, const tc::BInt &v, const tc::BInt &r, const tc::BInt &s)
{
    bool bResult = false;
    tc::Address pubkey = tc::Address();
    do
    {
        TC_Prints("fnRecoverHash start");
        pubkey = tc::Sign::recover(hash, v, r, s);
        TC_Prints("fnRecoverHash end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnRecoverHash", " SUCCESS!") : strconcat("fnRecoverHash", " FAILURE!");
    TC_Prints(result);
    return pubkey;
}

const char* WasmCPlusAPIFunc::fnTestCalcHash(const char* data)
{
    bool bResult = false;
    char* result = NULL;
    do
    {
        TC_Prints("fnTestCalcHash start");
        JsonRoot jsonRoot = TC_JsonNewObject();
        tc::Hash sha256Hash = tc::sha256(data);
        tc::json::PutHash(jsonRoot, "sha256Hash", sha256Hash);

        tc::Hash ripemd160Hash = tc::ripemd160(data);
        tc::json::PutHash(jsonRoot, "ripemd160Hash", ripemd160Hash);

        tc::Hash keccak256Hash = tc::keccak256(data);
        tc::json::PutHash(jsonRoot, "keccak256Hash", keccak256Hash);

        TC_Prints(TC_JsonToString(jsonRoot));
        result = TC_JsonToString(jsonRoot);
        TC_Prints("fnTestCalcHash end");
        bResult = true;
    } while (0);
    const char* result1 =  bResult ? strconcat("fnRecoverHash", " SUCCESS!") : strconcat("fnRecoverHash", " FAILURE!");
    TC_Prints(result1);
    return result;
}


const char* WasmCPlusAPIFunc::fnTestCalcStringHash(const string &data)
{
    bool bResult = false;
    char* result = NULL;    
    do
    {
        TC_Prints("fnTestCalcHash_string start");
        JsonRoot jsonRoot = TC_JsonNewObject();
        tc::Hash sha256Hash = tc::sha256(data);
        tc::json::PutHash(jsonRoot, "str_sha256Hash", sha256Hash);

        tc::Hash ripemd160Hash = tc::ripemd160(data);
        tc::json::PutHash(jsonRoot, "str_ripemd160Hash", ripemd160Hash);

        tc::Hash keccak256Hash = tc::keccak256(data);
        tc::json::PutHash(jsonRoot, "str_keccak256Hash", keccak256Hash);

        TC_Prints(TC_JsonToString(jsonRoot));
        result = TC_JsonToString(jsonRoot);
        TC_Prints("fnTestCalcHash_string end");
        bResult = true;
    } while (0);
    const char* result1 =  bResult ? strconcat("fnRecoverHash", " SUCCESS!") : strconcat("fnRecoverHash", " FAILURE!");
    TC_Prints(result1);
    return result;
}


const char* WasmCPlusAPIFunc::fnTestNotify(const char* eventId, const char* data)
{
    bool bResult = false;
    do
    {
        TC_Prints("fnTestNotify start");
        notify(eventId, data);
        Print("fnTestNotify", "const char*", "ok");

        tc::string strEventId = tc::string(eventId);
        tc::string strData = tc::string(data);
        notify(strEventId, strData);
        Print("fnTestNotify", "tc::string*", "ok");

        TC_Prints("fnTestNotify end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestNotify", " SUCCESS!") : strconcat("fnTestNotify", " FAILURE!");
    TC_Prints(result);
    return result;
}


const char* WasmCPlusAPIFunc::fnTestLog0(const char *data)
{
    bool bResult = false;
    do
    {
        TC_Prints("fnTestLog0 start");
        log(data);
        Print("fnTestLog0", "const char*", "ok");
        tc::string strData = tc::string(data);
        log(strData);
        Print("fnTestLog0", "tc::string*", "ok");
        TC_Prints("fnTestLog0 end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestLog0", " SUCCESS!") : strconcat("fnTestLog0", " FAILURE!");
    TC_Prints(result);
    return result;
}

const char* WasmCPlusAPIFunc::fnTestLog1(const char *data, const char* topic1)
{
    bool bResult = false;
    do
    {
        TC_Prints("fnTestLog1 start");
        log(data, topic1);
        Print("fnTestLog1", "const char*", "ok");
        tc::string strData = tc::string(data);
        tc::string strTopic1 = tc::string(topic1);
        log(strData, strTopic1);
        Print("fnTestLog1", "tc::string*", "ok");
        TC_Prints("fnTestLog1 end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestLog1", " SUCCESS!") : strconcat("fnTestLog1", " FAILURE!");
    TC_Prints(result);
    return result;
}
const char* WasmCPlusAPIFunc::fnTestLog2(const char *data, const char* topic1, const char* topic2)
{
    bool bResult = false;
    do
    {
        TC_Prints("fnTestLog2 start");
        log(data, topic1, topic2);
        Print("fnTestLog2", "const char*", "ok");
        tc::string strData = tc::string(data);
        tc::string strTopic1 = tc::string(topic1);
        tc::string strTopic2 = tc::string(topic2);
        log(strData, strTopic1, strTopic2);
        Print("fnTestLog2", "tc::string*", "ok");
        TC_Prints("fnTestLog2 end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestLog2", " SUCCESS!") : strconcat("fnTestLog2", " FAILURE!");
    TC_Prints(result);
    return result;
}

const char* WasmCPlusAPIFunc::fnTestLog3(const char *data, const char* topic1, const char* topic2, const char* topic3)
{
    bool bResult = false;
    do
    {
        TC_Prints("fnTestLog3 start");
        log(data, topic1, topic2, topic3);
        Print("fnTestLog3", "const char*", "ok");
        tc::string strData = tc::string(data);
        tc::string strTopic1 = tc::string(topic1);
        tc::string strTopic2 = tc::string(topic2);
        tc::string strTopic3 = tc::string(topic3);
        log(strData, strTopic1, strTopic2, strTopic3);
        Print("fnTestLog3", "tc::string*", "ok");
        TC_Prints("fnTestLog3 end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestLog3", " SUCCESS!") : strconcat("fnTestLog3", " FAILURE!");
    TC_Prints(result);
    return result;
}

const char* WasmCPlusAPIFunc::fnTestLog4(const char *data, const char* topic1, const char* topic2, const char* topic3, const char* topic4)
{
    bool bResult = false;
    do
    {
        TC_Prints("fnTestLog4 start");
        log(data, topic1, topic2, topic3, topic4);
        Print("fnTestLog4", "const char*", "ok");
        tc::string strData = tc::string(data);
        tc::string strTopic1 = tc::string(topic1);
        tc::string strTopic2 = tc::string(topic2);
        tc::string strTopic3 = tc::string(topic3);
        tc::string strTopic4 = tc::string(topic4);
        log(strData, strTopic1, strTopic2, strTopic3, strTopic4);
        Print("fnTestLog4", "tc::string*", "ok");
        TC_Prints("fnTestLog4 end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestLog4", " SUCCESS!") : strconcat("fnTestLog4", " FAILURE!");
    TC_Prints(result);
    return result;
}

bool WasmCPlusAPIFunc::fnTestStorageSet()
{
    bool bResult = false;
    //------------------------------------------------------
    char* STORAGE_KEY_CHAR = "STORAGE_KEY_CHAR";
    const char* STORAGE_VALUE_CHAR = "STORAGE_VALUE_CHAR";
    
    const char* STORAGE_KEY_CONST_CHAR = "STORAGE_KEY_CONST_CHAR";
    const uint64_t STORAGE_VALUE_UINT64 = 0x1231312313456;
    
    const tc::BInt STORAGE_KEY_BINT = tc::BInt("STORAGE_KEY_BINT");
    const tc::Hash STORAGE_VALUE_HASH = tc::Hash("0x123123123123123123123");
    
    const tc::Address STORAGE_KEY_ADDRESS = tc::Address("0x00000000000000000000000000000000000001");
    const tc::Token STORAGE_VALUE_TOKEN = tc::Token("123143213423423");
    
    const string STORAGE_KEY_STRING = "STORAGE_KEY_STRING";
    const bool STORAGE_VALUE_BOOL = true;
    //------------------------------------------------------
    do
    {
        TC_Prints("fnTestStorageSet start");
        TC_StorageSetValue(STORAGE_KEY_BINT, STORAGE_VALUE_HASH);
        TC_StorageSetValue(STORAGE_KEY_ADDRESS, STORAGE_VALUE_TOKEN);
        TC_StorageSetValue(STORAGE_KEY_CONST_CHAR, STORAGE_VALUE_UINT64);
        TC_StorageSetValue(STORAGE_KEY_STRING, STORAGE_VALUE_BOOL);
        TC_Prints("fnTestStorageSet end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestStorageSet", " SUCCESS!") : strconcat("fnTestStorageSet", " FAILURE!");
    TC_Prints(result);
    return bResult;
}
bool WasmCPlusAPIFunc::fnTestStorageGet()
{
    //------------------------------------------------------
    char* STORAGE_KEY_CHAR = "STORAGE_KEY_CHAR";
    const char* STORAGE_VALUE_CHAR = "STORAGE_VALUE_CHAR";
    
    const char* STORAGE_KEY_CONST_CHAR = "STORAGE_KEY_CONST_CHAR";
    const uint64_t STORAGE_VALUE_UINT64 = 0x1231312313456;
    
    const tc::BInt STORAGE_KEY_BINT = tc::BInt("STORAGE_KEY_BINT");
    const tc::Hash STORAGE_VALUE_HASH = tc::Hash("0x123123123123123123123");
    
    const tc::Address STORAGE_KEY_ADDRESS = tc::Address("0x00000000000000000000000000000000000001");
    const tc::Token STORAGE_VALUE_TOKEN = tc::Token("123143213423423");
    
    const string STORAGE_KEY_STRING = "STORAGE_KEY_STRING";
    const bool STORAGE_VALUE_BOOL = true;
    //------------------------------------------------------    
    bool bResult = false;
    do
    {
        TC_Prints("fnTestStorageGet start");
        tc::Hash getValueHash = TC_StorageGetValue<tc::BInt, tc::Hash>(STORAGE_KEY_BINT);
        FALSE_PRINT_STRCMP_BREAK(STORAGE_KEY_BINT, STORAGE_VALUE_HASH.toString(), getValueHash.toString());
        tc::Token getValueToken = TC_StorageGetValue<tc::Address, tc::Token>(STORAGE_KEY_ADDRESS);
        FALSE_PRINT_BREAK(STORAGE_KEY_ADDRESS, STORAGE_VALUE_TOKEN.value(), getValueToken.value());

        uint64_t getValueUint64 = TC_StorageGetValue<uint64_t>(STORAGE_KEY_CONST_CHAR);
        FALSE_PRINT_BREAK(STORAGE_KEY_CONST_CHAR, STORAGE_VALUE_UINT64, getValueUint64); 

        bool getValueBool = TC_StorageGetValue<bool>(STORAGE_KEY_STRING);
        FALSE_PRINT_BREAK(STORAGE_KEY_STRING, STORAGE_VALUE_BOOL, getValueBool);        
        TC_Prints("fnTestStorageGet end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestStorageGet", " SUCCESS!") : strconcat("fnTestStorageGet", " FAILURE!");
    TC_Prints(result);
    return bResult;
}
const char* WasmCPlusAPIFunc::fnTestStorage()
{
    bool bResult = false;
    do
    {
        TC_Prints("fnTestStorage start");
        if (!fnTestStorageSet()) {break;};
        if (!fnTestStorageGet()) {break;};
        TC_Prints("fnTestStorage end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestStorage", " SUCCESS!") : strconcat("fnTestStorage", " FAILURE!");
    TC_Prints(result);
    return result;
}

const char* WasmCPlusAPIFunc::fnTestRequireAndRevert(int iType, bool condition, const char* msg)
{
    bool bResult = false;
    do
    {
        TC_Prints("fnTestRequireAndRevert start");
        const string strMsg = string(msg);
        switch (iType){
            case 1:
                Print("require", "type", iType, "condition", condition);
                require(condition);
                break;
            case 2:
                Print("require", "type", iType, "condition", condition, "msg", msg);
                require(condition, msg);
                break;                
            case 3:
                Print("require", "type", iType, "condition", condition, "msg", msg);
                require(condition, strMsg);
                break;       
            case 4:
                Print("revert", "type", iType);
                revert();
                break;
            case 5:
                Print("revert", "type", iType, "msg", msg);
                revert(msg);
                break;                
            case 6:
                Print("revert", "type", iType, "msg", msg);
                revert(strMsg);
                break;             
            default:
                Print("fnTestRequireAndRevert para error", "type", iType);
        }
        TC_Prints("fnTestRequireAndRevert end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestStorage", " SUCCESS!") : strconcat("fnTestStorage", " FAILURE!");
    TC_Prints(result);
    return result;
}

const char* WasmCPlusAPIFunc::fnTestIssus(const tc::BInt amount)
{
    bool bResult = false;
    do
    {
        TC_Prints("fnTestIssus start");
        issue(amount);
        TC_Prints("fnTestIssus end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestIssus", " SUCCESS!") : strconcat("fnTestIssus", " FAILURE!");
    TC_Prints(result);
    return result;
}

tc::Hash WasmCPlusAPIFunc::fnTestBlockNum(const int64 height)
{
    bool bResult = false;
    tc::Hash hash = tc::Hash();
    do
    {
        TC_Prints("fnTestBlockNum start");
        hash = tc::blockHash(height);
        TC_Prints(hash.toString());
        TC_Prints("fnTestBlockNum end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestBlockNum", " SUCCESS!") : strconcat("fnTestBlockNum", " FAILURE!");
    TC_Prints(result);
    return hash;
}

const char* WasmCPlusAPIFunc::fnTestTypeCheck()
{
    bool bResult = false;
    do
    {
        TC_Prints("fnTestTypeCheck start");
        int64_t data1 = 0x10;
        int64_t data2 = 0x11;
        uint64_t data3 = 0x10;
        typeCheck(data1, data2);
        Print("typeCheck", "int64_t and int64_t", "ok");

        tc::Address data8("0x11111");
        tc::Address data9("0x11112");
        typeCheck(data8, data9);
        Print("typeCheck", "Address", "ok");   

        TC_Prints("fnTestTypeCheck end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTestTypeCheck", " SUCCESS!") : strconcat("fnTestTypeCheck", " FAILURE!");
    TC_Prints(result);
    return result;
}
