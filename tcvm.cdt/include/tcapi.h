/*
 * tcapi.h
 *
 * @COPYRIGHT BY XUNLEI THUNDERCHAIN
 */

#ifndef THUNDERCHAIN_API_H_
#define THUNDERCHAIN_API_H_

#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef EMPTY_CSTRING
#define EMPTY_CSTRING ((char *)"")
#endif

#define atoi64 atoll
/**
 * @defgroup capi C API
 * @brief 区块链虚拟机实现提供的接口，类似于系统调用
 * @ingroup capi
 * @{
 */

typedef uint64_t uint64;
typedef int64_t int64;
typedef char* BigInt;
typedef char* address;
typedef void* JsonRoot;

const bool FALSE = false;
const bool TRUE  = true;
const BigInt TC_ONETOKEN="1000000000000000000";
typedef const char* RetValue;

#define TC_TOKEN(NUM) TC_BigIntMul((BigInt)TC_ONETOKEN, (BigInt)#NUM)

#ifndef BIGINT_0
#define BIGINT_0 ((const BigInt)"0")
#endif

#ifndef BIGINT_1
#define BIGINT_1 ((const BigInt)"1")
#endif

#define ACTION_FUNC(_a, _fn) do{\
    if (strcmp(action, _a) == 0) {return _fn();}\
}while(0)

#define ACTION_CODE(_a, _code) do{\
    if (strcmp(action, _a) == 0) {_code}\
}while(0)

#define CMP_ADDRESS(a1, a2) (strcmp(a1, a2))
#define ZERO_ADDRESS ((char *)"0x0000000000000000000000000000000000000000")
#define TRUE_CSTRING ((char *)"1")
#define FALSE_CSTRING ((char *)"0")
///@}

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @defgroup token Token C API
 * @brief 迅雷链提供的token发行、余额、转账等函数
 * @ingroup capi
 */

/**
 * @defgroup blockchain Blockchain System API
 * 为方便wasm合约可以方便的访问区块链相关信息，wasm虚拟机对外提供了Storage、Message、Event、Contract相关的接口。
 * @brief 区块链相关信息访问接口
 * @ingroup capi
 */

/**
 * @defgroup cjson Json C API
 * @brief 为方便编解码json对象，暂时由wasm虚拟机提供json相关的接口。
 * @ingroup capi
 */

/**
 * @defgroup libc LibC API
 * @brief wasm虚拟机底层替换的部分C标准库接口，适配wasm虚拟机底层内存模型。
 * @ingroup capi
 */

/**
 * @defgroup sign Sign API
 * @brief wasm合约提供一些基础的签名算法接口。
 * @ingroup capi
 */

/**
 * @defgroup bigint BigInt API
 * 标准的C库不提供BigInt操作，考虑到区块链底层与余额有关的操作，都是以BigInt作为操作数，而第三方的C库与go本身的BigInt操作会存在不兼容的问题，因此统一由wasm虚拟机提供此类接口，以c字符串形式表示。
 * @brief BigInt操作接口
 * @ingroup capi
 */


/**
 * @brief 调用外部合约方法，语义与solidaty DelegateCall一致
 * 
 * @ingroup blockchain
 * @param contract 外部合约地址
 * @param action 外部合约方法名
 * @param args 外部合约方法参数，json格式
 * @return char* 与调用的外部合约方法返回值一致
 * @warning 不支持转账操作
 */
char* TC_CallContract(const char* contract, const char* action, const char* args);

/**
 * @brief 调用外部合约方法，语义与solidaty Call一致
 * 
 * @ingroup blockchain
 * @param contract 外部合约地址
 * @param action 外部合约方法名
 * @param args 外部合约方法参数，json格式
 * @return char* 与调用的外部合约方法返回值一致
 * @warning 不支持转账操作
 */
char* TC_DelegateCallContract(const char* contract, const char* action, const char* args);

/**
 * @brief 以字符串key为键，查询对应的数据 
 * 
 * @ingroup blockchain
 * @param key 已持久化存储数据对应的键(字符串)
 * @return char* 已持久化字符串
 * @warning 如果key或者value为非字符串会遇到'\0'截断
 */
char* TC_StorageGetString(const char* key);


/**
 * @brief 以字符串key为键，查询对应的数据 
 * 
 * @ingroup blockchain
 * @param key 已持久化存储数据对应的键(字符串)
 * @return uint8_t* 已持久化数据
 */
