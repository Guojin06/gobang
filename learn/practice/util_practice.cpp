#include <iostream>
#include <string>
#include <vector>
#include <json/json.h>

// 练习1：命名空间的使用
namespace math_util {
    int add(int a, int b) {
        return a + b;
    }
    
    double add(double a, double b) {
        return a + b;
    }
}

namespace string_util {
    void split(const std::string& str, const std::string& delimiter, 
               std::vector<std::string>& result) {
        size_t start = 0;
        size_t end = str.find(delimiter);
        
        while (end != std::string::npos) {
            result.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        result.push_back(str.substr(start));
    }
}

// 练习2：函数重载
class Calculator {
public:
    // 重载1：整数加法
    int add(int a, int b) {
        return a + b;
    }
    
    // 重载2：浮点数加法
    double add(double a, double b) {
        return a + b;
    }
    
    // 重载3：字符串连接
    std::string add(const std::string& a, const std::string& b) {
        return a + b;
    }
};

// 练习3：模板函数
template<typename T>
T getMax(T a, T b) {
    return (a > b) ? a : b;
}

// 练习4：异常处理
class SafeDivider {
public:
    double divide(double a, double b) {
        if (b == 0) {
            throw std::runtime_error("除数不能为零");
        }
        return a / b;
    }
};

// 练习5：JSON处理
void json_example() {
    Json::Value root;
    root["name"] = "张三";
    root["age"] = 25;
    root["scores"].append(90);
    root["scores"].append(85);
    root["scores"].append(95);
    
    // 输出JSON
    Json::FastWriter writer;
    std::string json_str = writer.write(root);
    std::cout << "JSON字符串: " << json_str << std::endl;
    
    // 解析JSON
    Json::Value parsed;
    Json::Reader reader;
    if (reader.parse(json_str, parsed)) {
        std::cout << "姓名: " << parsed["name"].asString() << std::endl;
        std::cout << "年龄: " << parsed["age"].asInt() << std::endl;
        std::cout << "分数: ";
        for (const auto& score : parsed["scores"]) {
            std::cout << score.asInt() << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::cout << "=== 练习1：命名空间使用 ===" << std::endl;
    std::cout << "整数加法: " << math_util::add(5, 3) << std::endl;
    std::cout << "浮点数加法: " << math_util::add(3.14, 2.86) << std::endl;
    
    std::cout << "\n=== 练习2：函数重载 ===" << std::endl;
    Calculator calc;
    std::cout << "整数: " << calc.add(10, 20) << std::endl;
    std::cout << "浮点数: " << calc.add(3.14, 2.86) << std::endl;
    std::cout << "字符串: " << calc.add("Hello", " World") << std::endl;
    
    std::cout << "\n=== 练习3：模板函数 ===" << std::endl;
    std::cout << "整数最大值: " << getMax(10, 20) << std::endl;
    std::cout << "浮点数最大值: " << getMax(3.14, 2.86) << std::endl;
    std::cout << "字符串最大值: " << getMax(std::string("apple"), std::string("banana")) << std::endl;
    
    std::cout << "\n=== 练习4：异常处理 ===" << std::endl;
    SafeDivider divider;
    try {
        std::cout << "10 / 2 = " << divider.divide(10, 2) << std::endl;
        std::cout << "10 / 0 = " << divider.divide(10, 0) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== 练习5：JSON处理 ===" << std::endl;
    json_example();
    
    std::cout << "\n=== 练习6：字符串分割 ===" << std::endl;
    std::string str = "apple,banana,orange,grape";
    std::vector<std::string> fruits;
    string_util::split(str, ",", fruits);
    
    std::cout << "分割结果: ";
    for (const auto& fruit : fruits) {
        std::cout << fruit << " ";
    }
    std::cout << std::endl;
    
    return 0;
} 