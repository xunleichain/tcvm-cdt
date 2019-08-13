/*
 *  * tcmethod.hpp
 *  *
 *  * @COPYRIGHT BY XUNLEI THUNDERCHAIN
 *  */

#ifndef THUNDERCHAIN_METHOD_H_
#define THUNDERCHAIN_METHOD_H_

#include<cstdlib>
#include<cstring>
#include<cstdio>

#include<type_traits>
#include<tuple>

#include<boost/fusion/include/for_each.hpp>
#include<boost/fusion/adapted/std_tuple.hpp>
#include<boost/fusion/include/std_tuple.hpp>

#include<boost/preprocessor/stringize.hpp>
#include<boost/preprocessor/seq/for_each.hpp>
#include<boost/preprocessor/seq/for_each_i.hpp>
#include<boost/core/noncopyable.hpp>

#include "tcapi.hpp"
#include "json.hpp"
#include "tcstring.hpp"

namespace tc {
/**
 * @defgroup storage Storage
 * @brief 数据存储模板类，持久化存储数据在区块链上。
 * @ingroup types
 * @{
 */

/**
 * @brief 存储单个元素
 * 以key为标识符存储单个元素
 * @tparam T 非枚举数据类型
 * @tparam void 枚举类型标识(自动判断，不用传参)
 */
template<typename T, typename Enable = void>
class StorValue : boost::noncopyable {
public:
    StorValue(const char* key): key_(key){}
    StorValue(const tc::string& key): key_(key){}

    /**
     * @brief 存储数据
     * 
     * @param value 存储数据值
     */
    void set(const T& value){
        TC_StorageSetValue(key_, value);
    }

    /**
     * @brief 读取数据
     * 
     * @return T 返回数据类型"值"
     */
    T get(){
        T value;
        value = TC_StorageGetValue<T>(key_);
        return value;
    }

    const tc::string key_;///<该数据的键值
};

/**
 * @brief 存储单个元素
 * 
 * @tparam T 枚举数据类型
 */
template<typename T>
class StorValue<T, typename std::enable_if<std::is_enum<T>::value>::type>  : boost::noncopyable {
public:
    StorValue(const char* key) : key_(key){}
    StorValue(const tc::string& key) : key_(key){}

    /**
     * @brief 存储数据
     * 
     * @param value 存储数据值
     */
    void set(const T& value){
        TC_StorageSetValue(key_, static_cast<int>(value));
    }

    /**
     * @brief 读取数据
     * 
     * @return T 返回数据类型"值"
     */
    T get(){
        int value;
        TC_StorageGetValue(key_, value);
        return (T)value;
    }

    const tc::string key_;///<该数据的键值
};

//数组
/**
 * @brief 存储数组，支持按index读写
 * 只支持在元素尾端添加或减少数据
 * @tparam T 数组元素类型
 */
template <typename T>
class StorArray : boost::noncopyable {
public:
    StorArray(const char* key): key_(key){}
    StorArray(const tc::string& key): key_(key){}

    /**
     * @brief 添加一个新的元素到数组结尾
     * 
     * @param value 新的元素值
     */
    void push(const T& value){
        tc::BInt length(TC_StorageGetString(key_)); 
		TC_StorageSetValue(strconcat(key_.c_str(), length.toString()), value);

        length++;
		TC_StorageSetString(key_, length.toString());
    }

    /**
     * @brief 设置位置为i的元素值为value
     * 
     * @param value 新的元素值
     * @param i 索引值
	 * @warning 索引值不能超过数组长度
     */
    void set(const int i, const T& value){
        tc::BInt length(TC_StorageGetString(key_)); 
        tc::BInt index(i);
        TC_RequireWithMsg(index >= tc::BInt(0) && index < length, "Over Array Index Out Of Bounds");

		TC_StorageSetValue(strconcat(key_.c_str(), index.toString()), value);
    }

    /**
     * @brief 设置位置为i的元素值为value
     * 
     * @param value 新的元素值
     * @param i 索引值
	 * @warning 索引值不能超过数组长度
     */
    void set(const tc::BInt& index, const T& value){
        tc::BInt length(TC_StorageGetString(key_)); 
        TC_RequireWithMsg(index >= tc::BInt(0) && index < length, "Over Array Index Out Of Bounds");

		TC_StorageSetValue(strconcat(key_.c_str(), index.toString()), value);
    }

    /**
     * @brief 获取位置i的元素
     * 
     * @param i 索引值
	 * @return T 返回"值"
	 * @warning 索引值不能超过数组长度
     */
    T operator[](const int i){
        tc::BInt length(TC_StorageGetString(key_)); 
        tc::BInt index(i);
        TC_RequireWithMsg(index >= tc::BInt(0) && index < length, "Over Array Index Out Of Bounds");

		return TC_StorageGetValue<T>(strconcat(key_.c_str(), index.toString()));
    }

