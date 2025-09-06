# C++智能指针学习笔记

## 练习题1：unique_ptr的基本操作
请完成以下任务：
1. 创建一个`unique_ptr`指向一个整数，初始值为100
2. 打印这个整数的值
3. 尝试创建一个新的`unique_ptr`指向同一个整数（这应该会失败）
4. 使用`move`将所有权转移给新的`unique_ptr`

请编写代码实现上述功能。

## 练习题2：shared_ptr的引用计数
请完成以下任务：
1. 创建一个`shared_ptr`指向一个字符串
2. 创建多个`shared_ptr`指向同一个字符串
3. 打印每个`shared_ptr`的引用计数
4. 释放一些`shared_ptr`，观察引用计数的变化

## 练习题3：智能指针与异常安全
请完成以下任务：
1. 创建一个类`Resource`，在构造函数和析构函数中打印信息
2. 使用`unique_ptr`管理这个资源
3. 在函数中抛出异常，观察资源是否正确释放

## 练习题4：智能指针与容器
请完成以下任务：
1. 创建一个`vector`存储`shared_ptr<Player>`
2. 实现一个函数，根据玩家名字查找玩家
3. 实现一个函数，删除指定名字的玩家
4. 确保在删除玩家时正确管理内存

## 练习题5：自定义删除器
请完成以下任务：
1. 创建一个文件资源类`FileResource`
2. 使用`unique_ptr`管理文件资源，并自定义删除器
3. 确保文件在`unique_ptr`被销毁时正确关闭

## 答案与解析

### 练习题1答案
```cpp
void unique_ptr_exercise() {
    // 创建unique_ptr
    std::unique_ptr<int> ptr1(new int(100));
    std::cout << "ptr1的值: " << *ptr1 << std::endl;

    // 尝试复制（这会导致编译错误）
    // std::unique_ptr<int> ptr2 = ptr1;  // 错误！

    // 使用move转移所有权
    std::unique_ptr<int> ptr2 = std::move(ptr1);
    std::cout << "ptr2的值: " << *ptr2 << std::endl;
    // 此时ptr1已经为空
    if (!ptr1) {
        std::cout << "ptr1已经被移动" << std::endl;
    }
}
```

### 练习题2答案
```cpp
void shared_ptr_exercise() {
    // 创建shared_ptr
    std::shared_ptr<std::string> str1(new std::string("Hello"));
    std::cout << "str1的引用计数: " << str1.use_count() << std::endl;

    // 创建新的shared_ptr指向同一个对象
    std::shared_ptr<std::string> str2 = str1;
    std::cout << "str1的引用计数: " << str1.use_count() << std::endl;
    std::cout << "str2的引用计数: " << str2.use_count() << std::endl;

    // 创建第三个shared_ptr
    {
        std::shared_ptr<std::string> str3 = str1;
        std::cout << "str1的引用计数: " << str1.use_count() << std::endl;
    } // str3离开作用域，引用计数减1

    std::cout << "str1的引用计数: " << str1.use_count() << std::endl;
}
```

### 练习题3答案
```cpp
class Resource {
public:
    Resource() { std::cout << "Resource创建" << std::endl; }
    ~Resource() { std::cout << "Resource销毁" << std::endl; }
};

void exception_safety_exercise() {
    try {
        std::unique_ptr<Resource> res(new Resource());
        throw std::runtime_error("测试异常");
    } catch (const std::exception& e) {
        std::cout << "捕获到异常: " << e.what() << std::endl;
    }
}
```

### 练习题4答案
```cpp
class Player {
public:
    Player(const std::string& name) : name(name) {}
    std::string name;
};

std::shared_ptr<Player> findPlayer(
    const std::vector<std::shared_ptr<Player>>& players,
    const std::string& name) {
    for (const auto& player : players) {
        if (player->name == name) {
            return player;
        }
    }
    return nullptr;
}

void removePlayer(
    std::vector<std::shared_ptr<Player>>& players,
    const std::string& name) {
    players.erase(
        std::remove_if(players.begin(), players.end(),
            [&name](const std::shared_ptr<Player>& p) {
                return p->name == name;
            }),
        players.end());
}
```

### 练习题5答案
```cpp
class FileResource {
public:
    FileResource(const std::string& filename) {
        file = fopen(filename.c_str(), "w");
        if (!file) {
            throw std::runtime_error("无法打开文件");
        }
    }

    void write(const std::string& content) {
        if (file) {
            fputs(content.c_str(), file);
        }
    }

    FILE* file;
};

// 自定义删除器
struct FileDeleter {
    void operator()(FileResource* res) {
        if (res && res->file) {
            fclose(res->file);
        }
        delete res;
    }
};

void custom_deleter_exercise() {
    std::unique_ptr<FileResource, FileDeleter> file(
        new FileResource("test.txt"));
    file->write("Hello, World!");
    // 文件会在unique_ptr销毁时自动关闭
}
``` 