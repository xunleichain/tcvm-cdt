/*
 * tcapi.hpp
 *
 * @COPYRIGHT BY XUNLEI THUNDERCHAIN
 */
#ifndef THUNDERCHAIN_API_HPP_
#define THUNDERCHAIN_API_HPP_

#include "tcapi.h"
#include "types.hpp"
#include "tlv.hpp"
#include "tcstring.hpp"

/**
 * @brief 记录合约触发事件
 * 
 * @ingroup cppapi
 * @param eventID 事件标识
 * @param data 事件数据
 */
void notify(const char* eventID, const char* data) {
    TC_Notify(eventID, data);
}

/**
 * @brief 记录合约触发事件
 * 
 * @ingroup cppapi
 * @param eventID 事件标识
 * @param data 事件数据
 */
void notify(const tc::string& eventID, const tc::string& data) {
    TC_Notify(eventID.c_str(), data.c_str());
}

/**
 * @brief 合约触发事件，无事件标识
 * 
 * @ingroup cppapi
 * @param data 事件数据
 */
void log(const char* data) {
    TC_Log0(data);
}

/**
 * @brief 合约触发事件，带一级事件主题标识
 * 
 * @ingroup cppapi
 * @param data 事件数据
 * @param topic 事件标识
 */
void log(const char* data, const char* topic) {
    TC_Log1(data, topic);
}

/**
 * @brief 合约触发事件，带二级事件主题标识
 * 
 * @ingroup cppapi
 * @param data 事件数据
 * @param topic1 一级事件标识
 * @param topic2 二级事件标识
 */
void log(const char* data, const char* topic1, const char* topic2) {
    TC_Log2(data, topic1, topic2);
}

/**
 * @brief 合约触发事件，带三级事件主题标识
 * 
 * @ingroup cppapi
 * @param data 事件数据
 * @param topic1 一级事件标识
 * @param topic2 二级事件标识
 * @param topic3 三级事件标识
 */
void log(const char* data, const char* topic1, const char* topic2, const char* topic3) {
    TC_Log3(data, topic1, topic2, topic3);
}

/**
 * @brief 合约触发事件，带四级事件主题标识
 * 
 * @ingroup cppapi
 * @param data   事件数据
 * @param topic1 一级事件标识
 * @param topic2 二级事件标识
 * @param topic3 三级事件标识
 * @param topic4 四级事件标识
 */
void log(const char* data, const char* topic1, const char* topic2, const char* topic3, const char* topic4) {
    TC_Log4(data, topic1, topic2, topic3, topic4);
}

/**
 * @brief 合约触发事件，无事件标识
 * 
 * @ingroup cppapi
 * @param data 事件数据
 */
void log(const string& data){
    TC_Log0(data.c_str());
}

/**
 * @brief 合约触发事件，带一级事件主题标识
 * 
 * @ingroup cppapi
 * @param data 事件数据
 * @param topic 事件标识
 */
void log(const string& data, const string& topic1){
    TC_Log1(data.c_str(), topic1.c_str());
}

/**
 * @brief 合约触发事件，带二级事件主题标识
 * 
 * @ingroup cppapi
 * @param data 事件数据
 * @param topic1 一级事件标识
 * @param topic2 二级事件标识
 */
void log(const string& data, const string& topic1, const string& topic2){
    TC_Log2(data.c_str(), topic1.c_str(), topic2.c_str());
}

/**
 * @brief 合约触发事件，带三级事件主题标识
 * 
 * @ingroup cppapi
 * @param data 事件数据
 * @param topic1 一级事件标识
 * @param topic2 二级事件标识
 * @param topic3 三级事件标识
 */
void log(const string& data, const string& topic1, const string& topic2, const string& topic3){
    TC_Log3(data.c_str(), topic1.c_str(), topic2.c_str(), topic3.c_str());
}

/**
 * @brief 合约触发事件，带四级事件主题标识
 * 
 * @ingroup cppapi
 * @param data   事件数据
 * @param topic1 一级事件标识
 * @param topic2 二级事件标识
 * @param topic3 三级事件标识
 * @param topic4 四级事件标识
 */
void log(const string& data, const string& topic1, const string& topic2, const string& topic3, const string& topic4){
    TC_Log4(data.c_str(), topic1.c_str(), topic2.c_str(), topic3.c_str(), topic4.c_str());
}

