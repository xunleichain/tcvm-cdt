#include "tcapi.h"
#include "string.h"
#include "stdio.h"


#define PRINT_BUFFER (2000)
// typedef char* address; 
// typedef char* string; 
typedef unsigned int uint; 

char* NAME = "wasm standard token Demo";
int DECIMALS = 0;
char* SYMBOL = "ZZT";
uint64 TOTAL_SUPPLY = 100000000000;
int RADIX = 10;


//-------------------------------------------------------------------
//内部函数
void _fnPrint(char* str1, char* str2, char* str3)
{
    char* str = (char*)malloc(PRINT_BUFFER);
    memset((char*)str, 0, PRINT_BUFFER);
    sprintf(str, "[WASM_TOKEN_TEST_DEBUG] | %s | %s | %s |", str1, str2, str3);
    TC_Prints(str);
    free((char*)str);
}

char* _fnOutputResult(char* value, char* type, bool success)
{
    char* pResult = TC_JsonNewObject();

    TC_JsonPutString((char*)pResult, "type", type);
    TC_JsonPutString((char*)pResult, "value", value);
    TC_JsonPutInt((char*)pResult, "success", success);

    return TC_JsonToString(pResult);
}
//-------------------------------------------------------------------

char *_balanceKey(address addr){
    char* balanceKey = strconcat("balance_",addr);
    _fnPrint("_balanceKey", "balanceKey", balanceKey);
	return balanceKey;
}

char *_allowedKey(address spender, address sender){
    char* allowedKey = strconcat(strconcat("allowed_", spender), sender);
    _fnPrint("_allowedKey", "allowedKey", allowedKey);
	return allowedKey;
}

uint64 _getBalance(address addr){
	char *balancekey = _balanceKey(addr);
	char *storeBalance = TC_StorageGetString(balancekey);
	uint64 ibalance = atoi64(storeBalance);
    _fnPrint("_getBalance", balancekey, storeBalance);
	return ibalance;
}

void _setBalance(address addr, uint64 value){
	char *balancekey = _balanceKey(addr);
    char* valueInfo = i64toa(value, RADIX);
	TC_StorageSetString(balancekey, valueInfo);
    _fnPrint("_setBalance", balancekey, valueInfo);
}

uint64 _getAllowedBalance(address spender, address sender){
	char *allowedkey = _allowedKey(spender, sender);
	char *storeAllowedBalance = TC_StorageGetString(allowedkey);
	uint64 iAllowed = atoi64(storeAllowedBalance);
    _fnPrint("_getAllowedBalance", allowedkey, storeAllowedBalance);
	return iAllowed;
}

void _setAllowedBalance(address spender, address sender, uint64 value){
	char *allowedkey = _allowedKey(spender, sender);
    char* valueInfo = i64toa(value, RADIX);
	TC_StorageSetString(allowedkey, valueInfo);
    _fnPrint("_setAllowedBalance", allowedkey, valueInfo);
}

//-------------------------------------------------------------------
//初始化函数
// init sender balance
char *Init(){
	//char *sender = TC_GetMsgSender();
	address sender = "0x54fb1c7d0f011dd63b08f85ed7b518ab82028100";
	TC_Prints(sender);
	uint64 beforeInit = _getBalance(sender);
	if(beforeInit != 0){
		return "init before";
	}
    _setBalance(sender, TOTAL_SUPPLY);
	return "init success";
}

//-------------------------------------------------------------------
//ERC20标准接口(事件)
//事件
void Transfer(address fromAddr, address toAddr, uint64 value)
{
    void *jsroot = TC_JsonNewObject();
    TC_JsonPutString(jsroot, "from", fromAddr);
    TC_JsonPutString(jsroot, "to", toAddr);
    BigInt valueInfo = (BigInt)i64toa(value, 10);
    TC_JsonPutString(jsroot, "value", valueInfo);
    TC_Notify("Transfer(address,address,uint256)", TC_JsonToString(jsroot));
}
void Approval(address sender, address spender, uint64 value)
{
    void *jsroot = TC_JsonNewObject();
    // TC_JsonPutString(jsroot, "sender", sender);
    // TC_JsonPutString(jsroot, "spender", spender);
    // BigInt valueInfo = (BigInt)i64toa(value, 10);
    // TC_JsonPutString(jsroot, "value", valueInfo);
    TC_JsonPutString(jsroot, "from", sender);
    TC_JsonPutString(jsroot, "to", spender);
    BigInt valueInfo = (BigInt)i64toa(value, 10);
    TC_JsonPutString(jsroot, "value", valueInfo);    
    TC_Notify("Approve(address,address,uint256)", TC_JsonToString(jsroot));    
}

//-------------------------------------------------------------------
//ERC20标准接口
bool transfer(address _to, uint64 _value)
{
	//-版本问题TC_Require(TC_IsHexAddress(_to));

    address sender = TC_GetMsgSender();
    uint64 senderBalance = _getBalance(sender);

    _fnPrint("transfer|sender", sender, i64toa(senderBalance, RADIX));
    _fnPrint("transfer|to", _to, i64toa(_value, RADIX));    
	TC_Require(senderBalance > _value);

	senderBalance = senderBalance - _value;

	uint64 toBalance = _getBalance(_to);
	toBalance = toBalance + _value;

    _setBalance(sender, senderBalance);
    _setBalance(_to, toBalance);

	Transfer(sender, _to, _value);

	return true;
}


