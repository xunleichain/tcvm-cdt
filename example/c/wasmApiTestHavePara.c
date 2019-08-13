
#include "tcapi.h"
#include "stdio.h"
#include "string.h"





//===================================================
//定义类型及宏
//---------------------------------------------------
typedef unsigned int uint; 
#define FALSE_BREAK   {break;}
#define BUFFER_SIZE 500
#define OUTPUT_BUFFER_SIZE 51200
#define OUTPUT_BUFFER_SIZE_BIGCHANGE 51200
#define PRINT_BUFFER 2000
#define LINK_ADDRESS ZERO_ADDRESS
//===================================================



//===================================================
//内部调用函数
//===================================================
void _fnPrint(char* str1, char* str2, char* str3)
{
    char* str = (char*)malloc(PRINT_BUFFER);
    memset((char*)str, 0, PRINT_BUFFER);
    sprintf(str, "[WASM_API_TEST_DEBUG] | %s | %s | %s |", str1, str2, str3);
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
// 合约在部署的时候，进行调用的方法，方法名固定
bool Init(char* pOutput)
{
   //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);         
    //-------------------------------------------
    do{
        //---------------------------------------
        //------------------
        _fnPrint("Init", "result", "no prcess, init success!");
        //------------------        
        pBuffer = "no process, init success!";
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pBuffer, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);
    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;     
    //-------------------------------------------
    return bResult;
}


//应用层：计算
bool fnTestApplNormalCalcHash(const char* pInput, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;     
    char* pResult = (char*)malloc(BUFFER_SIZE);       
    memset((char*)pResult, 0, BUFFER_SIZE); 
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_Ripemd160 验证， 结果以python中的信息进行验证import hashlib
        char* ripemd160 = TC_Ripemd160((char*)pInput);
        //------------------
        _fnPrint("fnTestApplNormalCalcHash", "TC_Ripemd160|input", (char*)pInput);
        _fnPrint("fnTestApplNormalCalcHash", "TC_Ripemd160|output", ripemd160);
        //------------------          
        //TC_Sha256 验证
        char* sha256 = TC_Sha256((char*)pInput);
        //------------------
        _fnPrint("fnTestApplNormalCalcHash", "TC_Sha256|input", (char*)pInput);
        _fnPrint("fnTestApplNormalCalcHash", "TC_Sha256|output", sha256);
        //------------------           
        //TC_Keccak256 验证
        char* keccak256 = TC_Keccak256((char*)pInput);
        //------------------
        _fnPrint("fnTestApplNormalCalcHash", "TC_Keccak256|input", (char*)pInput);
        _fnPrint("fnTestApplNormalCalcHash", "TC_Keccak256|output", keccak256);
        //------------------           
        sprintf(pResult, "{\"%s\":\"%s\", \"%s\":\"%s\", \"%s\":\"%s\"}", "TC_Ripemd160", ripemd160, "TC_Sha256", sha256, "TC_Keccak256", keccak256);
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pResult, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);
    //-------------------------------------------
    //释放内存空间
    free(pResult);
    pResult = NULL; 
    //-------------------------------------------
    return bResult;
}

//应用层：获取块hash
bool fnTestApplNormalBlockHash(int64 iInput, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);         
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_BlockHash 验证， 结果以python中的信息进行验证
        pBuffer = TC_BlockHash(iInput);
        //------------------
        _fnPrint("fnTestApplNormalBlockHash", "TC_BlockHash|input", i64toa(iInput, 10));
        _fnPrint("fnTestApplNormalBlockHash", "TC_BlockHash|output", pBuffer);
        //------------------              
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pBuffer, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);
    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}


//应用层：交易接口，转账
bool fnTestApplNormalTxTransfer(char* address, char* amount, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);             
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_Transfer 验证(该接口没有返回是，如果执行失败，底层会实现回滚，并且退出虚拟机)(该接口验证需要配合查询接口验证)
        //------------------
        _fnPrint("fnTestApplNormalTxTransfer", "TC_Transfer|input|address", address);
        _fnPrint("fnTestApplNormalTxTransfer", "TC_Transfer|input|amount", amount);
        //------------------      
        TC_Transfer(address, amount);
        //---------------------------------------
        memcpy((char*)pOutput, "TC_Transfer OK", BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);
    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}


