
#include "tcapi.h"
#include "stdio.h"
#include "string.h"


typedef char* string; 
typedef uint64 uint256;
typedef unsigned char uint8;
#define LINK_ADDRESS ZERO_ADDRESS
#define PRINT_BUFFER (2000)

/// WasmTRC3Token implement IERC20
class WasmTRC3Token{

public:
    WasmTRC3Token() ;
    ~WasmTRC3Token();
    // 定义token的名称
    const char* TOKEN_NAME = "tokenDemo_token2link";
    // token使用的小数点后几位
    uint8 decimals = 18;
    // 兑换价格，1个token可以额换多少本token
    uint256 _price = 5;
    const char* STORAGE_NAME = "STORAGE_NAME";
    const char* RESULT_FAILURE = "failure!";
    const char* RESULT_SUCCESS = "success!";
    //合约多层调用-层记录值
    const char* CONTRACT_LEVEL_NAME_CALL = "CONTRACT_LEVEL_NAME_CALL";
    const char* CONTRACT_LEVEL_NAME_DELCALL = "CONTRACT_LEVEL_NAME_DELCALL";
    const char* CONTRACT_LEVEL_NAME_CALL_STORE_MAX = "CONTRACT_LEVEL_NAME_CALL_STORE_MAX";
    const char* CONTRACT_LEVEL_NAME_CALL_STORE_ADDR = "CONTRACT_LEVEL_NAME_CALL_STORE_ADDR";
    const char* CONTRACT_LEVEL_NAME_CALL_STORE_ACT = "CONTRACT_LEVEL_NAME_CALL_STORE_ACT";
    const char* CONTRACT_LEVEL_NAME_CALL_STORE_ARG = "CONTRACT_LEVEL_NAME_CALL_STORE_ARG";
    const char* CONTRACT_LEVEL_NAME_DELCALL_STORE_ADDR = "CONTRACT_LEVEL_NAME_DELCALL_STORE_ADDR";
    const char* CONTRACT_LEVEL_NAME_DELCALL_STORE_MAX = "CONTRACT_LEVEL_NAME_DELCALL_STORE_MAX";
    const char* CONTRACT_LEVEL_NAME_DELCALL_STORE_ACT = "CONTRACT_LEVEL_NAME_DELCALL_STORE_ACT";
    const char* CONTRACT_LEVEL_NAME_DELCALL_STORE_ARG = "CONTRACT_LEVEL_NAME_DELCALL_STORE_ARG";

private:
    void _fnPrint(char* str1, char* str2, char* str3);
    char* _fnOutputResult(char* value, bool success);

public:
    void Transfer(address fromAddr, address toAddr, address tokenAddr, uint64 value);
    void Transfer(address fromAddr, address toAddr, uint64 value);
    char *Init();
    string fnGetName();
    address fnGetSelfAddress();
    bool fnSetInfo(string testInfo);
    string fnGetInfo();
    BigInt fnTokenBalance(address addr, address tokenAddr);
    bool exchangePrint();
    bool exchange();
    bool exchangeToLinkPrint();
    bool exchangeToLink();
    bool exchangeSelfToToken();
    bool exchangeToOtherToken();
    bool exchangeSupportZero();
    bool exchangeToLinkSupportZero();
    bool exchangeToOtherTokenSupportZero();
    bool testTransferLink(address _toAddress, uint256 linkValue);
    bool testTransferToken(address _toAddress, uint256 tokenValue);
    bool testTransferLinkAndToken(address _toAddress, uint256 tokenValue);
    bool testTransferLinkAndTokenForRequire(address _toAddress, uint256 tokenValue);
    bool testTransferTokenAndLinkForRequire(address _toAddress, uint256 tokenValue);
    bool transferLink(address _toAddress, uint256 linkValue);
    char *DoAction(char *action, char *args);
    void fnSetStorage2(char* key, char* value);
    char* fnGetStorage2(char* key);
    void fnDelStorage2(char* key);
    //增加合约调用
    bool fnContractLevelClear();
    char* fnContractCall(address contract, char* action, char* args);
    char* fnDelegateContractCall(address contract, char* action, char* args);
    char* fnContractCallStore();
    char* fnDelegateContractCallStore();    
    int fnRecursion(int num);
    //多笔合约交易
    bool transferMulOldLink(address toAddress1, address toAddress2, address toAddress3, uint256 linkValue);
    bool transferMulLink(address toAddress1, address toAddress2, address toAddress3, uint256 linkValue);
    bool transferMulToken(address toAddress1, address toAddress2, address toAddress3, uint256 tokenValue);
}; 

WasmTRC3Token::WasmTRC3Token()
{
    TC_Prints("WasmTRC3Token|structure|initual");
}

WasmTRC3Token::~WasmTRC3Token()
{

}