uint8_t* TC_StorageGetBytes(const char* key);

/**
 * @brief 以字符串key为键，查询对应的数据
 * 
 * @ingroup blockchain
 * @param key 已持久化存储数据对应的键的地址起始值(纯字节数据)
 * @param size key指向内存地址的数据大小
 * @return char* 已持久化字符串
 * @warning 如果返回value为非字符串会遇到'\0'截断
 */
char* TC_StoragePureGetString(const uint8_t* key, uint32_t size);

/**
 * @brief 以字符串key为键，查询对应的数据
 * 
 * @ingroup blockchain
 * @param key 已持久化存储数据对应的键的地址起始值(纯字节数据)
 * @param size key指向内存地址的数据大小
 * @return uint8_t* 已持久化数据
 */
uint8_t* TC_StoragePureGetBytes(const uint8_t* key, uint32_t size);


/**
 * @brief 以字符串key为键，持久化存储数据val
 * 
 * @ingroup blockchain
 * @param key 持久化存储数据对应的键(字符串)
 * @param val 已持久化字符串
 * @warning 如果key或者value为非字符串会遇到'\0'截断
 */
void TC_StorageSetString(const char* key, const char* val);


/**
 * @brief  以字符串key为键，持久化存储数据val
 * 
 * @ingroup blockchain
 * @param key 持久化存储数据对应的键(字符串)
 * @param val 需要持久化存储数据对应的起始地址(纯字节数据)
 * @param size val指向内存地址的数据大小
 * @warning 如果key为非字符串会遇到'\0'截断
 */
void TC_StorageSetBytes(const char* key, const uint8_t* val, uint32_t size);

/**
 * @brief 以字符串key为键，持久化存储数据val
 * 
 * @ingroup blockchain
 * @param key 持久化存储数据对应的键的地址起始值(纯字节数据)
 * @param size1 key指向内存地址的数据大小
 * @param val 已持久化字符串
 * @warning 如果val为非字符串会遇到'\0'截断
 */
void TC_StoragePureSetString(const uint8_t* key, uint32_t size1, const char* val);

/**
 * @brief 以字符串key为键，持久化存储数据val
 * 
 * @ingroup blockchain
 * @param key 已持久化存储数据对应的键的地址起始值(纯字节数据)
 * @param size1 key指向内存地址的数据大小
 * @param val 需要持久化存储数据对应的起始地址(纯字节数据)
 * @param size2 val指向内存地址的数据大小
 */
void TC_StoragePureSetBytes(const uint8_t* key, uint32_t size1, const uint8_t* val, uint32_t size2);

/**
 * @brief 删除以字符串key为键的数据
 * 
 * @ingroup blockchain
 * @param key 已持久化存储数据对应的键(字符串)
 */
void TC_StorageDel(const char* key);
//TODO key pure data


/**
 * @brief BigInt加法运算，等价于(a + b)
 * 
 * @ingroup bigint
 * @param a BigInt数字
 * @param b BigInt数字
 * @return BigInt 加法结果
 */
BigInt TC_BigIntAdd(const char* a, const char* b);

/**
 * @brief BigInt减法运算，等价于(a - b)
 * 
 * @ingroup bigint
 * @param a BigInt数字
 * @param b BigInt数字
 * @return BigInt 减法结果
 */
BigInt TC_BigIntSub(const char* a, const char* b);


/**
 * @brief BigInt乘法运算，等价于(a * b)
 * 
 * @ingroup bigint
 * @param a BigInt数字
 * @param b BigInt数字
 * @return BigInt 
 */
BigInt TC_BigIntMul(const char* a, const char* b);

/**
 * @brief BigInt除法运算，等价于(a / b)
 * 
 * @ingroup bigint
 * @param a BigInt数字
 * @param b BigInt数字
 * @return BigInt 除法结果
 */
BigInt TC_BigIntDiv(const char* a, const char* b);

/**
 * @brief BigInt取模运算，等价于(a  % b)
 * 
 * @ingroup bigint
 * @param a BigInt数字
 * @param b BigInt数字
 * @return BigInt 取模结果
 */
BigInt TC_BigIntMod(const char* a, const char* b);

/**
 * @brief BigInt大小比较
 * 
 * @ingroup bigint
 * @param a BigInt数字
 * @param b BigInt数字
 * @return int 返回值
 * @retval <0 a < b
 * @retval =0 a = b
 * @retval >0 a > b
 */
