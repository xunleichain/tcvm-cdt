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


struct Person {
    std::string name;
    int age;
};

struct Student {
    Person base;
    int score;
    uint8_t u8;
    int8_t s8;
    uint16_t u16;
    int16_t s16;
    uint u32;
    int s32;
    uint64_t u64;
    int64_t s64;
    char c;
    std::string str;
    std::vector<std::string> vs;
    std::map<int, std::string> m;
    std::tuple<int, std::string> t;
    tc::Address addr;
    tc::BInt bint;
    tc::Token token;
    //tc::Hash hash;
};

TC_STRUCT(Person,
    TC_FIELD_NAME(name, "name"),
    TC_FIELD_NAME(age, "age"));
    
TC_STRUCT(Student,
    TC_FIELD_NAME(base, "base"),
    TC_FIELD_NAME(score, "score"),
    TC_FIELD_NAME(u8, "u8"),
    TC_FIELD_NAME(u16, "u16"),
    TC_FIELD_NAME(u32, "u32"),
    TC_FIELD_NAME(u64, "u64"),
    TC_FIELD_NAME(s8, "s8"),
    TC_FIELD_NAME(s16, "s16"),
    TC_FIELD_NAME(s32, "s32"),
    TC_FIELD_NAME(s64, "s64"),
    TC_FIELD_NAME(c, "char"),
    TC_FIELD_NAME(str, "string"),
    TC_FIELD_NAME(vs, "vector-string"),
    TC_FIELD_NAME(m, "map"),
    TC_FIELD_NAME(t, "tuple"),
    TC_FIELD_NAME(addr, "tc::Address"),
    TC_FIELD_NAME(bint, "tc::BInt"),
    TC_FIELD_NAME(token, "tc::Token"));


class WasmCPlusAPITLV : public TCBaseContract {

private:
    bool fnCompareBytes(const char* name, const uint8_t* sourceBytes, const int sourceLength, const uint8_t* destBytes, const int destLength);
public:
    char* Init();
    const char* fnTLVTestBufferWriteAndReader();
    const char* fnTLVTestMarshalAndUnmarshal();
    const char* fnTLVTestPackAndUnpack();
    const char* fnTLVTestPackAndUnpackMany();
};

TC_ABI(WasmCPlusAPITLV, (fnTLVTestBufferWriteAndReader)\
                        (fnTLVTestMarshalAndUnmarshal)\
                        (fnTLVTestPackAndUnpack)\
                        (fnTLVTestPackAndUnpackMany)\
                        )

//=====================================================
char* WasmCPlusAPITLV::Init() {
    TC_Prints("WasmCPlusAPITLV Init");
    return "init ok";
}

bool WasmCPlusAPITLV::fnCompareBytes(const char* name, const uint8_t* sourceBytes, const int sourceLength, const uint8_t* destBytes, const int destLength) {
    bool bResult = true;
    Print("校验字节流信息", "校验名称", name);
    if (sourceLength != destLength)
    {
        bResult = false;
        Print("校验字节流长度不一致", "sourceLength", sourceLength, "destLength", destLength);
        return false;
    }
    Print("校验字节流长度", "sourceLength", sourceLength);
    for (int i = 0; i < sourceLength; i ++)
    {
        if (sourceBytes[i] != destBytes[i])
        {
            bResult = false;
            Print("校验字节流内容不一致", "no", i, "sourceBytes[i]", sourceBytes[i], "destBytes[i]", destBytes[i]);    
        }
    }
    return bResult;
}