//inline void assert(bool condition) {
//    TC_Assert(condition);
//}

/**
 * 断言判断，与C语言的assert()接口功能类似，如果输入的condition为false，则触发合约终止并回滚状态
 * @brief 断言判断，与C语言的assert()接口功能类似
 * 
 * @ingroup cppapi
 * @param condition 布尔类型的条件变量
 */
void require(bool condition) {
    TC_Require(condition);
}

/**
 * @brief 与TC_Require()接口功能一样
 * 与TC_Require()接口功能一样，若condition为false, msg参数将会打印到日志中，方便查看调试
 * @ingroup cppapi
 * @param condition 布尔类型的条件变量
 * @param msg 调试输出日志信息
 */
void require(bool condition, const char* msg) {
    TC_RequireWithMsg(condition, msg);
}

/**
 * @brief 与TC_Require()接口功能一样
 * 断言判断，与C语言的assert()接口功能类似，如果输入的condition为false，则触发合约终止并回滚状态
 * , msg参数将会打印到日志中，方便查看调试
 * @ingroup cppapi
 * @param condition 布尔类型的条件变量
 * @param msg 调试输出日志信息
 */
void require(bool condition, const string& msg) {
    TC_RequireWithMsg(condition, msg.c_str());
}

/**
 * @brief 触发合约终止并回滚状态
 * 
 * @ingroup cppapi
 */
void revert() {
    TC_Revert();
}

/**
 * @brief 触发合约终止并回滚状态, msg参数将会打印到日志中，方便查看调试
 * 
 * @ingroup cppapi
 * @param msg 调试输出日志信息
 */
void revert(const char *msg) {
    TC_RevertWithMsg(msg);
}

/**
 * @brief 触发合约终止并回滚状态, msg参数将会打印到日志中，方便查看调试
 * 
 * @ingroup cppapi
 * @param msg 调试输出日志信息
 */
void revert(const string& msg) {
    TC_RevertWithMsg(msg.c_str());
}

/**
 * @brief 发行token
 * 
 * @ingroup cppapi
 * @param amount 发行token总量
 * @note 多次调用发行token总量会累加
 */
void issue(const tc::BInt& amount) {
    TC_Issue((const BigInt)amount.toString());
}

/*
 * @function: set value to the json object with key
 * @params:
 * @return:
 */
//void TC_JsonPutString(JsonRoot root, const char* key, const tc::String& value){
//   TC_JsonPutAddress(root, key, value.c_str());
//}

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cppapi
 * @param root  TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @param value 字段值为tc::Address
 */
void TC_JsonPutAddress(JsonRoot root, const tc::string& key, const tc::Address& value){
   TC_JsonPutAddress(root, key.c_str(), value.toString());
}

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cppapi
 * @param root  TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @param value 字段值为tc::BInt
 */
void TC_JsonPutBigInt(JsonRoot root, tc::string key, const tc::BInt& value){
   TC_JsonPutBigInt(root, key.c_str(), value.toString());
}


/**
 * @brief 在root指向的json对象中，获取指定key字段的值为val
 * 
 * @ingroup cppapi
 * @param key 字段的标识符
 * @param val 字段值(字符串)
 * @param char* 字段值(字符串)
 */
