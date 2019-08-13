
#include "tcapi.h"
//#include "string.h"
#include "stdio.h"




//===================================================
//定义类型及宏
//---------------------------------------------------
typedef unsigned int uint; 
#define FALSE_BREAK   {break;}
#define BUFFER_SIZE 200
#define OUTPUT_BUFFER_SIZE 2000
#define PRINT_BUFFER (2000)
#define MAX_STACK_BUFFER (512)
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


//===================================================
//测试接口函数
//---------------------------------------------------
//测试底层接口，正常流程，内存操作
bool fnTestBottomNormalStory(char* pOutput)
{
   //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);         
    char* pResult = (char*)malloc(BUFFER_SIZE);
    memset((char*)pResult, 0, BUFFER_SIZE);      
    _fnPrint("fnTestBottomNormalStory", "malloc|0", itoa(BUFFER_SIZE));
    char* pCalloc = (char*)calloc(BUFFER_SIZE, sizeof(char));
    _fnPrint("fnTestBottomNormalStory", "calloc|0", itoa(BUFFER_SIZE));
    //-------------------------------------------
    do{
       //---------------------------------------
        //calloc 验证
        _fnPrint("fnTestBottomNormalStory", "pCalloc|0", itoa(pCalloc[0]));
        _fnPrint("fnTestBottomNormalStory", "pCalloc|-1", itoa(pCalloc[BUFFER_SIZE - 1]));
        if((0 != pCalloc[0]) || (0 != pCalloc[BUFFER_SIZE - 1])) FALSE_BREAK;
        memset((char*)pCalloc, 10, BUFFER_SIZE - 1);
        _fnPrint("fnTestBottomNormalStory", "pCalloc|0", itoa(pCalloc[0]));
        _fnPrint("fnTestBottomNormalStory", "pCalloc|-2", itoa(pCalloc[BUFFER_SIZE - 2]));
        if((10 != pCalloc[0]) || (10 != pCalloc[BUFFER_SIZE - 2])) FALSE_BREAK;
        _fnPrint("fnTestBottomNormalStory", "pCalloc|", "OK");
        //---------------------------------------
        //realloc 验证
        char* pBufferNull = NULL;
        _fnPrint("fnTestStorageMaxRealloc", "realloc|", itoa(BUFFER_SIZE));
        char* pBuffer1 = (char*)realloc((char*)pBufferNull, BUFFER_SIZE);
        _fnPrint("fnTestStorageMaxRealloc", "pBuffer1", (char*)pBuffer1);
        free(pBuffer1);
        pBuffer1 = NULL;
        //size 
        char* pReallocPtrNew = (char*)realloc(pCalloc, BUFFER_SIZE * sizeof(char));
        _fnPrint("fnTestBottomNormalStory", "pCalloc|addr", i64toa((int64)pCalloc, 16));
        _fnPrint("fnTestBottomNormalStory", "pReallocPtrNew|addr", i64toa((int64)pReallocPtrNew, 16));
        if(i64toa((int64)pCalloc, 16) == i64toa((int64)pReallocPtrNew, 16)) FALSE_BREAK;
        char* pReallocPtrNew2 = (char*)realloc(pCalloc, (BUFFER_SIZE + 10) * sizeof(char));
        _fnPrint("fnTestBottomNormalStory", "pCalloc|addr", i64toa((int64)pCalloc, 16));
        _fnPrint("fnTestBottomNormalStory", "pReallocPtrNew2|addr", i64toa((int64)pReallocPtrNew2, 16));
        free(pReallocPtrNew2);
        if(((int64)pReallocPtrNew == (int64)pReallocPtrNew2)) FALSE_BREAK;
        //---------------------------------------
        //memset 验证
        memset((char*)pBuffer, 0, BUFFER_SIZE);
        char tempCha1 = pBuffer[10];
        memset((char*)pBuffer, 10, 20);
        char tempCha2 = pBuffer[10];
        //------------------
        _fnPrint("fnTestBottomNormalStory", "memset|0", itoa(tempCha1));
        _fnPrint("fnTestBottomNormalStory", "memset|10", itoa(tempCha2));
        //------------------        
        if((0 != tempCha1) || (10 != tempCha2)) FALSE_BREAK;
        //---------------------------------------
        //memcpy 验证
        memset((char*)pResult, 0, BUFFER_SIZE);
        memcpy((char*)pResult, (char*)pBuffer, 20);
        tempCha2 = pResult[11];
        //------------------
        _fnPrint("fnTestBottomNormalStory", "memcpy|pResult", pResult);
        _fnPrint("fnTestBottomNormalStory", "memcpy|pBuffer", pBuffer);
        _fnPrint("fnTestBottomNormalStory", "memcpy|tempCha2", itoa(tempCha2));
        //------------------
         if(10 != tempCha2) FALSE_BREAK;
        //---------------------------------------
        //memmove 验证
        memset((char*)pBuffer, 0, BUFFER_SIZE);
        memcpy((char*)pBuffer, "123456789ABCDEF", 50);
        memmove((char*)pBuffer, "abcdef", 50);
        //------------------
        _fnPrint("fnTestBottomNormalStory", "memmove", pBuffer);
        _fnPrint("fnTestBottomNormalStory", "memmove|memcmp((char*)pBuffer, \"abcdef\", 21)", itoa(memcmp((char*)pBuffer, "abcdef", 21)));
        //------------------
        if(0 != memcmp((char*)pBuffer, "abcdef", 21)) FALSE_BREAK;
        //---------------------------------------
        //malloc and free 验证
        char* testMalloc = (char*)malloc(10);
        tempCha1 = testMalloc[3];
        memset((char*)pBuffer, 0, 10);
        tempCha2 = testMalloc[3];
        free(pBuffer);
        char tempCha3 = testMalloc[3];
        //------------------
        _fnPrint("fnTestBottomNormalStory", "memset|tempCha1", itoa(tempCha1));
        _fnPrint("fnTestBottomNormalStory", "memset|tempCha2", itoa(tempCha2));
        _fnPrint("fnTestBottomNormalStory", "memset|tempCha3", itoa(tempCha3));
        //------------------
        if(tempCha2 != tempCha3) FALSE_BREAK;
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pResult, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);
    //-------------------------------------------
    //释放内存空间
    free(pCalloc);
    pCalloc = NULL;
    free(pBuffer);
    pBuffer = NULL; 
    free(pResult);
    pResult = NULL;     
    _fnPrint("fnTestBottomNormalStory", "bResult", itoa(bResult));
    //-------------------------------------------
    return bResult;
}


