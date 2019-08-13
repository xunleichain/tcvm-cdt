#ifndef _TYPES_H_
#define _TYPES_H_

#include "tcstring.hpp"
#include <tuple>
#include <type_traits>
#include <memory>
#include "tcapi.h"

namespace tc {

class CPtr {
private:
    char *mPtr;
    bool mIsConst;
public:
    CPtr(const char *p, bool isConst = true) : mPtr((char *)p), mIsConst(isConst) {}
    CPtr(const CPtr& o) = delete;
    CPtr& operator =(const CPtr& rhs) = delete;
    ~CPtr() {
        if ((!mIsConst) && (mPtr != NULL)) {
            free(mPtr);
            mPtr = NULL;
        }
    }
    inline void set(char *p, bool isConst = false) {
        if ((!mIsConst) && (mPtr != NULL))
            free(mPtr);
        mPtr = p;
        mIsConst = isConst;
    }
    inline const char* data() const { return mPtr; }
    bool operator ==(const CPtr& rhs) { return (strcmp(mPtr, rhs.mPtr) == 0); }
};

using SharedCPtr = std::shared_ptr<CPtr>;
/**
 * @defgroup types Types
 * @brief C++代码封装的类型，用来标识大整数、地址、Token等
 * @ingroup cppapi
 * @{
 */
 
/**
 * @brief 区块链地址类
 */
class Address {
private:
    SharedCPtr mAddr;
    void copy(const Address& o) {
        char* tmp = (char*)calloc(1, strlen(o.toString())+1);
        strcpy(tmp, o.toString());
        mAddr->set(tmp);
    }

public:
    /**
     * @brief Construct a new Address object
     * 
     * @param addr 16进制地址
     * @param isConst 是否是常量，默认是常量，不用传参处理
     */
    Address(const char *addr, bool isConst = true) {
        if (addr == NULL || *addr == '\0'){
            mAddr = SharedCPtr(new CPtr(ZERO_ADDRESS)); 
        }else{
            mAddr = SharedCPtr(new CPtr(addr, isConst)); 
        }
    }
        
    /**
     * @brief Construct a new Address object
     * 
     */
    Address() : mAddr(SharedCPtr(new CPtr(ZERO_ADDRESS))) {}
    Address(const Address& o) {
        char* tmp = (char*)calloc(1, strlen(o.toString())+1);
        strcpy(tmp, o.toString());
		mAddr = SharedCPtr(new CPtr(tmp,false));
    }
    /**
     * @brief 赋值拷贝函数
     * 
     * @param o 赋值拷贝对象
     * @return Address& 赋值拷贝返回引用
     */
    Address& operator=(const Address& o) {
        if (&o != this)
            this->copy(o);
        return *this;
    }

    /**
     * @brief 设置地址
     * 
     * @param addr 16进制地址
     * @param isConst 是否是常量
     */
    inline void set(char *addr, bool isConst = false) { mAddr->set(addr, isConst); }
    /**
     * @brief 返回16进制字符串地址
     * 
     * @return const char* 字符串地址
     */
    inline const char* toString() const { return mAddr->data(); }
    /**
     * @brief 字符串是否是地址类型
     * 
     * @return true 字符串是地址类型
     * @return false 字符串不是地址类型
     */
    inline bool isHex() const { return TC_IsHexAddress(mAddr->data()); }
    /**
     * @brief 比较地址是否相等
     * 
     * @param rhs 比较值
     * @return true 相等
     * @return false 不相等
     */
    bool operator ==(const Address& rhs) const {
        if (&rhs != this)
            return (*mAddr == *rhs.mAddr);
        return true;
    }
    /**
     * @brief 比较地址是否相等
     * 
     * @param rhs 比较值
     * @return false 相等
     * @return true 不相等
     */
    bool operator !=(const Address& rhs) const {
        return !(*this == rhs);
    }

    /**
     * @brief 查看地址是否为0地址
     * 
     * @return true 是0地址
     * @return false 不是0地址
     */
    inline bool isZero() {
        return strcmp(mAddr->data(), ZERO_ADDRESS) == 0;
    }

	bool operator <(const tc::Address& b){
		return strcmp(this->toString(), b.toString()) < 0;
	}
};

/**
 * @brief 比较地址大小，字典序排列
 * 
 * @param rhs 比较值
 * @return true 相等
 * @return false 不相等
 */
 
bool operator <(const tc::Address& a, const tc::Address& b){
	return strcmp(a.toString(), b.toString()) < 0;
}

class BInt;
class App;

using TokenID = Address;
/**
 * @brief 大整数类
 */
class BInt {
    private:
        bool mIsMalloc;
        char* mData;
        void copy(const BInt& o) {
            this->mData = (BigInt)calloc(1, strlen(o.mData)+1);
            strcpy((char *)this->mData, o.mData);
            this->mIsMalloc = true;
        }
		