//应用层：交易接口，转账
bool fnTestApplNormalTxTransferToken(char* address, char* amount, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);             
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_Transfer 验证(该接口没有返回是，如果执行失败，底层会实现回滚，并且退出虚拟机)(该接口验证需要配合查询接口验证)
        //------------------
        _fnPrint("fnTestApplNormalTxTransferToken", "TC_TransferToken|input|address", address);
        _fnPrint("fnTestApplNormalTxTransferToken", "TC_TransferToken|input|amount", amount);
        //------------------      
        TC_TransferToken(address, LINK_ADDRESS, amount);
        //---------------------------------------
        memcpy((char*)pOutput, "TC_TransferToken OK", BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);
    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}


//应用层：交易接口，获取账户余额
bool fnTestApplNormalTxGetBalance(char* address, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);             
    //-------------------------------------------
    do{
        //TC_GetBalance 验证
        pBuffer = TC_GetBalance(address);
        //------------------
        _fnPrint("fnTestApplNormalTxGetBalance", "TC_Transfer|input", address);
        _fnPrint("fnTestApplNormalTxGetBalance", "TC_Transfer|output", pBuffer);
        //------------------           
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pBuffer, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}

//应用层：交易接口，恢复正常
bool fnTestApplNormalTxRecover(char* hash, char* v, char* r, char* s, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);             
    //-------------------------------------------
    do{
        //TC_Ecrecover 验证
        //------------------
        _fnPrint("fnTestApplNormalTxRecover", "TC_Ecrecover|input|hash", hash);
        _fnPrint("fnTestApplNormalTxRecover", "TC_Ecrecover|input|v", v);
        _fnPrint("fnTestApplNormalTxRecover", "TC_Ecrecover|input|s", s);
        _fnPrint("fnTestApplNormalTxRecover", "TC_Ecrecover|input|r", r);
        //------------------           
        pBuffer = TC_Ecrecover(hash, v, r, s);
        //------------------
        _fnPrint("fnTestApplNormalTxRecover", "TC_Ecrecover|output", pBuffer);
        //------------------        
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pBuffer, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}


//应用层：交易接口，自毁
bool fnTestApplNormalTxSelfDestruct(char *address, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);             
    //-------------------------------------------
    do{
        //TC_SelfDestruct 验证
        //------------------
        _fnPrint("fnTestApplNormalTxGetSelfAddr", "TC_GetSelfAddress|input", address);
        //------------------          
        pBuffer = TC_SelfDestruct(address);
        //------------------
        _fnPrint("fnTestApplNormalTxGetSelfAddr", "TC_GetSelfAddress|output", pBuffer);
        //------------------          
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pBuffer, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}
//应用层：调用其他合约方法
bool fnTestApplNormalCallContract(char* pContract, char* pAction, char* pArgs, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);              
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_CallContract 验证(调用合约的方法)
        //------------------
        _fnPrint("fnTestApplNormalCallContract", "TC_CallContract|pContract", pContract);
        _fnPrint("fnTestApplNormalCallContract", "TC_CallContract|pAction", pAction);
        _fnPrint("fnTestApplNormalCallContract", "TC_CallContract|pArgs", pArgs);
        //------------------           
        pBuffer = TC_CallContract(pContract, pAction, pArgs);
        //------------------
        _fnPrint("fnTestApplNormalCallContract", "TC_CallContract|output", pBuffer);
        //------------------           
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pBuffer, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}

//应用层：调用其他合约方法
bool fnTestApplNormalDelegateCallContract(char* pContract, char* pAction, char* pArgs, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);              
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_DelegateCallContract 验证(调用合约的方法)
        //------------------
        _fnPrint("fnTestApplNormalDelegateCallContract", "TC_DelegateCallContract|pContract", pContract);
        _fnPrint("fnTestApplNormalDelegateCallContract", "TC_DelegateCallContract|pAction", pAction);
        _fnPrint("fnTestApplNormalDelegateCallContract", "TC_DelegateCallContract|pArgs", pArgs);
        //------------------           
        pBuffer = TC_DelegateCallContract(pContract, pAction, pArgs);
        //------------------
        _fnPrint("fnTestApplNormalCallContract", "TC_DelegateCallContract|output", pBuffer);
        //------------------           
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pBuffer, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}


