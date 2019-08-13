#include "tcapi.h"
#include "string.h"



//===================================================
//定义类型及宏
//---------------------------------------------------
typedef unsigned int uint; 
typedef char* string; 
#define MAX_ADDR_NUM (10000)
#define BUFFER_RESULT_SIZE (2000)
#define BUFFER_SIZE (200)
#define LINK_ADDRESS ZERO_ADDRESS
//===================================================

//===================================================
//定义参数结构
//---------------------------------------------------
char* m_owner = "m_owner";
char* m_name = "m_name";
char* m_status = "m_status";

//===================================================
//内部方法
//内部函数
void fnPrint(char* str1, char* str2)
{
    char* str = strconcat("[WASM_SIMPLE]|", strconcat(str1, strconcat("|", str2)));
    TC_Prints(str);
}


char* _fnOutputResult(char* value, char* type, bool success)
{
    char* pResult = TC_JsonNewObject();

    TC_JsonPutString((char*)pResult, "type", type);
    TC_JsonPutString((char*)pResult, "value", value);
    TC_JsonPutInt((char*)pResult, "success", success);

    return TC_JsonToString(pResult);
}

//===================================================
//封装的方法
//---------------------------------------------------
//事件监听
void fnEventLogDepositMade(address fromAddr, address toAddr, uint64 value)
{
    void *jsroot = TC_JsonNewObject();
    TC_JsonPutString(jsroot, "from", fromAddr);
    TC_JsonPutString(jsroot, "to", toAddr);
    BigInt valueInfo = (BigInt)i64toa(value, 10);    
    TC_JsonPutString(jsroot, "value", valueInfo);
    TC_Notify("Transfer(address,address,uint256)", TC_JsonToString(jsroot));    
}

void fnEventTestSpecial()
{
    char* special = "abc~!@#$%^&*()adf 879889\t_+-=`1[]{}\\|;':\"<>,.?/erer";
    TC_Notify("Transfer(address,address,uint256)", special);    
}

void fnEventTestSpecialNullChar()
{
    char* special = "abc\t\r\n\0aslkdjferer";
    TC_Notify("Transfer(address,address,uint256)", special);    
}
void fnEventTestMultiTopics(address fromAddr, address toAddr, uint64 value)
{
    TC_Log0("Transfer(address,address,uint256)");    
    TC_Log4("Transfer(address,address,uint256)", fromAddr, toAddr, i64toa(LINK_ADDRESS, 16), i64toa(value, 16));    
}

// init sender balance
char *Init(){
	return "no process, init success！";
}


//初始化账户
void fnSetOwner(address owner)
{
    TC_StorageSetString(m_owner, owner);
}

address fnGetOwner()
{
    address owner = TC_StorageGetString(m_owner);
    return owner;
}

//设置name
void fnSetRegister(string name)
{
    TC_StorageSetString(m_name, name);
}
//获取name
string fnViewRegister()
{
    string name = TC_StorageGetString(m_name);
    return name; 
}

//设置name
void fnSetClearStatus()
{
    TC_StorageSetString(m_status, "0");
}

//设置name
void fnSetStatus()
{
    TC_StorageSetString(m_status, "1");
}
//获取name
bool fnViewStatus()
{
    char* status = TC_StorageGetString(m_status);
    return atoi(status);
}

//转账(单位只支持wei)
void transferInput()
{
    //转账成功触发事件
    address sender = TC_GetMsgSender();
    address contract = TC_GetSelfAddress();
    BigInt sendValue = TC_GetMsgValue();
    fnEventLogDepositMade(sender, contract, atoi64(sendValue));
    
    fnSetStatus();    
}

//转账(单位只支持wei)
void transferOutputCondition(address toAddr, uint64 value)
{
    BigInt bigNum = TC_TOKEN(1);
    int64 i64Num = TC_BigIntToInt64(bigNum);

    TC_Require(value > i64Num);

    TC_Transfer(toAddr, i64toa(value, 10));
    //转账成功触发事件
    address sender = TC_GetMsgSender();
    address contract = TC_GetSelfAddress();
    BigInt sendValue = TC_GetMsgValue();
    fnEventLogDepositMade(sender, contract, atoi64(sendValue));
    fnEventLogDepositMade(contract, toAddr, value);
    
    fnSetStatus();
}