    public:
        /**
         * @brief Construct a new BInt object
         * 
         */
        BInt() : mData(BIGINT_0), mIsMalloc(false) {}
        /**
         * @brief Construct a new BInt object
         * 
         * @param data BInt字符串数据
         * @param isMalloc 是否是栈上分配 不用传参处理
         */
        BInt(const char* data, bool isMalloc=false) : mData((char*)data), mIsMalloc(isMalloc) {
            if (data == NULL || *data == '\0'){
               mData = "0"; 
               mIsMalloc = false; 
            }
            
        }
        /**
         * @brief Construct a new BInt object
         * 
         * @param n 32位整型
         */
        BInt(const int n) {
            mData = (BigInt)itoa(n);
            mIsMalloc = true;
        }
        /**
         * @brief Construct a new BInt object
         * 
         * @param n 整型
         */
        BInt(const unsigned int n) {
            mData = (BigInt)i64toa(n, 10);
            mIsMalloc = true;
        }
	    /**
         * @brief Construct a new BInt object
         * 
         * @param n 长整型
         */
        BInt(const unsigned long n) {
            mData = (BigInt)i64toa(n, 10);
            mIsMalloc = true;
        }
	    /**
         * @brief Construct a new BInt object
         * 
         * @param n 长整型
         */
        BInt(const long n) {
            mData = (BigInt)i64toa(n, 10);
            mIsMalloc = true;
        }
        /**
         * @brief Construct a new BInt object
         * 
         * @param n 64位整型
         */
        BInt(const int64 n) {
            mData = (BigInt)i64toa(n, 10);
            mIsMalloc = true;
        }
        /**
         * @brief Construct a new BInt object
         * 
         * @param n 64位整型
         * @warning Pay attention to the number overflow
         */
        BInt(const uint64 n) {
            mData = (BigInt)i64toa((int64)n, 10);
            mIsMalloc = true;
        }
        /**
         * @brief Construct a new BInt object
         * 
         * @param o 构造拷贝
         */
        BInt(const BInt& o) {
            this->copy(o);
        }
        /**
         * @brief Destroy the BInt object
         * 
         */
        ~BInt() {
            if (mIsMalloc) {
                free(mData);
                mIsMalloc = false;
                mData = NULL;
            }
        }
        /**
         * @brief 赋值拷贝函数
         * 
         * @param o 赋值拷贝对象
         * @return BInt& 赋值拷贝返回引用
         */
        BInt& operator=(const BInt& o) {
            if (&o != this)
                this->copy(o);
            return *this;
        }
        /*
         * @brief Construct a new BInt object
         * 
         * @param o 右值拷贝
         */
        BInt(BInt&& o) {
            mData = o.mData;
            mIsMalloc = o.mIsMalloc; 

            o.mData = NULL;
            o.mIsMalloc = false;
        }
        
        /**
         * @brief 设置TOKEN数量
         * 
         * @param n 32位整型
         */
        void setToken(const int n) {
            *this = BInt(n) * BInt(TC_ONETOKEN);
        }

        /**
         * @brief 设置TOKEN数量
         * 
         * @param n 64位整型
         */
        void setToken(const int64 n) {
            *this = BInt(n) * BInt(TC_ONETOKEN);
        }
		
        // @Note: data is created by malloc...
        inline void set(BigInt data, bool ismalloc = true) {
            if ((mIsMalloc) && (mData == NULL))
                free(mData);
            mData = data;
            mIsMalloc = ismalloc;
        }

         /**
         * @brief 大整数置零
         */
        inline void reset() {
            set(BIGINT_0);
            mIsMalloc = false;
        }

        BInt& operator+=(const BInt& rhs) {
            set(TC_BigIntAdd(mData, rhs.mData));
            return *this;
        }
        BInt& operator-=(const BInt& rhs) {
            set(TC_BigIntSub(mData, rhs.mData));
            return *this;
        }
        BInt& operator*=(const BInt& rhs) {
            set(TC_BigIntMul(mData, rhs.mData));
            return *this;
        }
        BInt& operator/=(const BInt& rhs) {
            set(TC_BigIntDiv(mData, rhs.mData));
            return *this;
        }
        BInt& operator%=(const BInt& rhs) {
            set(TC_BigIntMod(mData, rhs.mData));
            return *this;
        }

