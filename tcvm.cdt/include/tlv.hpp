#ifndef _TLV_HPP_
#define _TLV_HPP_

#include <cstring>
#include <cstdlib>
#include <cassert>

#include <map>
#include <vector>
#include <tuple>
#include <set>
#include <list>
#include "reflect.hpp"
#include "types.hpp"
#include "tcstring.hpp"

typedef uint32_t uint;

namespace tc{
namespace tlv {
/**
 * @defgroup tlv TLV API
 * @ingroup serial
 * @brief C++ 序列化库，使用TLV(type-length-value)方式序列化
 * @{
 */ 
class BufferWriter {
private:
    int mCap;
    int mLen;
    uint8_t* mData;
    
private:
    #define grow(size) \
        if ((mLen + size) > mCap) { _grow(size); }
        
     void _grow(uint size) {
        int cap = mLen + size + 1;
        cap = cap + (cap % 256);
        uint8_t* tmp = (uint8_t *)realloc(mData, cap);
        if (tmp == NULL) // @Note: try again.
            tmp = (uint8_t*)calloc(1, cap);
        assert(tmp != NULL); 
        
        if (mData != tmp) {
            memcpy(tmp, mData, mLen);
            free(mData);
            mData = tmp;
        }
        mCap = cap;
    }

public:
/**
 * @brief Construct a new Buffer Writer object
 * 
 * @param cap 
 */
    BufferWriter(int cap = 1024) : mCap(cap), mLen(0) {
        mData = (uint8_t*)calloc(1, cap);
    }
    /**
     * @brief Destroy the Buffer Writer object
     * 
     */
    ~BufferWriter() {
        if (mData != NULL) {
            free(mData);
            mData = NULL;
        }
    }

/**
 * @brief Construct a new Buffer Writer object
 * 
 * @param rhs 
 */
    BufferWriter(BufferWriter&& rhs) {
        mData = rhs.mData;
        mCap = rhs.mCap;
        mLen = rhs.mLen;
        rhs.mData = NULL;
    }

/**
 * @brief 
 * 
 * @param rhs 
 * @return BufferWriter& 
 */
    BufferWriter& operator =(BufferWriter& rhs) {
        if (this != &rhs) {
            mData = rhs.mData;
            mCap = rhs.mCap;
            mLen = rhs.mLen;
            rhs.mData = NULL;
        }
        return *this;
    }

/**
 * @brief 
 * 
 * @return const uint8_t* 
 */
    inline const uint8_t* bytes() const { return mData; }

/**
 * @brief 
 * 
 * @return const int 
 */
    inline const int length() const { return mLen; }

/**
 * @brief 
 * 
 * @param n 
 * @param len 
 */
    inline void write(const uint64_t& n, const int len) {
        grow(len);
        for (int i = 0; i < len; i++) {
            mData[mLen+i] = ((n >> (i*8)) & 0xff);
        }
        mLen += len;
    }

    inline void write(const char* s, int len = 0) {
        if (len == 0)
            len = strlen(s) + 1;
        grow(len);
        memcpy(mData+mLen, s, len);
        mLen += len;
    }

    inline void write(const tc::string& s) {
        write((const char *)s.c_str(), s.length()+1);
    }
};
    
class BufferReader {
private:
    int mLen;
    const uint8_t* mData;
    
public:

/**
 * @brief Construct a new Buffer Reader object
 * 
 * @param buf 
 */
    BufferReader(const uint8_t* buf) : mLen(0), mData(buf) {}

/**
 * @brief 
 * 
 * @return const int 
 */
    inline const int offset() const { return mLen; }

/**
 * @brief 
 * 
 * @return const uint8_t* 
 */
    inline const uint8_t* bytes() const { return mData; }
    