//测试底层接口，异常流程，内存-操作
bool fnTestBottomAbnormalStory(char* pOutput)
{
   //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);         
    char* pResult = (char*)malloc(BUFFER_SIZE);
    memset((char*)pResult, 0, BUFFER_SIZE);      
    //-------------------------------------------
    do{
        //---------------------------------------
        //memset 验证
        memset((char*)pBuffer, 0, BUFFER_SIZE);
        char tempCha1 = pBuffer[10];
        memset((char*)pBuffer, 0x24, 20);
        char tempCha2 = pBuffer[10];
        //------------------
        _fnPrint("fnTestBottomAbnormalStory", "memset|0", itoa(tempCha1));
        _fnPrint("fnTestBottomAbnormalStory", "memset|$", itoa(tempCha2));
        //------------------        
        if((0 != tempCha1) || (0x24 != tempCha2)) FALSE_BREAK; //$ ascii 0x24
        //---------------------------------------
        //memcpy 验证
        memset((char*)pResult, 0, BUFFER_SIZE);
        memcpy((char*)pResult, (char*)pBuffer, 20);
        tempCha2 = pResult[11];
        //------------------
        _fnPrint("fnTestBottomAbnormalStory", "memcpy|pResult", pResult);
        _fnPrint("fnTestBottomAbnormalStory", "memcpy|pBuffer", pBuffer);
        //------------------
         if(0x24 != tempCha2) FALSE_BREAK;
        //---------------------------------------
        //memmove 验证
        memset((char*)pBuffer, 0, BUFFER_SIZE);
        memcpy((char*)pBuffer, "34123456789ABCD", 15);
        memmove(pBuffer, pBuffer+2, 15);
        //------------------
        TC_Prints((char*)"==================================");
        _fnPrint("fnTestBottomAbnormalStory", "memmove", pBuffer);
        _fnPrint("fnTestBottomAbnormalStory", "memmove", pBuffer);
        _fnPrint("fnTestBottomAbnormalStory", "memmove|[12]", itoa(pBuffer[12]));
        _fnPrint("fnTestBottomAbnormalStory", "memmove|[13]", itoa(pBuffer[13]));
        _fnPrint("fnTestBottomAbnormalStory", "memmove|[14]", itoa(pBuffer[14]));
        _fnPrint("fnTestBottomAbnormalStory", "memmove|memcmp((char*)pBuffer, \"123456789ABCD\", 15)", itoa(memcmp((char*)pBuffer, "123456789ABCD", 15)));
        _fnPrint("fnTestBottomAbnormalStory", "memmove|memcmp((char*)pBuffer, \"123456789ABCD\", 13)", itoa(memcmp((char*)pBuffer, "123456789ABCD", 13)));
        _fnPrint("fnTestBottomAbnormalStory", "memmove|memcmp((char*)pBuffer, \"123456789\", 15)", itoa(memcmp((char*)pBuffer, "123456789", 15)));
        _fnPrint("fnTestBottomAbnormalStory", "memmove|memcmp((char*)pBuffer, \"343456789ABCD\", 13)", itoa(memcmp((char*)pBuffer, "343456789ABCD", 13)));

        TC_Prints(itoa(memcmp((char*)pBuffer, "123456789ABCD", 15)));
        //------------------
        if(0 < memcmp((char*)pBuffer, "123456789ABCD", 15)) FALSE_BREAK;
        if(0 != memcmp((char*)pBuffer, "123456789ABCD", 13)) FALSE_BREAK;
        if(0 > memcmp((char*)pBuffer, "123456789", 15)) FALSE_BREAK;
        if(0 < memcmp((char*)pBuffer, "343456789ABCD", 13)) FALSE_BREAK;
        //---------------------------------------
        //memcpy + memcmp
        memset((char*)pBuffer, 0, BUFFER_SIZE);
        memcpy((char*)pBuffer, "~!@#$%^&*()_+\0\0\0~!@#$%^&*()", 30);
        //------------------
        _fnPrint("fnTestBottomAbnormalStory", "memcpy|memcmp", pBuffer);
        _fnPrint("fnTestBottomAbnormalStory", "memcpy|memcmp", itoa(memcmp((char*)pBuffer, "~!@#$%^&*()_+\0\0\0~!@#$%^&*()", 30)));
        //------------------
        if(0 != memcmp((char*)pBuffer, "~!@#$%^&*()_+\0\0\0~!@#$%^&*()", 30)) FALSE_BREAK;
        //---------------------------------------
        _fnPrint("fnTestBottomAbnormalStory", "memcpy|memcmp", "OK");
        memcpy((char*)pOutput, (char*)pResult, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);
    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL; 
    free(pResult);
    pResult = NULL;     
    //------------------
    _fnPrint("fnTestBottomAbnormalStory", "bResult", itoa(bResult));
    //------------------    
    //-------------------------------------------
    return bResult;
}
//测试底层接口，正常流程，字符串操作
bool fnTestBottomNormalString(char* pOutput)
{
   //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);         
    char* pResult = (char*)malloc(BUFFER_SIZE);
    memset((char*)pResult, 0, BUFFER_SIZE);      
    //-------------------------------------------
    do{
        //---------------------------------------
        //strlen 验证
        char* str1 = "zhangzitao test 1";
        char* str2 = "zhangzitao test 2 ";
        char* str3 = "zhangzitao test 2 ajdfa;djfjafdj;ajdfj;lj\r\najdf;ljal;dfj"; //56
        //------------------
        _fnPrint("fnTestBottomNormalString", "strlen|str1", itoa(strlen(str1)));
        _fnPrint("fnTestBottomNormalString", "strlen|str2", itoa(strlen(str2)));
        _fnPrint("fnTestBottomNormalString", "strlen|str3", itoa(strlen(str3)));
        //------------------        
        if((17 != strlen(str1)) || (18 != strlen(str2)) || (56 != strlen(str3))) FALSE_BREAK;
        //---------------------------------------
        //strcpy + strcmp 验证
        memset((char*)pBuffer, 0, BUFFER_SIZE);
        strcpy((char*)pBuffer, str3);
        //------------------
        _fnPrint("fnTestBottomNormalString", "memcpy|str2", str2);
        _fnPrint("fnTestBottomNormalString", "memcpy|str3", str3);
        _fnPrint("fnTestBottomNormalString", "memcpy|pBuffer", pBuffer);
        //------------------
        if(0 == strcmp(pBuffer, str2)) FALSE_BREAK;
        if(0 == strcmp(str3, str2)) FALSE_BREAK;
        if(0 > strcmp(str3, str2)) FALSE_BREAK;
        //---------------------------------------
        //strconcat 验证
        char* str12 = "zhangzitao test 1zhangzitao test 2 ";
        char* strConcat = strconcat(str1, str2);
        //------------------
        _fnPrint("fnTestBottomNormalString", "strconcat|str12", str12);
        _fnPrint("fnTestBottomNormalString", "strconcat|strConcat", strConcat);
        //------------------
        if(0 != strcmp(str12, strConcat)) FALSE_BREAK;
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pResult, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);
    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL; 
    free(pResult);
    pResult = NULL;     
    //-------------------------------------------
    return bResult;
}
//测试底层接口，异常流程，字符串操作
bool fnTestBottomAbnormalString(char* pOutput)
{
   //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);         
    char* pResult = (char*)malloc(BUFFER_SIZE);
    memset((char*)pResult, 0, BUFFER_SIZE);      
    //-------------------------------------------
    do{
        //---------------------------------------
        //strlen 验证(字符串定义中\0理论上会被截断)
        char* str1 = "~!@#$%^&*()";
        char* str2 = "~!@#$%^&*() ";
        char* str3 = "~!@#$%^&*() \0\0abcd"; //
        //------------------
        _fnPrint("fnTestBottomAbnormalString", "strlen|str1", itoa(strlen(str1)));
        _fnPrint("fnTestBottomAbnormalString", "strlen|str2", itoa(strlen(str2)));
        _fnPrint("fnTestBottomAbnormalString", "strlen|str3", itoa(strlen(str3)));
        //------------------        
        if((11 != strlen(str1)) || (12 != strlen(str2)) || (12 != strlen(str3))) FALSE_BREAK;
        //---------------------------------------
        //strcpy + strcmp 验证
        memset((char*)pBuffer, 0, BUFFER_SIZE);
        strcpy((char*)pBuffer, str3);
        //------------------
        _fnPrint("fnTestBottomAbnormalString", "memcpy|str2", str2);
        _fnPrint("fnTestBottomAbnormalString", "memcpy|str3", str3);
        _fnPrint("fnTestBottomAbnormalString", "memcpy|strcpy", pBuffer);
        _fnPrint("fnTestBottomAbnormalString", "memcpy|strcmp(str3, str2)", itoa(strcmp(str3, str2)));
        //------------------
        if(0 != strcmp(str3, pBuffer)) FALSE_BREAK;
        if(0 != strcmp(str3, str2)) FALSE_BREAK;
        //---------------------------------------
        //strconcat 验证
        char* str23 = "~!@#$%^&*() ~!@#$%^&*() ";
        char* strConcat = strconcat(str2, str3);
        //------------------
        _fnPrint("fnTestBottomAbnormalString", "strconcat|str23", str23);
        _fnPrint("fnTestBottomAbnormalString", "strconcat|strConcat", strConcat);
        //------------------
        if(0 != strcmp(str23, strConcat)) FALSE_BREAK;
        //---------------------------------------
        memcpy((char*)pOutput, (char*)pResult, BUFFER_SIZE);
        bResult = true;
        //---------------------------------------
    }while(0);
    //-------------------------------------------
    //释放内存空间
    free(pBuffer);
    pBuffer = NULL; 
    free(pResult);
    pResult = NULL;     
    //-------------------------------------------
    return bResult;
}

//基础底层函数（类型转换）
bool fnTestBottomNormalTypeChange(char* pOutput)
{
 
   //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);         
    //-------------------------------------------
    do{
        //---------------------------------------
        //atoi + itoa 验证
        char* pTargetInt = "99999999";
        int iResult = atoi(pTargetInt);
        char* pResult = itoa(iResult);
        //------------------
        _fnPrint("fnTestBottomNormalTypeChange", "atoi|itoa|1", pResult);
        _fnPrint("fnTestBottomNormalTypeChange", "atoi|itoa|1", itoa(strcmp(pTargetInt, pResult)));
        //------------------        
        if(0 != strcmp(pTargetInt, pResult))  FALSE_BREAK;
        //---------------------------------------
        //atoi64 + i64toa 验证 10进制
        pTargetInt = "9223372036854775807";//1e16  63位
        int64 i64Result = atoi64(pTargetInt);
        pResult = i64toa(i64Result, 10);
        //------------------
        _fnPrint("fnTestBottomNormalTypeChange", "atoi64|i64toa|2", pResult);
        _fnPrint("fnTestBottomNormalTypeChange", "atoi64|i64toa|2", itoa(strcmp(pTargetInt, pResult)));
        //------------------        
        if(0 != strcmp(pTargetInt, pResult))  FALSE_BREAK;
        //---------------------------------------
        // //atoi64 + i64toa 验证 16进制  atoi64实际调用atoill接口，该接口为C标准库接口，不支持16进制转换，只支持10进制
        // pTargetInt = "0x7FFFFFFFFFFFFFFF";//1e16
        // i64Result = atoi64(pTargetInt);
        // pResult = i64toa(i64Result, 16);
        // //------------------
        // _fnPrint("fnTestBottomNormalTypeChange", "atoi64|i64toa|3", pResult);
        // _fnPrint("fnTestBottomNormalTypeChange", "atoi64|i64toa|3", itoa(strcmp(pTargetInt, pResult)));
        // //------------------        
        // if(0 != strcmp(pTargetInt, pResult))  FALSE_BREAK;
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
//基础底层函数（类型转换）
bool fnTestBottomAbnormalTypeChange(char* pOutput)
{
  //-------------------------------------------
    //初始化内存
    bool bResult = false;
    char* pBuffer = (char*)malloc(BUFFER_SIZE);
    memset((char*)pBuffer, 0, BUFFER_SIZE);         
    //-------------------------------------------
    do{
        //---------------------------------------
        //atoi + itoa 验证
        char* pTargetInt = "99999999999999999999999999999999";
        int iResult = atoi(pTargetInt);
        char* pResult = itoa(iResult);
        //------------------
        _fnPrint("fnTestBottomAbnormalTypeChange", "atoi|itoa|1", pResult);
        _fnPrint("fnTestBottomAbnormalTypeChange", "atoi|itoa|1", itoa(strcmp(pTargetInt, pResult)));
        //------------------        
        if(0 == strcmp(pTargetInt, pResult))  FALSE_BREAK;
        //---------------------------------------
        //atoi64 + i64toa 验证 10进制
        pTargetInt = "18446744073709551615555555555555555555555555555555555555555";//
        int64 i64Result = atoi64(pTargetInt);
        pResult = i64toa(iResult, 10);
        //------------------
        _fnPrint("fnTestBottomAbnormalTypeChange", "atoi64|i64toa|2", pResult);
        _fnPrint("fnTestBottomAbnormalTypeChange", "atoi64|i64toa|2", itoa(strcmp(pTargetInt, pResult)));
        //------------------        
        if(0 == strcmp(pTargetInt, pResult))  FALSE_BREAK;
        //---------------------------------------
        //atoi64 + i64toa 验证 8进制
        pTargetInt = "9223372036854775807";//1e16
        i64Result = atoi64(pTargetInt);
        pResult = i64toa(iResult, 8);
        //------------------
        _fnPrint("fnTestBottomAbnormalTypeChange", "atoi64|i64toa|3", pResult);
        _fnPrint("fnTestBottomAbnormalTypeChange", "atoi64|i64toa|3", itoa(strcmp(pTargetInt, pResult)));
        //------------------        
        if(0 == strcmp(pTargetInt, pResult))  FALSE_BREAK;
        //---------------------------------------
        //atoi64 + i64toa 验证 2进制
        pTargetInt = "9223372036854775807";//1e16
        i64Result = atoi64(pTargetInt);
        pResult = i64toa(iResult, 2);
        //------------------
        _fnPrint("fnTestBottomAbnormalTypeChange", "atoi64|i64toa|4", pResult);
        _fnPrint("fnTestBottomAbnormalTypeChange", "atoi64|i64toa|4", itoa(strcmp(pTargetInt, pResult)));
        //------------------        
        if(0 == strcmp(pTargetInt, pResult))  FALSE_BREAK;
        //---------------------------------------
        //atoi64 + i64toa 验证 6进制
        pTargetInt = "9223372036854775807";//1e16
        i64Result = atoi64(pTargetInt);
        pResult = i64toa(iResult, 6);
        //------------------
        _fnPrint("fnTestBottomAbnormalTypeChange", "atoi64|i64toa|5", pResult);
        _fnPrint("fnTestBottomAbnormalTypeChange", "atoi64|i64toa|5", itoa(strcmp(pTargetInt, pResult)));
        //------------------        
        if(0 == strcmp(pTargetInt, pResult))  FALSE_BREAK;
        //---------------------------------------
        // //atoi64 + i64toa 验证 2进制
        // pTargetInt = "9223372036854775807";//1e16
        // i64Result = atoi64(pTargetInt);
        // pResult = i64toa(iResult, 99);//支持的范围：2~16
        // //------------------
        // _fnPrint("fnTestBottomAbnormalTypeChange", "atoi64|i64toa|6", pResult);
        // _fnPrint("fnTestBottomAbnormalTypeChange", "atoi64|i64toa|6", itoa(strcmp(pTargetInt, pResult)));
        // ------------------        
        // if(0 == strcmp(pTargetInt, pResult))  FALSE_BREAK;
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
    pInput = TC_JsonParse(args);
    //-----------------------------------------------    
    do
    {
        if (0 == strcmp(action ,"Init")){
            bool bResult = Init(pResult);
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestBottomNormalStory")){
            
            bool bResult = fnTestBottomNormalStory(pResult);
            pResult = bResult ? "success" : "failure";
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }
        else if (0 == strcmp(action ,"fnTestBottomAbnormalStory")){
            bool bResult = fnTestBottomAbnormalStory(pResult);
            pResult = bResult ? "success" : "failure";
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }    
        else if (0 == strcmp(action ,"fnTestBottomNormalString")){
            bool bResult = fnTestBottomNormalString(pResult);
            pResult = bResult ? "success" : "failure";
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }  
        else if (0 == strcmp(action ,"fnTestBottomAbnormalString")){
            bool bResult = fnTestBottomAbnormalString(pResult);
            pResult = bResult ? "success" : "failure";
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }  
        else if (0 == strcmp(action ,"fnTestBottomNormalTypeChange")){
            bool bResult = fnTestBottomNormalTypeChange(pResult);
            pResult = bResult ? "success" : "failure";
            char * result = _fnOutputResult((char*)pResult, "string", bResult);
            pOutput = result;
        }  
        else if (0 == strcmp(action ,"fnTestBottomAbnormalTypeChange")){
            bool bResult = fnTestBottomAbnormalTypeChange(pResult);
            pResult = bResult ? "success" : "failure";
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