        BInt operator+(const BInt& rhs) const {
            return BInt(*this)+=rhs;
        }
        BInt operator-(const BInt& rhs) const {
            return BInt(*this)-=rhs;
        }
        BInt operator*(const BInt& rhs) const {
            return BInt(*this)*=rhs;
        }
        BInt operator/(const BInt& rhs) const {
            return BInt(*this)/=rhs;
        }
        BInt operator%(const BInt& rhs) const {
            return BInt(*this)%=rhs;
        }

        /**
         * @brief 比较函数
         * 
         * @param rhs 比较对象
         * @return int 返回值
		 * @retval <0 小于比较对象
         * @retval =0 等于比较对象
		 * @retval >0 大于比较对象
         */
        int Compare(const BInt& rhs) const {
            return TC_BigIntCmp(mData, rhs.mData);
        }
        bool operator <(const BInt& rhs) const {
            return (Compare(rhs) < 0);
        }
        bool operator ==(const BInt& rhs) const {
            return (Compare(rhs) == 0);
        }
        bool operator >(const BInt& rhs) const {
            return (Compare(rhs) > 0);
        }
        bool operator >=(const BInt& rhs) const {
            return (Compare(rhs) >= 0);
        }
        bool operator <=(const BInt& rhs) const {
            return (Compare(rhs) <= 0);
        }
        bool operator !=(const BInt& rhs) const {
            return (Compare(rhs) != 0);
        }
        BInt& operator ++() {
            set(TC_BigIntAdd(mData, BIGINT_1));
            return *this;
        }
        const BInt operator ++(int) {
            BInt tmp = *this;
            set(TC_BigIntAdd(mData, BIGINT_1));
            return tmp;
        }
        BInt& operator --() {
            set(TC_BigIntSub(mData, BIGINT_1));
            return *this;
        }
        const BInt operator --(int) {
            BInt tmp = *this;
            set(TC_BigIntSub(mData, BIGINT_1));
            return tmp;
        }
        int64 toInt64() const {
            return TC_BigIntToInt64(mData);
        }
        /**
         * @brief 返回字符串指针地址
         * 
         * @return const char* 指针地址
         */
        const char *toString() const {
            return mData;
        }

        std::tuple<BInt, BInt> divm(const BInt& rhs) const {
            BInt x(TC_BigIntDiv(mData, rhs.mData), true);
            BInt y(TC_BigIntMod(mData, rhs.mData), true);
            return std::make_tuple(std::move(x), std::move(y));
        }
};

using Wei = BInt;

/**
 * @brief 区块链数据类
 * 
 */
class App {
private:
    App() : 
        mAddr(NULL),
        mCoinbase(NULL),
        mGasLimit(0),
        mHeight(0),
        mData(NULL),
        mGas(0),
        mSender(NULL),
        mSign(NULL),
        mValue(NULL),
        mTokenValue(NULL),
        mNow(0),
        mGasPrice(0),
        mTokenAddress(NULL),
        mTxOrigin(NULL) {}
    App(const App& ) = delete;
    App& operator= (const App&) = delete;
    
private:
    char *mAddr;
    char *mCoinbase;
    int64 mGasLimit;
    uint64 mHeight;
    char *mData;
    uint64 mGas;
    char *mSender;
    char *mSign;
    char *mValue;
    char *mTokenValue;
    uint64 mNow;
    uint64 mGasPrice;
    char *mTxOrigin;
    char *mTokenAddress;

    static App mInstance;

public:
    /**
     * @brief Get the Instance object
     * 
     * @return App* 返回区块数据句柄
     */
    static App* getInstance() {
        return &mInstance;
    }

    /**
     * @brief 获取合约自身地址
     * 
     * @return const Address& 合约自身地址
     */
    const Address address() {
        if (mAddr == NULL)
            mAddr = TC_GetSelfAddress();
        return Address(mAddr);
    }

    /**
     * @brief 获取区块链的coinbase账户地址
     * 
     * @return const Address& 区块链的coinbase账户地址
     */
    const Address coinbase() {
        if (mCoinbase == NULL)
            mCoinbase = TC_GetCoinbase();
        return Address(mCoinbase);
    }

    /**
     * @brief 获取当前区块的gas上限
     * 
     * @return const int64& 当前区块的gas上限
     */
    const int64 gasLimit() {
        if (mGasLimit == 0)
            mGasLimit = TC_GetGasLimit();
        return mGasLimit;
    }

    /**
     * @brief 获取区块链当前区块高度
     * 
     * @return const uint64& 区块链当前区块高度
     */
    const uint64 height() {
        if (mHeight == 0)
            mHeight = TC_GetNumber();
        return mHeight;
    }

