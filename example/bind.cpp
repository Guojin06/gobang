#include<iostream>
#include<functional>
#include<string>

void print(const char* str, int num) {
    std::cout << str << num << std::endl;
}

int main()
{
    //print("nihao", 10); // 调用自定义的 print 函数
    auto func=std::bind(print, "nihao", std::placeholders::_1);//绑定第一个参数
    print("nihao",10);
    func(20); // 调用自定义的 print 函数
    return 0;  
}