int TC_BigIntCmp(const char* a, const char* b);

/**
 * @brief BigInt转换为64位整数，超过64位的数值将会被截断
 * 
 * @ingroup bigint
 * @param a BigInt数字
 * @return int64 64位有符号整数
 */
int64 TC_BigIntToInt64(const char* a);

/**
 * @brief 分配指定大小的内存，并返回内存的起始地址
 * 
 * @ingroup libc
 * @param size 分配的内存大小
 * @return void* 分配的内存起始地址
 */
void* malloc(size_t size);

/**
 * @brief 分配nmemb个元素，每个元素的大小为size字节，并返回内存的起始地址
 * 
 * @ingroup libc
 * @param count 分配的元素个数
 * @param size 元素的大小，字节为单位
 * @return void* 内存的起始地址
 */
void* calloc(size_t count, size_t size);

/**
 * @brief 调整ptr指向内存的大小为size个字节，并返回新的内存起始地址
 * 
 * @ingroup libc
 * @param ptr 原始内存的起始地址
 * @param size 调整大小，以字节为单位
 * @return void* 新的内存起始地址
 */
void* realloc(void* ptr, size_t size);

/**
 * @brief 释放地址指向的内存
 * 
 * @ingroup libc
 * @param ptr 内存的地址
 * @warning 必须要是申请内存时返回的内存地址
 */
void free(void* ptr);

/**
 * @brief 内存拷贝
 * 
 * @ingroup libc
 * @param dest 目标内存起始地址
 * @param src 源内存起始地址
 * @param n 拷贝的内存大小，以字节为单位
 * @return void* 目标内存的起始地址
 * @warning src和dst内存段不可以有重叠
 */
void* memcpy(void* dest, const void* src, size_t n);

/**
 * @brief 填充内存，设置为指定的数值
 * 
 * @ingroup libc
 * @param dest 内存起始地址；
 * @param c 内存设置为常量字节c
 * @param n 填充的内存长度
 * @return void* 内存的起始地址
 */
void* memset(void* dest, int c, size_t n);

/**
 * @brief 内存拷贝
 * 
 * @ingroup libc
 * @param dest 目标内存起始地址
 * @param src 源内存起始地址
 * @param n 拷贝的内存大小，以字节为单位
 * @return void* 返回目标内存起始地址
 * @warning dst和src内存区域可以重叠
 */
void* memmove(void* dest, const void* src, size_t n);

/**
 * @brief 内存比较
 * 
 * @ingroup libc
 * @param str1 内存1起始地址
 * @param str2 内存2起始地址
 * @param n 比较大小
 * @return int 比较结果
 * @retval <0 a < b
 * @retval =0 a = b
 * @retval >0 a > b
 */
int memcmp(const void *str1, const void *str2, size_t n);

/**
 * @brief 把s2字符串拼接到s1字符串后面
 * 
 * @ingroup libc
 * @param str1 s1字符串起始地址
 * @param str2 s2字符串起始地址
 * @return char* 返回拼接后的字符串起始地址；新地址
 * @warning ！！strconcat返回的地址一定是新的地址。！！ 不可用str1地址
 */
char* strconcat(const char* str1, const char* str2);

/**
 * @brief 打印信息s到日志(非交易事件log)中，方便调试
 * 
 * @ingroup libc
 * @param cstr 日志信息，字符串表示
 * @param len 字符串长度
 */
void prints_l(const char* cstr, uint32_t len);

/**
 * @brief 32位整数转换为字符串
 * 
 * @ingroup libc
 * @param a 32位整数
 * @return char* 字符串起始地址
 */
char* itoa(int a);

/**
 * @brief 64位整数转换为字符串
 * 
 * @ingroup libc
 * @param amount 64位整数
 * @param radix 字符串进制
 * @return char* 字符串起始地址
 */
char* i64toa(int64 amount, int radix);

/**
 * @brief 记录合约触发事件
 * 
 * @ingroup blockchain
 * @param eventID 事件标识；
 * @param data 事件数据
 */
void TC_Notify(const char* eventID, const char* data);

/**
 * @brief 合约触发事件，无事件标识
 * 
 * @ingroup blockchain
 * @param data 
 */
void TC_Log0(const char* data);

