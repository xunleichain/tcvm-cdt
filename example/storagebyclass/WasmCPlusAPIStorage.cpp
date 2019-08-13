#define DEBUG

#include <stdio.h>
//#include "reflect.h"
//#include "tlv.h"
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

//==================================================
struct StorMapStruct {
    bool dataBool;
    tc::string dataChar;
    int dataInt;
    tc::BInt dataBInt;
};
TC_STRUCT(StorMapStruct, 
    TC_FIELD_NAME(dataBool, "dataBool"),
    TC_FIELD_NAME(dataChar, "dataChar"),
    TC_FIELD_NAME(dataInt, "dataInt"),
    TC_FIELD_NAME(dataBInt, "dataBInt"));
//==================================================

class WasmCPlusAPIStorage : public TCBaseContract {
private:
    tc::string testString = tc::string("wasm test!");
    tc::Address testAddr = tc::Address("0x54fb1c7d0f011dd63b08f85ed7b518ab82028100");
    tc::Address testAddr0 = tc::Address("0x0000000000000000000000000000000000000000");
    tc::Address testAddr1 = tc::Address("0x0000000000000000000000000000000000000001");
    tc::BInt testBInt = tc::BInt("0x12345678798abcd");

private:
    bool fnCompareBytes(const char* name, const uint8_t* sourceBytes, const int sourceLength, const uint8_t* destBytes, const int destLength);
public:
    const char* Init();
    const char* fnStorageTestStorValue();
    bool fnStorageTestStorValueSet();
    bool fnStorageTestStorValueGet();
    const char* fnStorageTestStorArray();
    bool fnStorageTestStorArraySet();
    bool fnStorageTestStorArrayGet();    
    const char* fnStorageTestStorMap();
    bool fnStorageTestStorMapSet();
    bool fnStorageTestStorMapGet();    
};

TC_ABI(WasmCPlusAPIStorage, (fnStorageTestStorValue)\
                            (fnStorageTestStorValueSet)\
                            (fnStorageTestStorValueGet)\
                            (fnStorageTestStorArray)\
                            (fnStorageTestStorArraySet)\
                            (fnStorageTestStorArrayGet)\
                            (fnStorageTestStorMap)\
                            (fnStorageTestStorMapSet)\
                            (fnStorageTestStorMapGet)\                            
                            )

//=====================================================
const char* WasmCPlusAPIStorage::Init() {
    TC_Prints("WasmCPlusAPIStorage Init");
    return "init ok";
}