//事件 tokenV2
void WasmTRC3Token::Transfer(address fromAddr, address toAddr, address tokenAddr, uint64 value)
{
    void *jsroot = TC_JsonNewObject();
    TC_JsonPutString(jsroot, "from", fromAddr);
    TC_JsonPutString(jsroot, "to", toAddr);
    TC_JsonPutString(jsroot, "token", tokenAddr);
    BigInt valueInfo = (BigInt)i64toa(value, 10);
    TC_JsonPutString(jsroot, "value", valueInfo);
    TC_Notify("Transfer(address,address,address,uint256)", TC_JsonToString(jsroot));    
}
//tokenV1
void WasmTRC3Token::Transfer(address fromAddr, address toAddr, uint64 value)
{
    void *jsroot = TC_JsonNewObject();
    TC_JsonPutString(jsroot, "from", fromAddr);
    TC_JsonPutString(jsroot, "to", toAddr);
    BigInt valueInfo = (BigInt)i64toa(value, 10);
    TC_JsonPutString(jsroot, "value", valueInfo);
    TC_Notify("Transfer(address,address,uint256)", TC_JsonToString(jsroot));    
}    
//初始化
char* WasmTRC3Token::Init() {
    //BigInt totalSupply = TC_BigIntMul((BigInt)TC_ONELINK, (BigInt)"10000");
    //BigInt totalSupply = TC_BigIntMul((BigInt)TC_ONELINK, "10000");
    BigInt totalSupply = TC_TOKEN(10000);
    TC_Issue(totalSupply);
    Transfer(LINK_ADDRESS, TC_GetSelfAddress(), TC_GetSelfAddress(), atoi64(totalSupply));
    return EMPTY_CSTRING;
}

//内部函数
void WasmTRC3Token::_fnPrint(char* str1, char* str2, char* str3)
{
    char* str = (char*)malloc(PRINT_BUFFER);
    memset((char*)str, 0, PRINT_BUFFER);
    sprintf(str, "[WASM_TRC3_TOKEN_TEST_DEBUG] | %s | %s | %s |", str1, str2, str3);
    TC_Prints(str);
}

char* WasmTRC3Token::_fnOutputResult(char* value, bool success)
{
    char* pResult = (char*)TC_JsonNewObject();

    TC_JsonPutString((char*)pResult, "type", "string");
    TC_JsonPutString((char*)pResult, "value", value);
    TC_JsonPutInt((char*)pResult, "success", success);

    return TC_JsonToString(pResult);
    // return (char*)"end";
}

string WasmTRC3Token::fnGetName(){
    _fnPrint((char*)"fnGetName", (char*)"step", (char*)"1");
    TC_Payable(false);
    _fnPrint((char*)"fnGetName", (char*)"step", (char*)"2");
    return (string)TOKEN_NAME;
}

address WasmTRC3Token::fnGetSelfAddress(){
    address addressInfo = TC_GetSelfAddress();
    return addressInfo;
}

bool WasmTRC3Token::fnSetInfo(string testInfo){
    _fnPrint((char*)"fnSetInfo", (char*)"step", (char*)"1");
    bool bResult = false;        
    TC_Payable(false);
    TC_StorageSetString(STORAGE_NAME, testInfo);
    bResult = true;
    _fnPrint((char*)"fnSetInfo", (char*)"step", (char*)"2");
    return bResult;
}
string WasmTRC3Token::fnGetInfo(){
    _fnPrint((char*)"fnGetInfo", (char*)"step", (char*)"1");
    TC_Payable(false);
    string testInfo = TC_StorageGetString(STORAGE_NAME);
    _fnPrint((char*)"fnGetInfo", (char*)"step", (char*)"2");
    return testInfo;
}

BigInt WasmTRC3Token::fnTokenBalance(address addr, address tokenAddr){
    _fnPrint((char*)"TC_TokenBalance", (char*)addr, (char*)addr);
    BigInt balance = TC_TokenBalance(addr, tokenAddr);
    _fnPrint((char*)"TC_TokenBalance", (char*)addr, (char*)balance);
    return balance;
}

// 使用链克兑换本token，比例为1:2
bool WasmTRC3Token::exchangePrint(){
    _fnPrint((char*)"exchange", (char*)"step", (char*)"1");
    bool bResult = false;
    TC_Payable(true);
    _fnPrint((char*)"exchange", (char*)"step", (char*)"2");

    // 判断支付的必须是链克
    TC_Require(0 == CMP_ADDRESS(TC_TokenAddress(), ZERO_ADDRESS)); 
    _fnPrint((char*)"exchange", (char*)"step", (char*)"3");
    // 金额必须大于0
    TC_Require(atoi64(TC_GetMsgValue()) > 0);
    _fnPrint((char*)"exchange", (char*)"step", (char*)"4");
    // 计算可买到的token数量
    uint256 count = atoi64(TC_GetMsgValue()) / _price;
    // 将合约账户的token转给msg.sender
    TC_TransferToken(TC_GetMsgSender(), TC_GetSelfAddress(), i64toa(count, 10));
    _fnPrint(TC_GetMsgSender(), TC_GetSelfAddress(), i64toa(count, 10));
    _fnPrint((char*)"exchange", (char*)"step", (char*)"5");
    // 看是否需要退款，需要的话就退回去
    uint256 refundValue = atoi64(TC_GetMsgValue()) - (count * _price);
    if(refundValue > 0) {
        TC_Transfer(TC_GetMsgSender(), i64toa(refundValue, 10));
        Transfer(TC_GetMsgSender(), LINK_ADDRESS, refundValue);    
        _fnPrint((char*)"exchange", (char*)"step", (char*)"6");
    }
    _fnPrint((char*)"exchange", (char*)"step", (char*)"7");
    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), LINK_ADDRESS, count * _price);
    _fnPrint((char*)"exchange", (char*)"step", (char*)"8");
    Transfer(TC_GetSelfAddress(), TC_GetMsgSender(), TC_GetSelfAddress(), count);
    _fnPrint((char*)"exchange", (char*)"step", (char*)"9");
    bResult = true;
    return bResult;
}

