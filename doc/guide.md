## 如何开发wasm合约
### Hello Thunderchain
```
#include "tcmethod.hpp"//声明合约方法
#include "tcapi.hpp"//虚拟机API，TC_Payable
class Hello : public TCBaseContract{ //TCBaseContract合约基类
public:
    const char* HelloThunder(){      //合约类公有成员函数
        TC_Payable(false);			 //接口属性Payable = false
        return "Hello Thunderchain"; //返回值
    }
};
TC_ABI(Hello, (HelloThunder))		//TC_ABI声明合约外部接口
```
为封装合约接口，需要实现一个特殊的类。该类继承于<b>TC_BaseContract</b>，其公有的成员函数可通过TC_ABI设置为合约外部接口。
外部只能调用TC_ABI声明过的接口，且TC_ABI宏只能定义一次。
```
TC_ABI(<合约类名>, (<方法名>)(<方法名>)(<方法名>)...)。
```

编译合约代码后，会生成abi。
Hello合约生成的abi内容如下，各字段与[solidity ABI](https://solidity.readthedocs.io/en/latest/abi-spec.html)兼容。

```
{
    "name":"HelloThunder",
    "payable":false,
    "inputs":[],
    "outputs":[
        {"type":"const char *"}
        ]
}
```

> wasm合约支持C/C++17，默认编译器为LLVM 7

### 合约创建
创建流程与solidity合约一致，具体见catalyst使用说明。

#### 合约初始化函数
创建合约时，会调用<b>"Init"</b>函数。如需初始化函数，可以重载TC_BaseContract <b>Init</b>方法。

### 合约调用
RPC方式调用合约时，交易结构体中data字段的数据格式为<<b>action</b>>|<<b>args</b>>，action与args以<b>"|"</b>分割。
> 普通合约调用时，无法调用Init函数

#### 调用合约接口
```
#include "tcmethod.hpp"//声明合约方法
#include "tcapi.hpp"//虚拟机API，TC_Payable
#include "types.hpp" //tc::App
class Hello : public TCBaseContract{ //TCBaseContract合约基类
public:
    const char* HelloThunder(){      //合约类公有成员函数
        TC_Payable(false);			 //接口属性Payable = false
        return "Hello Thunderchain"; //返回值
    }
    tc::Address GetSender(){      //获取合约调用地址
        TC_Payable(false);			 //接口属性Payable = false
        return tc::App::getInstance()->sender(); //返回值
    }
    uint64 GetHeight(){      //获取区块链当前高度
        TC_Payable(false);			 //接口属性Payable = false
        return tc::App::getInstance()->height(); //返回值
    }
};
TC_ABI(Hello, (HelloThunder)(GetSender)(GetHeight)) //TC_ABI声明合约外部接口
```
调用GetSender函数，data字段参数为"GetSender|{}"
调用GetHeight函数，data字段参数为"GetHeight|{}"

### 入参与返回值
入参与返回值字符串为json格式，调用合约时需要将参数转化为json字符串传入。

#### 结构体序列化
##### TC_STRUCT、TC_FIELD_NAME
对于自定义的结构体类型，其序列化方式由用户自己指定。
```
//定义结构体宏
TC_STRUCT(Struct, ...)
//指定结构体成员key值
TC_FIELD_NAME(StructField, FieldName)
```
- Struct为结构体类型名
- StructField为结构体成员变量名
- FieldName为该成员序列化时的key值

##### 序列化Person
定义Person结构体，以及其各个字段的key值，指定序列化方式
```
struct Person{
    int age;
    tc::Address addr;
    tc::BInt cash;
};

TC_STRUCT(Person,
    TC_FIELD_NAME(age, "age"),
    TC_FIELD_NAME(addr, "addr"),
    TC_FIELD_NAME(cash, "cash")
)
```

初始化代码
```
Person p {10, "0x000000000000000000000000000000000000000a","88888888"};
```

json序列化字符串
````
{"age":"10", "addr":"0x000000000000000000000000000000000000000a", "cash":"888888888"}
````

#### 入参序列化
调用合约接口时，参数按顺格式化。如：
```
void func(int i, const tc::string& str);
```
该函数接口的参数为
```
{"0":"2019", "1":"HelloWorld"}
```
> 可参见example/json、example/tlv样例

#### 返回值序列化
返回值可以设置为单返回值，多返回值。多返回值可以一次返回多个参数。

##### 单返回值
```
#include "tcmethod.hpp"//声明合约方法
#include "tcapi.hpp"//虚拟机API，TC_Payable
class Args : public TCBaseContract { //TCBaseContract合约基类
public:
    const char* HelloThunder(){      //合约类公有成员函数
        TC_Payable(false);			 //接口属性Payable = false
        return "Hello Thunderchain"; //返回值
    }
};
TC_ABI(Args, (HelloThunder))		//TC_ABI声明合约外部接口
```
返回结果为
```
{"ret":"Hello THunderchain"}
```

##### 多返回值
```
#include "tcmethod.hpp"//声明合约方法
#include "tcapi.hpp"//虚拟机API，TC_Payable
class Args : public TCBaseContract { //TCBaseContract合约基类
public:
    std::tuple<tc::address, tc::BInt> GetBalance(){      //合约类公有成员函数
        TC_Payable(false);		 //接口属性Payable = false
        tc::Address addr;
        tc::BInt balance;
        return {addr, balance}; //返回值
    }
};
TC_ABI(Args, (GetBalance))		//TC_ABI声明合约外部接口
```

返回结果为
```
{"ret":{"0":"0x000000000000000000000000000000000000000a","1":"0"}}
```

### 数据存储

> 参见example/storagebyapi、example/storagebyclass样例

#### 调用合约交易DATA字段
实际构造交易时，需要将action+args构成的字符串转为byte数组(ascii转hex)
````
eth.sendTransaction({ 
  "from": "0x54fb1c7d0f011dd63b08f85ed7b518ab82028100",
  "gas": "0x76c00",
  "gasPrice": "0x9184e72a",
  "data": "0x47657442616C616E63657C7B7D"
})
````
实际合约参数为

````
GetBalance|{}
````

> 以golang为例
> str := "ab";   
> data := []byte(str); //data = [0x61 0x62]


### webAssembly运行与语言限制

#### wasm运行参数
````
MinAllocMemSize         = 32//malloc内存最小粒度为32B
FixedStackIdx           = 16*1024//栈大小为16k
MaxDataMemSize          = 16*1024//Data段限制为16k
DefaultMinHeapMemSize   = 64*1024
DefaultMaxHeapMemSize   = 256*1024//默认堆大小最小为64k，最大为256k
````
#### C/C++ 不支持特性:
````
浮点数(float/double)
operator new/delete
typeid/dynamic_cast(-fno-rtti)
try-catch(-fno-exceptions)
````

#### Libc 不支持头文件:
````
signal.h
math.h
locale.h
errno.h
uchar.h
time.h(Incomplete support)
````
#### Libc++ 不支持头文件:
````
rand
atomics
thread
random
````