    /**
     * @brief 
     * 
     * @param len 
     * @return uint64_t 
     */
    inline uint64_t read(int len) {
        uint64_t n = 0;
        for (int i = len-1; i >= 0; i--) {
            n = (n << 8) + mData[mLen+i];
        }
        mLen += len;
        return n;
    }

/**
 * @brief 
 * 
 * @param p 
 * @param len 
 */
    inline void read(char*&p, int len) {
        p = (char*)(mData+mLen);
        mLen += len;
    }

/**
 * @brief 
 * 
 * @param s 
 * @param len 
 */
    inline void read(tc::string& s, int len) {
        s.assign((char *)(mData+mLen));
        mLen += len;
    }
};

const uint8_t TagUnknow = 0;///<
const uint8_t TagU8 = 1;///<
const uint8_t TagU16 = 2;///<
const uint8_t TagU32 = 3;///<
const uint8_t TagU64 = 4;///<
//const uint8_t TagChar = 5;///<
const uint8_t TagString = 6;///<
const uint8_t TagArray = 7;///<
const uint8_t TagTuple = 8;///<
const uint8_t TagMapBegin = 9;///<
const uint8_t TagMapEnd = 10;///<
const uint8_t TagMapItemKey = 11;///<
const uint8_t TagMapItemVal = 12;///<
const uint8_t TagObjectBegin = 13;///<
const uint8_t TagObjectEnd = 14;///<

/**
 * @brief 
 * 
 * @param buf 
 * @return uint8_t 
 */
inline uint8_t readTag(BufferReader& buf) {
    return (uint8_t)buf.read(sizeof(TagUnknow));
}

// forward declation
/**
 * @brief 
 * 
 * @tparam T 
 * @param val 
 * @param buf 
 * @return BufferWriter 
 */
template<typename T>
BufferWriter Marshal(const T& val, BufferWriter&& buf = BufferWriter());

/**
 * @brief 
 * 
 * @tparam T 
 * @param buf 
 * @param val 
 */
template<typename T>
void Unmarshal(BufferReader& buf, T& val);

// generic
/**
 * @brief 
 * 
 * @tparam T 
 * @param buf 
 * @param val 
 */
template<typename T>
void pack(BufferWriter& buf, const T& val) {
    BufferWriter tmp = Marshal(val, std::move(buf));
    buf = tmp;
}

/**
 * @brief 
 * 
 * @tparam T 
 * @param buf 
 * @param val 
 */
template<typename T>
void unpack(BufferReader& buf, T& val) {
    Unmarshal(buf, val);
}

// uint8
/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
void pack(BufferWriter& buf, const uint8_t& val) {
    buf.write(TagU8, sizeof(TagU8));
    buf.write(val, sizeof(val));
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
inline void pack(BufferWriter& buf, const int8_t& val) {
    pack(buf, (const uint8_t&)val);
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
void pack(BufferWriter& buf, const bool& val) {
    buf.write(TagU8, sizeof(TagU8));
    buf.write(val, sizeof(val));
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
void unpack(BufferReader& buf, bool& val) {
    assert(readTag(buf) == TagU8);
    val = buf.read(sizeof(val));
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
void unpack(BufferReader& buf, uint8_t& val) {
    assert(readTag(buf) == TagU8);
    val = buf.read(sizeof(val));
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
inline void unpack(BufferReader& buf, int8_t& val) {
    unpack(buf, (uint8_t&)val);
}

/**
 * @brief 
 * 
 * @param buf 
 * @param val 
 * @return template<> pack 
 */
// uint16
template<>
void pack(BufferWriter& buf, const uint16_t& val) {
    buf.write(TagU16, sizeof(TagU16));
    buf.write(val, sizeof(val));
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
inline void pack(BufferWriter& buf, const int16_t& val) {
    pack(buf, (const uint16_t&)val);
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
void unpack(BufferReader& buf, uint16_t& val) {
    assert(readTag(buf) == TagU16);
    val = buf.read(sizeof(val));
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
inline void unpack(BufferReader& buf, int16_t& val) {
    unpack(buf, (uint16_t&)val);
}

// uint32
/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
void pack(BufferWriter& buf, const uint& val) {
    buf.write(TagU32, sizeof(TagU32));
    buf.write(val, sizeof(val));
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
inline void pack(BufferWriter& buf, const int& val) {
    pack(buf, (const uint&)val);
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
void unpack(BufferReader& buf, uint& val) {
    assert(readTag(buf) == TagU32);
    val = buf.read(sizeof(val));
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
inline void unpack(BufferReader& buf, int& val) {
    unpack(buf, (uint&)val);
}

// uint64
/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
void pack(BufferWriter& buf, const uint64_t& val) {
    buf.write(TagU64, sizeof(TagU64));
    buf.write(val, sizeof(val));
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
inline void pack(BufferWriter& buf, const int64_t& val) {
    pack(buf, (const uint64_t&)val);
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
void unpack(BufferReader& buf, uint64_t& val) {
    assert(readTag(buf) == TagU64);
    val = buf.read(sizeof(val));
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
inline void unpack(BufferReader& buf, int64_t& val) {
    unpack(buf, (uint64_t&)val);
}


// c string
/**
 * @brief 
 * 
 * @param buf 
 * @param str 
 */
void packCString(BufferWriter& buf, const char *&str) {
    buf.write(TagString, sizeof(TagString));
    uint16_t len = strlen(str)+1;
    buf.write(len, sizeof(len));
    buf.write(str, len);
}

/**
 * @brief 
 * 
 * @param buf 
 * @param ptr 
 */
void unpackCString(BufferReader& buf, char *&ptr) {
    assert(readTag(buf) == TagString);
    uint16_t len = buf.read(sizeof(uint16_t));
	char* tmp;
    buf.read(tmp, len);

	ptr = (char*)calloc(1, len);
	strcpy(ptr, tmp);

}

// string
/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param s 
 */
template<>
void pack(BufferWriter& buf, const tc::string& s) {
    const char *str = s.c_str();
    packCString(buf, str);
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param s 
 */
template<>
void unpack(BufferReader& buf, tc::string& s) {
    char *ptr;
    unpackCString(buf, ptr);
    s.assign(ptr);
}


// tc::Address
/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param addr 
 */
template<>
inline void pack(BufferWriter& buf, const tc::Address& addr) {
    const char *str = addr.toString();
    packCString(buf, str);
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param addr 
 */
template<>
inline void unpack(BufferReader& buf, tc::Address& addr) {
    char *ptr;
    unpackCString(buf, ptr);
    addr.set(ptr, false);
}

// tc::BInt
/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
inline void pack(BufferWriter& buf, const tc::BInt& val) {
    const char *str = val.toString();
    packCString(buf, str);
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param val 
 */
template<>
inline void unpack(BufferReader& buf, tc::BInt& val) {
    char *ptr;
    unpackCString(buf, ptr);
    val = tc::BInt((const BigInt)ptr, true);
}

// tc::Token
/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param token 
 */
template<>
inline void pack(BufferWriter& buf, const tc::Token& token) {
    pack(buf, token.id());
    pack(buf, token.value());
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param token 
 */
template<>
inline void unpack(BufferReader& buf, tc::Token& token) {
    tc::TokenID id;
    tc::BInt value;
    unpack(buf, id);
    unpack(buf, value);
    token = tc::Token(id, value);
}

// tc::Hash
/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param hash 
 */
template<>
inline void pack(BufferWriter& buf, const tc::Hash& hash) {
    const char *str = hash.toString();
    packCString(buf, str);
}

/**
 * @brief 
 * 
 * @tparam  
 * @param buf 
 * @param hash 
 */
template<>
inline void unpack(BufferReader& buf, tc::Hash& hash) {
    char *ptr;
    unpackCString(buf, ptr);
    hash.set(ptr, false);
}

// vector
/**
 * @brief 
 * 
 * @tparam T 
 * @param buf 
 * @param val 
 */
template<typename T>
void pack(BufferWriter& buf, const std::vector<T>& val) {
    buf.write(TagArray, sizeof(TagArray));
    uint16_t size = val.size();
    buf.write(size, sizeof(size));
    for (auto const &n : val) {
        pack(buf, n);
    }
}

/**
 * @brief 
 * 
 * @tparam T 
 * @param buf 
 * @param val 
 */
template<typename T>
void unpack(BufferReader& buf, std::vector<T>& val) {
    assert(readTag(buf) == TagArray);
    uint16_t size = buf.read(sizeof(uint16_t));
    for (int i = 0; i < size; i++) {
        T item;
        unpack(buf, item);
        val.push_back(item);
    }
}

// Set
/**
 * @brief 
 * 
 * @tparam T 
 * @param buf 
 * @param val 
 */
template<typename T>
void pack(BufferWriter& buf, const std::set<T>& val) {
    buf.write(TagArray, sizeof(TagArray));
    uint16_t size = val.size();
    buf.write(size, sizeof(size));
    for (auto const &n : val) {
        pack(buf, n);
    }
}

/**
 * @brief 
 * 
 * @tparam T 
 * @param buf 
 * @param val 
 */
template<typename T>
void unpack(BufferReader& buf, std::set<T>& val) {
    assert(readTag(buf) == TagArray);
    uint16_t size = buf.read(sizeof(uint16_t));
    for (int i = 0; i < size; i++) {
        T item;
        unpack(buf, item);
        val.insert(item);
    }
}
// list
/**
 * @brief 
 * 
 * @tparam T 
 * @param buf 
 * @param val 
 */
template<typename T>
void pack(BufferWriter& buf, const std::list<T>& val) {
    buf.write(TagArray, sizeof(TagArray));
    uint16_t size = val.size();
    buf.write(size, sizeof(size));
    for (auto const &n : val) {
        pack(buf, n);
    }
}

/**
 * @brief 
 * 
 * @tparam T 
 * @param buf 
 * @param val 
 */
template<typename T>
void unpack(BufferReader& buf, std::list<T>& val) {
    assert(readTag(buf) == TagArray);
    uint16_t size = buf.read(sizeof(uint16_t));
    for (int i = 0; i < size; i++) {
        T item;
        unpack(buf, item);
        val.push_back(item);
    }
}
// map
/**
 * @brief 
 * 
 * @tparam K 
 * @tparam V 
 * @param buf 
 * @param val 
 */
template<typename K, typename V>
void pack(BufferWriter& buf, const std::map<K, V>& val) {
    buf.write(TagMapBegin, sizeof(TagMapBegin));
    for (auto& [key, value] : val) {
        buf.write(TagMapItemKey, sizeof(TagMapItemKey));
        pack(buf, key);
        buf.write(TagMapItemVal, sizeof(TagMapItemVal));
        pack(buf, value);
    }
    buf.write(TagMapEnd, sizeof(TagMapEnd));
}

/**
 * @brief 
 * 
 * @tparam K 
 * @tparam V 
 * @param buf 
 * @param val 
 */
template<typename K, typename V>
void unpack(BufferReader& buf, std::map<K, V>& val) {
    assert(readTag(buf) == TagMapBegin);
    uint8_t tag = TagUnknow;
    K itemKey;
    V itemVal;
    while(true) {
        tag = readTag(buf);
        if (tag == TagMapEnd)
            return;
        assert(tag == TagMapItemKey);
        
        unpack(buf, itemKey);
        tag = readTag(buf);
        assert(tag == TagMapItemVal);
        unpack(buf, itemVal);
        val[itemKey] = itemVal;
    }
}

// tuple
/**
 * @brief 
 * 
 * @tparam Args 
 * @param buf 
 * @param val 
 */
template<typename... Args>
void pack(BufferWriter& buf, const std::tuple<Args...>& val) {
    buf.write(TagTuple, sizeof(TagTuple));
    reflect::ForEachTuple(val, [&buf](const auto& item) {
       pack(buf, item); 
    });
}

/**
 * @brief 
 * 
 * @tparam Args 
 * @param buf 
 * @param val 
 */
template<typename... Args>
void unpack(BufferReader& buf, std::tuple<Args...>& val) {
    assert(readTag(buf) == TagTuple);
    reflect::ForEachTuple(val, [&buf](auto& item) {
        unpack(buf, item);
    });
}

// Marshal interface.
/**
 * @brief 
 * 
 * @tparam Args 
 * @param val 
 * @param buf 
 * @return BufferWriter 
 */
template<typename... Args>
inline BufferWriter Marshal(const std::tuple<Args...>& val, BufferWriter&& buf = BufferWriter()) {
    pack(buf, val);
    return std::move(buf);
}

/**
 * @brief 
 * 
 * @tparam T 
 * @param val 
 * @param buf 
 * @return BufferWriter 
 */
template<typename T>
BufferWriter Marshal(const T& val, BufferWriter&& buf) {
    buf.write(TagObjectBegin, sizeof(TagObjectBegin));
    ForEachField(std::forward<decltype(val)>(val), [&buf](auto&& field, auto&& name) {
       pack(buf, field); 
    });
    buf.write(TagObjectEnd, sizeof(TagObjectEnd));
    return std::move(buf);
}

// Unmarshal interface.
/**
 * @brief 
 * 
 * @tparam Args 
 * @param buf 
 * @param val 
 */
template<typename... Args>
inline void Unmarshal(BufferReader& buf, std::tuple<Args...>& val) {
    unpack(buf, val);
} 

/**
 * @brief 
 * 
 * @tparam T 
 * @param buf 
 * @param val 
 */
template<typename T>
void Unmarshal(BufferReader& buf, T& val) {
    assert(readTag(buf) == TagObjectBegin);
    ForEachField(std::forward<decltype(val)>(val), [&buf](auto&& field, auto&& name) {
        unpack(buf, field);
    });
    assert(readTag(buf) == TagObjectEnd);
}
//@}

} // namespace tlv
} // namespace tc

#endif // _TLV_HPP_