// 使用链克兑换本token，比例为1:2
bool WasmTRC3Token::exchange(){
    bool bResult = false;
    TC_Payable(true);

    // 判断支付的必须是链克
    TC_Require(0 == CMP_ADDRESS(TC_TokenAddress(), ZERO_ADDRESS)); 
    // 金额必须大于0
    TC_Require(atoi64(TC_GetMsgValue()) > 0);
    // 计算可买到的token数量
    uint256 count = atoi64(TC_GetMsgValue()) / _price;
    // 将合约账户的token转给msg.sender
    TC_TransferToken(TC_GetMsgSender(), TC_GetSelfAddress(), i64toa(count, 10));
    _fnPrint(TC_GetMsgSender(), TC_GetSelfAddress(), i64toa(count, 10));
    // 看是否需要退款，需要的话就退回去
    uint256 refundValue = atoi64(TC_GetMsgValue()) - (count * _price);
    if(refundValue > 0) {
        TC_Transfer(TC_GetMsgSender(), i64toa(refundValue, 10));
        Transfer(TC_GetMsgSender(), LINK_ADDRESS, refundValue);    
    }
    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), LINK_ADDRESS, count * _price);
    Transfer(TC_GetSelfAddress(), TC_GetMsgSender(), TC_GetSelfAddress(), count);
    bResult = true;
    return bResult;
}

// 使用token兑换链克
bool WasmTRC3Token::exchangeToLinkPrint() {
    bool bResult = false;
    TC_Payable(true);        
    // 金额必须大于0
    TC_Require(atoi64(TC_GetMsgTokenValue()) > 0);

    // 计算可买到的token数量
    uint256 link = atoi64(TC_GetMsgTokenValue()) * _price;
    _fnPrint((char*)"exchangeToLinkPrint", TC_GetMsgSender(), (char*)LINK_ADDRESS);
    _fnPrint((char*)"exchangeToLinkPrint", TC_GetMsgSender(), i64toa(link, 10));
    TC_TransferToken(TC_GetMsgSender(), LINK_ADDRESS, i64toa(link, 10));
    _fnPrint((char*)"exchangeToLinkPrint", TC_GetMsgSender(), (char*)"OK");
    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), TC_TokenAddress(), atoi64(TC_GetMsgTokenValue()));
    Transfer(TC_GetSelfAddress(), TC_GetMsgSender(), LINK_ADDRESS, link);
    bResult = true;
    return bResult;
}	

// 使用token兑换链克
bool WasmTRC3Token::exchangeToLink() {
    bool bResult = false;
    TC_Payable(true);        
    // 金额必须大于0
    TC_Require(atoi64(TC_GetMsgTokenValue()) > 0);

    // 计算可买到的token数量
    uint256 link = atoi64(TC_GetMsgTokenValue()) * _price;

    TC_TransferToken(TC_GetMsgSender(), LINK_ADDRESS, i64toa(link, 10));

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), TC_TokenAddress(), atoi64(TC_GetMsgTokenValue()));
    Transfer(TC_GetSelfAddress(), TC_GetMsgSender(), LINK_ADDRESS, link);
    bResult = true;
    return bResult;
}	

// 使用token兑换token （按照1：1兑换）
bool WasmTRC3Token::exchangeSelfToToken() {
    bool bResult = false;
    TC_Payable(true);        
    // 金额必须大于0
    TC_Require(atoi64(TC_GetMsgTokenValue()) > 0);

    TC_TransferToken(TC_GetMsgSender(), TC_TokenAddress(), BigInt(TC_GetMsgTokenValue()));

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), TC_TokenAddress(), atoi64(TC_GetMsgTokenValue()));
    Transfer(TC_GetSelfAddress(), TC_GetMsgSender(), TC_TokenAddress(), atoi64(TC_GetMsgTokenValue()));        
    bResult = true;
    return bResult;        
}

// 使用token兑换token （按照1：1兑换）
bool WasmTRC3Token::exchangeToOtherToken() {
    bool bResult = false;
    TC_Payable(true);        
    // 金额必须大于0
    TC_Require(atoi64(TC_GetMsgTokenValue()) > 0);

    TC_TransferToken(TC_GetMsgSender(), TC_GetSelfAddress(), BigInt(TC_GetMsgTokenValue()));

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), TC_TokenAddress(), atoi64(TC_GetMsgTokenValue()));
    Transfer(TC_GetSelfAddress(), TC_GetMsgSender(), TC_GetSelfAddress(), atoi64(TC_GetMsgTokenValue()));    
    bResult = true;
    return bResult;
}