//转账(单位只支持wei)
void transferOutput(address toAddr, uint64 value)
{
    TC_Transfer(toAddr, i64toa(value, 10));
    //转账成功触发事件
    address sender = TC_GetMsgSender();
    address contract = TC_GetSelfAddress();
    BigInt sendValue = TC_GetMsgValue();
    fnEventLogDepositMade(sender, contract, atoi64(sendValue));
    fnEventLogDepositMade(contract, toAddr, value);

    fnSetStatus();    
}

//转账(单位只支持wei)
void transferOnlyOutput(address toAddr, uint64 value)
{
    TC_Transfer(toAddr, i64toa(value, 10));
    //转账成功触发事件
    address contract = TC_GetSelfAddress();
    fnEventLogDepositMade(contract, toAddr, value);
    
    fnSetStatus();    
}

//转账(单位只支持wei)
char* kill()
{
    address sender = TC_GetMsgSender();
    address owner = fnGetOwner();
    TC_Require(0 == strcmp(sender, owner));
    //转账成功触发事件
    return TC_SelfDestruct(owner);
}
//--------------------------------------
//合约升级方法
void fnSetStorage1(char* key, char* value) {
    TC_StorageSetString(key, value);
}
char* fnGetStorage1(char* key) {
    return TC_StorageGetString(key);
}
void fnDelStorage1(char* key) {
    TC_StorageDel(key);
}
//--------------------------------------
//事件订阅
void transferOutputSpecial(address toAddr, uint64 value)
{
    TC_Transfer(toAddr, i64toa(value, 10));
    //转账成功触发事件
    fnEventTestSpecial();
}
void transferOutputSpecialNullChar(address toAddr, uint64 value)
{
    TC_Transfer(toAddr, i64toa(value, 10));
    //转账成功触发事件
    fnEventTestSpecialNullChar();
}

void transferOutputMultiTopics(address toAddr, uint64 value)
{
    TC_Transfer(toAddr, i64toa(value, 10));
    //转账成功触发事件
    fnEventTestMultiTopics(TC_GetMsgSender(), toAddr, value);
}

//--------------------------------------
//性能验证
char* fnEmptyFunc()
{
    return (char*)"fnEmptyFunc()";
}

//--------------------------------------
//增加异常场景 2019-03-21
//转账(单位只支持wei)
void transferOutputRequire(address toAddr, uint64 value)
{
    fnPrint("transferOutputRequire", "start");
    //构造转入后失败退出，验证是否回滚
    TC_Require(value != 3); 
    fnPrint("transferOutputRequire", "require!=3");
    if (value == 5)
    {
        fnPrint("transferOutputRequire", "value == 5");
        TC_Revert();
    }
    TC_Transfer(toAddr, i64toa(value, 10));
    fnPrint("transferOutputRequire", "TC_Transfer");
    //转账成功触发事件
    address sender = TC_GetMsgSender();
    address contract = TC_GetSelfAddress();
    BigInt sendValue = TC_GetMsgValue();
    fnEventLogDepositMade(contract, toAddr, value);
    fnPrint("transferOutputRequire", "fnEvent");
    //构造转入并转出失败退出，验证是否回滚
    TC_Require(value != 4); 
    fnPrint("transferOutputRequire", "value != 4");
    if (value == 6)
    {
        fnPrint("transferOutputRequire", "value == 6");
        TC_Revert();
    }

    fnSetStatus();    
}


//应用层：调用其他合约方法
char* callContract(char* contractAddr, char* action, char* args, uint64 value)
{
    //-------------------------------------------
    fnPrint("callContract", contractAddr);
    if (value == 1000000000000000000)
    {
        TC_Transfer(contractAddr, "1000000000000000000");
    }
    char* output = TC_CallContract(contractAddr, action, args);
    //-------------------------------------------
    return output;
}


//应用层：调用其他合约方法
char* delegateCallContract(char* contractAddr, char* action, char* args, uint64 value)
{
    //-------------------------------------------
    fnPrint("delegateCallContract", contractAddr);
    if (value == 1000000000000000000)
    {
        TC_Transfer(contractAddr, "1000000000000000000");
    }    
    char* output = TC_DelegateCallContract(contractAddr, action, args);
    //-------------------------------------------
    return output;
}


//应用层：调用其他合约方法
char* callContract_transferOutputRequire(char* contractAddr, address toAddr, uint64 amount, uint64 value)
{
    //-------------------------------------------
    fnPrint("callContract", contractAddr);
    if (value == 1000000000000000000)
    {
        TC_Transfer(contractAddr, "1000000000000000000");
    }
    char* args = strconcat("{\"address\":\"", strconcat(toAddr, strconcat("\",\"amount\":\"", strconcat(i64toa(amount,16), "\"}"))));
    char* output = TC_CallContract(contractAddr, "transferOutputRequire", args);
    //-------------------------------------------
    return output;
}