//应用层：上链数据存储
bool fnTestApplNormalTestStorageDel(char* key, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);            
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_StorageGet 验证
        //------------------
        _fnPrint("fnTestApplNormalTestStorageDel", "TC_StorageSet|key", key);
        //------------------            
        TC_StorageDel(key);        
        //------------------
        //---------------------------------------        
        memcpy((char*)pOutput, "test ok", BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}


//应用层：上链数据存储
bool fnTestApplNormalTestStorageGet(char* key, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);            
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_StorageGet 验证
        pBuffer = TC_StorageGet(key);
        //------------------
        _fnPrint("fnTestApplNormalTestStorageGet", "TC_StorageGet|key", key);
        _fnPrint("fnTestApplNormalTestStorageGet", "TC_StorageGet|pBuffer", pBuffer);
        //------------------            
        //---------------------------------------        
        memcpy((char*)pOutput, pBuffer, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}


//应用层：上链数据存储
bool fnTestApplNormalTestStorageSet(char* key, char* value, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);            
    //-------------------------------------------
    do{
        //---------------------------------------        
        //TC_StorageSet 验证
        //------------------
        _fnPrint("fnTestApplNormalTestStorageSet", "TC_StorageSet|key", key);
        _fnPrint("fnTestApplNormalTestStorageSet", "TC_StorageSet|value", value);        
        //------------------          
        TC_StorageSet(key, value);
        //---------------------------------------           
        memcpy((char*)pOutput, "test ok", BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}

//应用层：事件
//TC_Notify("Transfer(address,address,uint256)", "{\"from\":\"0x54fb1c7d0f011dd63b08f85ed7b518ab82028100\",\"to\":\"0x0000000000000000000000000000000000000000\",\"value\":10000}");
bool fnTestApplNormalNotify(char* from, char* to, int value, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);              
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_Notify 验证(事件测试结果通过getTransferReception来看)
        //char* message = "testdata 1 2 3 4";
        //------------------
        _fnPrint("fnTestApplNormalNotify", "TC_Notify|from", from);        
        _fnPrint("fnTestApplNormalNotify", "TC_Notify|to", to);        
        _fnPrint("fnTestApplNormalNotify", "TC_Notify|value", itoa(value));  
        //------------------           
        void *jsroot = TC_JsonNewObject();
        TC_JsonPutString(jsroot, "from", from);
        TC_JsonPutString(jsroot, "to", to);
        TC_JsonPutInt64(jsroot, "value", value);
        TC_Notify("Transfer(address,address,uint256)", TC_JsonToString(jsroot));
        //------------------
        _fnPrint("fnTestApplNormalNotify", "TC_Notify|end", "go on test");        
        //------------------         
        //---------------------------------------
        memcpy((char*)pOutput, "TC_Notify ok", BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}


//应用层：校验签名
bool fnTestApplNormalCheckSign(char* pubKey, char* data, char* sign, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);              
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_CheckSign 验证
        //------------------
        _fnPrint("fnTestApplNormalCheckSign", "TC_CheckSign|pubKey", pubKey);
        _fnPrint("fnTestApplNormalCheckSign", "TC_CheckSign|data", data);
        _fnPrint("fnTestApplNormalCheckSign", "TC_CheckSign|sign", sign);
        //------------------             
        int iResult = TC_CheckSign(pubKey, data, sign);
        char* result = itoa(iResult);
        //------------------
        _fnPrint("fnTestApplNormalCheckSign", "TC_CheckSign|result", result);        
        //------------------             
        //---------------------------------------
        memcpy((char*)pOutput, (char*)result, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}


//应用层：大数据转换
bool fnTestApplNormalBigDataChange(BigInt a, BigInt b, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(OUTPUT_BUFFER_SIZE_BIGCHANGE);
    memset((char*)pBuffer, 0, OUTPUT_BUFFER_SIZE_BIGCHANGE);          
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_BigIntAdd 验证
        //------------------
        _fnPrint("fnTestApplNormalBigDataChange", "para|a", a);
        _fnPrint("fnTestApplNormalBigDataChange", "para|b", b);
        //------------------
        //------------------
        BigInt c1 = TC_BigIntAdd(a, b);
        _fnPrint("fnTestApplNormalBigDataChange", "TC_BigIntAdd|c", c1);        
        //------------------        
        //---------------------------------------
        // //踢错
        // a = "333333333333333555555555555555555555555555555555555556333333333333333555555555555555555555555555555555555556       ";
        // b = "111111111111111111111111111111111222222222222222222223111111111111111111111111111111111222222222222222222223            ";
        // //------------------  
        //---------------------------------------           
        //TC_BigIntSub 验证
        BigInt c2 = TC_BigIntSub(a, b);
        _fnPrint("fnTestApplNormalBigDataChange", "TC_BigIntSub|c", c2);       
        //---------------------------------------         
        //TC_BigIntMul 验证
        BigInt c3 = TC_BigIntMul(a, b);
        _fnPrint("fnTestApplNormalBigDataChange", "TC_BigIntMul|c", c3);       
        //---------------------------------------    
        BigInt c4 = TC_BigIntDiv(a, b);
        _fnPrint("fnTestApplNormalBigDataChange", "TC_BigIntDiv|c", c4);    
        //---------------------------------------    
        BigInt c5 = TC_BigIntMod(a, b);
        _fnPrint("fnTestApplNormalBigDataChange", "TC_BigIntMod|c", c5);    
        //---------------------------------------    
        int c6 = TC_BigIntCmp(a, b);
        _fnPrint("fnTestApplNormalBigDataChange", "TC_BigIntCmp|c", itoa(c6));            
        //---------------------------------------    
        int64 test64 = TC_BigIntToInt64(a);//超过字符会截断
        _fnPrint("fnTestApplNormalBigDataChange", "TC_BigIntToInt64|test64", i64toa(test64, 10));    
        //---------------------------------------
        sprintf(pBuffer, "{\"TC_BigIntAdd\":\"%s\", \"TC_BigIntSub\":\"%s\", \"TC_BigIntMul\":\"%s\", \"TC_BigIntDiv\":\"%s\", \"TC_BigIntMod\":\"%s\", \"TC_BigIntCmp\":\"%d\", \"TC_BigIntToInt64\":\"%lld\"}", \
                c1, c2, c3, c4, c5, c6, test64);
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pBuffer, OUTPUT_BUFFER_SIZE_BIGCHANGE);
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}