// 使用链克兑换本token，比例为1:2
bool WasmTRC3Token::exchangeSupportZero() {
    bool bResult = false;
    TC_Payable(true);
    // 判断支付的必须是链克
    TC_Require(0 == CMP_ADDRESS(TC_TokenAddress(), ZERO_ADDRESS)); 
    // 计算可买到的token数量
    uint256 count = atoi64(TC_GetMsgValue()) / _price;
    // 将合约账户的token转给msg.sender
    TC_TransferToken(TC_GetMsgSender(), TC_GetSelfAddress(), i64toa(count, 10));

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), LINK_ADDRESS, count * _price);
    Transfer(TC_GetSelfAddress(), TC_GetMsgSender(), TC_GetSelfAddress(), count);
    
    // 看是否需要退款，需要的话就退回去
    uint256 refund = atoi64(TC_GetMsgValue()) - (count * _price);
    if(refund > 0) {
        TC_Transfer(TC_GetMsgSender(), i64toa(refund, 10));
        Transfer(TC_GetMsgSender(), LINK_ADDRESS, refund);    
    }

    bResult = true;
    return bResult;
}

// 使用token兑换链克
bool WasmTRC3Token::exchangeToLinkSupportZero() {
    bool bResult = false;
    TC_Payable(true);        

    // 计算可买到的token数量
    uint256 link = atoi64(TC_GetMsgTokenValue()) * _price;

    TC_TransferToken(TC_GetMsgSender(), LINK_ADDRESS, i64toa(link, 10));

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), TC_TokenAddress(), atoi64(TC_GetMsgTokenValue()));
    Transfer(TC_GetSelfAddress(), TC_GetMsgSender(), LINK_ADDRESS, link);
    bResult = true;
    return bResult;
}	


// 使用token兑换token （按照1：1兑换）
bool WasmTRC3Token::exchangeToOtherTokenSupportZero() {
    bool bResult= false;
    TC_Payable(true);        

    TC_TransferToken(TC_GetMsgSender(), TC_GetSelfAddress(), BigInt(TC_GetMsgTokenValue()));

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), TC_TokenAddress(), atoi64(TC_GetMsgTokenValue()));
    Transfer(TC_GetSelfAddress(), TC_GetMsgSender(), TC_GetSelfAddress(), atoi64(TC_GetMsgTokenValue()));    
    bResult = true;
    return bResult;        
}


bool WasmTRC3Token::testTransferLink(address _toAddress, uint256 linkValue) {
    bool bResult = false;
    TC_Payable(true);    
    // 将合约账户的token转给
    TC_TransferToken(_toAddress, LINK_ADDRESS, i64toa(linkValue, 10));

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), LINK_ADDRESS, atoi64(TC_GetMsgTokenValue()));
    Transfer(TC_GetSelfAddress(), _toAddress, LINK_ADDRESS, linkValue);   
    bResult = true;
    return bResult;        
}	

bool WasmTRC3Token::testTransferToken(address _toAddress, uint256 tokenValue) {
    bool bResult = false;
    TC_Payable(true);    
    // 将合约账户的token转给
    TC_TransferToken(_toAddress, TC_GetSelfAddress(), i64toa(tokenValue, 10));

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), TC_TokenAddress(), atoi64(TC_GetMsgTokenValue()));
    Transfer(TC_GetSelfAddress(), _toAddress, TC_GetSelfAddress(), tokenValue);   
    bResult = true;
    return bResult;        
}	

bool WasmTRC3Token::testTransferLinkAndToken(address _toAddress, uint256 tokenValue) {
    bool bResult = false;
    TC_Payable(true);    
    // 将合约账户的token转给
    TC_TransferToken(_toAddress, TC_GetSelfAddress(), i64toa(tokenValue, 10));
    TC_Transfer(_toAddress, i64toa(tokenValue, 10));

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), TC_TokenAddress(), atoi64(TC_GetMsgTokenValue()));
    Transfer(TC_GetSelfAddress(), _toAddress, TC_GetSelfAddress(), tokenValue);   
    Transfer(TC_GetSelfAddress(), _toAddress, tokenValue);   
    bResult = true;
    return bResult;        
}	


bool WasmTRC3Token::testTransferLinkAndTokenForRequire(address _toAddress, uint256 tokenValue) {
    bool bResult = false;
    TC_Payable(true);  
    // 将合约账户的token转给
	TC_Require(tokenValue == 4);
    TC_TransferToken(_toAddress, TC_GetSelfAddress(), i64toa(tokenValue, 10));
	TC_RequireWithMsg(tokenValue == 5, "TC_RequireWithMsg(tokenValue == 5)");
    TC_Transfer(_toAddress, i64toa(tokenValue, 10));
	TC_Require(tokenValue == 6);

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), TC_TokenAddress(), atoi64(TC_GetMsgTokenValue()));
    Transfer(TC_GetSelfAddress(), _toAddress, TC_GetSelfAddress(), tokenValue);   
    Transfer(TC_GetSelfAddress(), _toAddress, tokenValue);   
    bResult = true;
    return bResult;        
}	