bool approve(address _spender, uint64 _value){
	//-版本问题TC_Require(TC_IsHexAddress(_spender));

	char *sender = TC_GetMsgSender();
    uint64 senderBalance = _getBalance(sender);

    _fnPrint("approve|sender", sender, i64toa(senderBalance, RADIX));
    _fnPrint("approve|spender", _spender, i64toa(_value, RADIX));
	TC_Require(senderBalance > _value);

    _setAllowedBalance(_spender, sender, _value);

    Approval(sender, _spender, _value);

	return true;
}

uint64 allowance(address _owner, address _spender){
	//-版本问题TC_Require(TC_IsHexAddress(_owner));
	//-版本问题TC_Require(TC_IsHexAddress(_spender));

    uint64 leftAllowBalance = _getAllowedBalance(_spender, _owner);
    _fnPrint("allowance", _spender, i64toa(leftAllowBalance, RADIX));
    
	return leftAllowBalance;
}

bool transferFrom(address _from, address  _to, uint64 _value){
	//-版本问题TC_Require(TC_IsHexAddress(_from));
	//-版本问题TC_Require(TC_IsHexAddress(_to));

	address sender = TC_GetMsgSender();
	uint64 allowanceBalance = _getAllowedBalance(sender, _from);
	uint64 fromBalance = _getBalance(_from);

    _fnPrint("transferFrom|balance", _from, i64toa(fromBalance, RADIX));
    _fnPrint("transferFrom|allowedBalance", sender, i64toa(allowanceBalance, RADIX));
    _fnPrint("transferFrom|toValue", _to, i64toa(_value, RADIX));
	TC_Require(allowanceBalance >= _value && fromBalance >= _value);
	fromBalance = fromBalance - _value;

	allowanceBalance = allowanceBalance - _value;

	uint64 toBalance = _getBalance(_to);
	toBalance = toBalance + _value;

    _setBalance(_from, fromBalance);
    _setBalance(_to, toBalance);

    _setAllowedBalance(sender, _from, allowanceBalance);

    Transfer(_from, _to, _value);

	return true;
}

uint64 balanceOf(address _owner)
{
    //-版本问题	TC_Require(TC_IsHexAddress(_owner));
	return _getBalance(_owner);
}

uint64 totalSupply(){
	return TOTAL_SUPPLY;
}

int decimals(){
	return DECIMALS;
}

char * symbol(){
	return SYMBOL;
}

char * name(){
	return NAME;
}


//-----------------------------------------------------
//入口定义函数
char *thunderchain_main(char *action, char *args) {
    //---------------------------------------
    char* pOutput = NULL;
    //-----------------------------------------------
    //json转换  (转成字符串)  
    char* pInput = TC_JsonParse(args);
    //-----------------------------------------------   
    //测试底层接口-内存
    do{
        if (0 == strcmp(action ,"Init")){
            pOutput = Init();
        }
        else if (0 == strcmp(action ,"name")){
            char * nameInfo = name();
            pOutput = _fnOutputResult(nameInfo, "string", 1);
        }
        else if (0 == strcmp(action ,"symbol")){
            char * symbolInfo = symbol();
            pOutput = _fnOutputResult(symbolInfo, "string", 1);
        }
        else if (0 == strcmp(action ,"decimals")){
            int decimalsValue = decimals();
            pOutput = _fnOutputResult(itoa(decimalsValue), "int", 1);
        }
        else if (0 == strcmp(action ,"totalSupply")){
            uint64 total = totalSupply();
            pOutput = _fnOutputResult(i64toa(total, RADIX), "int", 1);
        }
        else if (0 == strcmp(action ,"balanceOf")){
            address addr = TC_JsonGetString(pInput, "address");            
            uint64 balance = balanceOf(addr);
            pOutput = _fnOutputResult(i64toa(balance, RADIX), "int", 1);
        }
        else if (0 == strcmp(action ,"transfer")){
            address to = TC_JsonGetString(pInput, "to");
            uint64 value = TC_JsonGetInt64(pInput, "value");
            bool result = transfer(to, value);
            pOutput = _fnOutputResult(itoa(result), "bool", result);
        }
        else if (0 == strcmp(action ,"approve")){
            address spender = TC_JsonGetString(pInput, "spender");
            uint64 value = TC_JsonGetInt64(pInput, "value");
            bool result = approve(spender, value);
            pOutput = _fnOutputResult(itoa(result), "bool", result);
        }
        else if (0 == strcmp(action ,"transferFrom")){
            address from = TC_JsonGetString(pInput, "from");
            address to = TC_JsonGetString(pInput, "to");
            uint64 value = TC_JsonGetInt64(pInput, "value");
            bool result = transferFrom(from, to, value);
            pOutput = _fnOutputResult(itoa(result), "bool", result);
        }
        else if (0 == strcmp(action ,"allowance")){
            address owner = TC_JsonGetString(pInput, "owner");
            address spender = TC_JsonGetString(pInput, "spender");
            uint64 allow = allowance(owner, spender);
            pOutput = _fnOutputResult(i64toa(allow, RADIX), "int", 1);
        }
        else
        {
            char* pResult = strconcat("当前接口不支持，请确认是否传入错误！", action);
            pOutput = _fnOutputResult((char*)pResult, "string", 0);
            TC_RevertWithMsg("No match action"); 
        }
    }while(0);
    //-----------------------------------------------    
     return pOutput;
    //-----------------------------------------------    
}