//应用层：断言-require
bool fnTestApplNormalRequire(bool condition, char* pOutput)
{
    //-------------------------------------------
    //断言
    //------------------
    _fnPrint("fnTestApplNormalRequire", "TC_Require|condition", itoa(condition));        
    //-------------------------------------------
    strcpy(pOutput, "TC_Require false");
    //------------------    
    TC_Require(condition);
    //------------------
    _fnPrint("fnTestApplNormalRequire", "TC_Require|result", "true");        
    //------------------     
    //-------------------------------------------
    strcpy(pOutput, "TC_Require true");
    //-------------------------------------------
    return true;
}
//应用层：断言-TC_RequireWithMsg
bool fnTestApplNormalRequireWithMsg(bool condition, char* msg, char* pOutput)
{
    //-------------------------------------------
    //断言
    //------------------
    _fnPrint("fnTestApplNormalRequireWithMsg", "TC_RequireWithMsg|condition", itoa(condition));
    _fnPrint("fnTestApplNormalRequireWithMsg", "TC_RequireWithMsg|msg", msg);
    //-------------------------------------------
    strcpy(pOutput, "TC_RequireWithMsg false");        
    //------------------    
    TC_RequireWithMsg(condition, msg);
    //------------------
    _fnPrint("fnTestApplNormalRequire", "TC_RequireWithMsg|result", "true");        
    //------------------ 
    //-------------------------------------------
    strcpy(pOutput, "TC_RequireWithMsg true");    
    //-------------------------------------------
    
    return true;
}
//应用层：断言-TC_Assert
bool fnTestApplNormalAssert(bool condition, char* pOutput)
{
    //-------------------------------------------
    //断言
    _fnPrint("fnTestApplNormalAssert", "TC_Assert|condition", itoa(condition));
    //-------------------------------------------
    strcpy(pOutput, "TC_Assert false");      
    //------------------    
    TC_Assert(condition);       
    //------------------
    _fnPrint("fnTestApplNormalRequire", "TC_Assert|result", "true");        
    //------------------     
    //-------------------------------------------
    strcpy(pOutput, "TC_Assert true");        
    //-------------------------------------------
    return true;
}
//应用层：断言-TC_Revert
bool fnTestApplNormalRevert(char* pOutput)
{
    //-------------------------------------------
    //断言
    //-------------------------------------------
    strcpy(pOutput, "TC_Revert false");       
    //-------------------------------------------    
    TC_Revert();
    //------------------
    _fnPrint("fnTestApplNormalRevert", "TC_Revert|result", "true");        
    //------------------ 
    //-------------------------------------------
    strcpy(pOutput, "TC_Revert true");          
    //-------------------------------------------
    return true;
}
//应用层：断言-TC_RevertWithMsg
bool fnTestApplNormalRevertWithMsg(char* msg, char* pOutput)
{
    //-------------------------------------------
    //断言
    _fnPrint("fnTestApplNormalAssert", "TC_RevertWithMsg|msg", msg);
    //-------------------------------------------
    strcpy(pOutput, "TC_RevertWithMsg false");      
    //------------------    
    TC_RevertWithMsg(msg);     
    //------------------
    _fnPrint("fnTestApplNormalRequire", "TC_RevertWithMsg|result", "true");        
    //------------------ 
    //-------------------------------------------
    strcpy(pOutput, "TC_RevertWithMsg true");          
    //-------------------------------------------
    return true;
}