    /**
     * @brief 获取位置为index的元素
     * 
     * @param index 
	 * @warning 索引值不能超过数组长度
     * @return T 返回"值"
     */
    T operator[](const tc::BInt& index){
        tc::BInt length(TC_StorageGetString(key_)); 
        TC_RequireWithMsg(index >= tc::BInt(0) && index < length, "Over Array Index Out Of Bounds");


		return TC_StorageGetValue<T>(strconcat(key_.c_str(), index.toString()));
    }

    /**
     * @brief 数组长度
     * 
     * @return tc::BInt 数组长度，类型为BInt
     */
    tc::BInt length(){
        return tc::BInt(TC_StorageGetString(key_)); 
    }

    /**
     * @brief 删除数组的最后一个元素
     * @warning 只修改数组长度
     */
    void pop(){
        tc::BInt length(TC_StorageGetString(key_)); 
        TC_RequireWithMsg(length >= tc::BInt(0), "Array Empty with pop");
        if (length == tc::BInt(0)){
            return;
        }
        length--;
        TC_StorageSetString(key_, length.toString());
    }

    const tc::string key_;///<该数据的键值
};

//map
template<typename T>
void key_gen(tlv::BufferWriter& buf, const T& key){
    pack(buf, key);
}

template <typename T>
inline void key_str(tlv::BufferWriter& str, const T& val){
    key_gen(str, val);
}

template<typename K, typename... Keys>
void key_str(tlv::BufferWriter& buf, const K k1, const Keys... ks){
    key_gen(buf, k1);
    key_str(buf, ks...);
}

template<typename... Keys>
tlv::BufferWriter key_str(const Keys&... ks){
    tlv::BufferWriter buf;
    key_str(buf, ks...);
    return buf;
}
/**
 * @brief 用于声明键类型
 * 声明键类型，用于为多个类型组合为键
 * @tparam Keys 键类型
 */
template<typename... Keys>
class Key{
public:
    static const tlv::BufferWriter keyStr(const Keys&... k){
        return key_str(k...);
    }

};

//散列表 K可以为多个元素组合
/**
 * @brief 散列表
 * 
 * @tparam K 键类型
 * @tparam V 值类型
 */
template<typename K, typename V>
class StorMap : boost::noncopyable {
public:
    StorMap(const char* key): key_(key){}
    StorMap(const tc::string& key): key_(key){}

    /**
     * @brief 设置map键值对
     * 
     * @tparam Args 键类型
     * @param v 值数据
     * @param k 键数据
     */
    template <typename... Args>
    void set(const V& v, const Args&... k){

        const tlv::BufferWriter key = K::keyStr(k...);
        uint8_t* tmp = getKeyBytes(key);

        tlv::BufferWriter buf;
        pack(buf, v);

        //call c storage api
        TC_StoragePureSetBytes(tmp, key_.length() + key.length(), buf.bytes(), buf.length());
        free(tmp);
    }

    /**
     * @brief 获取map键值对
     * 
     * @tparam Args 键类型
     * @param k 键数据
     * @return V 返回"值"
     */
    template <typename... Args>
    V get(const Args&... k){
        const tlv::BufferWriter key = K::keyStr(k...);
        uint8_t* value = getValueBytes(key);

        //call c storage api
		if (*value == 0){
            V v;//init nil value
			return v;
		}

        V v;
        tlv::BufferReader reader(value);
        tlv::unpack(reader, v);

        free(value);
		return v;
	}

    /**
     * @brief 获取键序列化之后的数据
     * 
     * @param key 键数据
     * @return uint8_t* 序列化之后的指针地址
     */
    uint8_t* getKeyBytes(const tlv::BufferWriter& key){
        uint8_t* tmp = (uint8_t*)calloc(1, key_.length()+key.length());
        memcpy(tmp, key_.c_str(), key_.length());
        memcpy(tmp+key_.length(), key.bytes(), key.length());
        return tmp;
    }
    
    /**
     * @brief 通过键序列化之后的数据，获取值原始数据
     * 
     * @param key 键数据
     * @return uint8_t* 值原始数据指针地址
     */
    uint8_t* getValueBytes(const tlv::BufferWriter& key){
        uint8_t* tmp = getKeyBytes(key);
		uint8_t* value = TC_StoragePureGetBytes(tmp, key_.length() + key.length());
        free(tmp);
        return value; 
    }

    const tc::string key_;///<该数据的键值
};

///@}
}

/**
 * @defgroup abi Method
 * @brief 编写合约代码时，使用该模块来定义合约接口
 * @ingroup cppapi 
 * @{
 */


/**
 * @brief 合约基类
 * 自定义的合约类，必需继承该类
 */
class TCBaseContract : boost::noncopyable {
public:
    void Init(){}
    void Callback(){}
};