    /**
     * @brief 获取合约调用的输入数据
     * 
     * @return const char* 合约输入数据，字符串表示
     */
    const char *data() {
        if (mData == NULL)
            mData = TC_GetMsgData();
        return mData;
    }

    /**
     * @brief 获取合约调用的gas上限
     * 
     * @return const uint64& 合约调用的gas上限
     */
    const uint64 gas() {
        if (mGas == 0)
            mGas = TC_GetMsgGas();
        return mGas;
    }

    /**
     * @brief 获取合约调用者
     * 
     * @return const Address& 合约调用者地址
     */
    const Address sender() {
        if (mSender == NULL)
            mSender = TC_GetMsgSender();
        return Address(mSender);
    }

    /**
     * @brief 获取合约调用参数中的方法名字段
     * 
     * @return const char* 合约调用参数中的方法名字段
     */
    const char *sign() {
        if (mSign == NULL)
            mSign = TC_GetMsgSign();
        return mSign;
    }

    /**
     * @brief 获取合约调用者转账给合约的数额
     * 
     * @return const BInt& 转账给合约的数额
     */
    const BInt value() {
        if (mValue == NULL)
            mValue = TC_GetMsgValue();
        return BInt(mValue);
    }

    /**
     * @brief 获取合约调用者转账给合约的token数额
     * 
     * @return const BInt& 转账给合约的token数额
     */
    const BInt tokenValue() {
        if (mTokenValue == NULL)
            mTokenValue = TC_GetMsgTokenValue();
        return BInt(mTokenValue);
    }

    /**
     * @brief 获取合约当前剩余的gas
     * 
     * @return uint64 合约当前剩余的gas
     */
    uint64 gasLeft() {
        return TC_GasLeft();
    }

    /**
     * @brief 返回当前区块的时间戳，以unix时间表示
     * 
     * @return const uint64& unix时间戳
     */
    const uint64 now() {
        if (mNow == 0)
            mNow = TC_Now();
        return mNow;
    }

    /**
     * @brief 获取合约调用的gas价格
     * 
     * @return const uint64& 合约调用的gas价格
     */
    const uint64 gasPrice() {
        if (mGasPrice == 0)
            mGasPrice = TC_GetTxGasPrice();
        return mGasPrice;
    }

    /**
     * @brief 获取执行的起始地址，原始交易的sender
     * 
     * @return const Address& 区块链的账户地址
     */
    const Address txOrigin() {
        if (mTxOrigin == NULL)
            mTxOrigin = TC_GetTxOrigin();
        return Address(mTxOrigin);
    }

    /**
     * @brief 查询msg中token类型
     * 
     * @return const Address& token类型地址
     */
    const Address tokenAddress() {
        if (mTokenAddress == NULL)
            mTokenAddress = TC_TokenAddress();
        return Address(mTokenAddress);
    }
};

App App::mInstance;


/**
 * @brief token类
 * @warning 地址为零时，默认为初始token
 */
class Token {
private:
    TokenID mID;
    BInt mValue;

public:
    /**
	* @brief 构造token类
	* 
	*/
    Token() : mID(ZERO_ADDRESS) {};
    /**
     * @brief Construct a new Token object
     * 
     * @param value token数量
     */
    Token(const BInt& value) : mID(ZERO_ADDRESS), mValue(value) {}
    /**
     * @brief Construct a new Token object
     * 
     * @param id token类型地址
     * @param value token数量
     */
    Token(const TokenID& id, const BInt& value) : mID(id), mValue(value) {}
    
    /**
     * @brief 返回token地址
     * 
     * @return const TokenID& token地址
     */
    inline const TokenID& id() const {
        return mID;
    }
    /**
     * @brief 返回token数量
     * 
     * @return const BInt& token数量
     */
    inline const BInt value() const {
        return mValue;
    }
    
    /**
     * @brief 判断是否能转账
     * 
     * @return true 能
     * @return false 不能
     */
    bool canTransfer() {
        /*
        address addr = (address)App::getInstance()->address().toString();
        address id = (address)mID.toString();
        BInt tmp(TC_TokenBalance(addr, id), true);
        return (mValue < tmp) && (mValue == tmp);
         */
        BInt totals = balance(App::getInstance()->address(), mID);
        return (mValue <= totals);
        
    }
    /**
     * @brief 给to地址转账
     * 
     * @param to 地址
     */
    inline void transfer(const Address& to) {
        TC_TransferToken((address)to.toString(), (address)mID.toString(), (BigInt)mValue.toString());
    }