bool WasmTRC3Token::testTransferTokenAndLinkForRequire(address _toAddress, uint256 tokenValue) {
    bool bResult = false;
    TC_Payable(true);  
    // 将合约账户的token转给
	TC_Require(tokenValue == 4);
    TC_Transfer(_toAddress, i64toa(tokenValue, 10));
	TC_RequireWithMsg(tokenValue == 5, "TC_RequireWithMsg(tokenValue == 5)");
    TC_TransferToken(_toAddress, TC_GetSelfAddress(), i64toa(tokenValue, 10));
	TC_Require(tokenValue == 6);

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), TC_TokenAddress(), atoi64(TC_GetMsgTokenValue()));
    Transfer(TC_GetSelfAddress(), _toAddress, tokenValue);   
    Transfer(TC_GetSelfAddress(), _toAddress, TC_GetSelfAddress(), tokenValue);   
    bResult = true;
    return bResult;
}	

bool WasmTRC3Token::transferLink(address _toAddress, uint256 linkValue) {
    bool bResult = false;
    TC_Payable(true);    
    // 将合约账户的token转给
    TC_TransferToken(_toAddress, LINK_ADDRESS, i64toa(linkValue, 10));

    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), LINK_ADDRESS, atoi64(TC_GetMsgTokenValue()));
    bResult = true;
    return bResult;        
}	

//--------------------------------------
//合约升级方法
void WasmTRC3Token::fnSetStorage2(char* key, char* value) {
    TC_StorageSetString(key, value);
}
char* WasmTRC3Token::fnGetStorage2(char* key) {
    return TC_StorageGetString(key);
}
void WasmTRC3Token::fnDelStorage2(char* key) {
    TC_StorageDel(key);
}
//--------------------------------------
//补充合约调合约用例
bool WasmTRC3Token::fnContractLevelClear() {
    TC_StorageSetString(CONTRACT_LEVEL_NAME_CALL, itoa(0));
    TC_StorageSetString(CONTRACT_LEVEL_NAME_DELCALL, itoa(0));
    return true;
}
char* WasmTRC3Token::fnContractCall(address contract, char* action, char* args) {
    char* contractLevel = TC_StorageGetString(CONTRACT_LEVEL_NAME_CALL);
    _fnPrint((char*)"fnContractCall1", contractLevel, (char*)contract);
    _fnPrint((char*)"fnContractCall1", contractLevel, (char*)action);
    _fnPrint((char*)"fnContractCall1", contractLevel, (char*)args);
    return TC_CallContract(contract, action, args);
}
char* WasmTRC3Token::fnDelegateContractCall(address contract, char* action, char* args) {
    char* contractLevel = TC_StorageGetString(CONTRACT_LEVEL_NAME_DELCALL);
    _fnPrint((char*)"fnDelegateContractCall1", contractLevel, (char*)contract);
    _fnPrint((char*)"fnDelegateContractCall1", contractLevel, (char*)action);
    _fnPrint((char*)"fnDelegateContractCall1", contractLevel, (char*)args);
    return TC_DelegateCallContract(contract, action, args);
}
char* WasmTRC3Token::fnContractCallStore() {
    char* contract = TC_StorageGetString(CONTRACT_LEVEL_NAME_CALL_STORE_ADDR);
    char* action = TC_StorageGetString(CONTRACT_LEVEL_NAME_CALL_STORE_ACT);
    char* args = TC_StorageGetString(CONTRACT_LEVEL_NAME_CALL_STORE_ARG);
    char* contractLevel = TC_StorageGetString(CONTRACT_LEVEL_NAME_CALL);
    int iContractLevel = atoi(contractLevel) + 1;
    TC_StorageSetString(CONTRACT_LEVEL_NAME_CALL, itoa(iContractLevel));    
    _fnPrint((char*)"fnContractCallStore1", contractLevel, (char*)contract);
    _fnPrint((char*)"fnContractCallStore1", contractLevel, (char*)action);
    _fnPrint((char*)"fnContractCallStore1", contractLevel, (char*)args);
    char* contractLevelMax = TC_StorageGetString(CONTRACT_LEVEL_NAME_CALL_STORE_MAX);
    int iContractLevelMax = atoi(contractLevelMax);
    if (iContractLevel <= iContractLevelMax)
    {
        return TC_CallContract(contract, action, args);
    }
    _fnPrint((char*)"fnContractCallStore1", (char*)"iContractLevelMax", itoa(iContractLevelMax));
    _fnPrint((char*)"fnContractCallStore1", contractLevel, (char*)"test1 end");
    return (char*)"test1 loop times over max, tese end!";   
}
char* WasmTRC3Token::fnDelegateContractCallStore() {
    char* contract = TC_StorageGetString(CONTRACT_LEVEL_NAME_DELCALL_STORE_ADDR);
    char* action = TC_StorageGetString(CONTRACT_LEVEL_NAME_DELCALL_STORE_ACT);
    char* args = TC_StorageGetString(CONTRACT_LEVEL_NAME_DELCALL_STORE_ARG);
    char* contractLevel = TC_StorageGetString(CONTRACT_LEVEL_NAME_DELCALL);
    int iContractLevel = atoi(contractLevel) + 1;
    TC_StorageSetString(CONTRACT_LEVEL_NAME_DELCALL, itoa(iContractLevel));
    _fnPrint((char*)"fnDelegateContractCallStore1", contractLevel, (char*)contract);
    _fnPrint((char*)"fnDelegateContractCallStore1", contractLevel, (char*)action);
    _fnPrint((char*)"fnDelegateContractCallStore1", contractLevel, (char*)args);
    char* contractLevelMax = TC_StorageGetString(CONTRACT_LEVEL_NAME_DELCALL_STORE_MAX);
    int iContractLevelMax = atoi(contractLevelMax);
    if (iContractLevel <= iContractLevelMax)
    {
        return TC_DelegateCallContract(contract, action, args);
    }
    _fnPrint((char*)"fnDelegateContractCallStore1", (char*)"iContractLevelMax", itoa(iContractLevelMax));
    _fnPrint((char*)"fnDelegateContractCallStore1", contractLevel, (char*)"test2 end");
    return (char*)"test1 loop times over max, tese end!";
}
int WasmTRC3Token::fnRecursion(int num) {
    int output = 0;
    if (0 < num)
        output = num + fnRecursion(num - 1);
    _fnPrint((char*)"fnRecursion", itoa(num), itoa(output));
    return output;
}