const char* WasmCPlusAPITLV::fnTLVTestBufferWriteAndReader() {
    bool bResult = false;
    do
    {

        TC_Prints("fnTLVTestBufferWriteAndReader start");
        //uint64_t
        tc::tlv::BufferWriter writeBuffer = tc::tlv::BufferWriter();
        uint64_t testValue = 0x1111111111111;
        writeBuffer.write(testValue, sizeof(testValue));
        TC_Prints("writeBuffer.write ok");
        tc::tlv::BufferReader readerBuffer = tc::tlv::BufferReader(writeBuffer.bytes());
        uint64_t testGetValue = readerBuffer.read(sizeof(uint64_t));
        TC_Prints("readerBuffer.read ok");
        if (testValue != testGetValue)
        {
            Print("校验字节流读写", "write", i64toa(testValue, 16), "read", i64toa(testGetValue, 16));
            bResult = false;
            break;
        }
        if (!fnCompareBytes("uint64_t", writeBuffer.bytes(), writeBuffer.length(), readerBuffer.bytes(), readerBuffer.offset()))
        {
            bResult = false;
            break;
        }
        TC_Prints("fnTLVTestBufferWriteAndReader end");
        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTLVTestBufferWrite", " SUCCESS!") : strconcat("fnTLVTestBufferWrite", " FAILURE!");
    return result;
}


const char* WasmCPlusAPITLV::fnTLVTestMarshalAndUnmarshal() {
    bool bResult = false;
    do
    {
        Student s;
        Person p;
        s.score = 100;
        p.age = 30;
        p.name = std::string("ywh");
        s.base = p;
        s.u8 = 8;
        s.u16 = 16;
        s.u32 = 32;
        s.u64 = 64;
        s.s8 = -8;
        s.s16 = -16;
        s.s32 = -32;
        s.s64 = -64;
        s.c = 'X';
        s.str = "hello";
        s.vs.push_back("hello");
        s.vs.push_back("world");
        s.m[1] = std::string("one");
        s.m[2] = std::string("two");
        s.t = std::make_tuple(100, "ywh");
        s.addr = tc::Address("0x123456789");
        s.token = tc::Token(s.addr, tc::BInt("0x123321"));
        s.bint = tc::BInt("0xabcdef");
        
        tlv::BufferWriter buf = tlv::Marshal(s);
        
        Student tmp;
        tlv::BufferReader reader(buf.bytes());
        tlv::Unmarshal(reader, tmp);
        
        FALSE_PRINT_BREAK("tmp.base.age", tmp.base.age, s.base.age);
        FALSE_PRINT_BREAK("tmp.base.name", tmp.base.name, s.base.name);
        FALSE_PRINT_BREAK("tmp.score", tmp.score, s.score);
        FALSE_PRINT_BREAK("tmp.u8", tmp.u8, s.u8);
        FALSE_PRINT_BREAK("tmp.s8", tmp.s8, s.s8);
        FALSE_PRINT_BREAK("tmp.u16", tmp.u16, s.u16);
        FALSE_PRINT_BREAK("tmp.s16", tmp.s16, s.s16);
        FALSE_NOPRINT_BREAK("tmp.u32", tmp.u32, s.u32);
        FALSE_NOPRINT_BREAK("tmp.s32", tmp.s32, s.s32);
        FALSE_NOPRINT_BREAK("tmp.u64", tmp.u64, s.u64);
        FALSE_NOPRINT_BREAK("tmp.s64", tmp.s64, s.s64);
        FALSE_PRINT_BREAK("tmp.c", tmp.c, s.c);
        FALSE_PRINT_BREAK("tmp.str", tmp.str, s.str);
        FALSE_PRINT_BREAK("tmp.vs[0]", tmp.vs[0], s.vs[0]);
        FALSE_PRINT_BREAK("tmp.vs[1]", tmp.vs[1], s.vs[1]);
        FALSE_PRINT_BREAK("tmp.m[1]", tmp.m[1], s.m[1]);
        FALSE_PRINT_BREAK("tmp.m[2]", tmp.m[2], s.m[2]);

        FALSE_PRINT_STRCMP_BREAK("tmp.addr", tmp.addr.toString(), s.addr.toString());
        FALSE_PRINT_BREAK("tmp.bint", tmp.bint, s.bint);
        FALSE_PRINT_BREAK("tmp.token", tmp.token.value(), s.token.value());

        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTLVTestMarshalAndUnmarshal", " SUCCESS!") : strconcat("fnTLVTestMarshalAndUnmarshal", " FAILURE!");
    return result;
}





const char* WasmCPlusAPITLV::fnTLVTestPackAndUnpack() {
    bool bResult = false;
    do
    {
        Student s;
        Person p;
        s.score = 100;
        p.age = 30;
        p.name = std::string("ywh");
        s.base = p;
        s.u8 = 8;
        s.u16 = 16;
        s.u32 = 32;
        s.u64 = 64;
        s.s8 = -8;
        s.s16 = -16;
        s.s32 = -32;
        s.s64 = -64;
        s.c = 'X';
        s.str = "hello";
        s.vs.push_back("hello");
        s.vs.push_back("world");
        s.m[1] = std::string("one");
        s.m[2] = std::string("two");
        s.t = std::make_tuple(100, "ywh");
        s.addr = tc::Address("0x123456789");
        s.token = tc::Token(s.addr, tc::BInt("0x123321"));
        s.bint = tc::BInt("0xabcdef");
        
        tlv::BufferWriter buf;
        tlv::pack(buf, s);
        Print("tc::BInt", "s", s.bint);
        tlv::BufferReader reader(buf.bytes());
        Student tmp;
        tlv::unpack(reader, tmp);
        Print("tc::BInt", "tmp", tmp.bint);        
        

        FALSE_PRINT_BREAK("tmp.base.age", tmp.base.age, s.base.age);
        FALSE_PRINT_BREAK("tmp.base.name", tmp.base.name, s.base.name);
        FALSE_PRINT_BREAK("tmp.score", tmp.score, s.score);
        FALSE_PRINT_BREAK("tmp.u8", tmp.u8, s.u8);
        FALSE_PRINT_BREAK("tmp.s8", tmp.s8, s.s8);
        FALSE_PRINT_BREAK("tmp.u16", tmp.u16, s.u16);
        FALSE_PRINT_BREAK("tmp.s16", tmp.s16, s.s16);
        FALSE_NOPRINT_BREAK("tmp.u32", tmp.u32, s.u32);
        FALSE_NOPRINT_BREAK("tmp.s32", tmp.s32, s.s32);
        FALSE_NOPRINT_BREAK("tmp.u64", tmp.u64, s.u64);
        FALSE_NOPRINT_BREAK("tmp.s64", tmp.s64, s.s64);
        FALSE_PRINT_BREAK("tmp.c", tmp.c, s.c);
        FALSE_PRINT_BREAK("tmp.str", tmp.str, s.str);
        FALSE_PRINT_BREAK("tmp.vs[0]", tmp.vs[0], s.vs[0]);
        FALSE_PRINT_BREAK("tmp.vs[1]", tmp.vs[1], s.vs[1]);
        FALSE_PRINT_BREAK("tmp.m[1]", tmp.m[1], s.m[1]);
        FALSE_PRINT_BREAK("tmp.m[2]", tmp.m[2], s.m[2]);

        FALSE_PRINT_STRCMP_BREAK("tmp.addr", tmp.addr.toString(), s.addr.toString());
        FALSE_PRINT_BREAK("tmp.bint", tmp.bint, s.bint);
        FALSE_PRINT_BREAK("tmp.token", tmp.token.value(), s.token.value());

        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTLVTestPackAndUnpack", " SUCCESS!") : strconcat("fnTLVTestPackAndUnpack", " FAILURE!");
    return result;
}


const char* WasmCPlusAPITLV::fnTLVTestPackAndUnpackMany() {
    bool bResult = false;
    do
    {
        tlv::BufferWriter buf;        
        int iPutValue = 10;
        tlv::pack(buf, iPutValue);
        int iGetValue = 0;
        tlv::BufferReader reader(buf.bytes());
        tlv::unpack(reader, iGetValue);
        Print("fnTLVTestPackAndUnpackMany", "iPutValue", itoa(iPutValue), "iGetValue", itoa(iGetValue));        
        FALSE_PRINT_BREAK("type_int", iPutValue, iGetValue);

        tlv::BufferWriter buf1;
        uint64_t u64PutValue = 0x1000000000;
        tlv::pack(buf1, u64PutValue);
        uint64_t u64GetValue = 0;
        tlv::BufferReader reader1(buf1.bytes());
        tlv::unpack(reader1, u64GetValue);
        Print("fnTLVTestPackAndUnpackMany", "u64PutValue", i64toa(u64PutValue,16), "u64GetValue", i64toa(u64GetValue,16));
        FALSE_PRINT_BREAK("type_uint64", u64PutValue, u64GetValue);
        
        tlv::BufferWriter buf2;
        const char* cpPutValue = "zhangzitao";
        tlv::packCString(buf2, cpPutValue);
        char* cpGetValue;
        tlv::BufferReader reader2(buf2.bytes());
        tlv::unpackCString(reader2, cpGetValue);
        Print("fnTLVTestPackAndUnpackMany", "cpPutValue", (const char*)cpPutValue, "cpGetValue", (const char*)cpGetValue);
        FALSE_PRINT_STRCMP_BREAK("type_char*", cpPutValue, cpGetValue);        

        tlv::BufferWriter buf3;
        tc::string sPutValue("hello world!");
        tlv::pack(buf3, sPutValue);
        tc::string sGetValue;
        tlv::BufferReader reader3(buf3.bytes());
        tlv::unpack(reader3, sGetValue);
        Print("fnTLVTestPackAndUnpackMany", "sPutValue", sPutValue, "sGetValue", sGetValue);
        FALSE_PRINT_BREAK("type_tc_string", sPutValue, sGetValue);

        tlv::BufferWriter buf4;
        tc::Address addPutValue("0x00000000000000000000000000000001");
        tlv::pack(buf4, addPutValue);
        tc::Address addGetValue;
        tlv::BufferReader reader4(buf4.bytes());
        tlv::unpack(reader4, addGetValue);
        Print("fnTLVTestPackAndUnpackMany", "addPutValue", addPutValue.toString(), "addGetValue", addGetValue.toString());
        FALSE_PRINT_STRCMP_BREAK("type_tc_address", addPutValue.toString(), addGetValue.toString());

        tlv::BufferWriter buf5;
        tc::BInt bPutValue("0x10000000000000adfasdf1234234234234234234234243234234234000000000000000001");
        tlv::pack(buf5, bPutValue);
        tc::BInt bGetValue;
        tlv::BufferReader reader5(buf5.bytes());
        tlv::unpack(reader5, bGetValue);
        Print("fnTLVTestPackAndUnpackMany", "bPutValue", bPutValue, "bGetValue", bGetValue);
        FALSE_PRINT_BREAK("type_tc_BInt", bPutValue, bGetValue);

        tlv::BufferWriter buf6;
        tc::Token tPutValue("0x10000000000000000000000000000001");
        tlv::pack(buf6, tPutValue);
        tc::Token tGetValue;
        tlv::BufferReader reader6(buf6.bytes());
        tlv::unpack(reader6, tGetValue);
        Print("fnTLVTestPackAndUnpackMany", "tPutValue", tPutValue.value(), "tGetValue", tGetValue.value());
        FALSE_PRINT_BREAK("type_tc_token", tPutValue.value(), tGetValue.value());

        tlv::BufferWriter buf7;
        tc::Hash hPutValue("0x100000000000000000000000023423423423423423423423434240000001");
        tlv::pack(buf7, hPutValue);
        tc::Hash hGetValue = tc::Hash("0x");
        tlv::BufferReader reader7(buf7.bytes());
        tlv::unpack(reader7, hGetValue);
        Print("fnTLVTestPackAndUnpackMany", "hPutValue", hPutValue.toString(), "hGetValue", hGetValue.toString());
        FALSE_PRINT_STRCMP_BREAK("type_tc_Hash", hPutValue.toString(), hGetValue.toString());

        bResult = true;
    } while (0);
    const char* result =  bResult ? strconcat("fnTLVTestPackAndUnpackMany", " SUCCESS!") : strconcat("fnTLVTestPackAndUnpackMany", " FAILURE!");
    return result;
}