    /**
     * @brief 获取某个地址的token余额
     * 
     * @param addr 账户地址
     * @param id token类型地址
     * @return BInt token数量
     */
    static BInt balance(const Address& addr, const TokenID& id = TokenID()) {
        return std::move(BInt(TC_TokenBalance((address)addr.toString(), (address)id.toString()), true));
    }
};

/**
 * @brief 表示一个Tokne
 * 
 * @return BInt 一个token的BInt
 */
inline BInt OneToken() {
    return std::move(BInt(TC_ONETOKEN));
}

/**
 * @brief hash类，用于存储计算结果数据
 * 
 */
class Hash {   
private:
    SharedCPtr mData;

public:
    Hash(const char *data, bool isConst = true) : mData(SharedCPtr(new CPtr(data, isConst))) {}
    Hash() : mData(SharedCPtr(new CPtr(ZERO_ADDRESS, true))) {}
    /**
     * @brief 获取字符串
     * 
     * @return const char* 字符串首地址
     */
    inline const char* toString() const { return mData->data(); }
    /**
     * @brief 设置hash类数据
     * 
     * @param data 数据
     * @param isConst 是否是常量，默认是常量 不用传参处理
     */
    inline void set(char *data, bool isConst = false) {
        mData->set(data, isConst);
    }
};

bool operator <(const tc::Hash& a, const tc::Hash& b){
	return strcmp(a.toString(), b.toString()) < 0;
}

/**
 * @brief sha256计算数据hash
 * 
 * @param data 数据
 * @return Hash 返回hash结果
 */
inline Hash sha256(const char *data) {
    return std::move(Hash(TC_Sha256(data), false));
}

/**
 * @brief ripemd160计算数据hash
 * 
 * @param data 数据
 * @return Hash 返回hash结果
 */
inline Hash ripemd160(const char *data) {
    return std::move(Hash(TC_Ripemd160(data), false));
}

/**
 * @brief keccak256计算数据hash
 * 
 * @param data 数据
 * @return Hash 返回hash结果
 */
inline Hash keccak256(const char *data) {
    return std::move(Hash(TC_Keccak256(data), false));
}

/**
 * @brief 获取指定高度的区块哈希
 * @ingroup cppapi
 * @param height 区块高度
 * @return Hash 区块哈希值
 */
inline Hash blockHash(const int64& height) {
    return std::move(Hash(TC_BlockHash(height), false));
}


/**
 * @brief sha256计算数据hash
 * 
 * @param data 数据
 * @return Hash 返回hash结果
 */
Hash sha256(const tc::string& data) {
    return std::move(Hash(TC_Sha256(data.c_str()), false));
}


/**
 * @brief ripemd160计算数据hash
 * 
 * @param data 数据
 * @return Hash 返回hash结果
 */
Hash ripemd160(const tc::string& data) {
    return std::move(Hash(TC_Ripemd160(data.c_str()), false));
}

/**
 * @brief keccak256计算数据hash
 * 
 * @param data 数据
 * @return Hash 返回hash结果
 */
Hash keccak256(const tc::string& data) {
    return std::move(Hash(TC_Keccak256(data.c_str()), false));
}


// @Todo: Fix Me!!!
using Pubkey = tc::string;
using Signature = tc::string;


class Sign {
public:
    /**
     * @brief 检查是否为给定公钥pubkey对信息data创建的签名signature，签名算法为secp256k1
     * 
     * @param pk 公钥字符串
     * @param data 信息数据
     * @param sig 签名
     * @return true 签名匹配
     * @return false 签名不匹配
     */
    static bool check(const Pubkey& pk, const tc::string& data, const Signature& sig) {
		//+2 for "0x"
        TC_RequireWithMsg(pk.size() == 40 + 2, "Pubkey size error, check missing 0x");
        TC_RequireWithMsg(sig.size() == 130 + 2, "Signature size error, check missing 0x");
        return (TC_CheckSign(pk.c_str(), data.c_str(), sig.c_str()) == 1);
    }
    /**
     * @brief 通过消息的hash和签名(v, r, s)恢复公钥
     * 
     * @param hash 消息的哈希
     * @param v 签名
     * @param r 签名
     * @param s 签名
     * @return Address 以地址格式输出公钥字符串
     */
    static Address recover(const Hash& hash, const BInt& v, const BInt& r, const BInt& s) {
        char *addr = TC_Ecrecover(hash.toString(), v.toString(), r.toString(), s.toString());
        return std::move(Address(addr, false));
    }
};
//@}

} // namespace tc

#endif