bool WasmTRC3Token::transferMulOldLink(address toAddress1, address toAddress2, address toAddress3, uint256 linkValue) {
    bool bResult = false;
    TC_Payable(true);    
    // 将合约账户的token转给
    uint256 value = linkValue;
    _fnPrint((char*)"transferMulOldLink", (char*)"TC_GetMsgSender()", (char*)TC_GetMsgSender());
    _fnPrint((char*)"transferMulOldLink", (char*)"TC_GetSelfAddress()", (char*)TC_GetSelfAddress());
    _fnPrint((char*)"transferMulOldLink", (char*)"TC_GetMsgValue", (char*)TC_GetMsgValue());
    _fnPrint((char*)"transferMulOldLink", (char*)"value", (char*)i64toa(value, 10));
    _fnPrint((char*)"transferMulOldLink", (char*)"toAddress1", (char*)toAddress1);
    _fnPrint((char*)"transferMulOldLink", (char*)"toAddress2", (char*)toAddress2);
    _fnPrint((char*)"transferMulOldLink", (char*)"toAddress3", (char*)toAddress3);    
    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), atoi64(TC_GetMsgValue()));
    // 将合约账户的token转给
    TC_Transfer(toAddress1, i64toa(value, 10));
    Transfer(TC_GetMsgSender(), toAddress1, value); 
    TC_Transfer(toAddress2, i64toa(value, 10));
    Transfer(TC_GetMsgSender(), toAddress2, value);
    TC_Transfer(toAddress3, i64toa(value, 10));
    Transfer(TC_GetMsgSender(), toAddress3, value);

    bResult = true;
    return bResult;        
}

bool WasmTRC3Token::transferMulLink(address toAddress1, address toAddress2, address toAddress3, uint256 linkValue) {
    bool bResult = false;
    TC_Payable(true);    
    // 将合约账户的token转给
    uint256 value = linkValue;
    _fnPrint((char*)"transferMulLink", (char*)"TC_GetMsgSender()", (char*)TC_GetMsgSender());
    _fnPrint((char*)"transferMulLink", (char*)"TC_GetSelfAddress()", (char*)TC_GetSelfAddress());
    _fnPrint((char*)"transferMulLink", (char*)"LINK_ADDRESS", (char*)LINK_ADDRESS);
    _fnPrint((char*)"transferMulLink", (char*)"TC_GetMsgTokenValue", (char*)TC_GetMsgTokenValue());
    _fnPrint((char*)"transferMulLink", (char*)"value", (char*)i64toa(value, 10));
    _fnPrint((char*)"transferMulLink", (char*)"toAddress1", (char*)toAddress1);
    _fnPrint((char*)"transferMulLink", (char*)"toAddress2", (char*)toAddress2);
    _fnPrint((char*)"transferMulLink", (char*)"toAddress3", (char*)toAddress3);
    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), LINK_ADDRESS, atoi64(TC_GetMsgTokenValue()));
    // 将合约账户的token转给
    TC_TransferToken(toAddress1, LINK_ADDRESS, i64toa(value, 10));
    Transfer(TC_GetMsgSender(), toAddress1, LINK_ADDRESS, value);
    TC_TransferToken(toAddress2, LINK_ADDRESS, i64toa(value, 10));
    Transfer(TC_GetMsgSender(), toAddress2, LINK_ADDRESS, value);
    TC_TransferToken(toAddress3, LINK_ADDRESS, i64toa(value, 10));
    Transfer(TC_GetMsgSender(), toAddress3, LINK_ADDRESS, value);

    bResult = true;
    return bResult;        
}	