void TC_StorageSetString(const tc::string& key, const tc::string& val){
    TC_StorageSetString(key.c_str(), val.c_str());
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值为val
 * 
 * @ingroup cppapi
 * @param key 字段的标识符
 * @param val 字段值(字符串)
 * @param char* 字段值(字符串)
 */
void TC_StorageSetString(const tc::string& key, const char* val){
    TC_StorageSetString(key.c_str(), val);
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值为val
 * 
 * @ingroup cppapi
 * @param key 字段的标识符
 * @param char* 字段值(字符串)
 */
void TC_StorageSetBytes(const tc::string& key, const uint8_t* val, uint32_t size){
    TC_StorageSetBytes(key.c_str(), val, size);
}

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cppapi
 * @tparam K 键字段类型
 * @tparam V 值字段类型
 * @param key 字段的标识符
 * @param value 字段值，类型皆可
 */
template<typename K, typename V>
void TC_StorageSetValue(const K& key, const V& value){
    tlv::BufferWriter buf;
    pack(buf, key);

    tlv::BufferWriter buf2;
    pack(buf2, value);

    TC_StoragePureSetBytes(buf.bytes(), buf.length(), buf2.bytes(), buf2.length());
}
/*
**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cppapi
 * @tparam K 键字段类型
 * @tparam V 值字段类型
 * @param key 字段的标识符
 * @param value 字段值，类型皆可
 */
template<typename K, typename V>
void TC_StorageSetValue(string& key, const V& value){
    tlv::BufferWriter buf;
    pack(buf, key);

    tlv::BufferWriter buf2;
    pack(buf2, value);

    TC_StoragePureSetBytes(buf.bytes(), buf.length(), buf2.bytes(), buf2.length());
}

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cppapi
 * @tparam V 值字段类型
 * @param key 字段的标识符(字符串)
 * @param value 字段值，类型皆可
 */
template<typename V>
void TC_StorageSetValue(char* key, const V& value){
    tlv::BufferWriter buf;
    pack(buf, value);

    TC_StorageSetBytes(key, buf.bytes(), buf.length());
}

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cppapi
 * @tparam V 值字段类型
 * @param key 字段的标识符(字符串)
 * @param value 字段值，类型皆可
 */
template<typename V>
void TC_StorageSetValue(const char* key, const V& value){
    tlv::BufferWriter buf;
    pack(buf, value);

    TC_StorageSetBytes(key, buf.bytes(), buf.length());
}
/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cppapi
 * @tparam V 值字段类型
 * @param key 字段的标识符(字符串)
 * @param value 字段值，类型皆可
 */
template<typename V>
void TC_StorageSetValue(const string& key, const V& value){
    tlv::BufferWriter buf;
    pack(buf, value);

    TC_StorageSetBytes(key.c_str(), buf.bytes(), buf.length());
}
/**
 * @brief 在root指向的json对象中，获取指定key字段的值value
 * 
 * @ingroup cppapi
 * @tparam K 键字段类型
 * @tparam V 值字段类型
 * @param key 字段的标识符
 * @return V 返回"值"
 */
template<typename K, typename V>
V TC_StorageGetValue(const K& key){
    tlv::BufferWriter buf;
    pack(buf, key);

    V tmp;
    const uint8_t* str = TC_StoragePureGetBytes(buf.bytes(), buf.length());
    if (*str != '\0'){
        tlv::BufferReader reader(str);
        tlv::unpack(reader, tmp);
    } 
    return tmp;
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值value
 * 
 * @ingroup cppapi
 * @tparam V 值字段类型
 * @param key 字段的标识符(字符串)
 * @return V 返回"值"
 */
template<typename V>
V TC_StorageGetValue(const string& key){
    V tmp;
    const uint8_t* str = TC_StorageGetBytes(key.c_str());
    if (*str != '\0'){
        tlv::BufferReader reader(str);
        tlv::unpack(reader, tmp);
    }
    return tmp;
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值为val
 * 
 * @ingroup cppapi
 * @param key 字段的标识符
 * @param char* 字段值(字符串)
 */
char* TC_StorageGetString(const tc::string& key){
    return TC_StorageGetString(key.c_str());
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值为val
 * 
 * @ingroup cppapi
 * @param key 字段的标识符
 * @param char* 字段值(字符串)
 */
uint8_t* TC_StorageGetBytes(const tc::string& key){
    return TC_StorageGetBytes(key.c_str());
}


/**
 * @brief 在root指向的json对象中，获取指定key字段的值value
 * 
 * @ingroup cppapi
 * @tparam V 值字段类型
 * @param key 字段的标识符(字符串)
 * @return V 返回"值"
 */
template<typename V>
V TC_StorageGetValue(char* key){
    V tmp;
    const uint8_t* str = TC_StorageGetBytes(key);
    if (*str != '\0'){
        tlv::BufferReader reader(str);
        tlv::unpack(reader,tmp);
    }
    return tmp;
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值value
 * 
 * @ingroup cppapi
 * @tparam V 值字段类型
 * @param key 字段的标识符(字符串)
 * @return V 返回"值"
 */
template<typename V>
V TC_StorageGetValue(const char* key){
    V tmp;
    const uint8_t* str = TC_StorageGetBytes(key);
    if (*str != '\0'){
        tlv::BufferReader reader(str);
        tlv::unpack(reader,tmp);
    }
    return tmp;
}

#endif /* THUNDERCHAIN_API_HPP_ */
