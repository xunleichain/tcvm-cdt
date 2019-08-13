#ifndef _JSON_H_
#define _JSON_H_

#include "tcapi.h"
#include <tuple>
#include <type_traits>
#include "reflect.hpp"
#include "types.hpp"
#include "tcstring.hpp"

#define STD_C++
#ifdef STD_C++
//#include <vector>
//#include <map>
#include <algorithm>
#include <iterator>
#endif

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>

namespace tc{
namespace json {
/**
 * @defgroup json Json API
 * @ingroup serial
 * @brief C++ 序列化库，使用json方式序列化
 * @{
 */

#ifdef STD_C++
/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @param root json句柄
 * @param key  字段的标识符
 * @param val  字段值为字符串
 */
inline void PutString(JsonRoot root, const char *key, const tc::string& val) {
    TC_JsonPutString(root, key, val.c_str());
}

#endif

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 字段为字符
 */
inline void PutChar(JsonRoot root, const char *key, const char val) {
    char s[2];
    s[0] = val;
    s[1] = '\0';
    TC_JsonPutString(root, key, s);
}

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 字段为bool类型
 */
inline void PutBool(JsonRoot root, const char* key, const bool val){
    if (val){ 
        TC_JsonPutInt(root, key, 1); 
    }else{ 
        TC_JsonPutInt(root, key, 0); 
    }
}
/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 字段为Address类型
 */
inline void PutAddress(JsonRoot root, const char *key, const tc::Address& val) {
    TC_JsonPutAddress(root, key, val.toString());
}
/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 字段为BInt类型
 */
inline void PutBInt(JsonRoot root, const char *key, const tc::BInt& val) {
    TC_JsonPutBigInt(root, key, val.toString());
}
/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 字段为Hash类型
 */
inline void PutHash(JsonRoot root, const char *key, const tc::Hash& val) {
    TC_JsonPutString(root, key, val.toString());
}
/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 字段为Token类型
 */
inline void PutToken(JsonRoot root, const char *key, const tc::Token& val) {
    void *child = TC_JsonNewObject();
    TC_JsonPutAddress(child, "id", (address)val.id().toString());
    TC_JsonPutBigInt(child, "val", (BigInt)val.value().toString());
    TC_JsonPutObject(root, key, child);
}
/**
 * @brief Marshal函数模板声明
 * 
 * @tparam T  需要序列化的元素类型
 * @param value 元素值
 * @return char* json序列化之后的字符串
 */
template<typename T> char *Marshal(const T& value);
template<typename T> void *MarshalRoot(const T& value);

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @tparam T 模板类，仅限枚举类型
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 字段为Enum类型
 */
template<typename T>
inline void PutEnum(JsonRoot root, const char *key, const T& val) {
    TC_JsonPutInt(root, key, val);
}

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 字段为uint32_t类型
  */
inline void PutUint(JsonRoot root, const char *key, const uint32_t val) {
    TC_JsonPutInt(root, key, (const int)val);
}

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 字段为uint64_t类型
  */
inline void PutUint64(JsonRoot root, const char *key, const uint64_t val) {
    TC_JsonPutInt64(root, key, (const long long)val);
}

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @tparam T 模板类型，仅限自定义类型
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 字段为自定义类型
  */
template<typename T>
inline void PutObject(JsonRoot root, const char *key, const T& val) {
    TC_JsonPutObject(root, key, MarshalRoot(val));
}

/**
 * @brief 根据传入类型选择对应函数
 * 
 * @tparam T 模板类型
 * @tparam void 枚举类型判断
 */
template<typename T, typename Enable = void>
struct PutF {
    constexpr static auto Fn = PutObject<T>;
};

/**
 * @brief 根据传入类型选择对应的序列化函数
 */
template<typename T>
struct PutF<T, typename std::enable_if<std::is_enum<T>::value>::type>{
    constexpr static auto Fn = PutEnum<T>;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<int> {
    constexpr static auto Fn = TC_JsonPutInt;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<uint32_t> {
    constexpr static auto Fn = PutUint;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<int64> {
    constexpr static auto Fn = TC_JsonPutInt64;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<uint64> {
    constexpr static auto Fn = TC_JsonPutInt64;
};
/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<char *> {
    constexpr static auto Fn = TC_JsonPutString;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<const char *> {
    constexpr static auto Fn = TC_JsonPutString;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<char> {
    constexpr static auto Fn = PutChar;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<bool> {
    constexpr static auto Fn = PutBool;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<tc::Address> {
    constexpr static auto Fn = PutAddress;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<tc::BInt> {
    constexpr static auto Fn = PutBInt;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<tc::Hash> {
    constexpr static auto Fn = PutHash;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<tc::Token> {
    constexpr static auto Fn = PutToken;
};

#ifdef STD_C++

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct PutF<tc::string> {
    constexpr static auto Fn = PutString;
};

/**
template<typename T>
inline void PutVector(void *root, const char *key, const std::vector<T>& val) {
    void *child = TC_JsonNewObject();
    int index = 0;
    auto fn = PutF<std::decay_t<T>>::Fn;
    std::for_each(val.begin(), val.end(), [&child, &index, &fn](auto& item) {
        fn(child, itoa(index), item);
        index = index+1;
    });
    TC_JsonPutObject(root, key, child);
}


template<typename T>
struct PutF<std::vector<T>> {
    constexpr static auto Fn = PutVector<T>;
};

template<typename T>
inline void PutMap(void *root, const char *key, const std::map<tc::string, T>& val) {
    void *child = TC_JsonNewObject();
    auto fn = PutF<std::decay_t<T>>::Fn;
    std::for_each(val.begin(), val.end(), [&child, &fn](auto& item) {
        fn(child, item.first.c_str(), item.second);
    });

    TC_JsonPutObject(root, key, child);
}

template<typename T>
inline void PutCMap(void *root, const char *key, const std::map<const char *, T>& val) {
    void *child = TC_JsonNewObject();
    auto fn = PutF<std::decay_t<T>>::Fn;
    std::for_each(val.begin(), val.end(), [&child, &fn](auto& item) {
        fn(child, item.first, item.second);
    });

    TC_JsonPutObject(root, key, child);
}

template<typename T>
struct PutF<std::map<tc::string, T>> {
    constexpr static auto Fn = PutMap<T>;
};

template<typename T>
struct PutF<std::map<const char *, T>> {
    constexpr static auto Fn = PutCMap<T>;
};
*/
#endif
/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @tparam Args tuple中各元素类型
 * @param root 返回的json句柄
 * @param key  字段的标识符
 * @param val  字段类型为std::tuple类型
 */
template<typename... Args>
inline void PutTuple(JsonRoot root, const char* key, const std::tuple<Args...>& val){
    void *child = TC_JsonNewObject();
    int index = 0;
    boost::fusion::for_each(val, [&](auto& item) {
        auto fn = PutF<std::decay_t<decltype(item)>>::Fn;
        fn(child, itoa(index), item);
        index = index+1;
    });
    TC_JsonPutObject(root, key, child);
}

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<typename... Args>
struct PutF<std::tuple<Args...>> {
    constexpr static auto Fn = PutTuple<Args...>;
};

/**
 * @brief 序列化自定义结构体类型
 * 
 * @tparam T  传入模板的结构体类型
 * @param value  需要序列化的数据
 * @return char*  序列化之后的字符串
 */
template<typename T>
char *Marshal(const T& value) {
    void *root = TC_JsonNewObject();
    ForEachField(std::forward<decltype(value)>(value), [&](auto&& field, auto&& name) {
        auto fn = PutF<std::decay_t<decltype(field)>>::Fn;
        fn(root, name, field);
    });
    return TC_JsonToString(root);
}

/**
 * @brief 序列化std::tuple结构体类型
 * 
 * @tparam Args tuple中各元素类型
 * @param value  需要序列化的数据
 * @return char*  序列化之后的字符串
 */
template<typename... Args>
char *Marshal(const std::tuple<Args...>& value) {
    void *root = TC_JsonNewObject();
    int index = 0;
    boost::fusion::for_each(value, [&](auto& item) {
        auto fn = PutF<std::decay_t<decltype(item)>>::Fn;
        fn(root, itoa(index), item);
        index = index+1;
    });

    return TC_JsonToString(root);
}

/**
 * @brief 序列化std::tuple结构体类型
 * 
 * @tparam Args tuple中各元素类型
 * @param value  需要序列化的数据
 * @return char*  序列化之后的字符串
 */
template<typename... Args>
char *Marshal(std::tuple<Args...>& value) {
    void *root = TC_JsonNewObject();
    int index = 0;
    boost::fusion::for_each(value, [&](auto& item) {
        auto fn = PutF<std::decay_t<decltype(item)>>::Fn;
        fn(root, itoa(index), item);
        index = index+1;
    });

    return TC_JsonToString(root);
}
template<typename T>
void* MarshalRoot(const T& value) {
    void *root = TC_JsonNewObject();
    ForEachField(std::forward<decltype(value)>(value), [&](auto&& field, auto&& name) {
        auto fn = PutF<std::decay_t<decltype(field)>>::Fn;
        fn(root, name, field);
    });
    return root;
}
template<typename... Args>
void *MarshalRoot(const std::tuple<Args...>& value) {
    void *root = TC_JsonNewObject();
    int index = 0;
    boost::fusion::for_each(value, [&](auto& item) {
        auto fn = PutF<std::decay_t<decltype(item)>>::Fn;
        fn(root, itoa(index), item);
        index = index+1;
    });

    return root;
}
template<typename... Args>
void* MarshalRoot(std::tuple<Args...>& value) {
    void *root = TC_JsonNewObject();
    int index = 0;
    boost::fusion::for_each(value, [&](auto& item) {
        auto fn = PutF<std::decay_t<decltype(item)>>::Fn;
        fn(root, itoa(index), item);
        index = index+1;
    });

    return root;
}


/**
 * @brief 序列化任意类型,并存入json对象中
 * 
 * @tparam T 传入模板的类型
 * @param root json对象句柄
 * @param key 字段的标识符
 * @param value 需要序列化的数据
 */
template<typename T>
void Marshal(JsonRoot root, const char *key, const T& value) {
    auto fn = PutF<std::decay_t<decltype(value)>>::Fn;
    fn(root, key, value);
}

/**
 * @brief 序列化任意类型,并存入json对象中
 * 
 * @tparam T 传入模板的类型
 * @param root json对象句柄
 * @param key 字段的标识符
 * @param value 需要序列化的数据
 */
template<typename T>
void Marshal(JsonRoot root, const tc::string& key, const T& value) {
    auto fn = PutF<std::decay_t<decltype(value)>>::Fn;
    fn(root, key.c_str(), value);
}

#define INVALID_RET ((int)(-1))

/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 接收字段对应的数据,类型为int
 * @return int 返回-1表示，key对应的字段为空
 * @warning GetInt/GetInt64 无法依据ret = -1, 判断是否为空
 */
inline int GetInt(JsonRoot root, const char *key, int& val) {
    val = TC_JsonGetInt(root, key);
    return val;
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root  json对象句柄
 * @param key  字段的标识符
 * @param val 接收字段对应的数据,类型为uint32_t类型
 * @return int 
 * @warning 字段对应的值为空时，val=0
 */
inline int GetUint(JsonRoot root, const char *key, uint32_t& val) {
    int i = TC_JsonGetInt(root, key);
    val = (uint32_t)i;
    return i;
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 接收字段对应的数据,类型为int64
 * @return int 返回-1表示，key对应的字段为空
 * @warning GetInt/GetInt64 无法依据ret = -1, 判断是否为空
 */
inline int GetInt64(JsonRoot root, const char *key, int64& val) {
    val = TC_JsonGetInt64(root, key);
    return (int)val;
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 接收字段对应的数据,类型为int64
 * @return int 返回-1表示，key对应的字段为空
 * @warning GetInt/GetInt64 无法依据ret = -1, 判断是否为空
 */
inline int GetUint64(JsonRoot root, const char *key, uint64& val) {
    val = (uint64)TC_JsonGetInt64(root, key);
    return (int)val;
}
#ifdef STD_C++

/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 接收字段对应的数据,类型为tc::string
 * @return int 返回-1表示，key对应的字段为空
 */
inline int GetString(JsonRoot root, const char *key, tc::string& val) {
    char *s = TC_JsonGetString(root, key);
    if (s == NULL)
        return INVALID_RET;
    val.assign(s);
    return 0;
}
#endif

/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 接收字段对应的数据,类型为char*
 * @return int 返回-1表示，key对应的字段为空
 */
inline int GetCString(JsonRoot root, const char *key, char *& val) {
    char *s = TC_JsonGetString(root, key);
    if (s == NULL)
        return INVALID_RET;
    val = s;
    return 0;
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 接收字段对应的数据,类型为const char*
 * @return int 返回-1表示，key对应的字段为空
 */
inline int GetCCString(JsonRoot root, const char *key, const char *& val) {
    char *s = TC_JsonGetString(root, key);
    if (s == NULL)
        return INVALID_RET;
    val = s;
    return 0;
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 接收字段对应的数据,类型为bool
 * @return int 返回-1表示，key对应的字段为空
 * @warning 如果key对应的字段为空，默认为false
 */
inline int GetBool(JsonRoot root, const char* key, bool& val){
    int i = TC_JsonGetInt(root, key);
    if (i==1){
        val = true;
    }else if (i==0){
        val = false;
    }else{
        TC_RequireWithMsg(false, "Bool Type Store Error");
        return INVALID_RET;
    }
    return i;
}

/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 接收字段对应的数据,类型为Address
 * @return int 返回-1表示，key对应的字段为空
 */
inline int GetAddress(JsonRoot root, const char *key, tc::Address& val) {
    val.set((char *)TC_JsonGetAddress(root, key));
    return 0;
}
/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 接收字段对应的数据,类型为BInt
 * @return int 返回-1表示，key对应的字段为空
 */
inline int GetBInt(JsonRoot root, const char *key, tc::BInt& val) {
    val.set((char *)TC_JsonGetBigInt(root, key));
    return 0;
}


/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 接收字段对应的数据,类型为Hash
 * @return int 返回-1表示，key对应的字段为空
 */
inline int GetHash(JsonRoot root, const char *key, tc::Hash& val) {
    val.set((char *)TC_JsonGetString(root, key));
    return 0;
}
/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @param root json对象的句柄
 * @param key 字段的标识符
 * @param val 接收字段对应的数据,类型为Token
 * @return int 返回-1表示，key对应的字段为空
 */
inline int GetToken(JsonRoot root, const char *key, tc::Token& val) {
    void *child = TC_JsonGetObject(root, key);
    if (child == NULL)
        return INVALID_RET;
    val = tc::Token(std::move(tc::TokenID((char *)TC_JsonGetAddress(child, "id"), false)), std::move(tc::BInt((char *)TC_JsonGetBigInt(child, "val"), false)));
    return 0;
}

/**
 * @brief Unmarshal函数模板声明
 * 
 * @tparam T  需要序列化的元素类型
 * @param value 元素值
 * @return char* json序列化之后的字符串
 */
template<typename T>
int Unmarshal(const char *data, T& value);

template<typename T>
int Unmarshal(const std::string& data, T& value);

template<typename T>
int Unmarshal(void* root, const char *key, T& value);


/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @tparam T 枚举类型
 * @param root  json对象句柄
 * @param key  字段的标识符
 * @param val 接收字段对应的数据,类型为枚举类型
 * @return int 
 * @warning 字段对应的值为空时，val=0
 */
template<typename T >
inline int GetEnum(JsonRoot root, const char *key, T& val) {
    int i = TC_JsonGetInt(root, key);
    val = (T)i;
    return i;
}



/**
 * @brief 在root指向的json对象中，获取指定key字段的值
 * 
 * @tparam T 模板类型，为自定义类型调用
 * 
 * @param root  json对象句柄
 * @param key  字段的标识符
 * @param val 接收字段对应的数据,类型为T
 * @return int 返回-1表示，key对应的字段为空
 */
//TODO:FIX '{' '}' missing!!!
template<typename T>
inline int GetObject(JsonRoot root, const char *key, T& val) {
    char* data = TC_JsonGetString(root, key);
    char* tmp = (char*)calloc(1, strlen(data)+2+1);
    strcpy(tmp+1, data);
    *tmp = '{';
    tmp[strlen(tmp)] = '}';
    //TC_Prints(tmp);
    int ret = Unmarshal(tmp, val); 
    free(tmp);
    return ret;
}

/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<typename T, typename Enable = void>
struct GetF {
    constexpr static auto Fn = GetObject<T>;
};

/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<typename T>
struct GetF<T, typename std::enable_if<std::is_enum<T>::value>::type>{
    constexpr static auto Fn = GetEnum<T>;
};

/**
 * @brief 根据传入类型选择对应的json序列化函数
 */
template<>
struct GetF<int> {
    constexpr static auto Fn = GetInt;
};

/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<>
struct GetF<uint32_t> {
    constexpr static auto Fn = GetUint;
};


/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<>
struct GetF<int64> {
    constexpr static auto Fn = GetInt64;
};

/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<>
struct GetF<uint64> {
    constexpr static auto Fn = GetUint64;
};
/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<>
struct GetF<char *> {
    constexpr static auto Fn = GetCString;
};

/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<>
struct GetF<const char *> {
    constexpr static auto Fn = GetCCString;
};

/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<>
struct GetF<bool> {
    constexpr static auto Fn = GetBool;
};

/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<>
struct GetF<tc::Address> {
    constexpr static auto Fn = GetAddress;
};

/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<>
struct GetF<tc::BInt> {
    constexpr static auto Fn = GetBInt;
};

/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<>
struct GetF<tc::Hash> {
    constexpr static auto Fn = GetHash;
};

/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<>
struct GetF<tc::Token> {
    constexpr static auto Fn = GetToken;
};

#ifdef STD_C++

/**
 * @brief 根据传入类型选择对应的json反序列化函数
 */
template<>
struct GetF<tc::string> {
    constexpr static auto Fn = GetString;
};

/*
template<typename T>
inline int GetVector(void *root, const char *key, std::vector<T>& val) {
    void *child = TC_JsonGetObject(root, key);
    int index = 0;
    auto fn = GetF<std::decay_t<T>>::Fn;
    int ret = INVALID_RET;
    T t;
    while(1) {
        ret = fn(child, itoa(index), t);
        if (ret == INVALID_RET)
            return ret;

        val.append(t);
        index++;
    }
    return 0;
}

template<typename T>
struct GetF<std::vector<T>> {
    constexpr static auto Fn = GetVector<T>;
};
*/
#endif

/**
 * @brief 反序列化json字符串data为T类型,并存入value中
 * 
 * @tparam T 传入模板的类型
 * @param data json字符串
 * @return int 如果json字符串中value为空,返回-1
 */
template<typename T>
int Unmarshal(const char *data, T& value) {
    void *root = TC_JsonParse(data);
    bool err =false;
    ForEachField(std::forward<decltype(value)>(value), [&](auto&& field, auto&& name) {
        if (err) return;
        auto fn = GetF<std::decay_t<decltype(field)>>::Fn;
        int ret = fn(root, name, field);
        if (ret == INVALID_RET)//field type is not int/int64/enum
            err= true;
    });
    if (err)
        return INVALID_RET;
    return 0;
}
  
/**
 * @brief 反序列化json字符串data为tuple,并存入value中
 * 
 * @tparam Args tuple中各元素类型
 * @param data json字符串
 * @param value  反序列化后的tuple对象
 * @return int 如果json字符串中value为空,返回-1
 */
template<typename... Args>
int Unmarshal(const char* data, std::tuple<Args...>& value) {
    void *root = TC_JsonParse(data);
    int index = 0;
    bool err = false;

    boost::fusion::for_each(value, [&](auto& item) {
        if (err) return;
        auto fn = GetF<std::decay_t<decltype(item)>>::Fn;
        int ret = fn(root, itoa(index), item);
        if (ret == INVALID_RET){
            TC_Prints("INVALID_RET:TUPLE");
            err = true;
        }
        index = index+1;
    });

    if (err){
        return INVALID_RET;
    }
    return 0;
}

/**
 * @brief 反序列化json字符串data为T类型,并存入value中
 * 
 * @tparam T 传入模板的类型
 * @param data json字符串
 * @param value  反序列化后的对象
 * @return int 如果json字符串中value为空,返回-1
 */
template<typename T>
int Unmarshal(char *data, T& value) {
    return Unmarshal((const char *)data, value);
}

/**
 * @brief 反序列化json字符串data为T类型,并存入value中
 * 
 * @tparam T 传入模板的类型
 * @param data json字符串
 * @param value  反序列化后的对象
 * @return int 如果json字符串中value为空,返回-1
 */
template<typename T>
int Unmarshal(const std::string& data, T& value) {
    return Unmarshal(data.c_str(), value);
}
/**
 * @brief 反序列化json对象,并存入value
 * 
 * @tparam T 传入模板的类型
 * @param root json对象句柄
 * @param key 字段的标识符
 * @param value  反序列化后的对象
 * @return int 如果json字符串中value为空,返回-1
 */ 
template<typename T>
int Unmarshal(JsonRoot root, const char *key, T& value) {
    auto fn = GetF<std::decay_t<decltype(value)>>::Fn;
    int ret = fn(root, key, value);
    return ret;
}

/**
 * @brief 反序列化json对象,并存入value
 * 
 * @tparam T 传入模板的类型
 * @param root json对象句柄
 * @param key 字段的标识符
 * @param value  反序列化后的对象
 * @return int 如果json字符串中value为空,返回-1
 */
template<typename T>
int Unmarshal(JsonRoot root, const tc::string& key, T& value) {
    auto fn = GetF<std::decay_t<decltype(value)>>::Fn;
    int ret = fn(root, key.c_str(), value);
    return ret;
}
//@}
} // namespace json
}//namespace tc

#endif