bool WasmTRC3Token::transferMulToken(address toAddress1, address toAddress2, address toAddress3, uint256 tokenValue) {
    bool bResult = false;
    TC_Payable(true);    
    address tokenAddr = TC_TokenAddress();
    uint256 value = tokenValue;
    _fnPrint((char*)"transferMulToken", (char*)"TC_GetMsgSender()", (char*)TC_GetMsgSender());
    _fnPrint((char*)"transferMulToken", (char*)"TC_GetSelfAddress()", (char*)TC_GetSelfAddress());
    _fnPrint((char*)"transferMulToken", (char*)"tokenAddr", (char*)tokenAddr);
    _fnPrint((char*)"transferMulToken", (char*)"TC_GetMsgTokenValue", (char*)TC_GetMsgTokenValue());
    _fnPrint((char*)"transferMulToken", (char*)"value", (char*)i64toa(value, 10));
    _fnPrint((char*)"transferMulToken", (char*)"toAddress1", (char*)toAddress1);
    _fnPrint((char*)"transferMulToken", (char*)"toAddress2", (char*)toAddress2);
    _fnPrint((char*)"transferMulToken", (char*)"toAddress3", (char*)toAddress3);
    Transfer(TC_GetMsgSender(), TC_GetSelfAddress(), tokenAddr, atoi64(TC_GetMsgTokenValue()));
    // 将合约账户的token转给
    TC_TransferToken(toAddress1, tokenAddr, i64toa(value, 10));
    Transfer(TC_GetMsgSender(), toAddress1, tokenAddr, value);
    TC_TransferToken(toAddress2, tokenAddr, i64toa(value, 10));
    Transfer(TC_GetMsgSender(), toAddress2, tokenAddr, value);
    TC_TransferToken(toAddress3, tokenAddr, i64toa(value, 10));
    Transfer(TC_GetMsgSender(), toAddress3, tokenAddr, value);

    bResult = true;
    return bResult;        
}

	
//--------------------------------------