//第一次转测增加接口 2018-11-12

//应用层：判断地址-TC_IsHexAddress
bool fnTestApplNormalIsHexAddress(char* address, char* pOutput)
{
    //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);              
    //-------------------------------------------
    do{
        //---------------------------------------
        //TC_IsHexAddress 验证
        //------------------
        _fnPrint("fnTestApplNormalIsHexAddress", "TC_IsHexAddress|address", address);
        //------------------    
        bool result = TC_IsHexAddress(address);    
        //------------------
        _fnPrint("fnTestApplNormalIsHexAddress", "TC_IsHexAddress|result", itoa(result));        
        //---------------------------------------
        memcpy((char*)pOutput, itoa(result), 1);
        //------------------
        bResult = true;
        //---------------------------------------
    }while(0);

    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL;
    //-------------------------------------------
    return bResult;
}

//以下四个token专用的接口以token合约形式进行验证
// void TC_Issue(BigInt amount);
// void TC_TransferToken(char *address, char* token, BigInt amount);
// BigInt TC_TokenBalance(char* addr, char* token);
// char* TC_TokenAddress(void);


//2019-03-15 增加LOG接口
bool fnTestApplNormalLog0(char* data)
{
    //-------------------------------------------
    TC_Log0(data);
    //-------------------------------------------
    return true;
}
bool fnTestApplNormalLog1(const char* data, const char* topic1)
{
    //-------------------------------------------
    TC_Log1(data, topic1);
    //-------------------------------------------
    return true;
}
bool fnTestApplNormalLog2(const char* data, const char* topic1, const char* topic2)
{
    //-------------------------------------------
    TC_Log2(data, topic1, topic2);
    //-------------------------------------------
    return true;
}
bool fnTestApplNormalLog3(const char* data, const char* topic1, const char* topic2, const char* topic3)
{
    //-------------------------------------------
    TC_Log3(data, topic1, topic2, topic3);
    //-------------------------------------------
    return true;
}
bool fnTestApplNormalLog4(const char* data, const char* topic1, const char* topic2, const char* topic3, const char* topic4)
{
    //-------------------------------------------
    TC_Log4(data, topic1, topic2, topic3, topic4);
    //-------------------------------------------
    return true;
}
//===================================================







