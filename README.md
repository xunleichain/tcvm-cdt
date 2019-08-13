# TCWasm Compile Tool

[![License Apache 2](https://img.shields.io/badge/license-apache%202-blue.svg?style=flat-square)](https://github.com/xunleichain/tcvm-cdt/blob/master/LICENSE)

迅雷链wasm合约编译工具，提供本地编译工具以及合约库文件源代码

### 编译
#### 在线编译
- [catalyst](https://catalyst.onethingcloud.com/#/catalyst)在线编译工具

#### 本地编译

###### 安装wasm编译环境
- 安装git、cmake > 3.4.3 、make、python2.7.x，保证磁盘可用空间大于5GB
- 执行tcvm.cdt/wasm_build.sh 脚本


###### 写入环境变量
- 在tcvm.cdt 目录下执行 source env.sh，写入环境变量<br>
或者
- ~/.bashrc文件末尾添加
```
TCVM_CDT_DIR=<tcvm.cdt绝对路径>
TCVM_CDT_BIN=<tcvm.cdt绝对路径>/bin
export TCVM_CDT_DIR
export TCVM_CDT_BIN
```
- 执行 source ~/.bashrc 


###### 编译合约

在代码工程目录下执行
```
  cmake -D CON_DIR=<代码项目目录> <tcvm.cdt目录>
```

例如在本仓库example/hello目录下,执行
```
    cmake -D CON_DIR=${PWD} ../../tcvm.cdt/
    make
```
之后，example/hello目录下生成output.wasm合约二进制文件



### 文档

wasm合约编写指导文档[Guide](doc/guide.md)

### Example

##### Hello Thunderchain
````
#include "tcmethod.hpp"//声明合约方法
class Hello : public TCBaseContract{ //TCBaseContract合约基类
public:
    const char* HelloThunder(){      //合约类公有成员函数
        return "Hello Thunderchain"; //返回值
    }
};
TC_ABI(Hello, (HelloThunder))		//TC_ABI声明合约外部接口
````
更多例子见example文件夹


### 合约测试
- [tc-wasm](https://github.com/xunleichain/tc-wasm)迅雷链虚拟机仓库: wasm-run工具
- [catalyst](https://catalyst.onethingcloud.com/#/catalyst)在线编译工具