/**
 * @brief 合约触发事件，带一级事件主题标识
 * 
 * @ingroup blockchain
 * @param data 事件数据
 * @param topic 事件标识
 */
void TC_Log1(const char* data, const char* topic);

/**
 * @brief 合约触发事件，带两级主题标识
 * 
 * @ingroup blockchain
 * @param data 事件数据
 * @param topic1 一级事件标识
 * @param topic2 二级事件标识
 */
void TC_Log2(const char* data, const char* topic1, const char* topic2);

/**
 * @brief 合约触发事件，带三级主题标识
 * 
 * @ingroup blockchain
 * @param data 事件数据
 * @param topic1 一级事件标识
 * @param topic2 二级事件标识
 * @param topic3 三级事件标识
 */
void TC_Log3(const char* data, const char* topic1, const char* topic2, const char* topic3);

/**
 * @brief 合约触发事件，带四级主题标识
 * 
 * @ingroup blockchain
 * @param data 事件数据
 * @param topic1 一级事件标识
 * @param topic2 二级事件标识
 * @param topic3 三级事件标识
 * @param topic4 四级事件标识
 */
void TC_Log4(const char* data, const char* topic1, const char* topic2, const char* topic3, const char* topic4);

/**
 * @brief 检查是否为给定公钥pubkey对信息data创建的签名signature，签名算法为secp256k1
 * 
 * @ingroup sign
 * @param pubkey 公钥字符串
 * @param data 信息数据
 * @param sig 签名
 * @return int 如果签名匹配，则返回1，否则返回0
 */
int TC_CheckSign(const char* pubkey, const char* data, const char* sig);

/**
 * @brief 获取指定高度的区块哈希
 * 
 * @ingroup blockchain
 * @param blockNumber 区块高度
 * @return char* 区块哈希值，16进制字符串表示
 */
char* TC_BlockHash(uint64 blockNumber);

/**
 * @brief 获取区块链的coinbase账户地址
 * 
 * @ingroup blockchain
 * @return char* 区块链的coinbase账户地址，16进制字符串表示
 */
char* TC_GetCoinbase(void);

/**
 * @brief 获取当前区块的gas上限
 * 
 * @ingroup blockchain
 * @return uint64 当前区块的gas上限
 */
uint64 TC_GetGasLimit(void);

/**
 * @brief 获取区块链当前区块高度
 * 
 * @ingroup blockchain
 * @return uint64 区块链当前区块高度
 */
uint64 TC_GetNumber(void);

/**
 * @brief 获取合约调用的输入数据
 * 
 * @ingroup blockchain
 * @return char* 合约输入数据，字符串表示
 */
char* TC_GetMsgData(void);

/**
 * @brief 获取合约调用的gas上限
 * 
 * @ingroup blockchain
 * @return uint64 合约调用的gas上限
 */
uint64 TC_GetMsgGas(void);

/**
 * @brief 获取合约调用者
 * 
 * @ingroup blockchain
 * @return address 合约调用者地址，16进制字符串表示
 */
address TC_GetMsgSender(void);

/**
 * @brief 获取合约调用参数中的方法名字段
 * 
 * @ingroup blockchain
 * @return char* 合约调用参数中的方法名字段
 */
char* TC_GetMsgSign(void);

/**
 * @brief 获取合约调用者转账给合约的数额
 * 
 * @ingroup blockchain
 * @return char* 16进制字符串表示
 */
char *TC_GetMsgValue(void);

/**
 * @brief 获取合约调用者转账给合约的token数额
 * 
 * @ingroup blockchain
 * @return char* 16进制字符串表示给合约的token数额
 */
char *TC_GetMsgTokenValue(void);

/**
 * @brief 断言判断，与C语言的assert()接口功能类似，如果输入的condition为false，则触发合约终止并回滚状态
 * 
 * @ingroup blockchain
 * @param 布尔类型的条件变量 
 */
void TC_Assert(bool condition);

/**
 * @brief 与TC_Assert()接口一样
 * 
 * @ingroup blockchain
 * @param condition 布尔类型的条件变量
 */
void TC_Require(bool condition);

/**
 * @brief 与TC_Require()接口功能一样，若condition为false, msg参数将会打印到日志中
 * 
 * @ingroup blockchain
 * @param condition 布尔类型的条件变量
 * @param msg 调试输出日志信息
 */
void TC_RequireWithMsg(bool condition, const char* msg);

