# C++基础语法速成指南

## 1. 基础语法（1-2天）
### 1.1 基本数据类型
```cpp
int number = 42;          // 整数
double pi = 3.14159;      // 浮点数
char letter = 'A';        // 字符
bool isTrue = true;       // 布尔值
string text = "Hello";    // 字符串
```

### 1.2 控制结构
```cpp
// if-else语句
if (condition) {
    // 代码块
} else if (another_condition) {
    // 代码块
} else {
    // 代码块
}

// for循环
for (int i = 0; i < 10; i++) {
    // 代码块
}

// while循环
while (condition) {
    // 代码块
}
```

### 1.3 函数
```cpp
// 函数定义
返回类型 函数名(参数类型 参数名) {
    // 函数体
    return 返回值;
}

// 示例
int add(int a, int b) {
    return a + b;
}
```

## 2. 面向对象编程（2-3天）
### 2.1 类和对象
```cpp
class Person {
private:
    string name;
    int age;
    
public:
    // 构造函数
    Person(string n, int a) : name(n), age(a) {}
    
    // 成员函数
    void display() {
        cout << "Name: " << name << ", Age: " << age << endl;
    }
};
```

### 2.2 继承和多态
```cpp
// 基类
class Animal {
public:
    virtual void makeSound() = 0;  // 纯虚函数
};

// 派生类
class Dog : public Animal {
public:
    void makeSound() override {
        cout << "Woof!" << endl;
    }
};
```

## 3. 内存管理（1-2天）
### 3.1 智能指针
```cpp
// unique_ptr
unique_ptr<int> ptr = make_unique<int>(42);

// shared_ptr
shared_ptr<int> shared = make_shared<int>(42);
```

### 3.2 引用
```cpp
void modifyValue(int& ref) {
    ref = 100;  // 修改原始值
}
```

## 4. STL容器（2-3天）
### 4.1 常用容器
```cpp
// vector
vector<int> numbers = {1, 2, 3, 4, 5};

// map
map<string, int> scores;
scores["Alice"] = 100;

// set
set<int> uniqueNumbers = {1, 2, 3, 3, 4};  // 只存储唯一值
```

### 4.2 迭代器
```cpp
vector<int> vec = {1, 2, 3, 4, 5};
for (auto it = vec.begin(); it != vec.end(); ++it) {
    cout << *it << endl;
}
```

## 5. 模板编程（2-3天）
### 5.1 函数模板
```cpp
template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}
```

### 5.2 类模板
```cpp
template<typename T>
class Stack {
private:
    vector<T> elements;
public:
    void push(T const& elem) {
        elements.push_back(elem);
    }
    T pop() {
        T elem = elements.back();
        elements.pop_back();
        return elem;
    }
};
```

## 学习建议
1. 每天固定2-3小时学习时间
2. 每个概念学习后立即编写示例代码
3. 使用在线编译器（如wandbox.org）快速测试代码
4. 完成每个章节后做一个小项目巩固知识

## 推荐资源
1. C++ Primer Plus（第6版）
2. C++ Reference (cppreference.com)
3. LeetCode C++题目练习

## 学习路线图
1. 第1周：基础语法 + 面向对象编程
2. 第2周：STL + 内存管理
3. 第3周：模板编程 + 项目实践

记住：多写代码，多调试，多思考。遇到问题及时解决，不要堆积。 