bool WasmCPlusAPIStorage::fnStorageTestStorValueSet() {
    bool bResult = false;
    do
    {
        tc::StorValue<int> StorageInt("StorageInt");
        tc::StorValue<tc::string> StorageString("StorageString");
        tc::StorValue<tc::Address> StorageAddr("StorageAddr");
        tc::StorValue<tc::BInt> StorageBInt("StorageBInt");       

        TC_Prints("fnStorageTestStorValueSet start");
        StorageInt.set(100);
        TC_Prints("test1");
        StorageString.set(testString);
        TC_Prints("test2");
        StorageAddr.set(testAddr);
        TC_Prints("test3");
        StorageBInt.set(testBInt);
        TC_Prints("test4");
        TC_Prints("fnStorageTestStorValueSet end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnStorageTestStorValueSet", " SUCCESS!") : strconcat("fnStorageTestStorValueSet", " FAILURE!");
    TC_Prints(result);
    return bResult;
}

bool WasmCPlusAPIStorage::fnStorageTestStorValueGet() {
    bool bResult = false;
    do
    {
        tc::StorValue<int> StorageInt("StorageInt");
        tc::StorValue<tc::string> StorageString("StorageString");
        tc::StorValue<tc::Address> StorageAddr("StorageAddr");
        tc::StorValue<tc::BInt> StorageBInt("StorageBInt");       


        TC_Prints("fnStorageTestStorValueGet start");
        int getInt = StorageInt.get();
        FALSE_PRINT_BREAK("StorageInt", 100, getInt);
        TC_Prints("fnStorageTestStorValueGet StorageInt ok");
        tc::string getString = StorageString.get();
        FALSE_PRINT_BREAK("StorageString", testString, getString);
        tc::Address getAddr = StorageAddr.get();
        FALSE_PRINT_STRCMP_BREAK("StorageAddr", testAddr.toString(), getAddr.toString());
        tc::BInt getData = StorageBInt.get();
        FALSE_PRINT_BREAK("StorageBInt", testBInt, getData);

        TC_Prints("fnStorageTestStorValueGet end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnStorageTestStorValueGet", " SUCCESS!") : strconcat("fnStorageTestStorValueGet", " FAILURE!");
    TC_Prints(result);
    return bResult;
}

const char* WasmCPlusAPIStorage::fnStorageTestStorValue() {
    bool bResult = false;
    do
    {
        TC_Prints("fnStorageTestStorValue start");
        if(!fnStorageTestStorValueSet()){break;}
        if(!fnStorageTestStorValueGet()){break;}
        TC_Prints("fnStorageTestStorValue end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnStorageTestStorValue", " SUCCESS!") : strconcat("fnStorageTestStorValue", " FAILURE!");
    return result;
}



bool WasmCPlusAPIStorage::fnStorageTestStorArraySet() {
    bool bResult = false;
    do
    {
        tc::StorArray<int> StorArrayInt("StorArrayInt");
        tc::StorArray<tc::BInt> StorArrayBInt("StorArrayBInt");

        tc::BInt getLengthInt = StorArrayInt.length();
        Print("StorArrayInt0", "getLengthInt", getLengthInt);    
        tc::BInt getLengthBInt = StorArrayBInt.length();
        Print("StorArrayBInt0", "getLengthBIntBefore", getLengthBInt);  
        for(tc::BInt i = tc::BInt(0); i < getLengthInt; i ++)
        { 
            Print("StorArrayInt3", "loop", i);    
            StorArrayInt.pop();   
        }    
        for(tc::BInt i = tc::BInt(0); i < getLengthBInt; i ++)
        { 
            Print("StorArrayBInt3", "loop", i);    
            StorArrayBInt.pop();   
        }  

        TC_Prints("fnStorageTestStorArraySet start");
        tc::BInt getLengthIntBefore = StorArrayInt.length();
        Print("StorArrayInt1", "getLengthIntBefore", getLengthIntBefore);    
        tc::BInt getLengthBIntBefore = StorArrayBInt.length();
        Print("StorArrayBInt1", "getLengthBIntBefore", getLengthBIntBefore);    
        
        StorArrayInt.push(100);
        StorArrayInt.set(0, 90);

        StorArrayBInt.push(tc::BInt(200));
        StorArrayBInt.set(0, 190);

        tc::BInt getLengthIntAfter = StorArrayInt.length();
        Print("StorArrayInt2", "getLengthIntAfter", getLengthIntAfter);    
        tc::BInt getLengthBIntAfter = StorArrayBInt.length();
        Print("StorArrayBInt2", "getLengthBIntAfter", getLengthBIntAfter);      
        TC_Prints("fnStorageTestStorArraySet end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnStorageTestStorArraySet", " SUCCESS!") : strconcat("fnStorageTestStorArraySet", " FAILURE!");
    TC_Prints(result);
    return bResult;
}

bool WasmCPlusAPIStorage::fnStorageTestStorArrayGet() {
    bool bResult = false;
    tc::BInt getLengthInt;
    tc::BInt getLengthBInt;
    tc::StorArray<int> StorArrayInt("StorArrayInt");
    tc::StorArray<tc::BInt> StorArrayBInt("StorArrayBInt");
    do
    {
        TC_Prints("fnStorageTestStorArrayGet start");
        getLengthInt = StorArrayInt.length();
        FALSE_PRINT_BREAK("StorArrayInt1", tc::BInt(1), getLengthInt);
        int getInt = StorArrayInt[0];
        FALSE_PRINT_BREAK("StorArrayInt2", 90, getInt);

        getLengthBInt = StorArrayBInt.length();
        FALSE_PRINT_BREAK("StorArrayBInt1", tc::BInt(1), getLengthBInt);
        tc::BInt getBInt = StorArrayBInt[0];
        FALSE_PRINT_BREAK("StorArrayBInt2", tc::BInt(190), getBInt);        
        TC_Prints("fnStorageTestStorArrayGet end");
        bResult = true;
    } while (0);
    tc::BInt getLengthIntAfter = StorArrayInt.length();
    Print("StorArrayInt4", "getLengthIntAfter", getLengthIntAfter);    
    tc::BInt getLengthBIntAfter = StorArrayBInt.length();
    Print("StorArrayBInt4", "getLengthBIntAfter", getLengthBIntAfter);      
    const char* result =  bResult ? strconcat("fnStorageTestStorArrayGet", " SUCCESS!") : strconcat("fnStorageTestStorArrayGet", " FAILURE!");
    TC_Prints(result);
    return bResult;
}

const char* WasmCPlusAPIStorage::fnStorageTestStorArray() {
    bool bResult = false;
    do
    {
        TC_Prints("fnStorageTestStorArray start");
        if(!fnStorageTestStorArraySet()){break;}
        if(!fnStorageTestStorArrayGet()){break;}
        TC_Prints("fnStorageTestStorArray end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnStorageTestStorArray", " SUCCESS!") : strconcat("fnStorageTestStorArray", " FAILURE!");
    return result;
}


bool WasmCPlusAPIStorage::fnStorageTestStorMapSet() {
    bool bResult = false;
    do
    {
        TC_Prints("fnStorageTestStorMapSet start");
        tc::StorMap<Key<tc::Address>, StorMapStruct>  StorMapKeyList("StorMapKeyList");
        tc::StorMap<Key<tc::Address, tc::string>, StorMapStruct>  StorMapKeyTwoList("StorMapKeyTwoList");

        StorMapStruct st;
        st.dataBool = true;
        st.dataInt = 100;
        st.dataChar = "zhangzitao";
        st.dataBInt = tc::BInt("0x111111");
        // StorMapKey.set(st, testAddr);

        StorMapKeyList.set(st, testAddr);
        StorMapKeyList.set(st, testAddr0);
        StorMapKeyList.set(st, testAddr1);

        StorMapKeyTwoList.set(st, testAddr, tc::string(""));
        StorMapKeyTwoList.set(st, testAddr0, tc::string("0"));
        StorMapKeyTwoList.set(st, testAddr1, tc::string("1"));

        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnStorageTestStorMapSet", " SUCCESS!") : strconcat("fnStorageTestStorMapSet", " FAILURE!");
    TC_Prints(result);
    return bResult;
}

bool WasmCPlusAPIStorage::fnStorageTestStorMapGet() {
    bool bResult = false;
    do
    {
        TC_Prints("fnStorageTestStorMapGet start");
        tc::StorMap<Key<tc::Address>, StorMapStruct>  StorMapKeyList("StorMapKeyList");
        tc::StorMap<Key<tc::Address, tc::string>, StorMapStruct>  StorMapKeyTwoList("StorMapKeyTwoList");        
        const char* getTestKey = (char*)StorMapKeyList.getKeyBytes(Key<tc::Address>::keyStr(testAddr)); //tc::tlv::Marshal
        Print("StorMapKeyList", "getTestKey", getTestKey);
        const char* getTestValue = (char*)StorMapKeyList.getValueBytes(Key<tc::Address>::keyStr(testAddr));
        Print("StorMapKeyList", "getTestValue", getTestValue);
        Print("StorMapKeyList", "getTestValue", "OK");
        StorMapStruct stGet2 = StorMapKeyList.get(testAddr);
        FALSE_PRINT_BREAK("StorMapKeyList", true, stGet2.dataBool);
        FALSE_PRINT_BREAK("StorMapKeyList", 100, stGet2.dataInt);
        FALSE_PRINT_STRCMP_BREAK("StorMapKeyList", "zhangzitao", stGet2.dataChar.c_str());
        FALSE_PRINT_BREAK("StorMapKeyList", tc::BInt("0x111111"),  stGet2.dataBInt);

        const char* getTestValue0 = (char*)StorMapKeyList.getKeyBytes(Key<tc::Address>::keyStr(testAddr0));
        Print("StorMapKeyList", "getTestValue0", getTestValue0);
        StorMapStruct stGet0 = StorMapKeyList.get(testAddr0);
        FALSE_PRINT_BREAK("StorMapKeyList", true, stGet0.dataBool);
        FALSE_PRINT_BREAK("StorMapKeyList", 100, stGet0.dataInt);
        FALSE_PRINT_STRCMP_BREAK("StorMapKeyList", "zhangzitao", stGet0.dataChar.c_str());
        FALSE_PRINT_BREAK("StorMapKeyList", tc::BInt("0x111111"),  stGet0.dataBInt);

        const char* getTestValue1 = (char*)StorMapKeyList.getKeyBytes(Key<tc::Address>::keyStr(testAddr1));
        Print("StorMapKeyList", "getTestValue1", getTestValue1);    
        StorMapStruct stGet1 = StorMapKeyList.get(testAddr1);
        FALSE_PRINT_BREAK("StorMapKeyList", true, stGet1.dataBool);
        FALSE_PRINT_BREAK("StorMapKeyList", 100, stGet1.dataInt);
        FALSE_PRINT_STRCMP_BREAK("StorMapKeyList", "zhangzitao", stGet1.dataChar.c_str());
        FALSE_PRINT_BREAK("StorMapKeyList", tc::BInt("0x111111"),  stGet1.dataBInt);

        const char* getTestTwoValue = (char*)StorMapKeyTwoList.getKeyBytes(Key<tc::Address, tc::string>::keyStr(testAddr, "2"));
        Print("StorMapKeyTwoList", "getTestTwoValue", getTestTwoValue);
        StorMapStruct stTwoGet2 = StorMapKeyTwoList.get(testAddr, "2");
        FALSE_PRINT_BREAK("StorMapKeyTwoList", 0, stTwoGet2.dataInt);

        const char* getTestTwoKey0 = (char*)StorMapKeyTwoList.getKeyBytes(Key<tc::Address, tc::string>::keyStr(testAddr0, "0"));
        Print("StorMapKeyTwoList", "getTestTwoKey0", getTestTwoKey0);
        const char* getTestTwoValue0 = (char*)StorMapKeyTwoList.getValueBytes(Key<tc::Address, tc::string>::keyStr(testAddr0, "0"));
        Print("StorMapKeyTwoList", "getTestValue0", getTestTwoValue0);   

        StorMapStruct stTwoGet0 = StorMapKeyTwoList.get(testAddr0, "0");
        FALSE_PRINT_BREAK("StorMapKeyTwoList", true, stTwoGet0.dataBool);
        FALSE_PRINT_BREAK("StorMapKeyTwoList", 100, stTwoGet0.dataInt);
        FALSE_PRINT_STRCMP_BREAK("StorMapKeyTwoList", "zhangzitao", stTwoGet0.dataChar.c_str());
        FALSE_PRINT_BREAK("StorMapKeyTwoList", tc::BInt("0x111111"),  stTwoGet0.dataBInt);

        const char* getTestTwoValue1 = (char*)StorMapKeyTwoList.getKeyBytes(Key<tc::Address, tc::string>::keyStr(testAddr1, "1"));
        Print("StorMapKeyTwoList", "getTestTwoValue1", getTestTwoValue1);    
        StorMapStruct stTwoGet1 = StorMapKeyTwoList.get(testAddr1, "1");
        FALSE_PRINT_BREAK("StorMapKeyTwoList", true, stTwoGet1.dataBool);
        FALSE_PRINT_BREAK("StorMapKeyTwoList", 100, stTwoGet1.dataInt);
        FALSE_PRINT_STRCMP_BREAK("StorMapKeyTwoList", "zhangzitao", stTwoGet1.dataChar.c_str());
        FALSE_PRINT_BREAK("StorMapKeyTwoList", tc::BInt("0x111111"),  stTwoGet1.dataBInt);

        const char* getTestTwoValue2 = (char*)StorMapKeyTwoList.getKeyBytes(Key<tc::Address, tc::string>::keyStr(testAddr1, ""));
        Print("StorMapKeyTwoList", "getTestTwoValue2", getTestTwoValue2);    
        StorMapStruct stTwoGet = StorMapKeyTwoList.get(testAddr, "");
        FALSE_PRINT_BREAK("StorMapKeyTwoList", true, stTwoGet.dataBool);
        FALSE_PRINT_BREAK("StorMapKeyTwoList", 100, stTwoGet.dataInt);
        FALSE_PRINT_STRCMP_BREAK("StorMapKeyTwoList", "zhangzitao", stTwoGet.dataChar.c_str());
        FALSE_PRINT_BREAK("StorMapKeyTwoList", tc::BInt("0x111111"),  stTwoGet.dataBInt);
        //---------------------------------------------

        TC_Prints("fnStorageTestStorMapGet end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnStorageTestStorMapGet", " SUCCESS!") : strconcat("fnStorageTestStorMapGet", " FAILURE!");
    TC_Prints(result);
    return bResult;
}

const char* WasmCPlusAPIStorage::fnStorageTestStorMap() {
    bool bResult = false;
    do
    {
        TC_Prints("fnStorageTestStorMap start");
        if(!fnStorageTestStorMapSet()){break;}
        if(!fnStorageTestStorMapGet()){break;}
        TC_Prints("fnStorageTestStorMap end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnStorageTestStorMap", " SUCCESS!") : strconcat("fnStorageTestStorMap", " FAILURE!");
    return result;
}