/**
 * @brief 获取合约当前剩余的gas
 * 
 * @ingroup blockchain
 * @return uint64 合约当前剩余的gas
 */
uint64 TC_GasLeft(void);

/**
 * @brief 返回当前区块的时间戳，以unix时间表示
 * 
 * @ingroup blockchain
 * @return uint64 unix时间戳
 */
uint64 TC_Now(void);

/**
 * @brief 获取合约调用的gas价格
 * 
 * @ingroup blockchain
 * @return uint64 合约调用的gas价格
 */
uint64  TC_GetTxGasPrice(void);

/**
 * @brief 获取执行的起始地址，原始交易的sender
 * 
 * @ingroup blockchain
 * @return address 区块链的账户地址，16进制字符串表示
 */
address TC_GetTxOrigin(void);

/**
 * @brief 类似TC_Require(false)
 * 
 * @ingroup blockchain
 */
void TC_Revert(void);

/**
 * @brief 类似TC_Require(false, msg)
 * 
 * @ingroup blockchain
 * @param msg 
 */
void TC_RevertWithMsg(const char *msg);

/**
 * @brief 打印信息s到日志(非交易事件log)中，方便调试
 * 
 * @ingroup blockchain
 * @param str 日志信息，字符串表示
 */
void TC_Prints(const char* str);

/**
 * @brief 销毁合约，并将合约账户剩余的余额转给address账户
 * 
 * @ingroup blockchain
 * @param recipient 接收合约账户剩余余额的账户地址
 * @return char* 暂未使用，预留接口
 */
char* TC_SelfDestruct(const char* recipient);

/**
 * @brief 获取合约自身地址
 * 
 * @ingroup blockchain
 * @return address 16进制字符串表示的合约地址
 */
address TC_GetSelfAddress(void);

/**
 * @brief 获取指定账户address的余额
 * 
 * @ingroup blockchain
 * @param addr 账户地址
 * @return char* address账户的当前余额
 */
char* TC_GetBalance(const char* addr);

/**
 * @brief 通过消息的hash和签名(v, r, s)恢复公钥
 * 
 * @ingroup sign
 * @param hash 消息的哈希
 * @param v 签名
 * @param r 签名
 * @param s 签名
 * @return char* 以16进制格式输出公钥字符串
 */
char* TC_Ecrecover(const char* hash, const char* v, const char* r, const char* s);

/**
 * @brief Ripemd160哈希运算
 * 
 * @ingroup sign
 * @param data 原始数据
 * @return char* 哈希运算结果，以字符串形式保存
 */
char* TC_Ripemd160(const char* data);

/**
 * @brief 哈希运算
 * 
 * @ingroup sign
 * @param data 原始数据
 * @return char* 哈希运算结果，以字符串形式保存
 */
char* TC_Sha256(const char* data);

/**
 * @brief 哈希运算
 * 
 * @ingroup sign
 * @param data 原始数据
 * @return char* 哈希运算结果，以字符串形式保存
 */
char* TC_Keccak256(const char* data);

/**
 * @brief 当前合约账户给address地址转账amount数额
 * 
 * @ingroup blockchain
 * @param to 目标账户地址，16进制字符串表示
 * @param amount 转账数额
 */
void TC_Transfer(const char* to, const char* amount);

/**
 * @brief 判断输入的字符串是否为地址的16进制字符串表示
 * 
 * @ingroup blockchain
 * @param str 输入的地址字符串
 * @return true 输入的字符串为地址的16进制字符串表示
 * @return false 输入的字符串不是为地址的16进制字符串表示
 */
bool TC_IsHexAddress(const char *str);



/**
 * @brief 解析输入字符串为json对象，并返回json对象的句柄
 * 
 * @ingroup cjson
 * @param data 字符串
 * @return JsonRoot json对象的句柄，如果失败则返回null
 * @warning 如果不符合json字符串规范，会退出合约执行
 */
JsonRoot TC_JsonParse(const char *data);

/**
 * @brief 在root指向的json对象中，获取指定key字段的整数值
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @return int 字段key对应的32位整数值
 */
int TC_JsonGetInt(JsonRoot root, const char* key);

/**
 * @brief 在root指向的json对象中，获取指定key字段的整数值
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @return int64 字段key对应的64位整数值
 */
int64 TC_JsonGetInt64(JsonRoot root, const char* key);

