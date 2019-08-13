#include "tcmethod.hpp"//声明合约方法
#include "tcapi.hpp"//虚拟机API，TC_Payable
class Hello : public TCBaseContract{ //TCBaseContract合约基类
public:
    const char* HelloThunder(){      //合约类公有成员函数
        TC_Payable(false);			 //接口属性Payable = false
        return "Hello Thunderchain"; //返回值
    }
};
TC_ABI(Hello, (HelloThunder))		//TC_ABI声明合约外部接