template<typename T, typename Q, typename R, typename... Args>
const char* call_method(T* obj, R (Q::*func)(Args...), const char* arg){
    std::tuple<std::decay_t<Args>...> t;
    int parseRet = json::Unmarshal(arg, t);
    if (parseRet != 0){
        TC_RequireWithMsg(false, "Input Args Parse Error");
    }
    auto f2 = [&](auto... a){ return (obj->*func)(a...);};
    
    R ret =  std::apply(f2, t);
    void* root = TC_JsonNewObject();
    json::Marshal(root, "ret", ret);
    return TC_JsonToString(root);
}

template<typename T, typename Q, typename... Args>
const char* call_method(T* obj, void (Q::*func)(Args...), const char* arg){
    std::tuple<std::decay_t<Args>...> t;
    int parseRet = json::Unmarshal(arg, t);
    if (parseRet != 0){
        TC_RequireWithMsg(false, "Input Args Parse Error");
    }
    auto f2 = [&](auto... a){ return (obj->*func)(a...);};
    std::apply(f2, t);
    return "{\"ret\":\"\"}";
}


/**
 * @brief 调用其他合约时使用的宏定义
 */
#define TC_CONTRACT(TYPE) \
public:\
    TYPE(Address addr) {addr_ = addr;}\
    Address getAddr(){return addr_;}\
    void setAddr(Address addr){addr_=addr;}\
private:\
    TYPE() {}\
    Address addr_;

/**
 * @brief 使用call方式调用其他合约
 * 
 */
#define CALL(CONTRACT, METHOD, ...) \
    call_contract(#METHOD, CONTRACT, &decltype(CONTRACT)::METHOD, std::make_tuple(__VA_ARGS__))

/**
 * @brief 使用Delegate_call方式调用其他合约
 * 
 */
#define DELEGATE_CALL(CONTRACT, METHOD, ...) \
    delegate_call_contract(#METHOD, CONTRACT, &decltype(CONTRACT)::METHOD, std::make_tuple(__VA_ARGS__))



/**
 * @brief 检查两个值是否是相同类型
 * 
 * @tparam T 自动匹配参数t类型
 * @tparam U 自动匹配参数u类型
 * @param t 待检查值t
 * @param u 待检查值u
 */
template<typename T, typename U>
void typeCheck(T t, U u) {
    static_assert(std::is_same<T, U>::value);
}

template<typename R, typename T, typename... Args>
R call_contract(const char* action, T& con, R (T::*func)(Args...), const std::tuple<std::decay_t<Args>...>& args)
{
    R r;
    std::tuple<std::decay_t<Args>...> t;
    typeCheck(t, args);
    void* root = TC_JsonParse(TC_CallContract(con.getAddr().toString(), action, json::Marshal(args)));
    json::Unmarshal(root, "ret", r);
	return r;
}

template<typename T, typename... Args, typename... Inputs>
void call_contract(const char* action, T& con, void (T::*func)(Args...), const std::tuple<std::decay_t<Args>...>& args)
{
    std::tuple<std::decay_t<Args>...> t;
    typeCheck(t, args);
    TC_CallContract(con.getAddr().toString(), action, json::Marshal(args));
}

template<typename R, typename T, typename... Args, typename... Inputs>
R delegate_call_contract(const char* action, T& con, R (T::*func)(Args...), const std::tuple<std::decay_t<Args>...>& args)
{
    R r;
    std::tuple<std::decay_t<Args>...> t;
    typeCheck(t, args);
    void* root = TC_JsonParse(TC_DelegateCallContract(con.getAddr().toString(), action, json::Marshal(args)));
    json::Unmarshal(root, "ret", r);
	return r;
}

template<typename T, typename... Args, typename... Inputs>
void delegate_call_contract(const char* action, T& con, void (T::*func)(Args...), const std::tuple<std::decay_t<Args>...>& args)
{
    std::tuple<std::decay_t<Args>...> t;
    typeCheck(t, args);
    TC_DelegateCallContract(con.getAddr().toString(), action, json::Marshal(args));
}


#define TC_API_CALL(r, OP, elem) \
    else if(strcmp(method, BOOST_PP_STRINGIZE(elem)) == 0){ \
        return call_method(&contract, &OP::elem, arg);\
    }


#define TC_API(TYPE, MEMBERS) \
    BOOST_PP_SEQ_FOR_EACH(TC_API_CALL, TYPE, MEMBERS)

/**
 * @brief 合约ABI定义宏
 * TC_ABI(<合约类>，(<方法名>)(<方法名>)(<方法名>)(<方法名>)
 */
#define TC_ABI(TYPE, MEMBERS) \
extern "C"{\
    const char* thunderchain_main(const char* method, const char* arg){ \
            TYPE contract;\
            if(strcmp(method, "Init")==0 || strcmp(method, "init")==0){\
                contract.Init();\
            }\
            TC_API(TYPE, MEMBERS)\
            else if(strcmp(method, "Callback")==0 || strcmp(method, "callback")==0){\
                contract.Callback();\
            }\
			else{\
			TC_RequireWithMsg(false, "Unknown Methods");\
			}\
            return 0;\
    }\
}

///@}
#endif