//===================================================
//合约接口入口
//---------------------------------------------------
char *thunderchain_main(char *action, char *args) 
{
    //-----------------------------------------------
    //输出申请堆空间
    char *pResult = (char*)malloc(OUTPUT_BUFFER_SIZE);
    memset((char*)pResult, 0, OUTPUT_BUFFER_SIZE);
    char *pInput = (char*)malloc(BUFFER_SIZE);
    memset((char*)pInput, 0, BUFFER_SIZE);
    char* pOutput = NULL;
    //-----------------------------------------------
    //json转换    
    _fnPrint("thunderchain_main", action, args);           
    pInput = TC_JsonParse(args);
     //-----------------------------------------------    
    do
    {
        if (0 == strcmp(action ,"Init")){
            bool bResult = Init(pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalCalcHash")){
            char* pData = TC_JsonGetString(pInput, "data");
            bool bResult = fnTestApplNormalCalcHash(pData, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;  
        }
        else if (0 == strcmp(action ,"fnTestApplNormalBlockHash")){
            int64 iblockNumber = TC_JsonGetInt64(pInput, "blockNumber");
            bool bResult = fnTestApplNormalBlockHash(iblockNumber, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalTxTransfer")){
            char* address = TC_JsonGetString(pInput, "address");
            char* amount = TC_JsonGetString(pInput, "amount");    
            bool bResult = fnTestApplNormalTxTransfer(address, amount, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalTxTransferToken")){
            char* address = TC_JsonGetString(pInput, "address");
            char* amount = TC_JsonGetString(pInput, "amount");    
            bool bResult = fnTestApplNormalTxTransferToken(address, amount, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }        
        else if (0 == strcmp(action ,"fnTestApplNormalTxGetBalance")){
            char* address = TC_JsonGetString(pInput, "address");
            bool bResult = fnTestApplNormalTxGetBalance(address, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }   
        else if (0 == strcmp(action ,"fnTestApplNormalTxRecover")){
            char* hash = TC_JsonGetString(pInput, "hash");
            char* v = TC_JsonGetString(pInput, "v");
            char* r = TC_JsonGetString(pInput, "r");
            char* s = TC_JsonGetString(pInput, "s");
            bool bResult = fnTestApplNormalTxRecover(hash, v, r, s, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalTxSelfDestruct")){
            char* address = TC_JsonGetString(pInput, "address");
            bool bResult = fnTestApplNormalTxSelfDestruct(address, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalCallContract")){
            char* inputInfo = TC_JsonToString(pInput);
            _fnPrint("fnTestApplNormalCallContract", "inputPara", inputInfo);     
            char* contract = TC_JsonGetString(pInput, "contract");
            char* action = TC_JsonGetString(pInput, "action");
            char* args = TC_JsonGetObject(pInput, "args");
            char* argsToString = TC_JsonToString(args);
            bool bResult = fnTestApplNormalCallContract(contract, action, argsToString, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        } 
        else if (0 == strcmp(action ,"fnTestApplNormalDelegateCallContract")){
            char* inputInfo = TC_JsonToString(pInput);
            _fnPrint("fnTestApplNormalDelegateCallContract", "inputPara", inputInfo);                 
            char* contract = TC_JsonGetString(pInput, "contract");
            char* action = TC_JsonGetString(pInput, "action");
            char* args = TC_JsonGetObject(pInput, "args");
            char* argsToString = TC_JsonToString(args);
            bool bResult = fnTestApplNormalDelegateCallContract(contract, action, argsToString, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }        
        else if (0 == strcmp(action ,"fnTestApplNormalTestStorageDel")){
            char* key = TC_JsonGetString(pInput, "key");
            bool bResult = fnTestApplNormalTestStorageDel(key, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        } 
        else if (0 == strcmp(action ,"fnTestApplNormalTestStorageGet")){
            char* key = TC_JsonGetString(pInput, "key");   
            bool bResult = fnTestApplNormalTestStorageGet(key, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalTestStorageSet")){
            char* key = TC_JsonGetString(pInput, "key");
            char* value = TC_JsonGetString(pInput, "value");        
            bool bResult = fnTestApplNormalTestStorageSet(key, value, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }                
        else if (0 == strcmp(action ,"fnTestApplNormalNotify")){
            char* from = TC_JsonGetString(pInput, "from");
            char* to = TC_JsonGetString(pInput, "to");
            int value = TC_JsonGetInt64(pInput, "value");
            bool bResult = fnTestApplNormalNotify(from, to, value, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        } 
        else if (0 == strcmp(action ,"fnTestApplNormalCheckSign")){
            char* pubKey = TC_JsonGetString(pInput, "pubKey");
            char* data = TC_JsonGetString(pInput, "data");
            char* sign = TC_JsonGetString(pInput, "sign");
            bool bResult = fnTestApplNormalCheckSign(pubKey, data, sign, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalBigDataChange")){
            BigInt a = TC_JsonGetString(pInput, "a");
            BigInt b = TC_JsonGetString(pInput, "b");            
            bool bResult = fnTestApplNormalBigDataChange(a, b, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalRequire")){
            int condition = TC_JsonGetInt(pInput, "condition");
            bool bResult = fnTestApplNormalRequire(condition, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalRequireWithMsg")){
            int condition = TC_JsonGetInt(pInput, "condition");
            char* message = TC_JsonGetString(pInput, "message");
            bool bResult = fnTestApplNormalRequireWithMsg(condition, message, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalAssert")){
            int condition = TC_JsonGetInt(pInput, "condition");
            bool bResult = fnTestApplNormalAssert(condition, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalRevert")){
            bool bResult = fnTestApplNormalRevert(pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalRevertWithMsg")){
            char* message = TC_JsonGetString(pInput, "message");
            bool bResult = fnTestApplNormalRevertWithMsg(message, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestApplNormalIsHexAddress")){
            char* address = TC_JsonGetString(pInput, "address");
            bool bResult = fnTestApplNormalIsHexAddress(address, pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }             
        else if (0 == strcmp(action ,"fnTestApplNormalLog0")){
            char* data = TC_JsonGetString(pInput, "data");
            bool bResult = fnTestApplNormalLog0(data);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }   
        else if (0 == strcmp(action ,"fnTestApplNormalLog1")){
            char* data = TC_JsonGetString(pInput, "data");
            char* topic1 = TC_JsonGetString(pInput, "topic1");
            bool bResult = fnTestApplNormalLog1(data, topic1);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }   
        else if (0 == strcmp(action ,"fnTestApplNormalLog2")){
            char* data = TC_JsonGetString(pInput, "data");
            char* topic1 = TC_JsonGetString(pInput, "topic1");
            char* topic2 = TC_JsonGetString(pInput, "topic2");
            bool bResult = fnTestApplNormalLog2(data, topic1, topic2);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }          
        else if (0 == strcmp(action ,"fnTestApplNormalLog3")){
            char* data = TC_JsonGetString(pInput, "data");
            char* topic1 = TC_JsonGetString(pInput, "topic1");
            char* topic2 = TC_JsonGetString(pInput, "topic2");
            char* topic3 = TC_JsonGetString(pInput, "topic3");
            bool bResult = fnTestApplNormalLog3(data, topic1, topic2, topic3);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }         
        else if (0 == strcmp(action ,"fnTestApplNormalLog4")){
            char* data = TC_JsonGetString(pInput, "data");
            char* topic1 = TC_JsonGetString(pInput, "topic1");
            char* topic2 = TC_JsonGetString(pInput, "topic2");
            char* topic3 = TC_JsonGetString(pInput, "topic3");
            char* topic4 = TC_JsonGetString(pInput, "topic4");
            bool bResult = fnTestApplNormalLog4(data, topic1, topic2, topic3, topic4);            
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }            
        else
        {
            pResult = strconcat("当前接口不支持，请确认是否传入错误！", action);
            char * result = _fnOutputResult((char*)pResult, "string", 0);
            pOutput = result;
            TC_RevertWithMsg("No match action"); 
        }
    }while(0);

    //-----------------------------------------------    
    //释放内存空间
    free(pResult);
    pResult = NULL;
    free(pInput);
    pInput = NULL;    
    //-----------------------------------------------    
    return pOutput;
}

//===================================================