//应用层：调用其他合约方法
char* delegateCallContract_transferOutputRequire(char* contractAddr, address toAddr, uint64 amount, uint64 value)
{
    //-------------------------------------------
    fnPrint("delegateCallContract", contractAddr);
    if (value == 1000000000000000000)
    {
        TC_Transfer(contractAddr, "1000000000000000000");
    }    
    char* args = strconcat("{\"address\":\"", strconcat(toAddr, strconcat("\",\"amount\":\"", strconcat(i64toa(amount,16), "\"}"))));
    char* output = TC_DelegateCallContract(contractAddr, "transferOutputRequire", args);
    //-------------------------------------------
    return output;
}

bool fnTestTransfer(char* address, char* amount)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_Transfer 验证(该接口没有返回是，如果执行失败，底层会实现回滚，并且退出虚拟机)(该接口验证需要配合查询接口验证)
        //------------------
        fnPrint("fnTestTransfer|input|address", address);
        fnPrint("fnTestTransfer|input|amount", amount);
        //------------------      
        TC_Transfer(address, amount);
        //------------------              
        fnPrint("fnTestTransfer|", "OK");
        //------------------      
        bResult = true;
        //---------------------------------------
    }while(0);
    //-------------------------------------------
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}




//===================================================
//合约接口入口
//合约功能：转账及查看合约记录
//---------------------------------------------------
char *thunderchain_main(char *action, char *args) 
{
    if (0 == strcmp(action, "fnEmptyFunc")){
        return fnEmptyFunc();
    }
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
        else if (0 == strcmp(action ,"fnSetOwner")){
            address addr = TC_JsonGetString(pInput, "address");
            fnSetOwner(addr);
            pOutput = _fnOutputResult("success!", "string", 1);
        }
        else if (0 == strcmp(action ,"fnGetOwner")){             
            address addr = fnGetOwner();
            pOutput = _fnOutputResult(addr, "string", 1);
        }
        else if (0 == strcmp(action ,"fnSetRegister")){             
            string name = TC_JsonGetString(pInput, "name");
            fnSetRegister(name);
            pOutput = _fnOutputResult("success!", "string", 1);
        }
        else if (0 == strcmp(action ,"fnViewRegister")){             
            string name = fnViewRegister();
            pOutput = _fnOutputResult(name, "string", 1);
        }
        else if (0 == strcmp(action ,"fnSetClearStatus")){             
            fnSetClearStatus();
            pOutput = _fnOutputResult("success", "string", 1);
        }
        else if (0 == strcmp(action ,"fnSetStatus")){             
            fnSetStatus();
            pOutput = _fnOutputResult("success", "string", 1);
        }
        else if (0 == strcmp(action ,"fnViewStatus")){             
            bool bStatus = fnViewStatus();
            pOutput = _fnOutputResult(itoa(bStatus), "string", 1);
        }
        else if (0 == strcmp(action ,"transferInput")){             
            transferInput();
            pOutput = _fnOutputResult("transferInput success", "string", 1);
        }
        else if (0 == strcmp(action ,"transferOutputCondition")){             
            address toAddr = TC_JsonGetString(pInput, "address");
            uint64 amount = TC_JsonGetInt64(pInput, "amount");
            transferOutputCondition(toAddr, amount);
            pOutput = _fnOutputResult("transferOutputCondition success!", "string", 1);
        }
        else if (0 == strcmp(action ,"transferOutput")){             
            address toAddr = TC_JsonGetString(pInput, "address");
            uint64 amount = TC_JsonGetInt64(pInput, "amount");
            transferOutput(toAddr, amount);
            pOutput = _fnOutputResult("transferOutput success!", "string", 1);
        }
        else if (0 == strcmp(action ,"transferOutputRequire")){             
            address toAddr = TC_JsonGetString(pInput, "address");
            uint64 amount = TC_JsonGetInt64(pInput, "amount");
            transferOutputRequire(toAddr, amount);
            pOutput = _fnOutputResult("transferOutputRequire success!", "string", 1);
        }        
        else if (0 == strcmp(action ,"callContract")){             
            char* contract = TC_JsonGetString(pInput, "contract");
            char* action = TC_JsonGetString(pInput, "action");
            char* args = TC_JsonGetString(pInput, "args");
            uint64 value = TC_JsonGetInt64(pInput, "value");
            callContract(contract, action, args, value);
            pOutput = _fnOutputResult("callContract success!", "string", 1);
        }        
        else if (0 == strcmp(action ,"delegateCallContract")){             
            char* contract = TC_JsonGetString(pInput, "contract");
            char* action = TC_JsonGetString(pInput, "action");
            char* args = TC_JsonGetString(pInput, "args");
            uint64 value = TC_JsonGetInt64(pInput, "value");
            delegateCallContract(contract, action, args, value);
            pOutput = _fnOutputResult("delegateCallContract success!", "string", 1);
        }
        else if (0 == strcmp(action ,"callContract_transferOutputRequire")){             
            char* contract = TC_JsonGetString(pInput, "contract");
            address toAddr = TC_JsonGetString(pInput, "address");
            uint64 amount = TC_JsonGetInt64(pInput, "amount");
            uint64 value = TC_JsonGetInt64(pInput, "value");
            callContract_transferOutputRequire(contract, toAddr, amount, value);
            pOutput = _fnOutputResult("callContract_transferOutputRequire success!", "string", 1);
        }      
        else if (0 == strcmp(action ,"delegateCallContract_transferOutputRequire")){             
            char* contract = TC_JsonGetString(pInput, "contract");
            address toAddr = TC_JsonGetString(pInput, "address");
            uint64 amount = TC_JsonGetInt64(pInput, "amount");
            uint64 value = TC_JsonGetInt64(pInput, "value");
            delegateCallContract_transferOutputRequire(contract, toAddr, amount, value);
            pOutput = _fnOutputResult("delegateCallContract_transferOutputRequire success!", "string", 1);
        }                        
        else if (0 == strcmp(action ,"transferOnlyOutput")){             
            address toAddr = TC_JsonGetString(pInput, "address");
            uint64 amount = TC_JsonGetInt64(pInput, "amount");
            transferOnlyOutput(toAddr, amount);
            pOutput = _fnOutputResult("transferOnlyOutput success!", "string", 1);
        }
        else if (0 == strcmp(action ,"kill")){             
            char* result = kill();
            pOutput = _fnOutputResult(result, "string", 1);
        }        
        else if (0 == strcmp(action ,"fnSetStorage1")){
            char* key = TC_JsonGetString(pInput, "key");
            char* value = TC_JsonGetString(pInput, "value");
            fnSetStorage1(key, value);
            pOutput = _fnOutputResult("fnSetStorage success!", "string", 1);
        }
        else if (0 == strcmp(action ,"fnGetStorage1")){ 
            char* key = TC_JsonGetString(pInput, "key");
            char* value = fnGetStorage1(key);
            pOutput = _fnOutputResult(value, "string", 1);
        }
        else if (0 == strcmp(action ,"fnDelStorage1")){ 
            char* key = TC_JsonGetString(pInput, "key");
            fnDelStorage1(key);
            pOutput = _fnOutputResult("fnDelStorage success!", "string", 1);
        }
        else if (0 == strcmp(action ,"transferOutputSpecial")){             
            address toAddr = TC_JsonGetString(pInput, "address");
            uint64 amount = TC_JsonGetInt64(pInput, "amount");
            transferOutputSpecial(toAddr, amount);
            pOutput = _fnOutputResult("transferOutputSpecial success!", "string", 1);
        }
        else if (0 == strcmp(action ,"transferOutputSpecialNullChar")){             
            address toAddr = TC_JsonGetString(pInput, "address");
            uint64 amount = TC_JsonGetInt64(pInput, "amount");
            transferOutputSpecialNullChar(toAddr, amount);
            pOutput = _fnOutputResult("transferOutputSpecialNullChar success!", "string", 1);
        }                
        else if (0 == strcmp(action ,"transferOutputMultiTopics")){             
            address toAddr = TC_JsonGetString(pInput, "address");
            uint64 amount = TC_JsonGetInt64(pInput, "amount");
            transferOutputMultiTopics(toAddr, amount);
            pOutput = _fnOutputResult("transferOutputMultiTopics success!", "string", 1);
        }       
        else if (0 == strcmp(action ,"fnTestTransfer")){
            char* address = TC_JsonGetString(pInput, "address");
            char* amount = TC_JsonGetString(pInput, "amount");    
            bool bResult = fnTestTransfer(address, amount);
            char* info = bResult ? "success" : "failure";
            char* result = _fnOutputResult(info, "string", bResult);
            pOutput = result;
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

//===================================================
