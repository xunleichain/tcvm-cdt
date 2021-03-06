## How to develop a wasm contract
### Hello Thunderchain
```
#include "tcmethod.hpp"//Declare contract method
Class Hello : public TCBaseContract{ //TCBaseContract contract base class
Public:
    Const char* HelloThunder(){ // Contract Class public member function
        Return "Hello Thunderchain"; //Return Value
    }
};
TC_ABI(Hello, (HelloThunder)) //TC_ABI declares contract External interface
```
To encapsulate the contract interface, you need to implement a special class. This class inherits from <b>TC_BaseContract</b>, and its public member functions can be set to contract external interfaces via TC_ABI.
The external can only call the interface declared by TC_ABI, and the TC_ABI macro can only be defined once.
```
TC_ABI (<Contract Class name>, (<Method name>) (<Method name>) (<Method name>)...).
```

After compiling the contract code, ABI file is generated.
The abi generated by the Hello contract is as follows, and each field is compatible with [solidity ABI] (https://solidity.readthedocs.io/en/latest/abi-spec.html).

```
{
    "name": "HelloThunder",
    "payable": false,
    "inputs":[],
    "outputs":[
        {"type":"const char *"}
        ]
}
```

> wasm contract supports C/C++17, the default compiler is LLVM 7

### Contract creation
The creation process is consistent with the solidity contract, as described in the thunderchain [Developer's Guide](https://blockchain.xunlei.com/site/docode.html).

#### Contract initialization function
When you create a contract, the <b>"Init"</b> function is called. To initialize the function, you can override the TC_BaseContract <b>Init</b> method.

### Contract call
When using RPC calls the contract, the data format of the data field in the transaction structure is <<B>action</b>>|<<b>args</b>>, and the action and args are <b>"|"</b> Split.
> `Init` function cannot be called when contract is created

#### Calling contract interface
##### Example
```
#include "tcmethod.hpp"//Declare contract method
#include "tcapi.hpp"//Virtual Machine API, TC_Payable
#include "types.hpp" //tc::App
Class Hello : public TCBaseContract{ //TCBaseContract contract base class
Public:
    Const char* HelloThunder(){ // contract class public member function
        TC_Payable(false); // interface property Payable = false
        Return "Hello Thunderchain"; //return value
    }
    Tc::Address GetSender(){ //Get the contract call address
        TC_Payable(false); // interface property Payable = false
        Return tc::App::getInstance()->sender(); //return value
    }
    Uint64 GetHeight(){ //Get the current height of the blockchain
        TC_Payable(false); // interface property Payable = false
        Return tc::App::getInstance()->height(); //return value
    }
};
TC_ABI(Hello, (HelloThunder)(GetSender)(GetHeight)) //TC_ABI declares the contract external interface
```
Call the `GetSender` function, the data field parameter is `GetSender|{}`
Call the `GetHeight` function, the data field parameter is `GetHeight|{}`

### Parameter & Return value
Parameter and return value string is in `json` format, so the parameter needs to be converted into a json string when the contract is called.

#### Structure Serialization
##### TC_STRUCT, TC_FIELD_NAME
For custom struct types, the serialization mode is specified by the user.
```
// Define the structure macro
TC_STRUCT(Struct, ...)
// specify the structure member key value
TC_FIELD_NAME (StructField, FieldName)
```
- `Struct` is the structure type name
- `StructField` is the structure member variable name
- `FieldName` is the key value when the member is serialized

##### Serializing Person
Define the Person structure, and the key value of its various fields, specifying the serialization method
```
Struct Person{
    Int age;
    Tc::Address addr;
    Tc::BInt cash;
};

TC_STRUCT(Person,
    TC_FIELD_NAME(age, "age"),
    TC_FIELD_NAME(addr, "addr"),
    TC_FIELD_NAME(cash, "cash")
)
```

Initialization code
```
Person p {10, "0x000000000000000000000000000000000000000a","88888888"};
```

Json serialized string
````
{"age":"10", "addr":"0x000000000000000000000000000000000000000a", "cash":"888888888"}
````

#### Parameter Serialization
When the contract interface is called, the parameters are formatted in order. Such as:
```
void func(int i, const tc::string& str);
```
The parameters of the function interface are
```
{"0":"2019", "1": "HelloWorld"}
```
> See `example/json`, `example/tlv`

#### Return value serialization
The return value can be set to a single return value, and more return values. Multiple return values ​​can return multiple parameters at once.

##### Single return value
```
#include "tcmethod.hpp"//Declare contract method
#include "tcapi.hpp"//Virtual Machine API, TC_Payable
Class Args : public TCBaseContract { //TCBaseContract contract base class
Public:
    Const char* HelloThunder(){ // contract class public member function
        TC_Payable(false); // interface property Payable = false
        Return "Hello Thunderchain"; //return value
    }
};
TC_ABI(Args, (HelloThunder)) //TC_ABI declares contract external interface
```
The result is
```
{"ret":"Hello THunderchain"}
```

##### Multiple return values
```
#include "tcmethod.hpp"//Declare contract method
#include "tcapi.hpp"//Virtual Machine API, TC_Payable
Class Args : public TCBaseContract { //TCBaseContract contract base class
Public:
    Std::tuple<tc::address, tc::BInt> GetBalance(){ //Contract class public member function
        TC_Payable(false); // interface property Payable = false
        Tc::Address addr;
        Tc::BInt balance;
        Return {addr, balance}; //return value
    }
};
TC_ABI(Args, (GetBalance)) //TC_ABI declares the contract external interface
```

The result is
```
{"ret":{"0":"0x000000000000000000000000000000000000000a","1":"0"}}
```

### Data Storage


> See `example/storagebyapi`, `example/storagebyclass`

#### RawTransaction DATA Field
When constructing a transaction, you need to convert the string formed by action+args into a byte array (ascii to hex)
````
eth.sendTransaction({
  "from": "0x54fb1c7d0f011dd63b08f85ed7b518ab82028100",
  "gas": "0x76c00",
  "gasPrice": "0x9184e72a",
  "data": "0x47657442616C616E63657C7B7D"
})
````
The actual contract parameters are

````
GetBalance|{}
````

> for example in golang 
> str := "ab";
> data := []byte(str); //data = [0x61 0x62]


### WebAssembly Running Restrictions

#### Runtime Parameters
````
MinAllocMemSize = 32//malloc memory minimum granularity is 32B
FixedStackIdx = 16*1024// stack size is 16k
MaxDataMemSize = 16*1024//Data segment is limited to 16k
DefaultMinHeapMemSize = 64*1024
DefaultMaxHeapMemSize = 256*1024//The default heap size is at least 64k and the maximum is 256k
````
#### C/C++ Unsupport features:
````
Floating point number (float/double)
Operator new/delete
Typeid/dynamic_cast(-fno-rtti)
Try-catch(-fno-exceptions)
````

#### Libc Unsupport header files:
````
Signal.h
Math.h
Locale.h
Errno.h
Uchar.h
Time.h(Incomplete support)
````
#### Libc++ Unsupport header files:
````
Rand
Atomics
Thread
Random
````