char* WasmTRC3Token::DoAction(char *action, char *args) {
    // readonly
    ACTION_FUNC("Init", Init);

    void *jsonroot = TC_JsonParse(args);
    ACTION_CODE("fnGetName", {
        string sName = fnGetName();
        return _fnOutputResult(sName,  true);
    });
    ACTION_CODE("fnGetSelfAddress", {
        string sName = fnGetSelfAddress();
        return _fnOutputResult(sName,  true);
    });    
    ACTION_CODE("fnGetInfo", {
        string info = fnGetInfo();
        return _fnOutputResult(info,  true);
    });
    ACTION_CODE("fnSetInfo", {
        _fnPrint((char*)"fnSetInfo", (char*)"step", (char*)"0");
        string testInfo = TC_JsonGetString(jsonroot, "info");
        _fnPrint((char*)"fnSetInfo", (char*)"step", (char*)"01");
        bool success = fnSetInfo(testInfo);
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
       return  _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("fnTokenBalance", {
        address addr = TC_JsonGetString(jsonroot, "address");
        address tokenAddr = TC_JsonGetString(jsonroot, "token");
        BigInt balance = fnTokenBalance(addr, tokenAddr);
        return  _fnOutputResult((char*)balance,  true);
    });
    ACTION_CODE("exchange", {     
        bool success = exchange();
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("exchangeToLink", {     
        bool success = exchangeToLink();
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("exchangeToLinkPrint", {     
        bool success = exchangeToLinkPrint();
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });    
    ACTION_CODE("exchangeSelfToToken", {     
        bool success = exchangeSelfToToken();
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("exchangeToOtherToken", {     
        bool success = exchangeToOtherToken();
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("exchangeSupportZero", {     
        bool success = exchangeSupportZero();
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("exchangeToLinkSupportZero", {     
        bool success = exchangeToLinkSupportZero();
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("exchangeToOtherTokenSupportZero", {     
        bool success = exchangeToOtherTokenSupportZero();
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("testTransferLink", {     
        address toAddr = TC_JsonGetAddress(jsonroot, "to");
        uint256 value = TC_JsonGetInt64(jsonroot, "value");
        bool success = testTransferLink(toAddr, value);
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("testTransferToken", {     
        address toAddr = TC_JsonGetAddress(jsonroot, "to");
        uint256 value = TC_JsonGetInt64(jsonroot, "value");
        bool success = testTransferToken(toAddr, value);
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("testTransferLinkAndToken", {     
        address toAddr = TC_JsonGetAddress(jsonroot, "to");
        uint256 value = TC_JsonGetInt64(jsonroot, "value");
        bool success = testTransferLinkAndToken(toAddr, value);
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("testTransferLinkAndTokenForRequire", {     
        address toAddr = TC_JsonGetAddress(jsonroot, "to");
        uint256 value = TC_JsonGetInt64(jsonroot, "value");
        bool success = testTransferLinkAndTokenForRequire(toAddr, value);
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("testTransferTokenAndLinkForRequire", {     
        address toAddr = TC_JsonGetAddress(jsonroot, "to");
        uint256 value = TC_JsonGetInt64(jsonroot, "value");
        bool success = testTransferTokenAndLinkForRequire(toAddr, value);
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("transferLink", {     
        address toAddr = TC_JsonGetAddress(jsonroot, "to");
        uint256 value = TC_JsonGetInt64(jsonroot, "value");
        bool success = transferLink(toAddr, value);
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("fnSetStorage2", {     
        char* key = TC_JsonGetString(jsonroot, "key");
        char* value = TC_JsonGetString(jsonroot, "value");
        fnSetStorage2(key, value);
        return _fnOutputResult((char*)RESULT_SUCCESS,  true);
    });
    ACTION_CODE("fnGetStorage2", {     
        char* key = TC_JsonGetString(jsonroot, "key");
        char* value = fnGetStorage2(key);
        return _fnOutputResult(value,  true);
    });
    ACTION_CODE("fnDelStorage2", {     
        char* key = TC_JsonGetString(jsonroot, "key");
        fnDelStorage2(key);
        return _fnOutputResult((char*)RESULT_SUCCESS,  true);
    });    
    ACTION_CODE("fnContractLevelClear", {     
        bool success = fnContractLevelClear();
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);        
    });
    ACTION_CODE("fnContractCall", {     
        _fnPrint((char*)"fnContractCall(INPUT)", action, args);
        address contract = TC_JsonGetAddress(jsonroot, "contract");
        char* action = TC_JsonGetString(jsonroot, "action");
        char* args = TC_JsonGetString(jsonroot, "args");
        char* result = fnContractCall(contract, action, args);
        return _fnOutputResult(result,  (char*)RESULT_SUCCESS);
    });
    ACTION_CODE("fnDelegateContractCall", {
        _fnPrint((char*)"fnDelegateContractCall(INPUT)", action, args);
        address contract = TC_JsonGetAddress(jsonroot, "contract");
        char* action = TC_JsonGetString(jsonroot, "action");
        char* args = TC_JsonGetString(jsonroot, "args");
        char* result = fnDelegateContractCall(contract, action, args);
        return _fnOutputResult(result,  (char*)RESULT_SUCCESS);
    });
    ACTION_CODE("fnContractCallStore", {     
        _fnPrint((char*)"fnContractCallStore(INPUT)", action, args);
        char* result = fnContractCallStore();
        return _fnOutputResult(result,  (char*)RESULT_SUCCESS);
    });
    ACTION_CODE("fnDelegateContractCallStore", {
        _fnPrint((char*)"fnDelegateContractCallStore(INPUT)", action, args);
        char* result = fnDelegateContractCallStore();
        return _fnOutputResult(result,  (char*)RESULT_SUCCESS);
    });    
    ACTION_CODE("fnRecursion", {     
        int num = TC_JsonGetInt(jsonroot, "num");
        int sum = fnRecursion(num);
        return _fnOutputResult(itoa(sum),  (char*)RESULT_SUCCESS);
    });
    ACTION_CODE("transferMulOldLink", {     
        address toAddr1 = TC_JsonGetAddress(jsonroot, "address1");
        address toAddr2 = TC_JsonGetAddress(jsonroot, "address2");
        address toAddr3 = TC_JsonGetAddress(jsonroot, "address3");
        uint256 value = TC_JsonGetInt64(jsonroot, "value");
        bool success = transferMulOldLink(toAddr1, toAddr2, toAddr3, value);
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });    
    ACTION_CODE("transferMulLink", {     
        address toAddr1 = TC_JsonGetAddress(jsonroot, "address1");
        address toAddr2 = TC_JsonGetAddress(jsonroot, "address2");
        address toAddr3 = TC_JsonGetAddress(jsonroot, "address3");
        uint256 value = TC_JsonGetInt64(jsonroot, "value");
        bool success = transferMulLink(toAddr1, toAddr2, toAddr3, value);
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });
    ACTION_CODE("transferMulToken", {     
        address toAddr1 = TC_JsonGetAddress(jsonroot, "address1");
        address toAddr2 = TC_JsonGetAddress(jsonroot, "address2");
        address toAddr3 = TC_JsonGetAddress(jsonroot, "address3");
        uint256 value = TC_JsonGetInt64(jsonroot, "value");
        bool success = transferMulToken(toAddr1, toAddr2, toAddr3, value);
        string sValue = (char*)(success ? RESULT_SUCCESS : RESULT_FAILURE);
        return _fnOutputResult(sValue,  success);
    });

    TC_RevertWithMsg("No match action");
    return EMPTY_CSTRING;
}

// ----------------------------------------------------------------
// main entry.
extern "C" {
    char *thunderchain_main(char *action, char *args) {
        WasmTRC3Token Trc3token;
        return Trc3token.DoAction(action, args);
    }
}