/**
 * @brief 在root指向的json对象中，获取指定key字段的字符串数值
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @return char* 字段key对应的字符串数值
 */
char* TC_JsonGetString(JsonRoot root, const char* key);

/**
 * @brief 在root指向的json对象中，获取指定key字段的地址数值
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @return address 字段key对应的地址数值
 */
address TC_JsonGetAddress(JsonRoot root, const char *key);

/**
 * @brief 在root指向的json对象中，获取指定key字段的BigInt数值
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @return BigInt 字段key对应的BigInt字符串
 */
BigInt TC_JsonGetBigInt(JsonRoot root, const char *key);

/**
 * @brief 在root指向的json对象中，获取指定key字段的子json对象
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @return JsonRoot 字段key对应的json对象句柄
 */
JsonRoot TC_JsonGetObject(JsonRoot root, const char* key);

/**
 * @brief 创建json对象，返回对应的句柄
 * 
 * @ingroup cjson
 * @return JsonRoot 新创建json对象的句柄
 */
JsonRoot TC_JsonNewObject(void);

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @param value 字段值为32位整数
 */
void TC_JsonPutInt(JsonRoot root, const char* key, const int value);

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @param value 字段值为64位整数
 */
void TC_JsonPutInt64(JsonRoot root, const char* key, const long long value);

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @param value 字段值为字符串
 */
void TC_JsonPutString(JsonRoot root, const char* key, const char* value);

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @param value 字段值为address
 * @warning 不会判断address是否符合地址格式
 */
void TC_JsonPutAddress(JsonRoot root, const char* key, const char* value);

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为val
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @param value 字段值为BigInt
 * @warning 不会判断bigint是否符合大整数格式
 */
void TC_JsonPutBigInt(JsonRoot root, const char* key, const char* value);

/**
 * @brief 在root指向的json对象中，设置指定key字段的值为子json对象child
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @param key 字段的标识符
 * @param child 子json对象句柄
 */
void TC_JsonPutObject(JsonRoot root, const char* key, void* child);

/**
 * @brief 把root句柄指向的json对象，转换为字符串
 * 
 * @ingroup cjson
 * @param root TC_JsonParse()接口返回的对象句柄
 * @return char* json对象的字符串
 */
char* TC_JsonToString(JsonRoot root);

/**
 * @brief 发行token
 * 
 * @ingroup token
 * @param amount 发行token总量
 */
void TC_Issue(const char* amount);

/**
 * @brief token转账
 * 
 * @ingroup token
 * @param addr token转入地址
 * @param token 转账的token类型
 * @param amount 转账数量
 */
void TC_TransferToken(const char* addr, const char* token, const char* amount);

/**
 * @brief token余额查询
 * 
 * @ingroup token
 * @param addr 查询地址
 * @param token token类型
 * @return BigInt token数量，BigInt类型
 */
BigInt TC_TokenBalance(const char* addr, const char* token);

/**
 * @brief 查询msg中token类型
 * 
 * @ingroup token
 * @return address token类型
 */
address TC_TokenAddress(void);

/**
 * 设置当前接口是否可以转账，默认为true，可以转账；若condition为false，且合约调用者传递的msg.Value > 0, 则会抛出异常终止合约执行，并回滚状态
 * @brief 设置当前接口是否可以转账
 * 
 * @ingroup blockchain
 * @param condition 布尔类型的条件变量
 * @warning 注意：如果接口A Payable=true 接口B Payable=false, 当msg.value > 0, 接口A调用接口B时,执行至B接口的TC_Payable会报错
 * @note 如上情况下，建议对B接口进行封装，对外调用提供不可转账的，对内调用提供可转账的。
 */
void TC_Payable(bool condition);

/**
 * 获取其他合约的存储
 * @brief get contract storage
 * 
 * @ingroup blockchain
 * @param addr 合约地址
 * @param key 键值
 * @return 请求返回body数据，char *类型
 */
uint8_t *TC_ContractStorageGet(const char* addr, const char* key);

/**
 * 获取其他合约的存储
 * @brief get contract storage
 * 
 * @ingroup blockchain
 * @param addr 合约地址
 * @param key 键值
 * @param size length
 * @return 请求返回body数据，char *类型
 */
uint8_t *TC_ContractStoragePureGet(const char* addr, uint8_t* key, size_t size);


#ifdef __cplusplus
}
#endif

#endif /* THUNDERCHAIN_API_H_ */
